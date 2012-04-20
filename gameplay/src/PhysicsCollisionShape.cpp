#include "Base.h"
#include "PhysicsCollisionShape.h"
#include "Node.h"
#include "Properties.h"

namespace gameplay
{

PhysicsCollisionShape::PhysicsCollisionShape(Type type, btCollisionShape* shape)
    : _type(type), _shape(shape)
{
    memset(&_shapeData, 0, sizeof(_shapeData));
}

PhysicsCollisionShape::PhysicsCollisionShape(const PhysicsCollisionShape& copy)
{
    // hidden
}

PhysicsCollisionShape::~PhysicsCollisionShape()
{
    if (_shape)
    {
        // Cleanup shape-specific cached data
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
            break;
        case SHAPE_HEIGHTFIELD:
            if (_shapeData.heightfieldData)
            {
                SAFE_DELETE_ARRAY(_shapeData.heightfieldData->heightData);
                SAFE_DELETE(_shapeData.heightfieldData);
            }
            break;
        }

        // Free the bullet shape
        SAFE_DELETE(_shape);
    }
}

PhysicsCollisionShape::Type PhysicsCollisionShape::getType() const
{
    return _type;
}

PhysicsCollisionShape::Definition::Definition()
    : isExplicit(false), centerAbsolute(false)
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
        data.heightfield->addRef();
        break;

    case PhysicsCollisionShape::SHAPE_MESH:
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
            data.heightfield->addRef();
            break;

        case PhysicsCollisionShape::SHAPE_MESH:
            data.mesh->addRef();
            break;
        }
    }

    return *this;
}

PhysicsCollisionShape::Definition* PhysicsCollisionShape::Definition::create(Node* node, Properties* properties)
{
    // Check if the properties is valid and has a valid namespace.
    assert(properties);
    if (!properties || 
        !(strcmp(properties->getNamespace(), "character") == 0 || 
        strcmp(properties->getNamespace(), "ghostObject") == 0 || 
        strcmp(properties->getNamespace(), "rigidBody") == 0))
    {
        WARN("Failed to load physics collision shape from properties object: must be non-null object and have namespace equal to \'character\', \'ghostObject\', or \'rigidBody\'.");
        return NULL;
    }

    // Set values to their defaults.
    PhysicsCollisionShape::Type type = PhysicsCollisionShape::SHAPE_BOX;
    Vector3* extents = NULL;
    Vector3* center = NULL;
    float radius = -1.0f;
    float height = -1.0f;
    bool centerIsAbsolute = false;
    const char* imagePath = NULL;
    bool typeSpecified = false;

    // Load the defined properties.
    properties->rewind();
    const char* name;
    while (name = properties->getNextProperty())
    {
        if (strcmp(name, "type") == 0)
        {
            std::string typeStr = properties->getString();
            if (typeStr == "BOX")
                type = SHAPE_BOX;
            else if (typeStr == "SPHERE")
                type = SHAPE_SPHERE;
            else if (typeStr == "MESH")
                type = SHAPE_MESH;
            else if (typeStr == "HEIGHTFIELD")
                type = SHAPE_HEIGHTFIELD;
            else if (typeStr == "CAPSULE")
                type = SHAPE_CAPSULE;
            else
            {
                WARN_VARG("Could not create physics collision shape; unsupported value for collision shape type: '%s'.", typeStr.c_str());
                return NULL;
            }

            typeSpecified = true;
        }
        else if (strcmp(name, "image") == 0)
        {
            imagePath = properties->getString();
        }
        else if (strcmp(name, "radius") == 0)
        {
            radius = properties->getFloat();
        }
        else if (strcmp(name, "height") == 0)
        {
            height = properties->getFloat();
        }
        else if (strcmp(name, "extents") == 0)
        {
            extents = new Vector3();
            properties->getVector3("extents", extents);
        }
        else if (strcmp(name, "center") == 0)
        {
            center = new Vector3();
            properties->getVector3("center", center);
        }
        else if (strcmp(name, "centerAbsolute") == 0)
        {
            centerIsAbsolute = properties->getBool();
        }
    }

    if (!typeSpecified)
    {
        WARN("Missing 'type' specifier for collision shape definition.");
        return NULL;
    }

    // Create the collision shape.
    PhysicsCollisionShape::Definition* shape = new PhysicsCollisionShape::Definition();
    switch (type)
    {
        case SHAPE_BOX:
            if (extents)
            {
                if (center)
                {
                    *shape = box(*extents, *center, centerIsAbsolute);
                }
                else
                {
                    *shape = box(*extents);
                }
            }
            else
            {
                *shape = box();
            }
            break;
        case SHAPE_SPHERE:
            if (radius != -1.0f)
            {
                if (center)
                {
                    *shape = sphere(radius, *center, centerIsAbsolute);
                }
                else
                {
                    *shape = sphere(radius);
                }
            }
            else
            {
                *shape = sphere();
            }
            break;
        case SHAPE_CAPSULE:
            if (radius != -1.0f && height != -1.0f)
            {
                if (center)
                {
                    *shape = capsule(radius, height, *center, centerIsAbsolute);
                }
                else
                {
                    *shape = capsule(radius, height);
                }
            }
            else
            {
                *shape = capsule();
            }
            break;
        case SHAPE_MESH:
        {
            // Mesh is required on node
            Mesh* nodeMesh = node->getModel() ? node->getModel()->getMesh() : NULL;
            if (nodeMesh == NULL)
            {
                WARN("Cannot create mesh rigid body for node without mode/mesh.");
                return NULL;
            }

            // Check that the node's mesh's primitive type is supported.
            switch (nodeMesh->getPrimitiveType())
            {
                case Mesh::TRIANGLES:
                {
                    *shape = mesh(nodeMesh);
                    break;
                }
                case Mesh::LINES:
                case Mesh::LINE_STRIP:
                case Mesh::POINTS:
                case Mesh::TRIANGLE_STRIP:
                    WARN("Mesh rigid bodies are currently only supported on meshes with primitive type equal to TRIANGLES.");
                    SAFE_DELETE(shape);
                    break;
            }

            break;
        }
        case SHAPE_HEIGHTFIELD:
            if (imagePath == NULL)
            {
                WARN("Heightfield rigid body requires an image path.");
            }
            else
            {
                // Load the image data from the given file path.
                Image* image = Image::create(imagePath);
                if (!image)
                    return NULL;

                // Ensure that the image's pixel format is supported.
                switch (image->getFormat())
                {
                    case Image::RGB:
                    case Image::RGBA:
                        break;
                    default:
                        WARN_VARG("Heightmap: pixel format is not supported: %d", image->getFormat());
                        return NULL;
                }

                *shape = PhysicsCollisionShape::heightfield(image);
                SAFE_RELEASE(image);
            }
            break;
        default:
            WARN("Unsupported value for physics collision shape type.");
            break;
    }

    SAFE_DELETE(extents);
    SAFE_DELETE(center);

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

PhysicsCollisionShape::Definition PhysicsCollisionShape::heightfield(Image* image)
{
    image->addRef();

    Definition d;
    d.type = SHAPE_HEIGHTFIELD;
    d.data.heightfield = image;
    d.isExplicit = true;
    d.centerAbsolute = false;
    return d;
}

PhysicsCollisionShape::Definition PhysicsCollisionShape::mesh(Mesh* mesh)
{
    mesh->addRef();

    Definition d;
    d.type = SHAPE_MESH;
    d.data.mesh = mesh;
    d.isExplicit = true;
    d.centerAbsolute = false;
    return d;
}

}
