#include "Base.h"
#include "AudioBuffer.h"
#include "FileSystem.h"

namespace gameplay
{

// Audio buffer cache
static std::vector<AudioBuffer*> __buffers;

AudioBuffer::AudioBuffer(const char* path, ALuint buffer)
    : _filePath(path), _alBuffer(buffer)
{
}

AudioBuffer::~AudioBuffer()
{
    // Remove the buffer from the cache.
    unsigned int bufferCount = (unsigned int)__buffers.size();
    for (unsigned int i = 0; i < bufferCount; i++)
    {
        if (this == __buffers[i])
        {
            __buffers.erase(__buffers.begin() + i);
            break;
        }
    }

    if (_alBuffer)
    {
        AL_CHECK( alDeleteBuffers(1, &_alBuffer) );
        _alBuffer = 0;
    }
}

AudioBuffer* AudioBuffer::create(const char* path)
{
    GP_ASSERT(path);

    // Search the cache for a stream from this file.
    unsigned int bufferCount = (unsigned int)__buffers.size();
    AudioBuffer* buffer = NULL;
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

    ALuint alBuffer;

    // Load audio data into a buffer.
    AL_CHECK( alGenBuffers(1, &alBuffer) );
    if (AL_LAST_ERROR())
    {
        GP_ERROR("Failed to create OpenAL buffer; alGenBuffers error: %d", AL_LAST_ERROR());
        AL_CHECK( alDeleteBuffers(1, &alBuffer) );
        return NULL;
    }
    
    // Load sound file.
    FILE* file = FileSystem::openFile(path, "rb");
    if (!file)
    {
        GP_ERROR("Failed to load audio file %s.", path);
        goto cleanup;
    }
    
    // Read the file header
    char header[12];
    if (fread(header, 1, 12, file) != 12)
    {
        GP_ERROR("Invalid header for audio file %s.", path);
        goto cleanup;
    }
    
    // Check the file format
    if (memcmp(header, "RIFF", 4) == 0)
    {
        if (!AudioBuffer::loadWav(file, alBuffer))
        {
            GP_ERROR("Invalid wave file: %s", path);
            goto cleanup;
        }
    }
    else if (memcmp(header, "OggS", 4) == 0)
    {
        if (!AudioBuffer::loadOgg(file, alBuffer))
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

    //NOTE: loadOgg actually sets this null, so it is expected
    if (file)    
        fclose(file);

    buffer = new AudioBuffer(path, alBuffer);

    // Add the buffer to the cache.
    __buffers.push_back(buffer);

    return buffer;
    
cleanup:
    
    if (file)
        fclose(file);
    if (alBuffer)
        AL_CHECK( alDeleteBuffers(1, &alBuffer) );
    return NULL;
}

bool AudioBuffer::loadWav(FILE* file, ALuint buffer)
{
    GP_ASSERT(file);
    unsigned char stream[12];
    
    // Verify the wave fmt magic value meaning format.
    if (fread(stream, 1, 8, file) != 8 || memcmp(stream, "fmt ", 4) != 0 )
    {
        GP_ERROR("Failed to verify the magic value for the wave file format.");
        return false;
    }
    
    unsigned int section_size;
    section_size  = stream[7]<<24;
    section_size |= stream[6]<<16;
    section_size |= stream[5]<<8;
    section_size |= stream[4];

    // Check for a valid pcm format.
    if (fread(stream, 1, 2, file) != 2 || stream[1] != 0 || stream[0] != 1)
    {
        GP_ERROR("Unsupported audio file format (must be a valid PCM format).");
        return false;
    }
    
    // Get the channel count (16-bit little-endian).
    int channels;
    if (fread(stream, 1, 2, file) != 2)
    {
        GP_ERROR("Failed to read the wave file's channel count.");
        return false;
    }
    channels  = stream[1]<<8;
    channels |= stream[0];
    
    // Get the sample frequency (32-bit little-endian).
    ALuint frequency;
    if (fread(stream, 1, 4, file) != 4)
    {
        GP_ERROR("Failed to read the wave file's sample frequency.");
        return false;
    }

    frequency  = stream[3]<<24;
    frequency |= stream[2]<<16;
    frequency |= stream[1]<<8;
    frequency |= stream[0];
    
    // The next 6 bytes hold the block size and bytes-per-second. 
    // We don't need that info, so just read and ignore it. 
    // We could use this later if we need to know the duration.
    if (fread(stream, 1, 6, file) != 6)
    {
        GP_ERROR("Failed to read past the wave file's block size and bytes-per-second.");
        return false;
    }
    
    // Get the bit depth (16-bit little-endian).
    int bits;
    if (fread(stream, 1, 2, file) != 2)
    {
        GP_ERROR("Failed to read the wave file's bit depth.");
        return false;
    }
    bits  = stream[1]<<8;
    bits |= stream[0];
    
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
        if (fread(stream, 1, length, file) != length)
        {
            GP_ERROR("Failed to read extension size from wave file.");
            return false;
        }
    }

    // Read in the rest of the file a chunk (section) at a time.
    while (true)
    {
        // Check if we are at the end of the file without reading the data.
        if (feof(file))
        {
            GP_ERROR("Failed to load wave file; file appears to have no data.");
            return false;
        }

        // Read in the type of the next section of the file.
        if (fread(stream, 1, 4, file) != 4)
        {
            GP_ERROR("Failed to read next section type from wave file.");
            return false;
        }

        // Data chunk.
        if (memcmp(stream, "data", 4) == 0)
        {
            // Read how much data is remaining and buffer it up.
            unsigned int dataSize;
            if (fread(&dataSize, sizeof(int), 1, file) != 1)
            {
                GP_ERROR("Failed to read size of data section from wave file.");
                return false;
            }

            char* data = new char[dataSize];
            if (fread(data, sizeof(char), dataSize, file) != dataSize)
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
            memcpy(chunk, stream, 4);

            // Read the chunk size.
            if (fread(stream, 1, 4, file) != 4)
            {
                GP_ERROR("Failed to read size of '%s' chunk from wave file.", chunk);
                return false;
            }

            section_size  = stream[3]<<24;
            section_size |= stream[2]<<16;
            section_size |= stream[1]<<8;
            section_size |= stream[0];

            // Seek past the chunk.
            if (fseek(file, section_size, SEEK_CUR) != 0)
            {
                GP_ERROR("Failed to seek past '%s' chunk in wave file.", chunk);
                return false;
            }
        }
    }
}
    
bool AudioBuffer::loadOgg(FILE*& file, ALuint buffer)
{
    GP_ASSERT(file);

    OggVorbis_File ogg_file;
    vorbis_info* info;
    ALenum format;
    long result;
    int section;
    long size = 0;

    rewind(file);

    if ((result = ov_open(file, &ogg_file, NULL, 0)) < 0)
    {
        fclose(file);
        GP_ERROR("Failed to open ogg file.");
        return false;
    }

    info = ov_info(&ogg_file, -1);
    GP_ASSERT(info);
    if (info->channels == 1)
        format = AL_FORMAT_MONO16;
    else
        format = AL_FORMAT_STEREO16;

    // size = #samples * #channels * 2 (for 16 bit).
    long data_size = ov_pcm_total(&ogg_file, -1) * info->channels * 2;
    char* data = new char[data_size];

    while (size < data_size)
    {
        result = ov_read(&ogg_file, data + size, data_size - size, 0, 2, 1, &section);
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

    AL_CHECK( alBufferData(buffer, format, data, data_size, info->rate) );

    SAFE_DELETE_ARRAY(data);
    ov_clear(&ogg_file);

    // ov_clear actually closes the file pointer as well.
    file = 0;

    return true;
}

}
