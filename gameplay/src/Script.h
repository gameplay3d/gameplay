#pragma once

#include "Component.h"
#include "SceneObject.h"

namespace gameplay
{

/**
 * Defines an script component.
 */
class Script : public Component
{
	friend class SceneObject;
    friend class Serializer::Activator;
public:

    /**
     * Constructor.
     */
    Script();

    /**
     * Destructor
     */
    ~Script();
};

}
