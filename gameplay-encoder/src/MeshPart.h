#ifndef MESHPART_H_
#define MESHPART_H_

#include "Base.h"
#include "Object.h"
#include "Vertex.h"

namespace gameplay
{

class MeshPart : public Object
{
public:

    enum PrimitiveType
    {
        TRIANGLES = 4,      // GL_TRIANGLES
        TRIANGLE_STRIP = 5, // GL_TRIANGLE_STRIP
        LINES = 1,          // GL_LINES
        LINE_STRIP = 3,     // GL_LINE_STRIP
        POINTS = 0          // GL_POINTS
    };

    enum IndexFormat
    {
        INDEX16 = 0x1403, // GL_UNSIGNED_SHORT
        INDEX32 = 0x1405  // GL_UNSIGNED_INT
    };

    /**
     * Constructor.
     */
    MeshPart(void);

    /**
     * Destructor.
     */
    virtual ~MeshPart(void);

    virtual unsigned int getTypeId(void) const;
    virtual const char* getElementName(void) const;
    virtual void writeBinary(FILE* file);
    virtual void writeText(FILE* file);

    /**
     * Adds an index to the list of indices.
     */
    void addIndex(unsigned int index);

    /**
     * Returns the number of indices.
     */
    size_t getIndicesCount() const;

    /**
     * Returns the index format.
     */
    IndexFormat getIndexFormat() const;

    /**
     * Gets the value of the index at the specificied location.
     */
    unsigned int getIndex(unsigned int i) const;

private:

    /**
     * Returns the size of the indices array in bytes.
     */
    unsigned int indicesByteSize() const;

    /**
     * Returns the size of an index.
     */
    unsigned int indexFormatSize() const;

    /**
     * Updates the index format if newIndex is larger than 256 or 65536.
     */
    void updateIndexFormat(unsigned int newIndex);

    /**
     * Writes the index to the binary file stream.
     * The number of bytes written depends on indexFormat.
     */
    void writeBinaryIndex(unsigned int index, FILE* file);

private:

    unsigned int _primitiveType;
    IndexFormat _indexFormat;
    std::vector<unsigned int> _indices;
};

}

#endif
