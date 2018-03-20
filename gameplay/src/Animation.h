#pragma once

#include "Component.h"
#include "SceneObject.h"

namespace gameplay
{

/**
 * Defines an animation componenent.
 */
class Animation : public Component
{
    friend class SceneObject;
    friend class Serializer::Activator;

public:

    /**
     * Constructor.
     */
    Animation();

    /**
     * Destructor
     */
    ~Animation();
};

}
