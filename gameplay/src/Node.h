#ifndef NODE_H_
#define NODE_H_

#include "Transform.h"
#include "ScriptTarget.h"
#include "Model.h"
#include "Sprite.h"
#include "TileSet.h"
#include "Text.h"
#include "Form.h"
#include "ParticleEmitter.h"
#include "Camera.h"
#include "Light.h"
#include "PhysicsRigidBody.h"
#include "PhysicsCollisionObject.h"
#include "BoundingBox.h"
#include "AIAgent.h"

namespace gameplay
{

class Scene;
class Camera;
class Light;
class AudioSource;
class AIAgent;
class Drawable;

/**
 * Defines a hierarchical structure of objects in 3D transformation spaces.
 *
 * This object allow you to attach components to a scene such as:
 * Drawable's(Model, Camera, Light, PhysicsCollisionObject, AudioSource, etc.
 *
 * @see http://gameplay3d.github.io/GamePlay/docs/file-formats.html#wiki-Node
 */
class Node : public Transform, public Ref
{
    friend class Scene;
    friend class SceneLoader;
    friend class Bundle;
    friend class MeshSkin;
    friend class Light;

    GP_SCRIPT_EVENTS_START();
    GP_SCRIPT_EVENT(update, "<Node>f");
    GP_SCRIPT_EVENT(messageReceived, "<Node><AIMessage>");
    GP_SCRIPT_EVENT(stateEnter, "<Node><AIState>");
    GP_SCRIPT_EVENT(stateExit, "<Node><AIState>");
    GP_SCRIPT_EVENT(stateUpdate, "<Node><AIState>f");
    GP_SCRIPT_EVENTS_END();

public:

    /**
     * Defines the types of nodes.
     */
    enum Type
    {
        NODE = 1,
        JOINT
    };

    /**
     * Creates a new node with the specified ID.
     *
     * @param id The ID for the new node.
     * @script{create}
     */
    static Node* create(const char* id = NULL);

    /**
     * Extends ScriptTarget::getTypeName() to return the type name of this class.
     *
     * @return The type name of this class: "Node"
     * @see ScriptTarget::getTypeName()
     */
    const char* getTypeName() const;

    /**
     * Gets the identifier for the node.
     *
     * @return The node identifier.
     */
    const char* getId() const;

    /**
     * Sets the identifier for the node.
     *
     * @param id The identifier to set for the node.
     */
    void setId(const char* id);

    /**
     * Returns the type of the node.
     */
    virtual Node::Type getType() const;

    /**
     * Adds a child node.
     *
     * @param child The child to add.
     */
    virtual void addChild(Node* child);

    /**
     * Removes a child node.
     *
     * @param child The child to remove.
     */
    virtual void removeChild(Node* child);

    /**
     * Removes all child nodes.
     */
    virtual void removeAllChildren();

    /**
     * Returns the first child for this node.
     *
     * @return The first child.
     */
    Node* getFirstChild() const;

    /**
     * Returns the first sibling of this node.
     *
     * @return The first sibling.
     */
    Node* getNextSibling() const;

    /**
     * Returns the previous sibling to this node.
     *
     * @return The previous sibling.
     */
    Node* getPreviousSibling() const;

    /**
     * Returns the parent of this node.
     *
     * @return The parent.
     */
    Node* getParent() const;

    /**
     * Returns the number of direct children of this item.
     *
     * @return The number of children.
     */
    unsigned int getChildCount() const;

    /**
     * Gets the top level node in this node's parent hierarchy.
     */
    Node* getRootNode() const;

    /**
     * Returns the first child node that matches the given ID.
     *
     * This method checks the specified ID against its immediate child nodes
     * but does not check the ID against itself.
     * If recursive is true, it also traverses the Node's hierarchy with a breadth first search.
     *
     * @param id The ID of the child to find.
     * @param recursive True to search recursively all the node's children, false for only direct children.
     * @param exactMatch true if only nodes whose ID exactly matches the specified ID are returned,
     *        or false if nodes that start with the given ID are returned.
     *
     * @return The Node found or NULL if not found.
     */
    Node* findNode(const char* id, bool recursive = true, bool exactMatch = true) const;

    /**
     * Returns all child nodes that match the given ID.
     *
     * @param id The ID of the node to find.
     * @param nodes A vector of nodes to be populated with matches.
     * @param recursive true if a recursive search should be performed, false otherwise.
     * @param exactMatch true if only nodes whose ID exactly matches the specified ID are returned,
     *        or false if nodes that start with the given ID are returned.
     *
     * @return The number of matches found.
     * @script{ignore}
     */
    unsigned int findNodes(const char* id, std::vector<Node*>& nodes, bool recursive = true, bool exactMatch = true) const;

