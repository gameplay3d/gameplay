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
     * Renders the scene with all the nodes visible from the active camera in the scene.
     *
     * @param set The visible set of node to render
     * @param wireframe true to show the wireframe 
     * @return The number of nodes visited.
     */
    virtual unsigned int render(VisibleSet* set, bool wireframe = false) = 0;

};

}

#endif