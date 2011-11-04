/*
 * Node.cpp
 */

#include "Base.h"
#include "Node.h"
#include "Scene.h"

#define NODE_DIRTY_WORLD 1
#define NODE_DIRTY_WORLD_VIEW_PROJ 2
#define NODE_DIRTY_BOUNDS 4
#define NODE_DIRTY_ALL (NODE_DIRTY_WORLD | NODE_DIRTY_WORLD_VIEW_PROJ | NODE_DIRTY_BOUNDS)

namespace gameplay
{

Node::Node(const char* id)
    : _scene(NULL), _firstChild(NULL), _nextSibling(NULL), _prevSibling(NULL), _parent(NULL), _childCount(NULL),
    _camera(NULL), _light(NULL), _model(NULL), _audioSource(NULL), _particleEmitter(NULL), _physicsRigidBody(NULL), 
    _dirtyBits(NODE_DIRTY_ALL), _notifyHierarchyChanged(true), _boundsType(NONE)
{
    if (id)
    {
        _id = id;
    }

    memset(&_bounds, 0, sizeof(_bounds));
}

Node::Node(const Node& node)
{
    // hidden
}

Node::~Node()
{
    removeAllChildren();

    // Free bounding volume.
    switch (_boundsType)
    {
    case BOX:
        SAFE_DELETE(_bounds.box);
        break;
    case SPHERE:
        SAFE_DELETE(_bounds.sphere);
        break;
    }

    SAFE_RELEASE(_camera);
    SAFE_RELEASE(_light);
    SAFE_RELEASE(_model);
    SAFE_RELEASE(_audioSource);
    SAFE_RELEASE(_particleEmitter);
}

Node* Node::create(const char* id)
{
    return new Node(id);
}

const char* Node::getId() const
{
    return _id.c_str();
}

void Node::setId(const char* id)
{
    if (id)
    {
        _id = id;
    }
}

Node::Type Node::getType() const
{
    return Node::NODE;
}

void Node::addChild(Node* child)
{
    assert(child);

    // If the item belongs to another hierarchy, remove it first.
    Node* parent = child->_parent;
    if (parent)
    {
        parent->removeChild(child);
    }

    // Order is irrelevant, so add to the beginning of the list.
    if (_firstChild)
    {
        _firstChild->_prevSibling = child;
        child->_nextSibling = _firstChild;
        _firstChild = child;
    }
    else
    {
        _firstChild = child;
    }

    child->_parent = this;

    ++_childCount;

    // Fire events.
    child->parentChanged(parent);
    childAdded(child);
}

void Node::removeChild(Node* child)
{
    if (child == NULL || child->_parent != this)
    {
        // The child is not in our hierarchy.
        return;
    }

    // Call remove on the child.
    child->remove();
}

void Node::removeAllChildren()
{
    _notifyHierarchyChanged = false;

    while (_firstChild)
    {
        removeChild(_firstChild);
    }

    _notifyHierarchyChanged = true;
    hierarchyChanged();
}

void Node   ::remove()
{
    // Re-link our neighbours.
    if (_prevSibling)
    {
        _prevSibling->_nextSibling = _nextSibling;
    }
    if (_nextSibling)
    {
        _nextSibling->_prevSibling = _prevSibling;
    }

    // Update our parent.
    Node* parent = _parent;
    if (parent)
    {
        if (this == parent->_firstChild)
        {
            parent->_firstChild = _nextSibling;
        }

        --parent->_childCount;
    }

    _nextSibling = NULL;
    _prevSibling = NULL;
    _parent = NULL;

    // Fire events.
    if (parent)
    {
        parentChanged(parent);
        parent->childRemoved(this);
    }
}

Node* Node::getFirstChild() const
{
    return _firstChild;
}

Node* Node::getNextSibling() const
{
    return _nextSibling;
}

Node* Node::getPreviousSibling() const
{
    return _prevSibling;
}

Node* Node::getParent() const
{
    return _parent;
}

unsigned int Node::getChildCount() const
{
    return _childCount;
}

Node* Node::findNode(const char* id, bool recursive, bool exactMatch)
{
    assert(id);
    
    // Search immediate children first.
    for (Node* child = getFirstChild(); child != NULL; child = child->getNextSibling())
    {
        // Does this child's ID match?
        if ((exactMatch && child->_id == id) || (!exactMatch && child->_id.find(id) == 0))
        {
            return child;
        }
    }

    // Recurse.
    if (recursive)
    {
        for (Node* child = getFirstChild(); child != NULL; child = child->getNextSibling())
        {
            Node* match = child->findNode(id, true, exactMatch);
            if (match)
            {
                return match;
            }
        }
    }

    return NULL;
}   

unsigned int Node::findNodes(const char* id, std::vector<Node*>& nodes, bool recursive, bool exactMatch)
{
    assert(id);
    
    unsigned int count = 0;

    // Search immediate children first.
    for (Node* child = getFirstChild(); child != NULL; child = child->getNextSibling())
    {
        // Does this child's ID match?
        if ((exactMatch && child->_id == id) || (!exactMatch && child->_id.find(id) == 0))
        {
            nodes.push_back(child);
            ++count;
        }
    }

    // Recurse.
    if (recursive)
    {
        for (Node* child = getFirstChild(); child != NULL; child = child->getNextSibling())
        {
            count += child->findNodes(id, nodes, true, exactMatch);
        }
    }

    return count;
}

Scene* Node::getScene() const
{
    // Search for a scene in our parents.
    for (Node* n = const_cast<Node*>(this); n != NULL; n = n->getParent())
    {
        if (n->_scene)
        {
            return n->_scene;
        }
    }

    return NULL;
}

Node* Node::getRootNode() const
{
    Node* n = const_cast<Node*>(this);
    while (n->getParent())
    {
        n = n->getParent();
    }
    return n;
}

const Matrix& Node::getWorldMatrix() const
{
    if (_dirtyBits & NODE_DIRTY_WORLD)
    {
        // Clear our dirty flag immediately to prevent this block from being entered if our
        // parent calls our getWorldMatrix() method as a result of the following calculations.
        _dirtyBits &= ~NODE_DIRTY_WORLD;

        // If we have a parent, multiply our parent world transform by our local
        // transform to obtain our final resolved world transform.
        Node* parent = getParent();
        if (parent && !_physicsRigidBody)
        {
            Matrix::multiply(parent->getWorldMatrix(), getMatrix(), &_world);
        }
        else
        {
            _world = getMatrix();
        }

        // Our world matrix was just updated, so call getWorldMatrix() on all child nodes
        // to force their resolved world matrices to be updated.
        Node* node = getFirstChild();
        while (node)
        {
            node->getWorldMatrix();
            node = node->getNextSibling();
        }
    }

    return _world;
}

const Matrix& Node::getWorldViewMatrix() const
{
    static Matrix worldView;
    
    Matrix::multiply(getViewMatrix(), getWorldMatrix(), &worldView);

    return worldView;
}

const Matrix& Node::getInverseTransposeWorldViewMatrix() const
{
    static Matrix invTransWorldView;

    // Assume the matrix is always dirty since the camera is moving
    // almost every frame in most games.
    //    
    // TODO: Optimize here to NOT calculate the inverse transpose if the matrix is orthogonal.
    Matrix::multiply(getViewMatrix(), getWorldMatrix(), &invTransWorldView);
    invTransWorldView.invert();
    invTransWorldView.transpose();

    return invTransWorldView;
}

const Matrix& Node::getViewMatrix() const
{
    Scene* scene = getScene();
    Camera* camera = scene ? scene->getActiveCamera() : NULL;
    if (camera)
    {
        return camera->getViewMatrix();
    }
    else
    {
        return Matrix::identity();
    }
}

const Matrix& Node::getInverseViewMatrix() const
{
    Scene* scene = getScene();
    Camera* camera = scene ? scene->getActiveCamera() : NULL;
    if (camera)
    {
        return camera->getInverseViewMatrix();
    }
    else
    {
        return Matrix::identity();
    }
}

const Matrix& Node::getViewProjectionMatrix() const
{
    Scene* scene = getScene();
    Camera* camera = scene ? scene->getActiveCamera() : NULL;
    if (camera)
    {
        return camera->getViewProjectionMatrix();
    }
    else
    {
        return Matrix::identity();
    }
}

const Matrix& Node::getInverseViewProjectionMatrix() const
{
    Scene* scene = getScene();
    Camera* camera = scene ? scene->getActiveCamera() : NULL;
    if (camera)
    {
        return camera->getInverseViewProjectionMatrix();
    }

    return Matrix::identity();
}

const Matrix& Node::getWorldViewProjectionMatrix() const
{
    static Matrix worldViewProj;

    //if (_dirtyBits & NODE_DIRTY_WORLD_VIEW_PROJ)
    //{
        // Note that currently we always keep the NODE_DIRTY_WORLD_VIEW_PROJ dirty bit set.
        // In order to correctly track whether the WorldViewProjection matrix is dirty, it
        // would be neccessary to track changes to the camera's view matrix, which would 
        // result in a large number of events/scene traversals each time the camera
        // was transformed.
        // 
        // A possible trick we can implement here would be to store a "dirty frame" in
        // the camera, which is updated to the current frame number whenever the camera
        // is modified. This method could then compare the camera's dirty frame to the 
        // current frame number - if they are the same, it means the camera was changed
        // in the current frame and our view/projection matrices are dirty.
        //
        //_dirtyBits &= ~NODE_DIRTY_WORLD_VIEW_PROJ;

        Matrix::multiply(getViewProjectionMatrix(), getWorldMatrix(), &worldViewProj);
    //}

    return worldViewProj;
}

Vector3 Node::getWorldTranslation() const
{
    Vector3 translation;
    getWorldMatrix().getTranslation(&translation);
    return translation;
}

Vector3 Node::getActiveCameraTranslation() const
{
    Scene* scene = getScene();
    if (scene)
    {
        Camera* camera = scene->getActiveCamera();
        if (camera)
        {
            Node* cameraNode = camera->getNode();
            if (cameraNode)
            {
                return cameraNode->getWorldTranslation();
            }
        }
    }

    return Vector3::zero();
}

void Node::hierarchyChanged()
{
    // When our hierarchy changes our world transform is affected, so we must dirty it.
    transformChanged();
}

void Node::transformChanged()
{
    // Our local transform was changed, so mark our world matrices dirty.
    _dirtyBits |= NODE_DIRTY_WORLD | NODE_DIRTY_WORLD_VIEW_PROJ | NODE_DIRTY_BOUNDS;

    // Notify our children that their transform has also changed (since transforms are inherited).
    Node* n = getFirstChild();
    while (n)
    {
        n->transformChanged();
        n = n->getNextSibling();
    }

    Transform::transformChanged();
}

Camera* Node::getCamera() const
{
    return _camera;
}

void Node::setCamera(Camera* camera)
{
    if (_camera != camera)
    {
        if (_camera)
        {
            _camera->setNode(NULL);
            SAFE_RELEASE(_camera);
        }

        _camera = camera;

        if (_camera)
        {
            _camera->addRef();
            _camera->setNode(this);
        }
    }
}

Light* Node::getLight() const
{
    return _light;
}

void Node::setLight(Light* light)
{
    if (_light != light)
    {
        if (_light)
        {
            _light->setNode(NULL);
            SAFE_RELEASE(_light);
        }
        
        _light = light;

        if (_light)
        {
            _light->addRef();
            _light->setNode(this);
        }
    }
}

void Node::setModel(Model* model)
{
    if (_model != model)
    {
        if (_model)
        {
            _model->setNode(NULL);
            SAFE_RELEASE(_model);
        }

        _model = model;

        if (_model)
        {
            _model->addRef();
            _model->setNode(this);
        }
    }
}

Model* Node::getModel() const
{
    return _model;
}

const BoundingBox& Node::getBoundingBox() const
{
    if (_boundsType != BOX)
    {
        return BoundingBox::empty();
    }

    if (_dirtyBits & NODE_DIRTY_BOUNDS)
    {
        _dirtyBits &= ~NODE_DIRTY_BOUNDS;

        if (_model && _model->getMesh())
        {
            // Use the bounding volume of our model's mesh.
            Mesh* mesh = _model->getMesh();
            _bounds.box->set(mesh->getBoundingBox());
        }
        else
        {
            _bounds.box->set(Vector3::zero(), Vector3::zero());
        }

        bool empty = _bounds.box->isEmpty();
        if (!empty)
        {
            // Transform the box into world space.
            _bounds.box->transform(getWorldMatrix());
        }

        // Merge this world-space bounding box with our childrens' bounding volumes.
        for (Node* n = getFirstChild(); n != NULL; n = n->getNextSibling())
        {
            switch (n->_boundsType)
            {
            case BOX:
                {
                    const BoundingBox& childBox = n->getBoundingBox();
                    if (!childBox.isEmpty())
                    {
                        if (empty)
                        {
                            _bounds.box->set(childBox);
                            empty = false;
                        }
                        else
                        {
                            _bounds.box->merge(childBox);
                        }
                    }
                }
                break;
            case SPHERE:
                {
                    const BoundingSphere& childSphere = n->getBoundingSphere();
                    if (!childSphere.isEmpty())
                    {
                        if (empty)
                        {
                            _bounds.box->set(childSphere);
                            empty = false;
                        }
                        else
                        {
                            _bounds.box->merge(childSphere);
                        }
                    }
                }
                break;
            }
        }
    }

    return *_bounds.box;
}

const BoundingSphere& Node::getBoundingSphere() const
{
    if (_boundsType != SPHERE)
    {
        return BoundingSphere::empty();
    }

    if (_dirtyBits & NODE_DIRTY_BOUNDS)
    {
        _dirtyBits &= ~NODE_DIRTY_BOUNDS;

        if (_model && _model->getMesh())
        {
            // Use the bounding volume of our model's mesh.
            Mesh* mesh = _model->getMesh();
            _bounds.sphere->set(mesh->getBoundingSphere());
        }
        else
        {
            _bounds.sphere->set(Vector3::zero(), 0);
        }

        bool empty = _bounds.sphere->isEmpty();
        if (!empty)
        {
            // Transform the sphere into world space.
            _bounds.sphere->transform(getWorldMatrix());
        }

        // Merge this world-space bounding sphere with our childrens' bounding volumes.
        for (Node* n = getFirstChild(); n != NULL; n = n->getNextSibling())
        {
            switch (n->getBoundsType())
            {
            case BOX:
                {
                    const BoundingBox& childBox = n->getBoundingBox();
                    if (!childBox.isEmpty())
                    {
                        if (empty)
                        {
                            _bounds.sphere->set(childBox);
                            empty = false;
                        }
                        else
                        {
                            _bounds.sphere->merge(childBox);
                        }
                    }
                }
                break;
            case SPHERE:
                {
                    const BoundingSphere& childSphere = n->getBoundingSphere();
                    if (!childSphere.isEmpty())
                    {
                        if (empty)
                        {
                            _bounds.sphere->set(childSphere);
                            empty = false;
                        }
                        else
                        {
                            _bounds.sphere->merge(childSphere);
                        }
                    }
                }
                break;
            }
        }
    }

    return *_bounds.sphere;
}

Node::BoundsType Node::getBoundsType() const
{
    return _boundsType;
}

void Node::setBoundsType(Node::BoundsType type)
{
    if (type != _boundsType)
    {
        switch (_boundsType)
        {
        case BOX:
            SAFE_DELETE(_bounds.box);
            break;
        case SPHERE:
            SAFE_DELETE(_bounds.sphere);
            break;
        }
        memset(&_bounds, 0, sizeof(_bounds));

        _boundsType = type;

        switch (_boundsType)
        {
        case BOX:
            _bounds.box = new BoundingBox();
            break;
        case SPHERE:
            _bounds.sphere = new BoundingSphere();
            break;
        }

        // We need to dirty the bounds for the parents in our hierarchy when
        // our bounding volume type changes.
        Node* parent = getParent();
        while (parent)
        {
            parent->_dirtyBits |= NODE_DIRTY_BOUNDS;
            parent = parent->getParent();
        }
    }
}

AudioSource* Node::getAudioSource() const
{
    return _audioSource;
}

void Node::setAudioSource(AudioSource* audio)
{
    if (_audioSource != audio)
    {
        if (_audioSource)
        {
            _audioSource->setNode(NULL);
            SAFE_RELEASE(_audioSource);
        }
        
        _audioSource = audio;

        if (_audioSource)
        {
            _audioSource->addRef();
            _audioSource->setNode(this);
        }
    }
}

ParticleEmitter* Node::getParticleEmitter() const
{
    return _particleEmitter;
}

void Node::setParticleEmitter(ParticleEmitter* emitter)
{
    if (_particleEmitter != emitter)
    {
        if (_particleEmitter)
        {
            _particleEmitter->setNode(NULL);
            SAFE_RELEASE(_particleEmitter);
        }
        
        _particleEmitter = emitter;

        if (_particleEmitter)
        {
            _particleEmitter->addRef();
            _particleEmitter->setNode(this);
        }
    }
}

PhysicsRigidBody* Node::getPhysicsRigidBody()
{
    return _physicsRigidBody;
}

void Node::setPhysicsRigidBody(PhysicsRigidBody::Type type, float mass, float friction,
        float restitution, float linearDamping, float angularDamping)
{
    if (_physicsRigidBody)
    {
        SAFE_RELEASE(_physicsRigidBody);
    }
    
    if (type != PhysicsRigidBody::PHYSICS_SHAPE_NONE)
        _physicsRigidBody = new PhysicsRigidBody(this, type, mass, friction, restitution, linearDamping, angularDamping);
}

void Node::childAdded(Node* child)
{
    child->addRef();

    if (_notifyHierarchyChanged)
    {
        hierarchyChanged();
    }
}

void Node::childRemoved(Node* child)
{
    SAFE_RELEASE(child);

    if (_notifyHierarchyChanged)
    {
        hierarchyChanged();
    }
}

void Node::parentChanged(Node* oldParent)
{
}

}
