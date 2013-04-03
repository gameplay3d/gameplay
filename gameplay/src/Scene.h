#ifndef SCENE_H_
#define SCENE_H_

#include "Node.h"
#include "MeshBatch.h"
#include "ScriptController.h"
#include "Light.h"

namespace gameplay
{

/**
 * Represents the root container for a hierarchy of nodes.
 */
class Scene : public Ref
{
public:

    /**
     * Enumeration of supported scene debug flags for debug drawing.
     */
    enum DebugFlags
    {
        DEBUG_BOXES = 1,
        DEBUG_SPHERES = 2
    };

    /**
     * Creates a new empty scene.
     * 
     * @param id ID of the new scene, or NULL to use an empty string for the ID (default).
     *
     * @return The newly created empty scene.
     * @script{create}
     */
    static Scene* create(const char* id = NULL);

    /**
     * Loads a scene from the given '.scene' or '.gpb' file.
     * 
     * @param filePath The path to the '.scene' or '.gpb' file to load from.
     * @return The loaded scene or <code>NULL</code> if the scene
     *      could not be loaded from the given file.
     * @script{create}
     */
    static Scene* load(const char* filePath);

    /**
     * Gets a currently active scene.
     *
     * If id is an NULL, the first active scene is returned.
     *
     * @param id ID of the scene to retrieve, or NULL to retrieve the first active scene.
     *
     * @return The scene that matches the specified ID, or NULL if no matching scene could be found.
     */
    static Scene* getScene(const char* id = NULL);

    /**
     * Gets the identifier for the scene.
     *
     * @return The scene identifier.
     */
    const char* getId() const;

    /**
     * Sets the identifier for the scene.
     *
     * @param id The identifier to set for the scene.
     */
    void setId(const char* id);

    /**
     * Returns the first node in the scene that matches the given ID.
     *
     * @param id The ID of the node to find.
     * @param recursive true if a recursive search should be performed, false otherwise.
     * @param exactMatch true if only nodes whose ID exactly matches the specified ID are returned,
     *      or false if nodes that start with the given ID are returned.
     * 
     * @return The first node found that matches the given ID.
     */
    Node* findNode(const char* id, bool recursive = true, bool exactMatch = true) const;

    /**
     * Returns all nodes in the scene that match the given ID.
     *
     * @param id The ID of the node to find.
     * @param nodes Vector of nodes to be populated with matches.
     * @param recursive true if a recursive search should be performed, false otherwise.
     * @param exactMatch true if only nodes who's ID exactly matches the specified ID are returned,
     *      or false if nodes that start with the given ID are returned.
     * 
     * @return The number of matches found.
     * @script{ignore}
     */
    unsigned int findNodes(const char* id, std::vector<Node*>& nodes, bool recursive = true, bool exactMatch = true) const;

    /**
     * Creates and adds a new node to the scene.
     *
     * @param id An optional node ID.
     * 
     * @return The new node.
     */
    Node* addNode(const char* id = NULL);

    /**
     * Adds the specified node to the scene.
     * 
     * @param node The node to be added to the scene.
     */
    void addNode(Node* node);

    /**
     * Removes the specified node from the scene.
     *
     * @param node The node to remove.
     */
    void removeNode(Node* node);

    /**
     * Removes all nodes from the scene.
     */
    void removeAllNodes();

    /**
     * Returns the number of nodes at the root level of the scene.
     *
     * @return The node count.
     */
    unsigned int getNodeCount() const;

    /**
     * Returns the first node in the scene.
     * 
     * @return The first node in the scene.
     */
    Node* getFirstNode() const;

    /**
     * Gets the active camera for the scene.
     *
     * @return The active camera for the scene.
     */
    Camera* getActiveCamera() const;

    /**
     * Sets the active camera on the scene.
     * 
     * @param camera The active camera to be set on the scene.
     */
    void setActiveCamera(Camera* camera);

    /**
     * Sets the audio listener to transform along with the active camera if set to true.  
     * If you have a 2D game that doesn't require it, then set to false.  This is on by default for the scene.
     * 
     * @param bind true if you want to the audio listener to follow the active camera's transform.
     */
    void bindAudioListenerToCamera(bool bind);

