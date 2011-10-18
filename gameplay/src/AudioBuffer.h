/*
 * AudioBuffer.h
 */

#ifndef AUDIOBUFFER_H_
#define AUDIOBUFFER_H_

#include "Ref.h"

namespace gameplay
{

class AudioSource;

/**
 * The actual audio buffer data.
 *
 * Currently only supports OpenAL supported formats: .wav, .au and .raw files.
 */
class AudioBuffer : public Ref
{
    friend class AudioSource;

private:

    /**
     * Constructor.
     */
    AudioBuffer(const char* path, ALuint buffer);

    /**
     * Destructor.
     */
    virtual ~AudioBuffer();

    /**
     * Creates an audio buffer from a file.
     * 
     * @param path The path to the audio buffer on the filesystem.
     * 
     * @return The buffer from a file.
     */
    static AudioBuffer* create(const char* path);

    std::string _filePath;
    ALuint _alBuffer;
};

}

#endif
