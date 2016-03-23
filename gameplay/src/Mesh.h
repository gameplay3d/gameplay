#ifndef MESH_H_
#define MESH_H_

#include "Ref.h"
#include "VertexFormat.h"
#include "Vector3.h"
#include "BoundingBox.h"
#include "BoundingSphere.h"

namespace gameplay
{

class MeshPart;
class Material;
class Model;

/**
 * Defines a mesh supporting various vertex formats and 1 or more
 * MeshPart(s) to define how the vertices are connected.
 */
class Mesh : public Ref
{
    friend class Model;
    friend class Bundle;

public:

    /**
     * Defines supported index formats.
     */
    enum IndexFormat
    {
        INDEX8 = GL_UNSIGNED_BYTE,
        INDEX16 = GL_UNSIGNED_SHORT,
        INDEX32 = GL_UNSIGNED_INT
    };

    /**
     * Defines supported primitive types.
     */
    enum PrimitiveType
    {
        TRIANGLES = GL_TRIANGLES,
        TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
        LINES = GL_LINES,
        LINE_STRIP = GL_LINE_STRIP,
        POINTS = GL_POINTS
    };

    /**
     * Constructs a new mesh with the specified vertex format.
     *
     * @param vertexFormat The vertex format.
     * @param vertexCount The number of vertices.
     * @param dynamic true if the mesh is dynamic; false otherwise.
     * 
     * @return The created mesh.
     * @script{create}
     */
    static Mesh* createMesh(const VertexFormat& vertexFormat, unsigned int vertexCount, bool dynamic = false);

    /**
     * Creates a new textured 3D quad.
     *
     * The specified points should describe a triangle strip with the first 3 points
     * forming a triangle wound in counter-clockwise direction, with the second triangle
     * formed from the last three points in clockwise direction.
     *
     * @param p1 The first point.
     * @param p2 The second point.
     * @param p3 The third point.
     * @param p4 The fourth point.
     * 
     * @return The created mesh.
     * @script{create}
     */
    static Mesh* createQuad(const Vector3& p1, const Vector3& p2, const Vector3& p3, const Vector3& p4);

    /**
     * Constructs a new textured 2D quad.
     * 
     * @param x The x coordinate.
     * @param y The y coordinate.
     * @param width The width of the quad.
     * @param height The height of the quad.
     * @param s1 The S texture coordinate of the bottom left point.
     * @param t1 The T texture coordinate of the bottom left point.
     * @param s2 The S texture coordinate of the top right point.
     * @param t2 The T texture coordinate of the top right point.
     * 
     * @return The newly created mesh.
     * @script{create}
     */
    static Mesh* createQuad(float x, float y, float width, float height, float s1 = 0.0f, float t1 = 0.0f, float s2 = 1.0f, float t2 = 1.0f);

    /**
     * Creates a new full-screen 2D quad.
     *
     * The returned mesh's vertex format includes a 2-element (x,y) position
     * and a 2-element texture coordinate.
     *
     * This method returns a mesh describing a fullscreen quad using
     * normalized device coordinates for vertex positions.
     * 
     * @return The newly created mesh.
     * @script{create}
     */
    static Mesh* createQuadFullscreen();

    /**
     * Creates lines between 2 or more points passed in as a Vector3 array.
     *
     * The mesh contains only position data using lines to connect the vertices.
     * This is useful for drawing basic color elements into a scene.
     * 
     * @param points The array of points.
     * @param pointCount The number of points.
     * 
     * @return The newly created mesh.
     * @script{create}
     */
    static Mesh* createLines(Vector3* points, unsigned int pointCount);

    /**
     * Creates a bounding box mesh when passed a BoundingBox.
     *
     * The mesh contains only position data using lines to connect the vertices.
     *
     * @param box The BoundingBox that will be used to create the mesh.
     * 
     * @return The newly created bounding box mesh.
     * @script{create}
     */
    static Mesh* createBoundingBox(const BoundingBox& box);

    /**
     * Returns a URL from which the mesh was loaded from.
     *
     * For meshes loaded from a Bundle, this URL will point
     * to the file and ID of the mesh within the bundle. For
     * all other meshes, an empty string will be returned.
     */
    const char* getUrl() const;

    /**
     * Gets the vertex format for the mesh.
     *
     * @return The vertex format.
     */
    const VertexFormat& getVertexFormat() const;

    /**
     * Gets the number of vertices in the mesh.
     *
     * @return The number of vertices in the mesh.
     */
    unsigned int getVertexCount() const;

    /**
     * Gets the size of a single vertex in the mesh.
     *
     * @return The size of 1 vertex in the mesh.
     */
    unsigned int getVertexSize() const;

    /**
     * Returns a handle to the vertex buffer for the mesh.
     *
     * @return The vertex buffer object handle.
     */
    VertexBufferHandle getVertexBuffer() const;

    /**
     * Determines if the mesh is dynamic.
     *
     * @return true if the mesh is dynamic; false otherwise.
     */
    bool isDynamic() const;

    /**
     * Returns the primitive type of the vertices in the mesh.
     *
     * The default primitive type for a Mesh is TRIANGLES.
     * 
     * @return The primitive type.
     *
     * @see setPrimitiveType(PrimitiveType)
     */
    PrimitiveType getPrimitiveType() const;

    /**
     * Sets the primitive type for the vertices in the mesh.
     *
     * The primitive type for a Mesh is only meaningful for meshes that do not
     * have any MeshParts. When there are no MeshParts associated with a mesh,
     * the Mesh is drawn as non-indexed geometry and the PrimitiveType of the Mesh
     * determines how the vertices are interpreted when drawn.
     *
     * @param type The new primitive type.
     */
    void setPrimitiveType(Mesh::PrimitiveType type);