    /**
     * Gets the scene this node is currenlty within.
     *
     * @return The scene.
     */
    virtual Scene* getScene() const;

    /**
     * Sets a tag on this Node.
     *
     * tags can be used for a variety of purposes within a game. For example,
     * a tag called "transparent" can be added to nodes, to indicate which nodes in
     * a scene are transparent. This tag can then be read during rendering to sort
     * transparent and opaque objects for correct drawing order.
     *
     * Setting a tag to NULL removes the tag from the Node.
     *
     * @param name Name of the tag to set.
     * @param value Optional value of the tag (empty string by default).
     */
    void setTag(const char* name, const char* value = "");

    /**
     * Returns the value of the custom tag with the given name.
     *
     * @param name Name of the tag to return.
     *
     * @return The value of the given tag, or NULL if the tag is not set.
     */
    const char* getTag(const char* name) const;

    /**
     * Determines if a custom tag with the specified name is set.
     *
     * @param name Name of the tag to query.
     *
     * @return true if the tag is set, false otherwise.
     */
    bool hasTag(const char* name) const;

    /**
     * Sets if the node is enabled in the scene.
     *
     * @param enabled if the node is enabled in the scene.
     */
    void setEnabled(bool enabled);

    /**
     * Gets if the node is enabled in the scene.
     *
     * @return if the node is enabled in the scene.
     */
    bool isEnabled() const;

    /**
     * Gets if the node inherently enabled.
     *
     * @return if components attached on this node should be running.
     */
    bool isEnabledInHierarchy() const;

    /**
     * Called to update the state of this Node.
     *
     * This method is called by Scene::update(float) to update the state of all active
     * nodes in a scene. A Node is considered active if Node::isActive() returns true.
     *
     * If any scripts are attached to the node, their update event will be fired.
     *
     * @param elapsedTime Elapsed time in milliseconds.
     */
    void update(float elapsedTime);

    /**
     * Returns whether the transformation of this node is static.
     *
     * Nodes that have static rigid bodies attached to them are considered static.
     *
     * @return True if the transformation of this Node is static, false otherwise.
     *
     * @see Transform::isStatic()
     */
    bool isStatic() const;

    /**
     * Gets the world matrix corresponding to this node.
     *
     * @return The world matrix of this node.
     */
    virtual const Matrix& getWorldMatrix() const;

    /**
     * Gets the world view matrix corresponding to this node.
     *
     * @return The world view matrix of this node.
     */
    const Matrix& getWorldViewMatrix() const;

    /**
     * Gets the inverse transpose world matrix corresponding to this node.
     *
     * This matrix is typically used to transform normal vectors into world space.
     *
     * @return The inverse world matrix of this node.
     */
    const Matrix& getInverseTransposeWorldMatrix() const;

    /**
     * Gets the inverse transpose world view matrix corresponding to this node.
     *
     * This matrix is typically used to transform normal vectors into view space.
     *
     * @return The inverse world view matrix of this node.
     */
    const Matrix& getInverseTransposeWorldViewMatrix() const;

    /**
     * Gets the view matrix corresponding to this node based
     * on the scene's active camera.
     *
     * @return The view matrix of this node.
     */
    const Matrix& getViewMatrix() const;

    /**
     * Gets the inverse view matrix corresponding to this node based
     * on the scene's active camera.
     *
     * @return The inverse view matrix of this node.
     */
    const Matrix& getInverseViewMatrix() const;

    /**
     * Gets the projection matrix corresponding to this node based
     * on the scene's active camera.
     *
     * @return The projection matrix of this node.
     */
    const Matrix& getProjectionMatrix() const;

    /**
     * Gets the view * projection matrix corresponding to this node based
     * on the scene's active camera.
     *
     * @return The view * projection matrix of this node.
     */
    const Matrix& getViewProjectionMatrix() const;

    /**
     * Gets the inverse view * projection matrix corresponding to this node based
     * on the scene's active camera.
     *
     * @return The inverse view * projection matrix of this node.
     */
    const Matrix& getInverseViewProjectionMatrix() const;

    /**
     * Gets the world * view * projection matrix corresponding to this node based
     * on the scene's active camera.
     *
     * @return The world * view * projection matrix of this node.
     */
    const Matrix& getWorldViewProjectionMatrix() const;

    /**
     * Gets the translation vector (or position) of this Node in world space.
     *
     * @return The world translation vector.
     */
    Vector3 getTranslationWorld() const;

