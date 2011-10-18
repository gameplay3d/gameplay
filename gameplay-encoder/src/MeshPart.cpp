#include "MeshPart.h"

namespace gameplay
{

MeshPart::MeshPart(void) :
    primitiveType(TRIANGLES),
    indexFormat(INDEX8)
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

    write(primitiveType, file);
    write(indexFormat, file);

    // write the number of bytes
    write(indicesByteSize(), file);
    // for each index
    for (std::vector<unsigned int>::const_iterator i = indices.begin(); i != indices.end(); i++)
    {
        writeBinaryIndex(*i, file);
    }
}
void MeshPart::writeText(FILE* file)
{
    fprintElementStart(file);
    fprintfElement(file, "primitiveType", primitiveType);
    fprintfElement(file, "indexFormat", indexFormat);
    fprintfElement(file, "%d ", "indices", indices);
    fprintElementEnd(file);
}

void MeshPart::addIndex(unsigned int index)
{
    updateIndexFormat(index);
    indices.push_back(index);
}

size_t MeshPart::getIndicesCount() const
{
    return indices.size();
}

unsigned int MeshPart::indicesByteSize() const
{
    return indices.size() * indexFormatSize();
}

unsigned int MeshPart::indexFormatSize() const
{
    switch (indexFormat)
    {
    case INDEX32:
        return 4;
    case INDEX16:
        return 2;
    case INDEX8:
    default:
        return 1;
    }
}

void MeshPart::writeBinaryIndex(unsigned int index, FILE* file)
{
    switch (indexFormat)
    {
    case INDEX32:
        write(index, file);
        break;
    case INDEX16:
        write((unsigned short)index, file);
        break;
    case INDEX8:
    default:
        write((unsigned char)index, file);
        break;
    }
}

void MeshPart::updateIndexFormat(unsigned int newIndex)
{
    if (newIndex >= 65536)
        indexFormat = INDEX32;
    else if (newIndex >= 256 && indexFormat != INDEX32)
        indexFormat = INDEX16;
}

}