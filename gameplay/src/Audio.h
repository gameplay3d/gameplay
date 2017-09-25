#pragma once

#include "AudioListener.h"
#include "AudioSource.h"

namespace gameplay
{

/**
 * Defines an abstract audio system.
 */
class Audio
{
    friend class Game;

public:

    /**
     * Gets the audio system.
     *
     * @return The audio sytem.
     */
    static Audio* getAudio();

    /**
     * Gets the audio listener.
     *
     * This repesents where you are listening from and output to your audio output (headphones/speakers).
     *
     * @return The audio listener
     */
    std::shared_ptr<AudioListener> getAudioListener() const;
 
    /**
     * Creates an audio source.
     *
     * This represents the source of where the audio is is being emitted from.
     *
     * @param url The url of the sound file.
     * @param streamed true if audio stream the data on demand, false if loaded first into a buffer.
     * @return The create audio source.
     */
    std::shared_ptr<AudioSource> createAudioSource(std::string url, bool streamed = false);

protected:

	/**
     * Event occurs when the game starts and audio system is initialized.
	 */
    void onInitialize();

    /**
     * Event occurs when game is paused so the audio system and 
     * all audio sources can also pause.
     */
    void onPause();

    /**
     * Event occurs when the game is resumed so the audio system 
	 * can play any previously playing audio sources.
     */
    void onResume();

    /**
     * Event occurs every game frame to allow the audio system to 
     * update state when running.
     *
     * @param elapsedTime The time elapsed since the last frame.
     */
    void onUpdate(float elapsedTime);

    static Audio* _audio;
};

}

