#pragma once

#include "Component.h"
#include "SceneObject.h"

namespace gameplay
{

/**
 * Defines an abstract audio system.
 */
class Audio
{
public:
    /**
     * Gets the audio system.
     *
     * @return The audio sytem.
     */
    static Audio* getAudio();

    /**
     * Defines the audio hardware buffer.
     */
    class Buffer
    {
    };

    /**
     * Defines the point that receive audio from sources
     * and plays them through the systems audio output device.
     */
    class Listener
    {

    };

    /**
     * Defines the source of a played audio clip.
     */
    class Source : public Component
    {
        friend class SceneObject;
        //friend class Serializer::Activator;

    };

protected:

    static Audio* _audio;
};

}
