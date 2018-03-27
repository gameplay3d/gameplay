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
    friend class Activator;
public:

    /**
     * Constructor.
     */
    Script();

    /**
     * Destructor
     */
    ~Script();    

    virtual void onInitialize();

    virtual void onFinalize();

    virtual void onUpdate(float elapsedTime);

    //virtual void onMessage(std::shared_ptr<SceneObject> sender, std::shared_ptr<Message> message, void* data);

};

}
