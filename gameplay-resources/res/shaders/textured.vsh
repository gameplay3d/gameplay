// Uniforms
uniform mat4 u_worldViewProjectionMatrix;    // Matrix to transform a position to clip space.

// Inputs
attribute vec4 a_position;                    // Vertex Position (x, y, z, w).
attribute vec2 a_texCoord;                    // Vertex Texture Coordinate (u, v).

// Outputs
varying vec2 v_texCoord;                    // Texture coordinate (u, v).

void main()
{
    // Transform position to clip space.
    gl_Position = u_worldViewProjectionMatrix * a_position;

    // Pass on texture coordinate to fragment shader.
    v_texCoord = a_texCoord;
}
