// Uniforms
uniform mat4 u_worldViewProjectionMatrix;           // Matrix to transform a position to clip space.

// Inputs
attribute vec4 a_position;                          // Vertex Position          (x, y, z, w)
#if defined(VERTEX_COLOR)
attribute vec3 a_color;                             // Vertex Color             (r, g, b)
varying vec3 v_color;								// Output Vertex color      (r, g, b)
#endif

#if defined(SKINNING)
#include "lib/attributes-skinning.vsh"
#else
#include "lib/attributes.vsh" 
#endif

void main()
{
    // Get the vertex position
    vec4 position = getPosition();
    
    // Transform position to clip space.a
    gl_Position = u_worldViewProjectionMatrix *  position;
    
     // Pass on vertex color to fragment shader
    #if defined(VERTEX_COLOR)
	v_color = a_color;
    #endif
}