// Inputs
attribute vec4 a_position;									// Vertex Position							(x, y, z, w)
attribute vec2 a_texCoord0;									// Vertex Texture Coordinate				(u, v)
#if defined(TEXCOORD1)
attribute vec2 a_texCoord1;                                 // Second tex coord for multi-texturing
#endif
#if defined(SKINNING)
attribute vec4 a_blendWeights;								// Vertex blend weight, up to 4				(0, 1, 2, 3) 
attribute vec4 a_blendIndices;								// Vertex blend index int u_matrixPalette	(0, 1, 2, 3)
#endif

// Uniforms
uniform mat4 u_worldViewProjectionMatrix;					// Matrix to transform a position to clip space
#if defined(SKINNING)
uniform vec4 u_matrixPalette[SKINNING_JOINT_COUNT * 3];		// Array of 4x3 matrices
#endif
#if defined(TEXTURE_REPEAT)
uniform vec2 u_textureRepeat;								// Texture repeat for tiling
#endif
#if defined(TEXTURE_OFFSET)
uniform vec2 u_textureOffset;								// Texture offset
#endif

// Outputs
varying vec2 v_texCoord0;									// Texture Coordinate
#if defined(TEXCOORD1)
varying vec2 v_texCoord1;                                   // Second tex coord for multi-texturing
#endif

// Vertex attribute accessors
#if defined(SKINNING)
#include "lib/attributes-skinning.vert"
#else
#include "lib/attributes.vert" 
#endif

// Vertex Program
void main()
{
    // Get the vertex position
    vec4 position = getPosition();

    // Transform position to clip space.
    gl_Position = u_worldViewProjectionMatrix * position;

    // Texture transformation.
    v_texCoord0 = a_texCoord0;
    #if defined(TEXCOORD1)
    v_texCoord1 = a_texCoord1;
    #endif
    #if defined(TEXTURE_REPEAT)
    v_texCoord0 *= u_textureRepeat;
    #endif
    #if defined(TEXTURE_OFFSET)
    v_texCoord0 += u_textureOffset;
    #endif
}
