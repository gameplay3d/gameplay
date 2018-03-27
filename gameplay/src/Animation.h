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
    friend class Activator;

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
