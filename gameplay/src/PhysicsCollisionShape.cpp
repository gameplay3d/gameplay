#include "Base.h"
#include "PhysicsCollisionShape.h"
#include "Node.h"
#include "Image.h"
#include "Properties.h"
#include "FileSystem.h"
#include "HeightField.h"

namespace gameplay
{

PhysicsCollisionShape::PhysicsCollisionShape(Type type, btCollisionShape* shape, btStridingMeshInterface* meshInterface)
    : _type(type), _shape(shape), _meshInterface(meshInterface)
{
    memset(&_shapeData, 0, sizeof(_shapeData));
}

PhysicsCollisionShape::~PhysicsCollisionShape()
{
    if (_shape)
    {
        // Cleanup shape-specific cached data.
        switch (_type)
        {
        case SHAPE_MESH:
            if (_shapeData.meshData)
            {
                SAFE_DELETE_ARRAY(_shapeData.meshData->vertexData);
                for (unsigned int i = 0; i < _shapeData.meshData->indexData.size(); i++)
                {
                    SAFE_DELETE_ARRAY(_shapeData.meshData->indexData[i]);
                }
                SAFE_DELETE(_shapeData.meshData);
            }

            // Also need to delete the btTriangleIndexVertexArray, if it exists.
            SAFE_DELETE(_meshInterface);
            break;

        case SHAPE_HEIGHTFIELD:
            if (_shapeData.heightfieldData)
            {
                SAFE_RELEASE(_shapeData.heightfieldData->heightfield);
                SAFE_DELETE(_shapeData.heightfieldData);
            }
            break;
        }

        // Free the bullet shape.
        SAFE_DELETE(_shape);
    }
}

PhysicsCollisionShape::Type PhysicsCollisionShape::getType() const
{
    return _type;
}

PhysicsCollisionShape::Definition::Definition()
    : type(SHAPE_NONE), isExplicit(false), centerAbsolute(false)
{
    memset(&data, 0, sizeof(data));
}

PhysicsCollisionShape::Definition::Definition(const Definition& definition)
{
    // Bitwise-copy the definition object (equivalent to default copy constructor).
    memcpy(this, &definition, sizeof(PhysicsCollisionShape::Definition));

    // Handle the types that have reference-counted members.
    switch (type)
    {
    case PhysicsCollisionShape::SHAPE_HEIGHTFIELD:
        if (data.heightfield)
            data.heightfield->addRef();
        break;

    case PhysicsCollisionShape::SHAPE_MESH:
        GP_ASSERT(data.mesh);
        data.mesh->addRef();
        break;
    }
}

PhysicsCollisionShape::Definition::~Definition()
{
    switch (type)
    {
    case PhysicsCollisionShape::SHAPE_HEIGHTFIELD:
        SAFE_RELEASE(data.heightfield);
        break;

    case PhysicsCollisionShape::SHAPE_MESH:
        SAFE_RELEASE(data.mesh);
        break;
    }
}

PhysicsCollisionShape::Definition& PhysicsCollisionShape::Definition::operator=(const Definition& definition)
{
    if (this != &definition)
    {
        // Bitwise-copy the definition object (equivalent to default copy constructor).
        memcpy(this, &definition, sizeof(PhysicsCollisionShape::Definition));

        // Handle the types that have reference-counted members.
        switch (type)
        {
        case PhysicsCollisionShape::SHAPE_HEIGHTFIELD:
            if (data.heightfield)
                data.heightfield->addRef();
            break;

        case PhysicsCollisionShape::SHAPE_MESH:
            GP_ASSERT(data.mesh);
            data.mesh->addRef();
            break;
        }
    }

    return *this;
}

bool PhysicsCollisionShape::Definition::isEmpty() const
{
    return type == SHAPE_NONE;
}

PhysicsCollisionShape::Definition PhysicsCollisionShape::Definition::create(Node* node, Properties* properties)
{
    GP_ASSERT(node);

    // Check if the properties is valid and has a valid namespace.
    if (!properties || !(strcmp(properties->getNamespace(), "collisionObject") == 0))
    {
        GP_ERROR("Failed to load physics collision shape from properties object: must be non-null object and have namespace equal to 'collisionObject'.");
        return Definition();
    }

    // Set values to their defaults.
    PhysicsCollisionShape::Type type = PhysicsCollisionShape::SHAPE_BOX;
    Vector3 extents, center;
    bool extentsSpecified = false;
    bool centerSpecified = false;
    float radius = -1.0f;
    float width = -1.0f;
    float height = -1.0f;
    bool centerIsAbsolute = false;
    const char* imagePath = NULL;
    float maxHeight = 0;
    float minHeight = 0;
    bool shapeSpecified = false;

    // Load the defined properties.
    properties->rewind();
    const char* name;
    while ((name = properties->getNextProperty()))
    {
        if (strcmp(name, "shape") == 0)
        {
            std::string shapeStr = properties->getString();
            if (shapeStr == "BOX")
                type = SHAPE_BOX;
            else if (shapeStr == "SPHERE")
                type = SHAPE_SPHERE;
            else if (shapeStr == "MESH")
                type = SHAPE_MESH;
            else if (shapeStr == "HEIGHTFIELD")
                type = SHAPE_HEIGHTFIELD;
            else if (shapeStr == "CAPSULE")
                type = SHAPE_CAPSULE;
            else
            {
                GP_ERROR("Could not create physics collision shape; unsupported value for collision shape type: '%s'.", shapeStr.c_str());
                return Definition();
            }

            shapeSpecified = true;
        }
        else if (strcmp(name, "image") == 0)
        {
            imagePath = properties->getString();
        }
        else if (strcmp(name, "maxHeight") == 0)
        {
            maxHeight = properties->getFloat();
        }
        else if (strcmp(name, "minHeight") == 0)
        {
            minHeight = properties->getFloat();
        }
        else if (strcmp(name, "radius") == 0)
        {
            radius = properties->getFloat();
        }
        else if (strcmp(name, "width") == 0)
        {
            width = properties->getFloat();
        }
        else if (strcmp(name, "height") == 0)
        {
            height = properties->getFloat();
        }
        else if (strcmp(name, "extents") == 0)
        {
            properties->getVector3("extents", &extents);
            extentsSpecified = true;
        }
        else if (strcmp(name, "center") == 0)
        {
            properties->getVector3("center", &center);
            centerSpecified = true;
        }
        else if (strcmp(name, "centerAbsolute") == 0)
        {
            centerIsAbsolute = properties->getBool();
        }
        else
        {
            // Ignore this case (these are the properties for the rigid body, character, or ghost object that this collision shape is for).
        }
    }

    if (!shapeSpecified)
    {
        GP_ERROR("Missing 'shape' specifier for collision shape definition.");
        return Definition();
    }

    // Create the collision shape.
    Definition shape;
    switch (type)
    {
    case SHAPE_BOX:
        if (extentsSpecified)
        {
            if (centerSpecified)
            {
                shape = box(extents, center, centerIsAbsolute);
            }
            else
            {
                shape = box(extents);
            }
        }
        else
        {
            shape = box();
        }
        break;

    case SHAPE_SPHERE:
        if (radius != -1.0f)
        {
            if (centerSpecified)
            {
                shape = sphere(radius, center, centerIsAbsolute);
            }
            else
            {
                shape = sphere(radius);
            }
        }
        else
        {
            shape = sphere();
        }
        break;

    case SHAPE_CAPSULE:
        if (radius != -1.0f && height != -1.0f)
        {
            if (centerSpecified)
            {
                shape = capsule(radius, height, center, centerIsAbsolute);
            }
            else
            {
                shape = capsule(radius, height);
            }
        }
        else
        {
            shape = capsule();
        }
        break;

    case SHAPE_MESH:
        {
            // Mesh is required on node.
            Mesh* nodeMesh = node->getModel() ? node->getModel()->getMesh() : NULL;
            if (nodeMesh == NULL)
            {
                GP_ERROR("Cannot create mesh collision object for node without model/mesh.");
            }
            else
            {
                // Check that the node's mesh's primitive type is supported.
                switch (nodeMesh->getPrimitiveType())
                {
                case Mesh::TRIANGLES:
                    shape = mesh(nodeMesh);
                    break;
                case Mesh::LINES:
                case Mesh::LINE_STRIP:
                case Mesh::POINTS:
                case Mesh::TRIANGLE_STRIP:
                    GP_ERROR("Mesh collision objects are currently only supported on meshes with primitive type equal to TRIANGLES.");
                    break;
                }
            }
        }
        break;

    case SHAPE_HEIGHTFIELD:
        {
            if (imagePath == NULL)
            {
                // Node requires a valid terrain
                if (node->getTerrain() == NULL)
                {
                    GP_ERROR("Heightfield collision objects can only be specified on nodes that have a valid terrain, or that specify an image path.");
                }
                else
                {
                    shape = PhysicsCollisionShape::heightfield();
                }
            }
            else
            {
                std::string ext = FileSystem::getExtension(imagePath);
                HeightField* heightfield = NULL;
                if (ext == ".PNG")
                    heightfield = HeightField::createFromImage(imagePath, minHeight, maxHeight);
                else if (ext == ".RAW" || ext == ".R16")
                    heightfield = HeightField::createFromRAW(imagePath, (unsigned int)width, (unsigned int)height, minHeight, maxHeight);

                if (heightfield)
                {
                    shape = PhysicsCollisionShape::heightfield(heightfield);
                    SAFE_RELEASE(heightfield);
                }
            }
        }
        break;

    default:
        GP_ERROR("Unsupported physics collision shape type (%d).", type);
        break;
    }

    return shape;
}

PhysicsCollisionShape::Definition PhysicsCollisionShape::box()
{
    Definition d;
    d.type = SHAPE_BOX;
    d.isExplicit = false;
    d.centerAbsolute = false;
    return d;
}

PhysicsCollisionShape::Definition PhysicsCollisionShape::box(const Vector3& extents, const Vector3& center, bool absolute)
{
    Definition d;
    d.type = SHAPE_BOX;
    memcpy(d.data.box.extents, &extents.x, sizeof(float) * 3);
    memcpy(d.data.box.center, &center.x, sizeof(float) * 3);
    d.isExplicit = true;
    d.centerAbsolute = absolute;
    return d;
}

PhysicsCollisionShape::Definition PhysicsCollisionShape::sphere()
{
    Definition d;
    d.type = SHAPE_SPHERE;
    d.isExplicit = false;
    d.centerAbsolute = false;
    return d;
}

PhysicsCollisionShape::Definition PhysicsCollisionShape::sphere(float radius, const Vector3& center, bool absolute)
{
    Definition d;
    d.type = SHAPE_SPHERE;
    d.data.sphere.radius = radius;
    memcpy(d.data.sphere.center, &center.x, sizeof(float) * 3);
    d.isExplicit  = true;
    d.centerAbsolute = absolute;
    return d;
}

PhysicsCollisionShape::Definition PhysicsCollisionShape::capsule()
{
    Definition d;
    d.type = SHAPE_CAPSULE;
    d.isExplicit = false;
    d.centerAbsolute = false;
    return d;
}

PhysicsCollisionShape::Definition PhysicsCollisionShape::capsule(float radius, float height, const Vector3& center, bool absolute)
{
    Definition d;
    d.type = SHAPE_CAPSULE;
    d.data.capsule.radius = radius;
    d.data.capsule.height = height;
    memcpy(d.data.capsule.center, &center.x, sizeof(float) * 3);
    d.isExplicit = true;
    d.centerAbsolute = absolute;
    return d;
}

PhysicsCollisionShape::Definition PhysicsCollisionShape::heightfield()
{
    Definition d;
    d.type = SHAPE_HEIGHTFIELD;
    d.isExplicit = false;
    d.centerAbsolute = false;
    return d;
}

PhysicsCollisionShape::Definition PhysicsCollisionShape::heightfield(HeightField* heightfield)
{
    GP_ASSERT(heightfield);

    heightfield->addRef();

    Definition d;
    d.type = SHAPE_HEIGHTFIELD;
    d.data.heightfield = heightfield;
    d.isExplicit = true;
    d.centerAbsolute = false;
    return d;
}

PhysicsCollisionShape::Definition PhysicsCollisionShape::mesh(Mesh* mesh)
{
    GP_ASSERT(mesh);
    mesh->addRef();

    Definition d;
    d.type = SHAPE_MESH;
    d.data.mesh = mesh;
    d.isExplicit = true;
    d.centerAbsolute = false;
    return d;
}

}
