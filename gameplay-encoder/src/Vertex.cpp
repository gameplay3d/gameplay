#include "Base.h"
#include "Vertex.h"

namespace gameplay
{

Vertex::Vertex(void)
    : hasNormal(false), hasTangent(false), hasBinormal(false), hasDiffuse(false), hasWeights(false)
{
    for (unsigned int i = 0; i < MAX_UV_SETS; ++i)
        hasTexCoord[i] = false;
}

Vertex::~Vertex(void)
{
}

unsigned int Vertex::byteSize() const
{
    unsigned int count = POSITION_COUNT;
    if (hasNormal)
        count += NORMAL_COUNT;
    if (hasTangent)
        count += TANGENT_COUNT;
    if (hasBinormal)
        count += BINORMAL_COUNT;
    for (unsigned int i = 0; i < MAX_UV_SETS; ++i)
    {
        if (hasTexCoord[i])
            count += TEXCOORD_COUNT;
    }
    if (hasWeights)
        count += BLEND_WEIGHTS_COUNT + BLEND_INDICES_COUNT;
    if (hasDiffuse)
        count += DIFFUSE_COUNT;
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
    for (unsigned int i = 0; i < MAX_UV_SETS; ++i)
    {
        if (hasTexCoord[i])
        {
            writeVectorBinary(texCoord[i], file);
        }
    }
    if (hasDiffuse)
    {
        writeVectorBinary(diffuse, file);
    }
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
    for (unsigned int i = 0; i < MAX_UV_SETS; ++i)
    {
        if (hasTexCoord[i])
        {
            write("// texCoord\n", file);
            writeVectorText(texCoord[i], file);
        }
    }
    if (hasDiffuse)
    {
        write("// diffuse\n", file);
        writeVectorText(diffuse, file);
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
