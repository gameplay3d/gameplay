#ifndef AUDIOBUFFER_H_
#define AUDIOBUFFER_H_

#include "Ref.h"

namespace gameplay
{

class AudioSource;

/**
 * The actual audio buffer data.
 *
 * Currently only supports supported formats: .wav, .au and .raw files.
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
     * Hidden copy assignment operator.
     */
    AudioBuffer& operator=(const AudioBuffer&);

    /**
     * Creates an audio buffer from a file.
     * 
     * @param path The path to the audio buffer on the filesystem.
     * 
     * @return The buffer from a file.
     */
    static AudioBuffer* create(const char* path);
    
    static bool loadWav(FILE* file, ALuint buffer);
    
    static bool loadOgg(FILE*& file, ALuint buffer);

    std::string _filePath;
    ALuint _alBuffer;
};

}

#endif
