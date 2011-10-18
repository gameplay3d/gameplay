// Uniforms
uniform mat4 u_worldViewProjectionMatrix;            // Matrix to transform a position to clip space.
uniform mat4 u_inverseTransposeWorldViewMatrix;        // Matrix to transform a normal to view space.
uniform mat4 u_worldMatrix;                            // Matrix to tranform a position to world space.
uniform vec3 u_cameraPosition;                        // Position of the camera.

// Inputs
attribute vec4 a_position;                            // Vertex Position (x, y, z, w)
attribute vec3 a_normal;                            // Vertex Normal (x, y, z)
attribute vec2 a_texCoord;                            // Vertex Texture Coordinate (u, v)

// Outputs
varying vec3 v_normalVector;                        // NormalVector in view space.
varying vec2 v_texCoord;                            // Texture coordinate (u, v).
varying vec3 v_cameraDirection;                     // Camera direction

void main()
{
    // Transform position to clip space.
    gl_Position = u_worldViewProjectionMatrix * a_position;

    // Transform normal to view space.
    mat3 inverseTransposeWorldViewMatrix = mat3(u_inverseTransposeWorldViewMatrix);
    v_normalVector = inverseTransposeWorldViewMatrix * a_normal;

    // Compute the camera direction.
    vec4 positionWorldSpace = u_worldMatrix * a_position;
    v_cameraDirection = u_cameraPosition - positionWorldSpace.xyz;

    // Pass on the texture coordinates to Fragment shader.
    v_texCoord = a_texCoord;
}
