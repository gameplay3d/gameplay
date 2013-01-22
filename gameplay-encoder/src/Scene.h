#ifndef SCENE_H_
#define SCENE_H_

#include "Object.h"
#include "Node.h"
#include "FileIO.h"

namespace gameplay
{

class Scene : public Object
{
public:

    /**
     * Constructor.
     */
    Scene(void);

    /**
     * Destructor.
     */
    virtual ~Scene(void);

    virtual unsigned int getTypeId(void) const;
    virtual const char* getElementName(void) const;
    virtual void writeBinary(FILE* file);
    virtual void writeText(FILE* file);

    /**
     * Adds the given node as a child of this scene.
     * 
     * @param node The node to add.
     */
    void add(Node* node);

    /**
     * Sets the activate camera node. This node should contain a camera.
     */
    void setActiveCameraNode(Node* node);

    /**
     * Returns the first node the this scene that contains a camera.
     */
    Node* getFirstCameraNode() const;

    /**
     * Calculates the ambient color of this scene by combining all of the ambient lights
     * that are in this scene.
     */
    void calcAmbientColor();

    /**
     * Sets the scene's ambient color.
     */
    void setAmbientColor(float red, float green, float blue);

private:

    /**
     * Recursively calculates the ambient color of the scene starting at the given node.
     * The ambient light color is added to the given float array.
     * 
     * @param node The node in this scene to traverse from.
     * @param values Pointer to 3 floats that contains the calculated ambient color.
     */
    void calcAmbientColor(const Node* node, float* values) const;

private:
    std::list<Node*> _nodes;
    Node* _cameraNode;
    float _ambientColor[Light::COLOR_SIZE];
};

}

#endif
