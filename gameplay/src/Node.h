#ifndef NODE_H_
#define NODE_H_

#include "Transform.h"
#include "Camera.h"
#include "Light.h"
#include "Model.h"
#include "AudioSource.h"
#include "ParticleEmitter.h"
#include "PhysicsRigidBody.h"
#include "BoundingBox.h"

namespace gameplay
{

class Package;
class Scene;

/**
 * Defines a basic hierachial structure of transformation spaces.
 */
class Node : public Transform, public Ref
{
    friend class Scene;
    friend class Package;

public:

    /**
     * Defines the types of nodes.
     */
    enum Type
    {
        NODE = 1,
        JOINT = 2
    };

    /**
     * Defines types of bounding volumes for nodes.
     */
    enum BoundsType
    {
        NONE,
        BOX,
        SPHERE
    };

    /**
     * Creates a new node with the specified ID.
     *
     * @param id The ID for the new node.
     */
    static Node* create(const char* id = NULL);

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
     * Returns the first child node that matches the given ID.
     *
     * This method checks the specified ID against its own ID, as well as its 
     * immediate children nodes. If recursive is true, it also traverses the
     * Node's hierarchy.
     *
     * @param id The ID of the child to find.
     * @param recursive true to search recursively all the node's children, false for only direct children.
     * @param exactMatch true if only nodes whose ID exactly matches the specified ID are returned,
     *        or false if nodes that start with the given ID are returned.
     * 
     * @return The Node found or NULL if not found.
     */
    Node* findNode(const char* id, bool recursive = true, bool exactMatch = true);

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
     */
    unsigned int findNodes(const char* id, std::vector<Node*>& nodes, bool recursive = true, bool exactMatch = true);

    /**
     * Gets the scene.
     *
     * @return The scene.
     */
    Scene* getScene() const;

    /**
     * Gets the top level node in this node's parent hierarchy.
     */
    Node* getRootNode() const;

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
     * Gets the inverse transpose world view matrix corresponding to this node.
     *
     * This matrix is typically used to transform normal vectors.
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
     */
    Vector3 getForwardVectorWorld() const;

    /**
     *  Returns the forward vector of the Node in view space.
     */
    Vector3 getForwardVectorView() const;

    /**
     * Returns the translation vector of the currently active camera for this node's scene.
     *
     * @return The translation vector of the scene's active camera.
     */
    Vector3 getActiveCameraTranslationWorld() const;

    /**
     * Returns the pointer to this node's camera.
     *
     * @return The pointer to this node's camera or NULL.
     */
    Camera* getCamera() const;

    /**
     * Assigns a camera to this node.
     * 
     * This will increase the reference count of the new camera and decrease
     * the reference count of the old camera.
     *
     * @param camera A pointer to a camera. May be NULL.
     */
    void setCamera(Camera* camera);

    /**
     * Returns the pointer to this node's light.
     *
     * @return The pointer to this node's light or NULL.
     */
    Light* getLight() const;

    /**
     * Assigns a light to this node.
     * 
     * This will increase the reference count of the new light and decrease
     * the reference count of the old light.
     *
     * @param light The new light. May be NULL.
     */
    void setLight(Light* light);

    /**
     * Returns the pointer to this node's model.
     * 
     * @return The pointer to this node's model or NULL.
     */
    Model* getModel() const;

    /**
     * Assigns a model to this node.
     * 
     * This will increase the reference count of the new model and decrease
     * the reference count of the old model.
     *
     * @param model The new model. May be NULL.
     */
    void setModel(Model* model);

    /**
     * Returns the pointer to this node's audio source.
     *
     * @return The pointer to this node's audio source or NULL.
     */
    AudioSource* getAudioSource() const;

    /**
     * Assigns an audio source to this node.
     * 
     * This will increase the reference count of the new audio source and decrease
     * the reference count of the old audio source.
     *
     * @param audio The new audio source. May be NULL.
     */
    void setAudioSource(AudioSource* audio);

