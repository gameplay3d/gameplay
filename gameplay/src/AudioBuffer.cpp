/*
 * AudioBuffer.cpp
 */

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
    else if(memcmp(header, "OGG", 3) == 0)
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
    
    // Check for a valid pcm format.
    if(fread(stream, 1, 2, file) != 2 || stream[1] != 0 || stream[0] != 1)
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
    if(bits == 8)
    {
        if(channels == 1)
            format = AL_FORMAT_MONO8;
        else if(channels == 2)
            format = AL_FORMAT_STEREO8;
    }
    else if(bits == 16)
    {
        if(channels == 1)
            format = AL_FORMAT_MONO16;
        else if(channels == 2)
            format = AL_FORMAT_STEREO16;
    }
    else
    {
        LOG_ERROR_VARG("Incompatible format: (%d, %d)", channels, bits);
        return false;
    }
    
    // Read the data chunk, which will hold the decoded sample data 
    if (fread(stream, 1, 4, file) != 4 || memcmp(stream, "data", 4) != 0)
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
    
// TODO:
bool AudioBuffer::loadOgg(FILE* file, ALuint buffer)
{
    LOG_ERROR("Ogg Vorbis not supported yet");
    
    return false;
}

}
