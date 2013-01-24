// Attributes
attribute vec4 a_position;									// Vertex Position							(x, y, z, w)
#if defined(TEXTURE_LIGHTMAP)
attribute vec2 a_texCoord;                                  // Texture Coordinate (for lightmapping)
#endif
#if defined(SKINNING)
attribute vec4 a_blendWeights;								// Vertex blend weight, up to 4				(0, 1, 2, 3) 
attribute vec4 a_blendIndices;								// Vertex blend index int u_matrixPalette	(0, 1, 2, 3)
#endif
#if defined(VERTEX_COLOR)
attribute vec3 a_color;										// Vertex Color								(r, g, b)
#endif

// Uniforms
uniform mat4 u_worldViewProjectionMatrix;					// Matrix to transform a position to clip space.
#if defined(SKINNING)
uniform vec4 u_matrixPalette[SKINNING_JOINT_COUNT * 3];		// Array of 4x3 matrices as an array of floats
#endif

// Varyings
#if defined(TEXTURE_LIGHTMAP)
varying vec2 v_texCoord;                                    // Output Texture Coordinate
#endif
#if defined(VERTEX_COLOR)
varying vec3 v_color;										// Output Vertex color						(r, g, b)
#endif

// Skinning
#if defined(SKINNING)
#include "skinning.vert"
#else
#include "skinning-none.vert" 
#endif


void main()
{
    // Get the vertex position
    vec4 position = getPosition();
    
    // Transform position to clip space.a
    gl_Position = u_worldViewProjectionMatrix *  position;
    
    // Pass lightmap tex coord to fragment shader
    #if defined(TEXTURE_LIGHTMAP)
    v_texCoord0 = a_texCoord0;
    #endif

     // Pass on vertex color to fragment shader
    #if defined(VERTEX_COLOR)
	v_color = a_color;
    #endif
}
