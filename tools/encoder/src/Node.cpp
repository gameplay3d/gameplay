#include "Base.h"
#include "Node.h"
#include "Matrix.h"
#include "EncoderArguments.h"

#define NODE 1
#define JOINT 2

namespace gameplay
{

Node::Node(void) :
    _childCount(0),
    _nextSibling(NULL), _previousSibling(NULL),
    _firstChild(NULL), _lastChild(NULL), _parent(NULL),
    _camera(NULL), _light(NULL), _model(NULL), _joint(false)
{
}

Node::~Node(void)
{
}

unsigned int Node::getTypeId(void) const
{
    return NODE_ID;
}

const char* Node::getElementName(void) const
{
    return "Node";
}

void Node::writeBinary(FILE* file)
{
    Object::writeBinary(file);

    // node type
    unsigned int type = _joint ? JOINT : NODE;
    write(type, file);

    write(_transform.m, 16, file);

    // write parent's id
    write((_parent) ? _parent->getId() : std::string(), file);

    // children
    write(getChildCount(), file); // write number of children
    for (Node* node = getFirstChild(); node != NULL; node = node->getNextSibling())
    {
        node->writeBinary(file);
    }

    // camera
    if (_camera != NULL)
    {
        _camera->writeBinary(file);
    }
    else
    {
        write((unsigned char)0, file);
    }
    // light
    if (_light != NULL && !_light->isAmbient())
    {
        _light->writeBinary(file);
    }
    else
    {
        write((unsigned char)0, file);
    }
    // mesh
    if (_model != NULL)
    {
        _model->writeBinary(file);
    }
    else
    {
        writeZero(file);
    }
}

void Node::writeText(FILE* file)
{
    if (isJoint())
    {
        fprintf(file, "<%s id=\"%s\" type=\"%s\">\n", getElementName(), getId().c_str(), "JOINT");
    }
    else
    {
        fprintElementStart(file);
    }
    fprintf(file, "<transform>");
    fprintfMatrix4f(file, _transform.m);
    fprintf(file, "</transform>\n");

    // children
    for (Node* node = getFirstChild(); node != NULL; node = node->getNextSibling())
    {
        node->writeText(file);
    }
    // camera
    if (_camera != NULL)
    {
        _camera->writeText(file);
    }
    // light
    if (_light != NULL && !_light->isAmbient())
    {
        _light->writeText(file);
    }
    // mesh
    if (_model != NULL)
    {
        _model->writeText(file);
    }
    fprintElementEnd(file);
}

void Node::addChild(Node* child)
{
    // If this child is already parented, remove it from its parent
    if (child->_parent)
    {
        child->_parent->removeChild(child);
    }

    if (_firstChild == NULL)
    {
        // No children yet
        _firstChild = child;
        child->_previousSibling = NULL;
    }
    else
    {
        // We already have children, so append to them
        child->_previousSibling = _lastChild;
        _lastChild->_nextSibling = child;
    }

    child->_parent = this;
    child->_nextSibling = NULL;
    this->_lastChild = child;

    ++_childCount;
}


void Node::removeChild(Node* child)
{
    // The child must have already been our child to remove it
    if (child->_parent != this)
    {
        return;
    }

    // Remove the child by un-linking it from our child list
    if (child->_nextSibling)
    {
        child->_nextSibling->_previousSibling = child->_previousSibling;
    }
    if (child->_previousSibling)
    {
        child->_previousSibling->_nextSibling = child->_nextSibling;
    }

    // Was this child our first or last child?
    if (child == _firstChild)
    {
        _firstChild = child->_nextSibling;
    }
    if (child == _lastChild)
    {
        _lastChild = child->_previousSibling;
    }

    // Remove parent and sibling info from the child, now that it is no longer parented
    child->_parent = NULL;
    child->_nextSibling = NULL;
    child->_previousSibling = NULL;

    --_childCount;
}

void Node::removeChildren()
{
    Node* child;
    while ((child = _firstChild) != NULL)
    {
        removeChild(child);
    }
}

bool Node::hasChildren() const
{
    return (_firstChild != NULL);
}

unsigned int Node::getChildCount() const
{
    return _childCount;
}

Node* Node::getNextSibling() const
{
    return _nextSibling;
}

Node* Node::getPreviousSibling() const
{
    return _previousSibling;
}

Node* Node::getFirstChild() const
{
    return _firstChild;
}

Node* Node::getLastChild() const
{
    return _lastChild;
}

Node* Node::getParent() const
{
    return _parent;
}

void Node::setCamera(Camera* camera)
{
    _camera = camera;
}

void Node::setLight(Light* light)
{
    _light = light;
}

void Node::setModel(Model* model)
{
    _model = model;
}

const Matrix& Node::getTransformMatrix() const
{
    return _transform;
}

void Node::setTransformMatrix(float matrix[])
{
    memcpy(_transform.m, matrix, 16 * sizeof(float));
}

const Matrix& Node::getWorldMatrix() const
{
    if (_parent)
    {
        Matrix::multiply(_parent->getWorldMatrix().m, _transform.m, _worldTransform.m);
    }
    else
    {
        memcpy(_worldTransform.m, _transform.m, 16 * sizeof(float));
    }

    return _worldTransform;
}

void Node::resetTransformMatrix()
{
    Matrix::setIdentity(_transform.m);
}

void Node::setIsJoint(bool value)
{
    _joint = value;
}

bool Node::isJoint() const
{
    return _joint;
}

Camera* Node::getCamera() const
{
    return _camera;
}

Light* Node::getLight() const
{
    return _light;
}

Model* Node::getModel() const
{
    if (_model)
    {
        return _model;
    }
    return NULL;
}

Node* Node::getFirstCameraNode() const
{
    if (hasCamera())
    {
        return const_cast<Node*>(this);
    }
    for (Node* node = getFirstChild(); node != NULL; node = node->getNextSibling())
    {
        Node* n = node->getFirstCameraNode();
        if (n)
        {
            return n;
        }
    }
    return NULL;
}

bool Node::hasCamera() const
{
    return _camera != NULL;
}

bool Node::hasLight() const
{
    return _light != NULL;
}

}
