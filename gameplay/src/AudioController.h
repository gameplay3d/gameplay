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
    void update(float elapsedTime);


    ALCdevice* _alcDevice;
    ALCcontext* _alcContext;
    std::set<AudioSource*> _playingSources;
    AudioSource* _pausingSource;
};

}

#endif
