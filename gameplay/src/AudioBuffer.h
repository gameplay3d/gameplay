#ifndef AUDIOBUFFER_H_
#define AUDIOBUFFER_H_

#include "Ref.h"
#include "Stream.h"

namespace gameplay
{

class AudioSource;

/**
 * Defines the actual audio buffer data.
 *
 * Currently only supports supported formats: .ogg, .wav, .au and .raw files.
 */
class AudioBuffer : public Ref
{
    friend class AudioSource;

private:
    
    /**
     * Constructor.
     */
    AudioBuffer(const char* path, ALuint* buffers, bool streamed);

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
    static AudioBuffer* create(const char* path, bool streamed);

    struct AudioStreamStateWav
    {
        long dataStart;
        unsigned int dataSize;
        ALuint format;
        ALuint frequency;
    };

    struct AudioStreamStateOgg
    {
        long dataStart;
        unsigned int dataSize;
        ALuint format;
        ALuint frequency;
        OggVorbis_File oggFile;
    };

    enum { STREAMING_BUFFER_QUEUE_SIZE = 3 };
    enum { STREAMING_BUFFER_SIZE = 48000 };

    static bool loadWav(Stream* stream, ALuint buffer, bool streamed, AudioStreamStateWav* streamState);
    
    static bool loadOgg(Stream* stream, ALuint buffer, bool streamed, AudioStreamStateOgg* streamState);

    bool streamData(ALuint buffer, bool looped);

    ALuint _alBufferQueue[STREAMING_BUFFER_QUEUE_SIZE];
    std::string _filePath;
    bool _streamed;
    std::unique_ptr<Stream> _fileStream;
    std::unique_ptr<AudioStreamStateWav> _streamStateWav;
    std::unique_ptr<AudioStreamStateOgg> _streamStateOgg;
    int _buffersNeededCount;
};

}

#endif
