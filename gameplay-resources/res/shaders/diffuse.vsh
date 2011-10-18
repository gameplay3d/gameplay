// Uniforms
uniform mat4 u_worldViewProjectionMatrix;            // Matrix to transform a position to clip space.
uniform mat4 u_inverseTransposeWorldViewMatrix;        // Matrix to transform a normal to view space.

// Inputs
attribute vec4 a_position;                            // Vertex Position (x, y, z, w).
attribute vec3 a_normal;                            // Vertex Normal (x, y, z).
attribute vec2 a_texCoord;                            // Vertex Texture Coordinate (u, v).

// Outputs
varying vec3 v_normalVector;                        // NormalVector in view space (Normalized).
varying vec2 v_texCoord;                            // Texture coordinate (u, v).

void main()
{
    // Transform position to clip space.
    gl_Position = u_worldViewProjectionMatrix * a_position;

    // Transform normal to view space.
    mat3 inverseTransposeWorldViewMatrix = mat3(u_inverseTransposeWorldViewMatrix);
    v_normalVector = normalize(inverseTransposeWorldViewMatrix * a_normal);

    // Pass on texture coordinate to fragment shader.
    v_texCoord = a_texCoord;
}
