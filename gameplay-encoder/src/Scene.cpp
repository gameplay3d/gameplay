#include <algorithm>

#include "Scene.h"

namespace gameplay
{

Scene::Scene(void) : cameraNode(NULL)
{
    ambientColor[0] = 0.0f;
    ambientColor[1] = 0.0f;
    ambientColor[2] = 0.0f;
}

Scene::~Scene(void)
{
}

unsigned int Scene::getTypeId(void) const
{
    return SCENE_ID;
}

const char* Scene::getElementName(void) const
{
    return "Scene";
}

void Scene::writeBinary(FILE* file)
{
    Object::writeBinary(file);
    writeBinaryObjects(nodes, file);
    if (cameraNode)
    {
        cameraNode->writeBinaryXref(file);
    }
    else
    {
        writeZero(file);
    }
    write(ambientColor, Light::COLOR_SIZE, file);
}
void Scene::writeText(FILE* file)
{
    fprintElementStart(file);
    for (std::list<Node*>::const_iterator i = nodes.begin(); i != nodes.end(); i++)
    {
        (*i)->writeText(file);
    }
    if (cameraNode)
    {
        fprintfElement(file, "activeCamera", cameraNode->getId());
    }
    fprintfElement(file, "ambientColor", ambientColor, Light::COLOR_SIZE);
    fprintElementEnd(file);
}

void Scene::add(Node* node)
{
    nodes.push_back(node);
}

void Scene::setActiveCameraNode(Node* node)
{
    cameraNode = node;
}

Node* Scene::getFirstCameraNode() const
{
    for (std::list<Node*>::const_iterator i = nodes.begin(); i != nodes.end(); i++)
    {
        Node* n = (*i)->getFirstCameraNode();
        if (n)
        {
            return n;
        }
    }
    return NULL;
}

void Scene::calcAmbientColor()
{
    float values[3] = {0.0f, 0.0f, 0.0f};
    for (std::list<Node*>::const_iterator i = nodes.begin(); i != nodes.end(); i++)
    {
        calcAmbientColor(*i, values);
    }
    
    ambientColor[0] = std::min(values[0], 1.0f);
    ambientColor[1] = std::min(values[1], 1.0f);
    ambientColor[2] = std::min(values[2], 1.0f);
}

void Scene::calcAmbientColor(const Node* node, float* values) const
{
    if (!node)
    {
        return;
    }
    if (node->hasLight())
    {
        Light* light = node->getLight();
        if (light->isAmbient())
        {
            values[0] += light->getRed();
            values[1] += light->getGreen();
            values[2] += light->getBlue();
        }
    }
    for (Node* child = node->getFirstChild(); child != NULL; child = child->getNextSibling())
    {
        calcAmbientColor(child, values);
    }
}

}