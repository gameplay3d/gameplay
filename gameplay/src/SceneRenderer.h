#ifndef SCENERENDERER_H_
#define SCENERENDERER_H_

#include "Scene.h"
#include "VisibleSet.h"

namespace gameplay
{

/**
 * Represents a class that support rendering of the active models in a scene 
 * and that are visible to the camera. 
 */
class SceneRenderer
{
public:

    /**
     * Constructor.
     *
     * @param scene The scene to render.
     * @param lighting The type of lighting system to use.
     */
    SceneRenderer();

    /**
     * Destructor.
     */
    virtual ~SceneRenderer();

    /**
     * Sets if the scene should be rendered in wireframe mode.
     *
     * @param wireframe if the scene should be rendered in wireframe mode.
     */
    void setWireframe(bool wireframe);

    /**
     * Determines if the scene should be rendered in wireframe mode.
     *
     * @return if the scene should be rendered in wireframe mode.
     */
    bool isWireframe() const;

    /**
     * Renders the scene with all the nodes visible from the active camera in the scene.
     *
     * @param scene
     * param
     * @return The number of nodes visited.
     */
    virtual unsigned int render(VisibleSet* set) = 0;

protected:

    bool _wireframe;

};

}

#endif