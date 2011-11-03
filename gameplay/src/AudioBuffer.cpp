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
    unsigned int bufferCount = __buffers.size();
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
    ALboolean loop = AL_TRUE;
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

    std::string fullPath = FileSystem::getResourcePath();
    fullPath += "/";
    fullPath += path;
    
    // Load sound file.
    alBuffer = alutCreateBufferFromFile(fullPath.c_str());
    if (alBuffer == AL_NONE)
    {
        LOG_ERROR_VARG("AudioBuffer error (%d) loading file: %s", fullPath.c_str());
        return NULL;
    }

    buffer = new AudioBuffer(path, alBuffer);

    // Add the buffer to the cache.
    __buffers.push_back(buffer);

    return buffer;
}

}
