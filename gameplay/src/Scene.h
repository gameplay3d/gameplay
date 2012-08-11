#ifndef SCENE_H_
#define SCENE_H_

#include "Node.h"
#include "MeshBatch.h"
#include "ScriptController.h"

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
     * @return The newly created empty scene.
     * @script{create}
     */
    static Scene* create();

    /**
     * Loads a scene from the given '.scene' file.
     * 
     * @param filePath The path to the '.scene' file to load from.
     * @return The loaded scene or <code>NULL</code> if the scene
     *      could not be loaded from the given file.
     * @script{create}
     */
    static Scene* load(const char* filePath);

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
     * @param exactMatch true if only nodes whos ID exactly matches the specified ID are returned,
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
     * Returns the ambient color of the scene. Black is the default color.
     * 
     * @return The ambient color of the scene.
     */
    const Vector3& getAmbientColor() const;

    /**
     * Sets the ambient color of the scene.
     * 
     * @param red The red channel between 0.0 and 1.0.
     * @param green The green channel between 0.0 and 1.0.
     * @param blue The blue channel between 0.0 and 1.0.
     */
    void setAmbientColor(float red, float green, float blue);

    /**
     * Visits each node in the scene and calls the specified method pointer.
     *
     * Calling this method invokes the specified method pointer for each node
     * in the scene hierarchy.
     *
     * The visitMethod parameter must be a pointer to a method that has a bool
     * return type and accepts a single parameter of type Node*.
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
     * user-specified type. The scene traversal continues while visitMethod return
     * true. Returning false will cause the traversal to stop.
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
     * @param visitMethod The name of the Lua function to call for each node in the scene.
     */
    inline void visit(const char* visitMethod);

    /**
     * Draws debugging information (bounding volumes, etc.) for the scene.
     *
     * @param debugFlags Bitwise combination of debug flags from mthe DebugFlags 
     *        enumeration, specifying which debugging information to draw.
     */
    void drawDebug(unsigned int debugFlags);

private:

    /**
     * Constructor.
     */
    Scene();

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
    bool visitNode(Node* node, T* instance, bool (T::*visitMethod)(Node*));

    /**
     * Visits the given node and all of its children recursively.
     */
    template <class T, class C>
    bool visitNode(Node* node, T* instance, bool (T::*visitMethod)(Node*,C), C cookie);

    /**
     * Visits the given node and all of its children recursively.
     */
    inline bool visitNode(Node* node, const char* visitMethod);

    std::string _id;
    Camera* _activeCamera;
    Node* _firstNode;
    Node* _lastNode;
    unsigned int _nodeCount;
    Vector3 _ambientColor;
    bool _bindAudioListenerToCamera;
    MeshBatch* _debugBatch;
};

template <class T>
void Scene::visit(T* instance, bool (T::*visitMethod)(Node*))
{
    for (Node* node = getFirstNode(); node != NULL; node = node->getNextSibling())
    {
        if (!visitNode(node, instance, visitMethod))
            return;
    }
}


template <class T, class C>
void Scene::visit(T* instance, bool (T::*visitMethod)(Node*,C), C cookie)
{
    for (Node* node = getFirstNode(); node != NULL; node = node->getNextSibling())
    {
        if (!visitNode(node, instance, visitMethod, cookie))
            return;
    }
}

inline void Scene::visit(const char* visitMethod)
{
    for (Node* node = getFirstNode(); node != NULL; node = node->getNextSibling())
    {
        if (!visitNode(node, visitMethod))
            return;
    }
}

template <class T>
bool Scene::visitNode(Node* node, T* instance, bool (T::*visitMethod)(Node*))
{
    // Invoke the visit method for this node.
    if (!(instance->*visitMethod)(node))
        return false;

    // Recurse for all children.
    for (Node* child = node->getFirstChild(); child != NULL; child = child->getNextSibling())
    {
        if (!visitNode(child, instance, visitMethod))
            return false;
    }

    return true;
}

template <class T, class C>
bool Scene::visitNode(Node* node, T* instance, bool (T::*visitMethod)(Node*,C), C cookie)
{
    // Invoke the visit method for this node.
    if (!(instance->*visitMethod)(node, cookie))
        return false;

    // Recurse for all children.
    for (Node* child = node->getFirstChild(); child != NULL; child = child->getNextSibling())
    {
        if (!visitNode(child, instance, visitMethod, cookie))
            return false;
    }

    return true;
}

inline bool Scene::visitNode(Node* node, const char* visitMethod)
{
    // Invoke the visit method for this node.
    if (!Game::getInstance()->getScriptController()->executeFunction<bool>(visitMethod, "<Node>", node))
        return false;

    // Recurse for all children.
    for (Node* child = node->getFirstChild(); child != NULL; child = child->getNextSibling())
    {
        if (!Game::getInstance()->getScriptController()->executeFunction<bool>(visitMethod, "<Node>", child))
            return false;
    }

    return true;
}

}

#endif
