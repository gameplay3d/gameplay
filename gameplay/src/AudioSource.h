#pragma once

#include "Component.h"
#include "SceneObject.h"

namespace gameplay
{

/**
 * Defines an audio source.
 */
class AudioSource : public Component
{
	friend class SceneObject;
    friend class Serializer::Activator;

public:

    /**
     * Constructor.
     */
    AudioSource();

    /**
     * Destructor
     */
    ~AudioSource();
};

}
