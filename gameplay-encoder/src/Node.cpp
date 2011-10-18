#include "Node.h"

#define NODE 1
#define JOINT 2

namespace gameplay
{

Node::Node(void) :
    _childCount(0),
    _nextSibling(NULL), _previousSibling(NULL), 
    _firstChild(NULL), _lastChild(NULL), _parent(NULL),
    camera(NULL), light(NULL), model(NULL), joint(false)
{
    setIdentityMatrix(transform);
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
    unsigned int type = joint ? JOINT : NODE;
    write(type, file);

    write(transform, 16, file);
    // children
    write(getChildCount(), file); // write number of children
    for (Node* node = getFirstChild(); node != NULL; node = node->getNextSibling())
    {
        node->writeBinary(file);
    }

    // camera
    if (camera != NULL)
    {
        camera->writeBinary(file);
    }
    else
    {
        write((unsigned char)0, file);
    }
    // light
    if (light != NULL && !light->isAmbient())
    {
        light->writeBinary(file);
    }
    else
    {
        write((unsigned char)0, file);
    }
    // mesh
    if (model != NULL)
    {
        model->writeBinary(file);
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
        fprintf(file, "<%s id=\"%s\" type=\"%s\">\n", getElementName(), id.c_str(), "JOINT");
    }
    else
    {
        fprintElementStart(file);
    }
    fprintf(file, "<transform>");
    fprintfMatrix4f(file, transform);
    fprintf(file, "</transform>\n");

    // children
    for (Node* node = getFirstChild(); node != NULL; node = node->getNextSibling())
    {
        node->writeText(file);
    }
    // camera
    if (camera != NULL)
    {
        camera->writeText(file);
    }
    // light
    if (light != NULL && !light->isAmbient())
    {
        light->writeText(file);
    }
    // mesh
    if (model != NULL)
    {
        model->writeText(file);
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
        _firstChild = child->_nextSibling;
    if (child == _lastChild)
        _lastChild = child->_previousSibling;

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

void Node::setCameraInstance(CameraInstance* cameraInstance)
{
    camera = cameraInstance;
}
void Node::setLightInstance(LightInstance* lightInstance)
{
    light = lightInstance;
}
void Node::setModel(Model* model)
{
    this->model = model;
}

void Node::setTransformMatrix(float matrix[])
{
    for (int i = 0; i < 16; i++)
    {
        transform[i] = matrix[i];
    }
}

void Node::setIsJoint(bool value)
{
    joint = value;
}

bool Node::isJoint()
{
    return joint;
}

Camera* Node::getCamera() const
{
    if (camera)
    {
        return camera->getCamera();
    }
    return NULL;
}

Light* Node::getLight() const
{
    if (light)
    {
        return light->getLight();
    }
    return NULL;
}

Model* Node::getModel() const
{
    if (model)
    {
        return model;
    }
    return NULL;
}

Node* Node::getFirstCameraNode() const
{
    if (hasCamera())
    {
        return (Node*)this;
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
    return camera != NULL;
}

bool Node::hasLight() const
{
    return light != NULL;
}

} 