// Uniforms
uniform mat4 u_worldViewProjectionMatrix;           // Matrix to transform a position to clip space.
uniform mat4 u_inverseTransposeWorldViewMatrix;     // Matrix to transform a normal to view space.

// Inputs
attribute vec4 a_position;                          // Vertex Position (x, y, z, w)
attribute vec3 a_normal;                            // Vertex Normal (x, y, z)
#if defined(VERTEX_COLOR)
attribute vec3 a_color;                             // Vertex Color (r, g, b)
varying vec3 v_color;								// Output Vertex color 
#endif

// Outputs
varying vec3 v_normalVector;                        // Normal vector in view space.

#include "lib/lighting.vsh"
#if defined(POINT_LIGHT)
#include "lib/lighting-point.vsh"
#elif defined(SPOT_LIGHT)
#include "lib/lighting-spot.vsh"
#else
#include "lib/lighting-directional.vsh"
#endif

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
    mat3 inverseTransposeWorldViewMatrix = mat3(u_inverseTransposeWorldViewMatrix[0].xyz, u_inverseTransposeWorldViewMatrix[1].xyz, u_inverseTransposeWorldViewMatrix[2].xyz);
    v_normalVector = inverseTransposeWorldViewMatrix * normal;

    // Apply light.
    applyLight(position);
    
    // Pass the vertex color to fragment shader
    #if defined(VERTEX_COLOR)
	v_color = a_color;
    #endif
}
