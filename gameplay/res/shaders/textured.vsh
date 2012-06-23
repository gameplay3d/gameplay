// Uniforms
uniform mat4 u_worldViewProjectionMatrix;           // Matrix to transform a position to clip space
uniform mat4 u_inverseTransposeWorldViewMatrix;     // Matrix to transform a normal to view space
#if defined(TEXTURE_REPEAT)
uniform vec2 u_textureRepeat;
#endif
#if defined(TEXTURE_OFFSET)
uniform vec2 u_textureOffset;
#endif

// Inputs
attribute vec4 a_position;                          // Vertex position                      (x, y, z, w)
attribute vec3 a_normal;                            // Vertex normal                        (x, y, z)
attribute vec2 a_texCoord;                          // Vertex texture coordinate            (u, v)

// Outputs
varying vec3 v_normalVector;                        // Output normal vector in view space   (x, y, z)
varying vec2 v_texCoord;                            // Output texture coordinate            (u, v)

// Lighting
#include "lib/lighting.vsh"
#if defined(POINT_LIGHT)
#include "lib/lighting-point.vsh"
#elif defined(SPOT_LIGHT)
#include "lib/lighting-spot.vsh"
#else
#include "lib/lighting-directional.vsh"
#endif

// Attribute Accessors (getPosition(), getNormal(), etc.)
#if defined(SKINNING)
#include "lib/attributes-skinning.vsh"
#else
#include "lib/attributes.vsh" 
#endif

void main()
{
    // Get the position and normal
    vec4 position = getPosition();
    vec3 normal = getNormal();

    // Transform position to clip space.
    gl_Position = u_worldViewProjectionMatrix * position;

    // Transform normal to view space.
    mat3 normalMatrix = mat3(u_inverseTransposeWorldViewMatrix[0].xyz, u_inverseTransposeWorldViewMatrix[1].xyz, u_inverseTransposeWorldViewMatrix[2].xyz);
    v_normalVector = normalMatrix * normal;

    // Apply light.
    applyLight(position);
    
    // Texture transformation
    v_texCoord = a_texCoord;
    #if defined(TEXTURE_REPEAT)
    v_texCoord *= u_textureRepeat;
    #endif
    #if defined(TEXTURE_OFFSET)
    v_texCoord += u_textureOffset;
    #endif
}
