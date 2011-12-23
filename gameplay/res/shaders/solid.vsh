// Uniforms
uniform mat4 u_worldViewProjectionMatrix;        // Matrix to transform a position to clip space.

// Inputs
attribute vec4 a_position;                        // Vertex Position (x, y, z, w)

void main()
{
    gl_Position = u_worldViewProjectionMatrix *  a_position;
}