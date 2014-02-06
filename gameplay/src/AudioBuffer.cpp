#include "Base.h"
#include "AudioBuffer.h"
#include "FileSystem.h"

namespace gameplay
{

// Audio buffer cache
static std::vector<AudioBuffer*> __buffers;

// Callbacks for loading an ogg file using Stream
static size_t readStream(void *ptr, size_t size, size_t nmemb, void *datasource)
{
    GP_ASSERT(datasource);
    Stream* stream = reinterpret_cast<Stream*>(datasource);
    return stream->read(ptr, size, nmemb);
}

static int seekStream(void *datasource, ogg_int64_t offset, int whence)
{
    GP_ASSERT(datasource);
    Stream* stream = reinterpret_cast<Stream*>(datasource);
    return !stream->seek(offset, whence);
}

static int closeStream(void *datasource)
{
    GP_ASSERT(datasource);
    Stream* stream = reinterpret_cast<Stream*>(datasource);
    stream->close();
    return 0;
}

static long tellStream(void *datasource)
{
    GP_ASSERT(datasource);
    Stream* stream = reinterpret_cast<Stream*>(datasource);
    return stream->position();
}

AudioBuffer::AudioBuffer(const char* path, ALuint * buffers, bool streamed)
    : _filePath(path), _useStreaming(streamed)
    , _buffersNeededCount(0)
{
    memcpy(_alBufferQueue, buffers, sizeof(_alBufferQueue));
}

AudioBuffer::~AudioBuffer()
{
    // Remove the buffer from the cache.
    if (!_useStreaming)
    {
        unsigned int bufferCount = (unsigned int)__buffers.size();
        for (unsigned int i = 0; i < bufferCount; i++)
        {
            if (this == __buffers[i])
            {
                __buffers.erase(__buffers.begin() + i);
                break;
            }
        }
    }
    else if (_oggStreamingState.get())
    {
        ov_clear (&_oggStreamingState->ogg_file);
    }

    for (int i = 0; i < STREAMING_BUFFER_QUEUE_SIZE; i++)
        if (_alBufferQueue[i])
        {
            AL_CHECK( alDeleteBuffers(1, &_alBufferQueue[i]) );
            _alBufferQueue[i] = 0;
        }
}

AudioBuffer* AudioBuffer::create(const char* path, bool streamed)
{
    GP_ASSERT(path);

    // Search the cache for a stream from this file.
    AudioBuffer* buffer = NULL;
    if( !streamed )
    {
        unsigned int bufferCount = (unsigned int)__buffers.size();
        for (unsigned int i = 0; i < bufferCount; i++)
        {
            buffer = __buffers[i];
            GP_ASSERT(buffer);
            if (buffer->_filePath.compare(path) == 0)
            {
                buffer->addRef();
                return buffer;
            }
        }
    }

    int i;
    ALuint alBuffer[STREAMING_BUFFER_QUEUE_SIZE];
    memset(alBuffer, 0, sizeof(alBuffer));

    // Create 1 buffer for non-streamed sounds or full queue for streamed ones.
    int buffersToGenerate = streamed ? STREAMING_BUFFER_QUEUE_SIZE : 1;
    for(i = 0; i < buffersToGenerate; i++)
    {
        // Load audio data into a buffer.
        AL_CHECK( alGenBuffers(1, &alBuffer[i]) );
        if (AL_LAST_ERROR())
        {
            GP_ERROR("Failed to create OpenAL buffer; alGenBuffers error: %d", AL_LAST_ERROR());
            AL_CHECK( alDeleteBuffers(1, &alBuffer[i]) );
            return NULL;
        }
    }
    
    std::auto_ptr< WavStreamingState > wavStreamingState;
    std::auto_ptr< OggStreamingState > oggStreamingState;
    
    // Load sound file.
    std::auto_ptr<Stream> stream(FileSystem::open(path));
    if (stream.get() == NULL || !stream->canRead())
    {
        GP_ERROR("Failed to load audio file %s.", path);
        goto cleanup;
    }

    // Read the file header
    char header[12];
    if (stream->read(header, 1, 12) != 12)
    {
        GP_ERROR("Invalid header for audio file %s.", path);
        goto cleanup;
    }
    
    // Check the file format
    if (memcmp(header, "RIFF", 4) == 0)
    {
        // Fill at least one buffer with sound data.
        wavStreamingState.reset( new WavStreamingState( ) );
        if (!AudioBuffer::loadWav(stream.get(), alBuffer[0], streamed, wavStreamingState.get( )))
        {
            GP_ERROR("Invalid wave file: %s", path);
            goto cleanup;
        }
    }
    else if (memcmp(header, "OggS", 4) == 0)
    {
        // Fill at least one buffer with sound data.
        oggStreamingState.reset( new OggStreamingState( ) );
        if (!AudioBuffer::loadOgg(stream.get(), alBuffer[0], streamed, oggStreamingState.get()))
        {
            GP_ERROR("Invalid ogg file: %s", path);
            goto cleanup;
        }
    }
    else
    {
        GP_ERROR("Unsupported audio file: %s", path);
        goto cleanup;
    }

    buffer = new AudioBuffer(path, alBuffer, streamed);
    buffer->_fileStream.reset( stream.release( ) );
    buffer->_wavStreamingState.reset( wavStreamingState.release( ) );
    buffer->_oggStreamingState.reset( oggStreamingState.release( ) );
    if (buffer->_wavStreamingState.get())
        buffer->_buffersNeededCount = (buffer->_wavStreamingState->dataSize + STREAMING_BUFFER_SIZE - 1) / STREAMING_BUFFER_SIZE;
    else if (buffer->_oggStreamingState.get())
        buffer->_buffersNeededCount = (buffer->_oggStreamingState->dataSize + STREAMING_BUFFER_SIZE - 1) / STREAMING_BUFFER_SIZE;

    // Add the buffer to the cache.
    if( !streamed )
        __buffers.push_back(buffer);

    return buffer;
    
cleanup:
    for(i = 0; i < STREAMING_BUFFER_QUEUE_SIZE; i++)
        if(alBuffer[i])
            AL_CHECK( alDeleteBuffers(1, &alBuffer[i]) );
    return NULL;
}

bool AudioBuffer::loadWav(Stream* stream, ALuint buffer, bool streamed, WavStreamingState * wavStreamingState)
{
    GP_ASSERT(stream);

    unsigned char data[12];
    
    // Verify the wave fmt magic value meaning format.
    if (stream->read(data, 1, 8) != 8 || memcmp(data, "fmt ", 4) != 0 )
    {
        GP_ERROR("Failed to verify the magic value for the wave file format.");
        return false;
    }
    
    unsigned int section_size;
    section_size  = data[7]<<24;
    section_size |= data[6]<<16;
    section_size |= data[5]<<8;
    section_size |= data[4];

    // Check for a valid pcm format.
    if (stream->read(data, 1, 2) != 2 || data[1] != 0 || data[0] != 1)
    {
        GP_ERROR("Unsupported audio file format (must be a valid PCM format).");
        return false;
    }
    
    // Get the channel count (16-bit little-endian).
    int channels;
    if (stream->read(data, 1, 2) != 2)
    {
        GP_ERROR("Failed to read the wave file's channel count.");
        return false;
    }
    channels  = data[1]<<8;
    channels |= data[0];
    
    // Get the sample frequency (32-bit little-endian).
    ALuint frequency;
    if (stream->read(data, 1, 4) != 4)
    {
        GP_ERROR("Failed to read the wave file's sample frequency.");
        return false;
    }

    frequency  = data[3]<<24;
    frequency |= data[2]<<16;
    frequency |= data[1]<<8;
    frequency |= data[0];
    
    // The next 6 bytes hold the block size and bytes-per-second. 
    // We don't need that info, so just read and ignore it. 
    // We could use this later if we need to know the duration.
    if (stream->read(data, 1, 6) != 6)
    {
        GP_ERROR("Failed to read past the wave file's block size and bytes-per-second.");
        return false;
    }
    
    // Get the bit depth (16-bit little-endian).
    int bits;
    if (stream->read(data, 1, 2) != 2)
    {
        GP_ERROR("Failed to read the wave file's bit depth.");
        return false;
    }
    bits  = data[1]<<8;
    bits |= data[0];
    
    // Now convert the given channel count and bit depth into an OpenAL format. 
    ALuint format = 0;
    if (bits == 8)
    {
        if (channels == 1)
            format = AL_FORMAT_MONO8;
        else if (channels == 2)
            format = AL_FORMAT_STEREO8;
    }
    else if (bits == 16)
    {
        if (channels == 1)
            format = AL_FORMAT_MONO16;
        else if (channels == 2)
            format = AL_FORMAT_STEREO16;
    }
    else
    {
        GP_ERROR("Incompatible wave file format: (%d, %d)", channels, bits);
        return false;
    }
    
    // Check against the size of the format header as there may be more data that we need to read.
    if (section_size > 16)
    {
        unsigned int length = section_size - 16;

        // Extension size is 2 bytes.
        if (stream->read(data, 1, length) != length)
        {
            GP_ERROR("Failed to read extension size from wave file.");
            return false;
        }
    }

    // Read in the rest of the file a chunk (section) at a time.
    while (true)
    {
        // Check if we are at the end of the file without reading the data.
        if (stream->eof())
        {
            GP_ERROR("Failed to load wave file; file appears to have no data.");
            return false;
        }

        // Read in the type of the next section of the file.
        if (stream->read(data, 1, 4) != 4)
        {
            GP_ERROR("Failed to read next section type from wave file.");
            return false;
        }

        // Data chunk.
        if (memcmp(data, "data", 4) == 0)
        {
            // Read how much data is remaining and buffer it up.
            unsigned int dataSize;
            if (stream->read(&dataSize, sizeof(int), 1) != 1)
            {
                GP_ERROR("Failed to read size of data section from wave file.");
                return false;
            }

            if (streamed)
            {
                // Save streaming state for later use.
                wavStreamingState->dataStart = stream->position( );
                wavStreamingState->dataSize = dataSize;
                wavStreamingState->format = format;
                wavStreamingState->frequency = frequency;

                // Limit data size to STREAMING_BUFFER_SIZE.
                if( dataSize > STREAMING_BUFFER_SIZE )
                    dataSize = STREAMING_BUFFER_SIZE;
            }

            char* data = new char[dataSize];
            if (stream->read(data, sizeof(char), dataSize) != dataSize)
            {
                GP_ERROR("Failed to load wave file; file is missing data.");
                SAFE_DELETE_ARRAY(data);
                return false;
            }

            AL_CHECK( alBufferData(buffer, format, data, dataSize, frequency) );
            SAFE_DELETE_ARRAY(data);

            // We've read the data, so return now.
            return true;
        }
        // Other chunk - could be any of the following:
        // - Fact ("fact")
        // - Wave List ("wavl")
        // - Silent ("slnt")
        // - Cue ("cue ")
        // - Playlist ("plst")
        // - Associated Data List ("list")
        // - Label ("labl")
        // - Note ("note")
        // - Labeled Text ("ltxt")
        // - Sampler ("smpl")
        // - Instrument ("inst")
        else
        {
            // Store the name of the chunk so we can report errors informatively.
            char chunk[5] = { 0 };
            memcpy(chunk, data, 4);

            // Read the chunk size.
            if (stream->read(data, 1, 4) != 4)
            {
                GP_ERROR("Failed to read size of '%s' chunk from wave file.", chunk);
                return false;
            }

            section_size  = data[3]<<24;
            section_size |= data[2]<<16;
            section_size |= data[1]<<8;
            section_size |= data[0];

            // Seek past the chunk.
            if (stream->seek(section_size, SEEK_CUR) == false)
            {
                GP_ERROR("Failed to seek past '%s' chunk in wave file.", chunk);
                return false;
            }
        }
    }
    return false;
}

bool AudioBuffer::loadOgg(Stream* stream, ALuint buffer, bool streamed, OggStreamingState * oggStreamingState)
{
    GP_ASSERT(stream);

    vorbis_info* info;
    ALenum format;
    long result;
    int section;
    long size = 0;

    stream->rewind();

    ov_callbacks callbacks;
    callbacks.read_func = readStream;
    callbacks.seek_func = seekStream;
    callbacks.close_func = closeStream;
    callbacks.tell_func = tellStream;

    if ((result = ov_open_callbacks(stream, &oggStreamingState->ogg_file, NULL, 0, callbacks)) < 0)
    {
        GP_ERROR("Failed to open ogg file.");
        return false;
    }

    info = ov_info(&oggStreamingState->ogg_file, -1);
    GP_ASSERT(info);
    if (info->channels == 1)
        format = AL_FORMAT_MONO16;
    else
        format = AL_FORMAT_STEREO16;

    // size = #samples * #channels * 2 (for 16 bit).
    long data_size = ov_pcm_total(&oggStreamingState->ogg_file, -1) * info->channels * 2;

    if (streamed)
    {
        // Save streaming state for later use.
        oggStreamingState->dataStart = ov_pcm_tell(&oggStreamingState->ogg_file);
        oggStreamingState->dataSize = data_size;
        oggStreamingState->format = format;
        oggStreamingState->frequency = info->rate;

        // Limit data size to STREAMING_BUFFER_SIZE.
        if (data_size > STREAMING_BUFFER_SIZE)
            data_size = STREAMING_BUFFER_SIZE;
    }

    char* data = new char[data_size];

    while (size < data_size)
    {
        result = ov_read(&oggStreamingState->ogg_file, data + size, data_size - size, 0, 2, 1, &section);
        if (result > 0)
        {
            size += result;
        }
        else if (result < 0)
        {
            SAFE_DELETE_ARRAY(data);
            GP_ERROR("Failed to read ogg file; file is missing data.");
            return false;
        }
        else
        {
            break;
        }
    }
    
    if (size == 0)
    {
        SAFE_DELETE_ARRAY(data);
        GP_ERROR("Filed to read ogg file; unable to read any data.");
        return false;
    }

    AL_CHECK( alBufferData(buffer, format, data, size, info->rate) );

    SAFE_DELETE_ARRAY(data);

    if (!streamed)
        ov_clear(&oggStreamingState->ogg_file);

    return true;
}

bool AudioBuffer::streamData(ALuint buffer, bool loop)
{
    static char buf[STREAMING_BUFFER_SIZE];

    if (_wavStreamingState.get())
    {
        ALsizei bytesRead = _fileStream->read(buf, sizeof(char), STREAMING_BUFFER_SIZE);
        if (bytesRead != STREAMING_BUFFER_SIZE)
        {
            if (loop)
                _fileStream->seek(_wavStreamingState->dataStart, SEEK_SET);
        }

        if (bytesRead > 0)
            AL_CHECK( alBufferData(buffer, _wavStreamingState->format, buf, bytesRead, _wavStreamingState->frequency) );
        return bytesRead > 0 || loop;
    }
    else if (_oggStreamingState.get())
    {
        int section;
        int result = 0;
        ALsizei bytesRead = 0;

        while (bytesRead < STREAMING_BUFFER_SIZE)
        {
            result = ov_read(&_oggStreamingState->ogg_file, buf + bytesRead, STREAMING_BUFFER_SIZE - bytesRead, 0, 2, 1, &section);
            if (result > 0)
            {
                bytesRead += result;
            }
            else
            {
                if (loop)
                    ov_pcm_seek(&_oggStreamingState->ogg_file, _oggStreamingState->dataStart);
                break;
            }
        }

        if (bytesRead > 0)
            AL_CHECK( alBufferData(buffer, _oggStreamingState->format, buf, bytesRead, _oggStreamingState->frequency) );
        return bytesRead > 0 || loop;
    }

    return false;
}

}
