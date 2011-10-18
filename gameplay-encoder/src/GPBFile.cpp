#include "GPBFile.h"

namespace gameplay
{

GPBFile::GPBFile(void)
    : file(NULL)
{
}

GPBFile::~GPBFile(void)
{
}

void GPBFile::saveBinary(const std::string& filepath)
{
    file = fopen(filepath.c_str(), "w+b");

    // identifier
    char identifier[] = { '«', 'G', 'P', 'B', '»', '\r', '\n', '\x1A', '\n' };
    fwrite(identifier, 1, sizeof(identifier), file);

    // version
    fwrite(VERSION, 1, sizeof(VERSION), file);

    // write refs
    refTable.writeBinary(file);

    // meshes
    write(geometry.size(), file);
    for (std::list<Mesh*>::const_iterator i = geometry.begin(); i != geometry.end(); i++)
    {
        (*i)->writeBinary(file);
    }

    // Objects
    write(objects.size(), file);
    for (std::list<Object*>::const_iterator i = objects.begin(); i != objects.end(); i++)
    {
        (*i)->writeBinary(file);
    }

    refTable.updateOffsets(file);
    
    fclose(file);
}

void GPBFile::saveText(const std::string& filepath)
{
    file = fopen(filepath.c_str(), "w");

    fprintf(file, "<root>\n");

    // write refs
    refTable.writeText(file);

    // meshes
    for (std::list<Mesh*>::const_iterator i = geometry.begin(); i != geometry.end(); i++)
    {
        (*i)->writeText(file);
    }

    // Objects
    for (std::list<Object*>::const_iterator i = objects.begin(); i != objects.end(); i++)
    {
        (*i)->writeText(file);
    }

    fprintf(file, "</root>");

    fclose(file);
}

void GPBFile::add(Object* obj)
{
    objects.push_back(obj);
}

void GPBFile::addScene(Scene* scene)
{
    addToRefTable(scene);
    objects.push_back(scene);
}

void GPBFile::addCamera(Camera* camera)
{
    addToRefTable(camera);
    cameras.push_back(camera);
}

void GPBFile::addLight(Light* light)
{
    addToRefTable(light);
    lights.push_back(light);
}

void GPBFile::addMesh(Mesh* mesh)
{
    addToRefTable(mesh);
    geometry.push_back(mesh);
}

void GPBFile::addNode(Node* node)
{
    addToRefTable(node);
    nodes.push_back(node);
}

void GPBFile::addAnimation(Animation* animation)
{
    if (!idExists(animation->getId()))
    {
        addToRefTable(animation);
        objects.push_back(animation);
    }
}

void GPBFile::addToRefTable(Object* obj)
{
    if (obj)
    {
        const std::string& id = obj->getId();
        if (id.length() > 0)
        {
            if (refTable.get(id) == NULL)
                refTable.add(id, obj);
        }
    }
}

Object* GPBFile::getFromRefTable(const std::string& id)
{
    return refTable.get(id);
}

bool GPBFile::idExists(const std::string& id)
{
    return refTable.get(id) != NULL;
}

Camera* GPBFile::getCamera(const char* id)
{
    // TODO: O(n) search is not ideal
    for (std::list<Camera*>::const_iterator i = cameras.begin(); i != cameras.end(); i++)
    {
        const std::string& _id = (*i)->getId();
        if (_id.length() > 0 && strncmp(id, _id.c_str(), 255) == 0)
        {
            return *i;
        }
    }
    return NULL;
}

Light* GPBFile::getLight(const char* id)
{
    // TODO: O(n) search is not ideal
    for (std::list<Light*>::const_iterator i = lights.begin(); i != lights.end(); i++)
    {
        const std::string& _id = (*i)->getId();
        if (_id.length() > 0 && strncmp(id, _id.c_str(), 255) == 0)
        {
            return *i;
        }
    }
    return NULL;
}

Mesh* GPBFile::getMesh(const char* id)
{
    // TODO: O(n) search is not ideal
    for (std::list<Mesh*>::const_iterator i = geometry.begin(); i != geometry.end(); i++)
    {
        const std::string& _id = (*i)->getId();
        if (_id.length() > 0 && strncmp(id, _id.c_str(), 255) == 0)
        {
            return *i;
        }
    }
    return NULL;
}

Node* GPBFile::getNode(const char* id)
{
    // TODO: O(n) search is not ideal
    for (std::list<Node*>::const_iterator i = nodes.begin(); i != nodes.end(); i++)
    {
        const std::string& _id = (*i)->getId();
        if (_id.length() > 0 && strncmp(id, _id.c_str(), 255) == 0)
        {
            return *i;
        }
    }
    return NULL;
}

void GPBFile::adjust()
{
    // calculate the ambient color for each scene
    for (std::list<Object*>::iterator i = objects.begin(); i != objects.end(); i++)
    {
        Object* obj = *i;
        if (obj->getTypeId() == Object::SCENE_ID)
        {
            Scene* scene = dynamic_cast<Scene*>(obj);
            scene->calcAmbientColor();
        }
    }

    // TODO:
    // remove ambient lights
    // for each node
    //   if node has ambient light
    //     if node has no camera, mesh or children but 1 ambient light
    //       delete node and remove from ref table
    //     delete light and remove from ref table
    // 
    // merge animations if possible
    //   Search for animations that have the same target and key times and see if they can be merged.
    //   Blender will output a simple translation animation to 3 separate animations with the same key times but targetting X, Y and Z.
    //   This can be merged into one animation. Same for scale animations.
}

Animation* GPBFile::findAnimationForJoint(const char* id)
{
    for (std::map<std::string, Reference>::iterator i = refTable.begin(); i != refTable.end(); i++)
    {
        Reference& ref = i->second;
        Object* obj = ref.getObj();
        if (obj->getTypeId() == Object::NODE_ID)
        {
            Node* node = (Node*)obj;
            Model* model = node->getModel();
            if (model)
            {
                MeshSkin* skin = model->getSkin();
                if (skin)
                {
                    if (skin->hasJoint(id))
                    {
                        return skin->getAnimation();
                    }
                }
            }
        }
    }
    return NULL;
}

}