    /**
     * Gets the translation vector (or position) of this Node in view space.
     *
     * @return The view space translation vector.
     */
    Vector3 getTranslationView() const;

    /**
     * Returns the forward vector of the Node in world space.
     *
     * @return The forward vector in world space.
     */
    Vector3 getForwardVectorWorld() const;

    /**
     * Returns the forward vector of the Node in view space.
     *
     * @return The forward vector in view space.
     */
    Vector3 getForwardVectorView() const;

    /**
     * Returns the right vector of the Node in world space.
     *
     * @return The right vector in world space.
     */
    Vector3 getRightVectorWorld() const;

    /**
     * Returns the up vector of the Node in world space.
     *
     * @return The up vector in world space.
     */
    Vector3 getUpVectorWorld() const;

    /**
     * Returns the translation vector of the currently active camera for this node's scene.
     *
     * @return The translation vector of the scene's active camera.
     */
    Vector3 getActiveCameraTranslationWorld() const;

    /**
     * Returns the view-space translation vector of the currently active camera for this node's scene.
     *
     * @return The translation vector of the scene's active camera, in view-space.
     */
    Vector3 getActiveCameraTranslationView() const;

    /**
     * Gets the first animation in the node hierarchy with the specified ID.
     *
     * @param id The ID of the animation to get. Returns the first animation if ID is NULL.
     * @return The first animation with the specified ID.
     */
    Animation* getAnimation(const char* id = NULL) const;

    /**
     * Gets the drawable object attached to this node.
     *
     * @return The drawable component attached to this node.
     */
    Drawable* getDrawable() const;

    /**
     * Set the drawable object to be attached to this node
     *
     * This is typically a Model, ParticleEmiiter, Form, Terrrain, Sprite, TileSet or Text.
     *
     * This will increase the reference count of the new drawble and decrease
     * the reference count of the old drawable.
     *
     * @param drawable The new drawable component. May be NULL.
     */
    void setDrawable(Drawable* drawable);

    /**
     * Gets the camera attached to this node.
     *
     * @return Gets the camera attached to this node.
     */
    Camera* getCamera() const;

    /**
     * Attaches a camera to this node.
     *
     * This will increase the reference count of the new camera and decrease
     * the reference count of the old camera.
     *
     * @param camera The new camera. May be NULL.
     */
    void setCamera(Camera* camera);

    /**
     * Get the light attached to this node.
     *
     * @return The light attached to this node.
     */
    Light* getLight() const;

    /**
     * Attaches a light to this node.
     *
     * This will increase the reference count of the new light and decrease
     * the reference count of the old light.
     *
     * @param light The new light. May be NULL.
     */
    void setLight(Light* light);

    /**
     * Gets the audio source attached to this node.
     *
     * @return The audio source attached to this node.
     */
    AudioSource* getAudioSource() const;

    /**
     * Attaches an audio source to this node.
     *
     * This will increase the reference count of the new audio source and decrease
     * the reference count of the old audio source.
     *
     * @param audio The new audio source. May be NULL.
     */
    void setAudioSource(AudioSource* audio);

    /**
     * Returns the pointer to this node's physics collision object.
     *
     * The type of the returned collision object can be queried using
     * the PhysicsCollisionObject::getType() method.
     *
     * @return The pointer to this node's physics collision object.
     */
    PhysicsCollisionObject* getCollisionObject() const;