    /**
     * Returns the pointer to this node's particle emitter.
     *
     * @return The pointer to this node's particle emitter or NULL.
     */
    ParticleEmitter* getParticleEmitter() const;

    /**
     * Assigns a particle emitter to this node.
     * 
     * This will increase the reference count of the new particle emitter and decrease
     * the reference count of the old particle emitter.
     *
     * @param emitter The new particle emitter. May be NULL.
     */
    void setParticleEmitter(ParticleEmitter* emitter);

    /**
     * Returns the pointer to this node's physics rigid body or NULL.
     *
     * @return The pointer to this node's physics rigid body or NULL.
     */
    PhysicsRigidBody* getPhysicsRigidBody() const;

    /**
     * Sets (or disables) the physics rigid body for this node.
     * 
     * Note: This is only allowed for nodes that have a model attached to them.
     *
     * @param type The type of rigid body to set; to disable the physics rigid
     *      body, pass PhysicsRigidBody#SHAPE_NONE.
     * @param mass The mass of the rigid body, in kilograms.
     * @param friction The friction of the rigid body (between 0.0 and 1.0, where 0.0 is
     *      minimal friction and 1.0 is maximal friction).
     * @param restitution The restitution of the rigid body (this controls the bounciness of
     *      the rigid body; between 0.0 and 1.0, where 0.0 is minimal bounciness and 1.0 is maximal bounciness).
     * @param linearDamping The percentage of linear velocity lost per second (between 0.0 and 1.0).
     * @param angularDamping The percentage of angular velocity lost per second (between 0.0 and 1.0).
     */
    void setPhysicsRigidBody(PhysicsRigidBody::Type type, float mass = 0.0f, float friction = 0.5f,
        float restitution = 0.0f, float linearDamping = 0.0f, float angularDamping = 0.0f);

    /**
     * Returns the bounding box for the Node, in world space.
     *
     * The returned box is only meaningful for nodes who have a
     * bounds type of BOUNDS_TYPE_BOX, which can be specified
     * via the setBoundsType method. Additionally, bounding volumes
     * are only meaningful for nodes that contain data which itself
     * contains a bounding volume, such as Model/Mesh.
     *
     * @return The world-space bounding box for the node.
     */
    const BoundingBox& getBoundingBox() const;

    /**
     * Returns the bounding sphere for the Node, in world space.
     *
     * The returned sphere is only meaningful for nodes who have a
     * bounds type of BOUNDS_TYPE_SPHERE, which can be specified
     * via the setBoundsType method. Additionally, bounding volumes
     * are only meaningful for nodes that contain data which itself
     * contains a bounding volume, such as Model/Mesh.
     *
     * @return The world-space bounding sphere for the node.
     */
    const BoundingSphere& getBoundingSphere() const;

    /**
     * Returns the current bounding volume type of the node.
     */
    Node::BoundsType getBoundsType() const;

    /**
     * Sets the bounding volume type of the node.
     */
    void setBoundsType(Node::BoundsType type);

protected:

    /**
     * Constructor.
     */
    Node(const char* id);

    /**
     * Copy constructor.
     */
    Node(const Node& copy);

    /**
     * Destructor.
     */
    virtual ~Node();

    /**
     * Removes this node from its parent.
     */
    void remove();

    void transformChanged();

    void hierarchyChanged();

    Scene* _scene;
    std::string _id;
    Node* _firstChild;
    Node* _nextSibling;
    Node* _prevSibling;
    Node* _parent;
    unsigned int _childCount;
    Camera* _camera;
    Light* _light;
    Model* _model;
    AudioSource* _audioSource;
    ParticleEmitter* _particleEmitter;
    PhysicsRigidBody* _physicsRigidBody;
    mutable Matrix _world;
    mutable int _dirtyBits;
    bool _notifyHierarchyChanged;
    mutable union
    {
        BoundingBox* box;
        BoundingSphere* sphere;
    } _bounds;
    BoundsType _boundsType;
};

}

#endif
