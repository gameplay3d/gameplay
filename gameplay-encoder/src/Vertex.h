#ifndef VERTEX_H_
#define VERTEX_H_

#include "FileIO.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

// Maximum number of supported UV sets
#define MAX_UV_SETS 8

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
    Vector2 texCoord[MAX_UV_SETS];
    Vector4 diffuse;

    Vector4 blendWeights;
    Vector4 blendIndices;

    bool hasNormal, hasTangent, hasBinormal, hasTexCoord[MAX_UV_SETS], hasDiffuse, hasWeights;

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
                        if (diffuse == v.diffuse)
                        {
                            if (blendWeights == v.blendWeights)
                            {
                                if (blendIndices == v.blendIndices)
                                {
                                    for (unsigned int i = 0; i < MAX_UV_SETS; ++i)
                                    {
                                        if (!(texCoord[i] == v.texCoord[i]))
                                            return texCoord[i] < v.texCoord[i];
                                    }
                                    return false;
                                }
                                return blendIndices < v.blendIndices;
                            }
                            return blendWeights < v.blendWeights;
                        }
                        return diffuse < v.diffuse;
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
        for (unsigned int i = 0; i < MAX_UV_SETS; ++i)
        {
            if (!(texCoord[i] == v.texCoord[i]))
                return false;
        }
        return position==v.position && normal==v.normal && tangent==v.tangent && binormal==v.binormal &&
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
