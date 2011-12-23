#include "Base.h"
#include "MeshPart.h"

namespace gameplay
{

MeshPart::MeshPart(void) :
    _primitiveType(TRIANGLES),
    _indexFormat(INDEX16)
{
}

MeshPart::~MeshPart(void)
{
}

unsigned int MeshPart::getTypeId(void) const
{
    return MESHPART_ID;
}

const char* MeshPart::getElementName(void) const
{
    return "MeshPart";
}

void MeshPart::writeBinary(FILE* file)
{
    Object::writeBinary(file);

    write(_primitiveType, file);
    write((unsigned int)_indexFormat, file);

    // write the number of bytes
    write(indicesByteSize(), file);
    // for each index
    for (std::vector<unsigned int>::const_iterator i = _indices.begin(); i != _indices.end(); ++i)
    {
        writeBinaryIndex(*i, file);
    }
}

void MeshPart::writeText(FILE* file)
{
    fprintElementStart(file);
    fprintfElement(file, "primitiveType", _primitiveType);
    fprintfElement(file, "indexFormat", (unsigned int)_indexFormat);
    fprintfElement(file, "%d ", "indices", _indices);
    fprintElementEnd(file);
}

void MeshPart::addIndex(unsigned int index)
{
    updateIndexFormat(index);
    _indices.push_back(index);
}

size_t MeshPart::getIndicesCount() const
{
    return _indices.size();
}

unsigned int MeshPart::indicesByteSize() const
{
    return _indices.size() * indexFormatSize();
}

unsigned int MeshPart::indexFormatSize() const
{
    switch (_indexFormat)
    {
    case INDEX32:
        return 4;
    default: // INDEX16
        return 2;
    }
}

MeshPart::IndexFormat MeshPart::getIndexFormat() const
{
    return _indexFormat;
}

unsigned int MeshPart::getIndex(unsigned int i) const
{
    return _indices[i];
}

void MeshPart::writeBinaryIndex(unsigned int index, FILE* file)
{
    switch (_indexFormat)
    {
    case INDEX32:
        write(index, file);
        break;
    default: // INDEX16
        write((unsigned short)index, file);
        break;
    }
}

void MeshPart::updateIndexFormat(unsigned int newIndex)
{
    if (newIndex >= 65536)
    {
        _indexFormat = INDEX32;
    }
    else if (newIndex >= 256 && _indexFormat != INDEX32)
    {
        _indexFormat = INDEX16;
    }
}

}