    /**
     * Sets (or disables) the physics collision object for this node.
     *
     * The supported collision object types include rigid bodies, ghost objects,
     * characters, vehicles, and vehicle wheels.
     *
     * Rigid bodies are used to represent most physical objects in a game. The important
     * feature of rigid bodies is that they can be simulated by the physics system as other
     * rigid bodies or collision objects collide with them. To support this physics simulation,
     * rigid bodies require additional parameters, such as mass, friction and restitution to
     * define their physical features. These parameters can be passed into the
     * 'rigidBodyParameters' parameter.
     *
     * Vehicles consist of a rigid body with wheels. The rigid body parameters can be passed-in
     * via the 'rigidBodyParameters' parameter, and wheels can be added to the vehicle.
     *
     * Ghost objects are a simple type of collision object that are not simulated. By default
     * they pass through other objects in the scene without affecting them. Ghost objects do
     * receive collision events however, which makes them useful for representing non-simulated
     * entities in a game that still require collision events, such as volumetric triggers,
     * power-ups, etc.
     *
     * Characters are an extension of ghost objects which provide a number of additional features
     * for animating and moving characters within a game. Characters are represented as ghost
     * objects instead of rigid bodies to allow more direct control over character movement,
     * since attempting to model a physics character with a simulated rigid body usually results
     * in unresponsive and unpredictable character movement. Unlike normal ghost objects,
     * characters to react to other characters and rigid bodies in the world. Characters react
     * to gravity and collide (and respond) with rigid bodies to allow them to walk on the ground,
     * slide along walls and walk up/down slopes and stairs.
     *
     * @param type The type of the collision object to set; to disable the physics
     *        collision object, pass PhysicsCollisionObject::NONE.
     * @param shape Definition of a physics collision shape to be used for this collision object.
     *        Use the static shape methods on the PhysicsCollisionShape class to specify a shape
     *        definition, such as PhysicsCollisionShape::box().
     * @param rigidBodyParameters If type is PhysicsCollisionObject::RIGID_BODY or
     *        PhysicsCollisionObject::VEHICLE, this must point to a valid rigid body
     *        parameters object containing information about the rigid body;
     *        otherwise, this parameter may be NULL.
     * @param group Group identifier of the object for collision filtering.
     * @param mask Bitmask to filter groups of objects to collide with this one.
     */
    PhysicsCollisionObject* setCollisionObject(PhysicsCollisionObject::Type type,
                                               const PhysicsCollisionShape::Definition& shape = PhysicsCollisionShape::box(),
                                               PhysicsRigidBody::Parameters* rigidBodyParameters = NULL,
                                               int group = PHYSICS_COLLISION_GROUP_DEFAULT,
                                               int mask = PHYSICS_COLLISION_MASK_DEFAULT);
    /**
     * Sets the physics collision object for this node using the data from the Properties object defined at the specified URL,
     * where the URL is of the format "<file-path>.<extension>#<namespace-id>/<namespace-id>/.../<namespace-id>"
     * (and "#<namespace-id>/<namespace-id>/.../<namespace-id>" is optional).
     *
     * @param url The URL pointing to the Properties object defining the physics collision object.
     */
    PhysicsCollisionObject* setCollisionObject(const char* url);

    /**
     * Gets the AI agent assigned to this node
     *
     * @return The AI agent for this node.
     */
    AIAgent* getAgent() const;

    /**
     * Sets the AI agent for this node.
     *
     * @param agent The AI agent to set.
     */
    void setAgent(AIAgent* agent);

    /**
     * Gets the user object assigned to this node.
     *
     * @return The user object assigned object to this node.
     */
    Ref* getUserObject() const;

    /**
    * Sets a user object to be assigned object to this node.
    *
    * @param obj The user object assigned object to this node.
    */
    void setUserObject(Ref* obj);

    /**
     * Returns the bounding sphere for the Node, in world space.
     *
     * The bounding sphere for a node represents the area, in world
     * space, that the node contains. This includes the space occupied
     * by any child nodes as well as the space occupied by any data
     * inside the node (such as models).
     *
     * Bounding spheres for nodes are rough approximations of the data
     * contained within a node and they are intended for visibility
     * testing or first-pass intersection testing only. They are not
     * appropriate for accurate collision detection since they most often
     * do not tightly contain a node's content.
     *
     * A node that does not occupy any space will return a bounding sphere
     * with a center point equal to the node translation and a radius of zero.
     *
     * @return The world-space bounding sphere for the node.
     */
    const BoundingSphere& getBoundingSphere() const;

    /**
     * Clones the node and all of its child nodes.
     *
     * @return A new node.
     * @script{create}
     */
    Node* clone() const;

protected:

    /**
     * Constructor.
     */
    Node(const char* id);

    /**
     * Destructor.
     */
    virtual ~Node();

    /**
     * Clones a single node and its data but not its children.
     *
     * @param context The clone context.
     *
     * @return Pointer to the newly created node.
     */
    virtual Node* cloneSingleNode(NodeCloneContext &context) const;

    /**
     * Recursively clones this node and its children.
     *
     * @param context The clone context.
     *
     * @return The newly created node.
     */
    Node* cloneRecursive(NodeCloneContext &context) const;

    /**
     * Copies the data from this node into the given node.
     *
     * @param node The node to copy the data to.
     * @param context The clone context.
     */
    void cloneInto(Node* node, NodeCloneContext &context) const;

    /**
     * Removes this node from its parent.
     */
    void remove();

    /**
     * Called when this Node's transform changes.
     */
    void transformChanged();

    /**
     * Called when this Node's hierarchy changes.
     */
    void hierarchyChanged();

    /**
     * Marks the bounding volume of the node as dirty.
     */
    void setBoundsDirty();

