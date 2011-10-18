#include "Vertex.h"

namespace gameplay
{

Vertex::Vertex(void)
{
    reset();
}

Vertex::~Vertex(void)
{
}

void Vertex::reset()
{
    hasNormal = false;
    hasTangent = false;
    hasBinormal = false;
    hasTexCoord = false;
    hasColor = false;
    hasWeights = false;
}

unsigned int Vertex::byteSize() const
{
    unsigned int count = 3;
    if (hasNormal)
        count += 3;
    if (hasTangent)
        count += 3;
    if (hasBinormal)
        count += 3;
    if (hasTexCoord)
        count += 2;
    if (hasWeights)
        count += 8;
    return count * sizeof(float);
}

void Vertex::writeBinary(FILE* file) const
{
    position.writeBinary(file);
    if (hasNormal)
    {
        normal.writeBinary(file);
    }
    if (hasTangent)
    {
        tangent.writeBinary(file);
    }
    if (hasBinormal)
    {
        binormal.writeBinary(file);
    }
    if (hasTexCoord)
    {
        texCoord.writeBinary(file);
    }
    // TODO add vertex color?
    //if (hasColor)
    //{
    //    color.writeBinary(file);
    //}
    if (hasWeights)
    {
        blendWeights.writeBinary(file);
        blendIndices.writeBinary(file);
    }
}

void Vertex::writeText(FILE* file) const
{
    write("// position\n", file);
    position.writeText(file);
    if (hasNormal)
    {
        write("// normal\n", file);
        normal.writeText(file);
    }
    if (hasTangent)
    {
        write("// tanget\n", file);
        tangent.writeText(file);
    }
    if (hasBinormal)
    {
        write("// binormal\n", file);
        binormal.writeText(file);
    }
    if (hasTexCoord)
    {
        write("// texCoord\n", file);
        texCoord.writeText(file);
    }
    if (hasWeights)
    {
        write("// blendWeights\n", file);
        blendWeights.writeText(file);
        write("// blendIndices\n", file);
        blendIndices.writeText(file);
    }
}

}