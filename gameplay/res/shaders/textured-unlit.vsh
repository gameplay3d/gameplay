// Uniforms
uniform mat4 u_worldViewProjectionMatrix;       // Matrix to transform a position to clip space
#if defined(TEXTURE_REPEAT)
uniform vec2 u_textureRepeat;
#endif
#if defined(TEXTURE_OFFSET)
uniform vec2 u_textureOffset;
#endif

// Inputs
attribute vec4 a_position;                      // Vertex Position              (x, y, z, w)
attribute vec2 a_texCoord;                      // Vertex Texture Coordinate    (u, v)

// Outputs
varying vec2 v_texCoord;                        // Output Texture Coordinate    (u, v)

// Attribute Accessors (getPosition(), getNormal(), etc.)
#if defined(SKINNING)
#include "lib/attributes-skinning.vsh"
#else
#include "lib/attributes.vsh" 
#endif

void main()
{
    // Get the vertex position
    vec4 position = getPosition();

    // Transform position to clip space.
    gl_Position = u_worldViewProjectionMatrix * position;

    // Texture transformation.
    v_texCoord = a_texCoord;
    #if defined(TEXTURE_REPEAT)
    v_texCoord *= u_textureRepeat;
    #endif
    #if defined(TEXTURE_OFFSET)
    v_texCoord += u_textureOffset;
    #endif
}
