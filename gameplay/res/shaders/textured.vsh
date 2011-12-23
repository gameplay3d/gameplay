// Uniforms
uniform mat4 u_worldViewProjectionMatrix;       // Matrix to transform a position to clip space.

// Inputs
attribute vec4 a_position;                      // Vertex Position (x, y, z, w).
attribute vec2 a_texCoord;                      // Vertex Texture Coordinate (u, v).

// Outputs
varying vec2 v_texCoord;                        // Texture coordinate (u, v).

#if defined(SKINNING)

attribute vec4 a_blendWeights;
attribute vec4 a_blendIndices;

// 32 4x3 matrices as an array of floats
uniform vec4 u_matrixPalette[SKINNING_JOINT_COUNT * 3];

// Common vectors.
vec4 _skinnedPosition;

void skinPosition(float blendWeight, int matrixIndex)
{
    vec4 tmp;

    tmp.x = dot(a_position, u_matrixPalette[matrixIndex]);
    tmp.y = dot(a_position, u_matrixPalette[matrixIndex + 1]);
    tmp.z = dot(a_position, u_matrixPalette[matrixIndex + 2]);
    tmp.w = a_position.w;

    _skinnedPosition += blendWeight * tmp;
}

vec4 getPosition()
{
    _skinnedPosition = vec4(0.0);

    // Transform position to view space using 
    // matrix palette with four matrices used to transform a vertex.

    float blendWeight = a_blendWeights[0];
    int matrixIndex = int (a_blendIndices[0]) * 3;
    skinPosition(blendWeight, matrixIndex);

    blendWeight = a_blendWeights[1];
    matrixIndex = int(a_blendIndices[1]) * 3;
    skinPosition(blendWeight, matrixIndex);

    blendWeight = a_blendWeights[2];
    matrixIndex = int(a_blendIndices[2]) * 3;
    skinPosition(blendWeight, matrixIndex);

    blendWeight = a_blendWeights[3];
    matrixIndex = int(a_blendIndices[3]) * 3;
    skinPosition(blendWeight, matrixIndex);

    return _skinnedPosition;    
}

#else

vec4 getPosition()
{
    return a_position;    
}

#endif

void main()
{
    vec4 position = getPosition();

    // Transform position to clip space.
    gl_Position = u_worldViewProjectionMatrix * position;

    // Pass on texture coordinate to fragment shader.
    v_texCoord = a_texCoord;
}