    /**
     * Returns the ambient color of the scene.
     *
     * The default ambient light color is black (0,0,0).
     *
     * This value can be bound to materials using the SCENE_LIGHT_AMBIENT_COLOR auto binding.
     * 
     * @return The scene's ambient color.
     */
    const Vector3& getAmbientColor() const;

    /**
     * Sets the ambient color of the scene.
     * 
     * @param red The red channel between 0.0 and 1.0.
     * @param green The green channel between 0.0 and 1.0.
     * @param blue The blue channel between 0.0 and 1.0.
     *
     * @see getAmbientColor()
     */
    void setAmbientColor(float red, float green, float blue);

    /**
     * Returns the light color of the scene.
     *
     * The default light color is white (1,1,1).
     *
     * This color is typically used to represent the color of the main directional light (i.e. the Sun) in a scene.
     * The corresponding direction can be queried using Scene::getLightDirection().
     *
     * This value can be bound to materials using the SCENE_LIGHT_COLOR auto binding.
     *
     * @return The scene's light color.
     */
    const Vector3& getLightColor() const;

    /**
     * Sets the scene's light color.
     *
     * @param red The red channel between 0.0 and 1.0.
     * @param green The green channel between 0.0 and 1.0.
     * @param blue The blue channel between 0.0 and 1.0.
     *
     * @see getLightColor()
     */
    void setLightColor(float red, float green, float blue);

    /**
     * Returns the current light direction for the scene.
     *
     * The default value is (0,-1,0), which is a pointing directly down the Y-axis.
     *
     * This value is typically used to represent the the main directional lihgt (i.e. the Sun) in a scene.
     * The corresponding light color can be queried using Scene::getLightColor().
     *
     * This value can be bound to materials using the SCENE_LIGHT_DIRECTION auto binding.
     *
     * @return The scene's light direction.
     */
    const Vector3& getLightDirection() const;

    /**
     * Sets the scene's light direction.
     *
     * @param direction The new light direction.
     *
     * @see getLightDirection()
     */
    void setLightDirection(const Vector3& direction);

    /**
     * Visits each node in the scene and calls the specified method pointer.
     *
     * Calling this method invokes the specified method pointer for each node
     * in the scene hierarchy.
     *
     * The visitMethod parameter must be a pointer to a method that has a bool
     * return type and accepts a single parameter of type Node*.
     *
     * A depth-first traversal of the scene continues while the visit method
     * returns true. Returning false will stop traversing further children for
     * the given node and the traversal will continue at the next sibling.
     *
     * @param instance The pointer to an instance of the object that contains visitMethod.
     * @param visitMethod The pointer to the class method to call for each node in the scene.
     */
    template <class T>
    void visit(T* instance, bool (T::*visitMethod)(Node*));

    /**
     * Visits each node in the scene and calls the specified method pointer.
     *
     * Calling this method invokes the specified method pointer for each node
     * in the scene hierarchy, passing the Node and the specified cookie value.
     * 
     * The visitMethod parameter must be a pointer to a method that has a bool
     * return type and accepts two parameters: a Node pointer and a cookie of a
     * user-specified type.
     *
     * A depth-first traversal of the scene continues while the visit method
     * returns true. Returning false will stop traversing further children for
     * the given node and the traversal will continue at the next sibling.
     *
     * @param instance The pointer to an instance of the object that contains visitMethod.
     * @param visitMethod The pointer to the class method to call for each node in the scene.
     * @param cookie An optional user-defined parameter that will be passed to each invocation of visitMethod.
     */
    template <class T, class C>
    void visit(T* instance, bool (T::*visitMethod)(Node*,C), C cookie);

    /**
     * Visits each node in the scene and calls the specified Lua function.
     *
     * Calling this method invokes the specified Lua function for each node
     * in the scene hierarchy.
     *
     * The visitMethod parameter must be a string containing the name of a
     * valid Lua function that has a boolean return type and accepts a 
     * single parameter of type Node*.
     *
     * A depth-first traversal of the scene continues while the visit method
     * returns true. Returning false will stop traversing further children for
     * the given node and the traversal will continue at the next sibling.
     *
     * @param visitMethod The name of the Lua function to call for each node in the scene.
     */
    inline void visit(const char* visitMethod);

