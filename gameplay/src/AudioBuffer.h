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
#ifndef __ANDROID__
    /**
     * Constructor.
     */
    AudioBuffer(const char* path, ALuint buffer);
#else
    /**
     * Constructor.
     */
    AudioBuffer(const char* path);
#endif

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
    
#ifndef __ANDROID__
    static bool loadWav(FILE* file, ALuint buffer);
    
    static bool loadOgg(FILE* file, ALuint buffer);
#endif

    std::string _filePath;
#ifndef __ANDROID__
    ALuint _alBuffer;
#else
    SLDataLocator_AndroidFD _data;
    SLDataFormat_MIME _mime;
#endif
};

}

#endif
