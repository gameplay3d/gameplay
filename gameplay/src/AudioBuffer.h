#ifndef AUDIOBUFFER_H_
#define AUDIOBUFFER_H_

#include "Ref.h"
#include "Stream.h"

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

    struct WavStreamingState;
    struct OggStreamingState;

private:
    
    /**
     * Constructor.
     */
    AudioBuffer(const char* path, ALuint * buffer, bool streamed);

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
     * @param streamed Use streaming.
     * 
     * @return The buffer from a file.
     */
    static AudioBuffer* create(const char* path, bool streamed);
    
    static bool loadWav(Stream* stream, ALuint buffer, bool streamed, WavStreamingState * wavStreamingState);
    
    static bool loadOgg(Stream* stream, ALuint buffer, bool streamed, OggStreamingState * oggStreamingState);

    bool streamData(ALuint buffer, bool loop);

    std::string _filePath;

    
    // Streaming data.
    
    enum { STREAMING_BUFFER_QUEUE_SIZE = 3 };
    enum { STREAMING_BUFFER_SIZE = 48000 };     // 48k bytes is aboud 1 sec of high quality sound.

    bool _useStreaming;
    ALuint _alBufferQueue[STREAMING_BUFFER_QUEUE_SIZE];
    
    struct WavStreamingState
    {
        long dataStart;         ///< Actual start position of data in the stream, needed to implement looping.
        unsigned int dataSize;  ///< Total data size.
        ALuint format;
        ALuint frequency;
    };

    struct OggStreamingState
    {
        OggVorbis_File ogg_file;
        long dataStart;         ///< Actual start position of data in the stream, needed to implement looping.
        unsigned int dataSize;  ///< Total data size.
        ALuint format;
        ALuint frequency;
    };

    std::auto_ptr< Stream > _fileStream;
    std::auto_ptr< WavStreamingState > _wavStreamingState;
    std::auto_ptr< OggStreamingState > _oggStreamingState;
    int _buffersNeededCount;
};

}

#endif
