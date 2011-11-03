/*
 * AudioController.h
 */

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


    static std::list<AudioSource*> _playingSources;     // List of currently running sources.
};

}

#endif
