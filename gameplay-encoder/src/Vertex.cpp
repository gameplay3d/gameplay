#include "Vertex.h"

namespace gameplay
{

Vertex::Vertex(void)
    : hasNormal(false), hasTangent(false), hasBinormal(false), hasTexCoord(false), hasColor(false), hasWeights(false)
{
}

Vertex::~Vertex(void)
{
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
    writeVectorBinary(position, file);
    if (hasNormal)
    {
        writeVectorBinary(normal, file);
    }
    if (hasTangent)
    {
        writeVectorBinary(tangent, file);
    }
    if (hasBinormal)
    {
        writeVectorBinary(binormal, file);
    }
    if (hasTexCoord)
    {
        writeVectorBinary(texCoord, file);
    }
    // TODO add vertex color?
    //if (hasColor)
    //{
    //    writeVectorBinary(color, file);
    //}
    if (hasWeights)
    {
        writeVectorBinary(blendWeights, file);
        writeVectorBinary(blendIndices, file);
    }
}

void Vertex::writeText(FILE* file) const
{
    write("// position\n", file);
    writeVectorText(position, file);
    if (hasNormal)
    {
        write("// normal\n", file);
        writeVectorText(normal, file);
    }
    if (hasTangent)
    {
        write("// tanget\n", file);
        writeVectorText(tangent, file);
    }
    if (hasBinormal)
    {
        write("// binormal\n", file);
        writeVectorText(binormal, file);
    }
    if (hasTexCoord)
    {
        write("// texCoord\n", file);
        writeVectorText(texCoord, file);
    }
    if (hasWeights)
    {
        write("// blendWeights\n", file);
        writeVectorText(blendWeights, file);
        write("// blendIndices\n", file);
        writeVectorText(blendIndices, file);
    }
}

void Vertex::normalizeBlendWeight()
{
    float total = blendWeights.x + blendWeights.y + blendWeights.z + blendWeights.w;
    if (total > 0.0f)
    {
        blendWeights.x = blendWeights.x / total;
        blendWeights.y = blendWeights.y / total;
        blendWeights.z = blendWeights.z / total;
        blendWeights.w = blendWeights.w / total;
    }   
}

}