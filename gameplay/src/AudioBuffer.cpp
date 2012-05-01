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
        alDeleteBuffers(1, &_alBuffer);
        _alBuffer = 0;
    }
}

AudioBuffer* AudioBuffer::create(const char* path)
{
    assert(path);

    // Search the cache for a stream from this file.
    unsigned int bufferCount = (unsigned int)__buffers.size();
    AudioBuffer* buffer = NULL;
    for (unsigned int i = 0; i < bufferCount; i++)
    {
        buffer = __buffers[i];
        if (buffer->_filePath.compare(path) == 0)
        {
            buffer->addRef();
            return buffer;
        }
    }

    ALuint alBuffer;
    ALCenum al_error;

    // Load audio data into a buffer.
    alGenBuffers(1, &alBuffer);
    al_error = alGetError();
    if (al_error != AL_NO_ERROR)
    {
        LOG_ERROR_VARG("AudioBuffer alGenBuffers AL error: %d", al_error);
        alDeleteBuffers(1, &alBuffer);
        return NULL;
    }
    
    // Load sound file.
    FILE* file = FileSystem::openFile(path, "rb");
    if (!file)
    {
        LOG_ERROR_VARG("Invalid audio buffer file: %s", path);
        goto cleanup;
    }
    
    // Read the file header
    char header[12];
    if (fread(header, 1, 12, file) != 12)
    {
        LOG_ERROR_VARG("Invalid audio buffer file: %s", path);
        goto cleanup;
    }
    
    // Check the file format
    if (memcmp(header, "RIFF", 4) == 0)
    {
        if (!AudioBuffer::loadWav(file, alBuffer))
        {
            LOG_ERROR_VARG("Invalid wave file: %s", path);
            goto cleanup;
        }
    }
    else if (memcmp(header, "OggS", 4) == 0)
    {
        if (!AudioBuffer::loadOgg(file, alBuffer))
        {
            LOG_ERROR_VARG("Invalid ogg file: %s", path);
            goto cleanup;
        }
    }
    else
    {
        LOG_ERROR_VARG("Unsupported audio file: %s", path);
    }
    
    fclose(file);

    buffer = new AudioBuffer(path, alBuffer);

    // Add the buffer to the cache.
    __buffers.push_back(buffer);

    return buffer;
    
cleanup:
    
    if (file)
        fclose(file);
    if (alBuffer)
        alDeleteBuffers(1, &alBuffer);
    return NULL;
}

bool AudioBuffer::loadWav(FILE* file, ALuint buffer)
{
    unsigned char stream[12];
    
    // Verify the wave fmt magic value meaning format.
    if (fread(stream, 1, 8, file) != 8 || memcmp(stream, "fmt ", 4) != 0 )
        return false;
    
    unsigned int section_size;
    section_size  = stream[7]<<24;
    section_size |= stream[6]<<16;
    section_size |= stream[5]<<8;
    section_size |= stream[4];

    // Check for a valid pcm format.
    if (fread(stream, 1, 2, file) != 2 || stream[1] != 0 || stream[0] != 1)
    {
        LOG_ERROR("Unsupported audio file, not PCM format.");
        return false;
    }
    
    // Get the channel count (16-bit little-endian)
    int channels;
    if (fread(stream, 1, 2, file) != 2)
        return false;
    channels  = stream[1]<<8;
    channels |= stream[0];
    
    // Get the sample frequency (32-bit little-endian) 
    ALuint frequency;
    if (fread(stream, 1, 4, file) != 4)
        return false;

    frequency  = stream[3]<<24;
    frequency |= stream[2]<<16;
    frequency |= stream[1]<<8;
    frequency |= stream[0];
    
    // The next 6 bytes hold the block size and bytes-per-second. 
    // We don't need that info, so just read and ignore it. 
    // We could use this later if we need to know the duration.
    if (fread(stream, 1, 6, file) != 6)
        return false;
    
    // Get the bit depth (16-bit little-endian)
    int bits;
    if (fread(stream, 1, 2, file) != 2)
        return false;
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
        LOG_ERROR_VARG("Incompatible format: (%d, %d)", channels, bits);
        return false;
    }
    
    // Check against the size of the format header as there may be more data that we need to read
    if (section_size > 16)
    {
        unsigned int length = section_size - 16;

        // extension size is 2 bytes
        if (fread(stream, 1, length, file) != length)
            return false;
    }

    if (fread(stream, 1, 4, file) != 4)
        return false;

    // read the next chunk, could be fact section or the data section
    if (memcmp(stream, "fact", 4) == 0)
    {
        if (fread(stream, 1, 4, file) != 4)
            return false;

        section_size  = stream[3]<<24;
        section_size |= stream[2]<<16;
        section_size |= stream[1]<<8;
        section_size |= stream[0];

        // read in the rest of the fact section
        if (fread(stream, 1, section_size, file) != section_size)
            return false;

        if (fread(stream, 1, 4, file) != 4)
            return false;
    }

    // should now be the data section which holds the decoded sample data
    if (memcmp(stream, "data", 4) != 0)
    {
        LOG_ERROR("WAV file has no data.");
        return false;
    }

    // Read how much data is remaining and buffer it up.
    unsigned int dataSize;
    fread(&dataSize, sizeof(int), 1, file);

    char* data = new char[dataSize];
    if (fread(data, sizeof(char), dataSize, file) != dataSize)
    {
        LOG_ERROR("WAV file missing data.");
        SAFE_DELETE_ARRAY(data);
        return false;
    }

    alBufferData(buffer, format, data, dataSize, frequency);
    SAFE_DELETE_ARRAY(data);
    return true;
}
    
bool AudioBuffer::loadOgg(FILE* file, ALuint buffer)
{
    OggVorbis_File ogg_file;
    vorbis_info* info;
    ALenum format;
    int result;
    int section;
    unsigned int size = 0;

    rewind(file);

    if ((result = ov_open(file, &ogg_file, NULL, 0)) < 0)
    {
        fclose(file);
        LOG_ERROR("Could not open Ogg stream.");
        return false;
    }

    info = ov_info(&ogg_file, -1);

    if (info->channels == 1)
        format = AL_FORMAT_MONO16;
    else
        format = AL_FORMAT_STEREO16;

    // size = #samples * #channels * 2 (for 16 bit)
    unsigned int data_size = ov_pcm_total(&ogg_file, -1) * info->channels * 2;
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
            LOG_ERROR("OGG file missing data.");
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
        LOG_ERROR("Unable to read OGG data.");
        return false;
    }

    alBufferData(buffer, format, data, data_size, info->rate);

    SAFE_DELETE_ARRAY(data);
    ov_clear(&ogg_file);

    // ov_clear actually closes the file pointer as well
    file = 0;

    return true;
}

}
