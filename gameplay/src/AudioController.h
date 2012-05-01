#ifndef AUDIOCONTROLLER_H_
#define AUDIOCONTROLLER_H_

namespace gameplay
{

class AudioListener;
class AudioSource;

/**
 * Defines a class for controlling game audio.
 */
class AudioController
{
    friend class Game;
    friend class AudioSource;

public:
    
    /**
     * Destructor.
     */
    virtual ~AudioController();

private:
    
    /**
     * Constructor.
     */
    AudioController();

    /**
     * Controller initialize.
     */
    void initialize();

    /**
     * Controller finalize.
     */
    void finalize();

    /**
     * Controller pause.
     */
    void pause();

    /**
     * Controller resume.
     */
    void resume();

    /**
     * Controller update.
     */
    void update(long elapsedTime);

#ifndef __ANDROID__
    ALCdevice* _alcDevice;
    ALCcontext* _alcContext;
#else
    SLObjectItf _engineObject;
    SLEngineItf _engineEngine;
    SLObjectItf _outputMixObject;
    SLObjectItf _listenerObject;
    SL3DDopplerItf _listenerDoppler;
    SL3DLocationItf _listenerLocation;
#endif
    std::set<AudioSource*> _playingSources;
    AudioSource* _pausingSource;
};

}

#endif