    /**
     * Draws debugging information (bounding volumes, etc.) for the scene.
     *
     * @param debugFlags Bitwise combination of debug flags from the DebugFlags
     *        enumeration, specifying which debugging information to draw.
     */
    void drawDebug(unsigned int debugFlags);

private:

    /**
     * Constructor.
     */
    Scene(const char* id);

    /**
     * Hidden copy constructor.
     */
    Scene(const Scene& copy);

    /**
     * Destructor.
     */
    virtual ~Scene();

    /**
     * Hidden copy assignment operator.
     */
    Scene& operator=(const Scene&);

    /**
     * Visits the given node and all of its children recursively.
     */
    template <class T>
    void visitNode(Node* node, T* instance, bool (T::*visitMethod)(Node*));

    /**
     * Visits the given node and all of its children recursively.
     */
    template <class T, class C>
    void visitNode(Node* node, T* instance, bool (T::*visitMethod)(Node*,C), C cookie);

    /**
     * Visits the given node and all of its children recursively.
     */
    void visitNode(Node* node, const char* visitMethod);

    std::string _id;
    Camera* _activeCamera;
    Node* _firstNode;
    Node* _lastNode;
    unsigned int _nodeCount;
    Vector3 _ambientColor;
    Vector3 _lightColor;
    Vector3 _lightDirection;
    bool _bindAudioListenerToCamera;
    MeshBatch* _debugBatch;
};

template <class T>
void Scene::visit(T* instance, bool (T::*visitMethod)(Node*))
{
    for (Node* node = getFirstNode(); node != NULL; node = node->getNextSibling())
    {
        visitNode(node, instance, visitMethod);
    }
}

template <class T, class C>
void Scene::visit(T* instance, bool (T::*visitMethod)(Node*,C), C cookie)
{
    for (Node* node = getFirstNode(); node != NULL; node = node->getNextSibling())
    {
        visitNode(node, instance, visitMethod, cookie);
    }
}

inline void Scene::visit(const char* visitMethod)
{
    for (Node* node = getFirstNode(); node != NULL; node = node->getNextSibling())
    {
        visitNode(node, visitMethod);
    }
}

template <class T>
void Scene::visitNode(Node* node, T* instance, bool (T::*visitMethod)(Node*))
{
    // Invoke the visit method for this node.
    if (!(instance->*visitMethod)(node))
        return;

    // If this node has a model with a mesh skin, visit the joint hierarchy within it
    // since we don't add joint hierarcies directly to the scene. If joints are never
    // visited, it's possible that nodes embedded within the joint hierarchy that contain
    // models will never get visited (and therefore never get drawn).
    if (node->_model && node->_model->_skin && node->_model->_skin->_rootNode)
    {
        visitNode(node->_model->_skin->_rootNode, instance, visitMethod);
    }

    // Recurse for all children.
    for (Node* child = node->getFirstChild(); child != NULL; child = child->getNextSibling())
    {
        visitNode(child, instance, visitMethod);
    }
}

template <class T, class C>
void Scene::visitNode(Node* node, T* instance, bool (T::*visitMethod)(Node*,C), C cookie)
{
    // Invoke the visit method for this node.
    if (!(instance->*visitMethod)(node, cookie))
        return;

    // If this node has a model with a mesh skin, visit the joint hierarchy within it
    // since we don't add joint hierarcies directly to the scene. If joints are never
    // visited, it's possible that nodes embedded within the joint hierarchy that contain
    // models will never get visited (and therefore never get drawn).
    if (node->_model && node->_model->_skin && node->_model->_skin->_rootNode)
    {
        visitNode(node->_model->_skin->_rootNode, instance, visitMethod, cookie);
    }

    // Recurse for all children.
    for (Node* child = node->getFirstChild(); child != NULL; child = child->getNextSibling())
    {
        visitNode(child, instance, visitMethod, cookie);
    }
}

}

#endif
