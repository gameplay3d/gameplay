#ifndef VERTEX_H_
#define VERTEX_H_

#include "FileIO.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

namespace gameplay
{

class Vertex
{
public:

    static const unsigned int POSITION_COUNT = 3;
    static const unsigned int NORMAL_COUNT = 3;
    static const unsigned int TANGENT_COUNT = 3;
    static const unsigned int BINORMAL_COUNT = 3;
    static const unsigned int TEXCOORD_COUNT = 2;
    static const unsigned int DIFFUSE_COUNT = 4;
    static const unsigned int BLEND_WEIGHTS_COUNT = 4;
    static const unsigned int BLEND_INDICES_COUNT = 4;

    /**
     * Constructor.
     */
    Vertex(void);

    /**
     * Destructor.
     */
    ~Vertex(void);

    Vector3 position;
    Vector3 normal;
    Vector3 tangent;
    Vector3 binormal;
    Vector2 texCoord;
    Vector4 diffuse;

    Vector4 blendWeights;
    Vector4 blendIndices;

    bool hasNormal, hasTangent, hasBinormal, hasTexCoord, hasDiffuse, hasWeights;

    inline bool operator<(const Vertex& v) const
    {
        if (position == v.position)
        {
            if (normal == v.normal)
            {
                if (tangent == v.tangent)
                {
                    if (binormal == v.binormal)
                    {
                        if (texCoord == v.texCoord)
                        {
                            if (diffuse == v.diffuse)
                            {
                                if (blendWeights == v.blendWeights)
                                {
                                    if (blendIndices == v.blendIndices)
                                    {
                                        return false;
                                    }
                                    return blendIndices < v.blendIndices;
                                }
                                return blendWeights < v.blendWeights;
                            }
                            return diffuse < v.diffuse;
                        }
                        return texCoord < v.texCoord;
                    }
                    return binormal < v.binormal;
                }
                return tangent < v.tangent;
            }
            return normal < v.normal;
        }
        return position < v.position;
    }

    inline bool operator==(const Vertex& v) const
    {
        return position==v.position && normal==v.normal && tangent==v.tangent && binormal==v.binormal && texCoord==v.texCoord &&
            diffuse==v.diffuse && blendWeights==v.blendWeights && blendIndices==v.blendIndices;
    }

    /**
     * Returns the size of this vertex in bytes.
     */
    unsigned int byteSize() const;

    /**
     * Writes this vertex to the binary file stream.
     */
    void writeBinary(FILE* file) const;

    /**
     * Writes this vertex to a text file stream.
     */
    void writeText(FILE* file) const;

    /**
     * Normalizes the blend weights of this vertex so that they add up to 1.0.
     */
    void normalizeBlendWeight();
};
}

#endif