    /**
     * Maps the vertex buffer for the specified access.
     *
     * Mapping vertex data causes a synchronizing issue. To avoid gpu idle
     * If GPU is still working with the buffer object, mapVertexBuffer will not
     * return until GPU finishes its job with the corresponding buffer object.
     *
     * To avoid waiting (idle), you can call first setVertexBuffer with NULL pointer,
     * then call mapVertexBuffer(). In this case, the previous data will be discarded
     * and mapVertexBuffer() returns a new allocated pointer immediately even if GPU is
     * still working with the previous data.
     *
     * However, this method is valid only if you want to update entire data set because
     * you discard the previous data. If you want to change only portion of data or to
     * read data, you better not release the previous data.
     *
     * After modifying the data of VBO, it must be unmapped the buffer object from the client's
     * memory. unmapVertexBuffer returns true if success. When it returns false, the contents of
     * vertex buffer become corrupted while the buffer was mapped. The corruption results from screen
     * resolution change or window system specific events. In this case, the data must be resubmitted.
     *
     * @return The mapped vertex buffer
     */
    void* mapVertexBuffer();

    /**
     * Unmaps the vertex buffer.
     *
     * @return false if unmapping buffer was unsuccessful
     */
    bool unmapVertexBuffer();

    /**
     * Sets the specified vertex data into the mapped vertex buffer.
     *
     * @param vertexData The vertex data to be set.
     * @param vertexStart The index of the starting vertex (0 by default).
     * @param vertexCount The number of vertices to be set (default is 0, for all vertices).
     */
    void setVertexData(const void* vertexData, unsigned int vertexStart = 0, unsigned int vertexCount = 0);

    /**
     * Creates and adds a new part of primitive data defining how the vertices are connected.
     *
     * @param primitiveType The type of primitive data to connect the indices as.
     * @param indexFormat The format of the indices. SHORT or INT.
     * @param indexCount The number of indices to be contained in the part.
     * @param dynamic true if the index data is dynamic; false otherwise.
     * 
     * @return The newly created/added mesh part.
     */
    MeshPart* addPart(PrimitiveType primitiveType, Mesh::IndexFormat indexFormat, unsigned int indexCount, bool dynamic = false);

    /**
     * Gets the number of mesh parts contained within the mesh.
     *
     * @return The number of mesh parts contained within the mesh.
     */
    unsigned int getPartCount() const;

    /**
     * Gets a MeshPart by index.
     * 
     * @param index The index of the MeshPart to get.
     * 
     * @return The MeshPart at the specified index.
     */
    MeshPart* getPart(unsigned int index);

    /**
     * Returns the bounding box for the points in this mesh.
     * 
     * Only meshes loaded from bundle files are imported with valid
     * bounding volumes. Programmatically created meshes will contain
     * empty bounding volumes until the setBoundingBox and/or
     * setBoundingSphere methods are called to specify the mesh's
     * local bounds.
     *
     * Meshes that are attached to a Model with a MeshSkin will have
     * a bounding volume that is not necessarily tight fighting on the
     * Mesh vertices. Instead, the bounding volume will be an approximation
     * that contains all possible vertex positions in all possible poses after
     * skinning is applied. This is necessary since skinning vertices 
     * result in vertex positions that lie outside the original mesh bounds
     * and could otherwise result in a bounding volume that does not fully
     * contain an animated/skinned mesh.
     *
     * @return The bounding box for the mesh.
     */
    const BoundingBox& getBoundingBox() const;

    /**
     * Sets the bounding box for this mesh.
     *
     * @param box The new bounding box for the mesh.
     */
    void setBoundingBox(const BoundingBox& box);

    /**
     * Returns the bounding sphere for the points in the mesh.
     *
     * Only meshes loaded from bundle files are imported with valid
     * bounding volumes. Programmatically created meshes will contain
     * empty bounding volumes until the setBoundingBox and/or
     * setBoundingSphere methods are called to specify the mesh's
     * local bounds.
     *
     * Meshes that are attached to a Model with a MeshSkin will have
     * a bounding volume that is not necessarily tight fighting on the
     * Mesh vertices. Instead, the bounding volume will be an approximation
     * that contains all possible vertex positions in all possible poses after
     * skinning is applied. This is necessary since skinning vertices 
     * result in vertex positions that lie outside the original mesh bounds
     * and could otherwise result in a bounding volume that does not fully
     * contain an animated/skinned mesh.
     *
     * @return The bounding sphere for the mesh.
     */
    const BoundingSphere& getBoundingSphere() const;

    /**
     * Sets the bounding sphere for this mesh.
     *
     * @param sphere The new bounding sphere for the mesh.
     */
    void setBoundingSphere(const BoundingSphere& sphere);

    /**
     * Destructor.
     */
    virtual ~Mesh();

private:

    /**
     * Constructor.
     */
    Mesh(const VertexFormat& vertexFormat);

    /**
     * Constructor.
     */
    Mesh(const Mesh& copy);

    /**
     * Hidden copy assignment operator.
     */
    Mesh& operator=(const Mesh&);

    std::string _url;
    const VertexFormat _vertexFormat;
    unsigned int _vertexCount;
    VertexBufferHandle _vertexBuffer;
    PrimitiveType _primitiveType;
    unsigned int _partCount;
    MeshPart** _parts;
    bool _dynamic;
    BoundingBox _boundingBox;
    BoundingSphere _boundingSphere;
};

}

#endif
