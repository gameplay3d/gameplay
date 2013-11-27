#ifndef SCENERENDERERFORWARD_H_
#define SCENERENDERERFORWARD_H_

#include "SceneRenderer.h"

namespace gameplay
{

/**
 * Represents a class that support rendering of the active models in a scene 
 * and that are visible to the camera. 
 */
class SceneRendererForward : public SceneRenderer
{
public:

    /**
     * Creates a forward scene renderer
     */
    static SceneRendererForward* create();

    /**
     * @see SceneRenderer#render
     */
    unsigned int render(VisibleSet* set, bool wireframe);

private:

    /**
     * Constructor.
     */
    SceneRendererForward();

    /**
     * Constructor.
     */
    SceneRendererForward(const SceneRendererForward& copy);

    /**
     * Destructor.
     */
    ~SceneRendererForward();

    unsigned int drawNode(Node* node, bool wireframe);
};

}

#endif