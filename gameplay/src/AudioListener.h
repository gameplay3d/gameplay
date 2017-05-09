#pragma once

#include "Component.h"
#include "SceneObject.h"

namespace gameplay
{

/**
 * Defines an audio listener.
 */
class AudioListener
{
	friend class SceneObject;
    friend class Serializer::Activator;

public:

    /**
     * Constructor.
     */
    AudioListener();

    /**
     * Destructor
     */
    ~AudioListener();
};

}
