#ifndef PHYSICSCOLLISIONSHAPE_H_
#define PHYSICSCOLLISIONSHAPE_H_

#include "Vector3.h"
#include "Mesh.h"
#include "HeightField.h"

namespace gameplay
{
    class Node;
    class Properties;

/**
 * Defines the physics collision shape class that all supported shapes derive from.
 */
class PhysicsCollisionShape : public Ref
{
    friend class PhysicsController;
    friend class PhysicsRigidBody;

public:

    /**
     * Defines the supported collision shape types.
     */
    enum Type
    {
        SHAPE_NONE,
        SHAPE_BOX,
        SHAPE_SPHERE,
        SHAPE_CAPSULE,
        SHAPE_MESH,
        SHAPE_HEIGHTFIELD
    };

    /**
     * Structure representing the definition of a collision shape, which is used
     * during collision shape construction time.
     *
     * Use the static methods on the PhysicsCollisionShape class to return
     * 
     */
    struct Definition
    {
        friend class PhysicsCollisionShape;
        friend class PhysicsController;
        friend class PhysicsRigidBody;
        friend class PhysicsCharacter;
        friend class PhysicsGhostObject;

    public:

        /**
         * Constructor.
         */
        Definition();

        /** 
         * Constructs a new Definition that is a copy of the specified Definition.
         *
         * @param definition The Definition to copy.
         */ 
        Definition(const Definition& definition);

        /**
         * Assigns the specified Definition as the Definition.
         *
         * @param definition The Definition to assign to the Definition.
         */
        Definition& operator=(const Definition& definition);
        
        /**
         * Destructor.
         */
        ~Definition();

        /**
         * Determines if this is an empty/undefined collision shape definition.
         */
        bool isEmpty() const;

    private:

        /**
         * Creates a PhysicsCollisionShape::Definition object from the given properties object (for the given node).
         * 
         * @param node The node to create the PhysicsCollisionShape::Definition object for.
         * @param properties The properties object to create the PhysicsCollisionShape::Definition object from.
         * @return A PhysicsCollisionShape::Definition object.
         */
        static Definition create(Node* node, Properties* properties);

        // Shape type.
        PhysicsCollisionShape::Type type;

        // Shape data.
        struct BoxData { float center[3], extents[3]; };
        struct SphereData { float center[3]; float radius; };
        struct CapsuleData { float center[3]; float radius, height; };

        union
        {
            /** @script{ignore} */
            BoxData box;
            /** @script{ignore} */
            SphereData sphere;
            /** @script{ignore} */
            CapsuleData capsule;
            /** @script{ignore} */
            HeightField* heightfield;
            /** @script{ignore} */
            Mesh* mesh;
        } data;

        // Whether the shape definition is explicit, or if it is inherited from node bounds.
        bool isExplicit;

        // Whether the center position is absolute or relative to the node position.
        bool centerAbsolute;
    };

    /**
     * Returns the type of this collision shape.
     *
     * @return The collision shape type.
     */
    PhysicsCollisionShape::Type getType() const;

    /**
     * Returns the internal bullet physics shape object.
     *
     * @return The bullet shape object.
     * @script{ignore}
     */
    btCollisionShape* getShape() const
    {
        return _shape;
    }

    /**
     * Defines a box shape, using the bounding volume of the node it is attached to.
     *
     * @return Definition of a box shape.
     */
    static PhysicsCollisionShape::Definition box();

    /**
     * Defines a box shape, using the specified shape information and center.
     *
     * @param extents Extents of the box shape along the x, y and z axes.
     * @param center Center point of the box.
     * @param absolute True to specify that the given center point is an absolute position.
     *        By default the center point is treated as relative to the location of the node
     *        that the shape is attached to.
     *
     * @return Definition of a box shape.
     */
    static PhysicsCollisionShape::Definition box(const Vector3& extents, const Vector3& center = Vector3::zero(), bool absolute = false);

    /**
     * Defines a sphere shape, using the bounding volume of the node it is attached to.
     *
     * @return Definition of a sphere shape.
     */
    static PhysicsCollisionShape::Definition sphere();

    /**
     * Defines a sphere shape, using the specified shape information and center.
     *
     * @param radius Radius of the sphere.
     * @param center Center point of the sphere.
     * @param absolute True to specify that the given center point is an absolute position.
     *        By default the center point is treated as relative to the location of the node
     *        that the shape is attached to.
     *
     * @return Definition of a sphere shape.
     */
    static PhysicsCollisionShape::Definition sphere(float radius, const Vector3& center = Vector3::zero(), bool absolute = false);

    /**
     * Defines a capsule shape, using the bounding volume of the node it is attached to.
     *
     * @return Definition of a capsule shape.
     */
    static PhysicsCollisionShape::Definition capsule();

    /**
     * Defines a capsule shape, using the specified shape information and center.
     *
     * @param radius Radius of the capsule.
     * @param height Height of the capsule.
     * @param center Center point of the capsule.
     * @param absolute True to specify that the given center point is an absolute position.
     *        By default the center point is treated as relative to the location of the node
     *        that the shape is attached to.
     *
     * @return Definition of a capsule shape.
     */
    static PhysicsCollisionShape::Definition capsule(float radius, float height, const Vector3& center = Vector3::zero(), bool absolute = false);

    /**
     * Defines a heightfield shape, using the height data of a terrain on the node that is attached to.
     *
     * This method only results in a valid heightfield collision object when the shape is used
     * to create a collision object on a node that has a Terrain attached to it. If there is no
     * Terrain attached to the node, the collision object creation will fail.
     *
     * @return Definition of a heightfield shape.
     */
    static PhysicsCollisionShape::Definition heightfield();

    /**
     * Defines a heightfield shape using the specified array of height values.
     *
     * The dimensions of the heightfield will be (width, maxHeight, height), where width and
     * height are the dimensions of the passed in height array and maxHeight is the maximum
     * height value in the height array.
     *
     * Heightfield rigid bodies are always assumed be Y-up (height value on the Y axis) and 
     * be centered around the X and Z axes.
     *
     * The heightfield can be scaled once a PhysicsRigidBody has been created for it, using the
     * PhysicsRigidBody::setLocalScaling method.
     *
     * @param heightfield HeightField object containing the array of height values representing the heightfield.
     *
     * @return Definition of a heightfield shape.
     */
    static PhysicsCollisionShape::Definition heightfield(HeightField* heightfield);

    /**
     * Defines a mesh shape using the specified mesh.
     *
     * @return Definition of a mesh shape.
     */
    static PhysicsCollisionShape::Definition mesh(Mesh* mesh);

private:

    struct MeshData
    {
        float* vertexData;
        std::vector<unsigned char*> indexData;
    };

    struct HeightfieldData
    {
        HeightField* heightfield;
        bool inverseIsDirty;
        Matrix inverse;
        float minHeight;
        float maxHeight;
    };

    /**
     * Constructor.
     */
    PhysicsCollisionShape(Type type, btCollisionShape* shape, btStridingMeshInterface* meshInterface = NULL);

    /** 
     * Hidden copy constructor.
     */
    PhysicsCollisionShape(const PhysicsCollisionShape& copy);

    /**
     * Destructor.
     */
    ~PhysicsCollisionShape();

    // Shape type
    Type _type;

    // Bullet shape object
    btCollisionShape* _shape;

    // Bullet mesh interface for mesh types (NULL otherwise)
    btStridingMeshInterface* _meshInterface;

    // Shape specific cached data
    union
    {
        /** @script{ignore} */
        MeshData* meshData;
        /** @script{ignore} */
        HeightfieldData* heightfieldData;
    } _shapeData;

};

}

#endif