    /**
     * Returns the first child node that matches the given ID.
     *
     * This method checks the specified ID against its immediate child nodes
     * but does not check the ID against itself.
     * If recursive is true, it also traverses the Node's hierarchy with a breadth first search.
     *
     * @param id The ID of the child to find.
     * @param recursive True to search recursively all the node's children, false for only direct children.
     * @param exactMatch true if only nodes whose ID exactly matches the specified ID are returned,
     *        or false if nodes that start with the given ID are returned.
     * @param skipSkin Set true to skip skin hierarchy, initial find may set false to include skin hierarchy.
     *
     * @return The Node found or NULL if not found.
     */
    Node* findNode(const char* id, bool recursive, bool exactMatch, bool skipSkin) const;


    /**
     * Returns all child nodes that match the given ID.
     *
     * @param id The ID of the node to find.
     * @param nodes A vector of nodes to be populated with matches.
     * @param recursive true if a recursive search should be performed, false otherwise.
     * @param exactMatch true if only nodes whose ID exactly matches the specified ID are returned,
     *        or false if nodes that start with the given ID are returned.
     * @param skipSkin Set true to skip skin hierarchy, initial find may set false to include skin hierarchy.
     * 
     * @return The number of matches found.
     * @script{ignore}
     */
    unsigned int findNodes(const char* id, std::vector<Node*>& nodes, bool recursive, bool exactMatch, bool skipSkin) const;

private:

    /**
     * Hidden copy constructor.
     */
    Node(const Node& copy);

    /**
     * Hidden copy assignment operator.
     */
    Node& operator=(const Node&);

    PhysicsCollisionObject* setCollisionObject(Properties* properties);

protected:

    /** The scene this node is attached to. */
    Scene* _scene;
    /** The nodes id. */
    std::string _id;
    /** The nodes first child. */
    Node* _firstChild;
    /** The nodes next sibiling. */
    Node* _nextSibling;
    /** The nodes previous sibiling. */
    Node* _prevSibling;
    /** The nodes parent. */
    Node* _parent;
    /** The number of child nodes. */
    unsigned int _childCount;
    /** If this node is enabled. Maybe different if parent is enabled/disabled. */
    bool _enabled; 
    /** Tags assigned to this node. */
    std::map<std::string, std::string>* _tags;
    /** The drawble component attached to this node. */
    Drawable* _drawable;
    /** The camera component attached to this node. */
    Camera* _camera;
    /** The light component attached to this node. */
    Light* _light;
    /** The audio source component attached to this node. */
    AudioSource* _audioSource;
    /** The collision object component attached to this node. */
    PhysicsCollisionObject* _collisionObject;
    /** The AI agent component attached to this node. */
    mutable AIAgent* _agent;
    /** The user object component attached to this node. */
    Ref* _userObject;
    /** The world matrix for this node. */
    mutable Matrix _world;
    /** The bounding sphere for this node. */
    mutable BoundingSphere _bounds;
    /** The dirty bits used for optimization. */
    mutable int _dirtyBits;
};

/**
 * NodeCloneContext represents the context data that is kept when cloning a node.
 * The NodeCloneContext is used to make sure objects don't get cloned twice.
 */
class NodeCloneContext
{
public:

    /**
     * Constructor.
     */
    NodeCloneContext();

    /**
     * Destructor.
     */
    ~NodeCloneContext();

    /**
     * Finds the cloned animation of the given animation or NULL if this animation was not registered with this context.
     *
     * @param animation The animation to search for the cloned copy of.
     *
     * @return The cloned animation or NULL if not found.
     */
    Animation* findClonedAnimation(const Animation* animation);

    /**
     * Registers the cloned animation with this context so that it doesn't get cloned twice.
     *
     * @param original The pointer to the original animation.
     * @param clone The pointer to the cloned animation.
     */
    void registerClonedAnimation(const Animation* original, Animation* clone);

    /**
     * Finds the cloned node of the given node or NULL if this node was not registered with this context.
     *
     * @param node The node to search for the cloned copy of.
     *
     * @return The cloned node or NULL if not found.
     */
    Node* findClonedNode(const Node* node);

    /**
     * Registers the cloned node with this context so that it doens't get cloned twice.
     *
     * @param original The pointer to the original node.
     * @param clone The pointer to the cloned node.
     */
    void registerClonedNode(const Node* original, Node* clone);

private:

    /**
     * Hidden copy constructor.
     */
    NodeCloneContext(const NodeCloneContext&);

    /**
     * Hidden copy assignment operator.
     */
    NodeCloneContext& operator=(const NodeCloneContext&);

    std::map<const Animation*, Animation*> _clonedAnimations;
    std::map<const Node*, Node*> _clonedNodes;
};

}

#endif
