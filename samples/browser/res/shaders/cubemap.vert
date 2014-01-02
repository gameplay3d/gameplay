// Attributes
attribute vec4 a_position;                         // Vertex Position  (x, y, z, w)
attribute vec3 a_normal;                           // Vertex Normal    (x, y, z)

// Uniforms
uniform mat4 u_worldViewProjectionMatrix;          // Matrix to transform a position to clip space.
uniform mat4 u_normalMatrix;                       // Matrix to transform a normal to view space
uniform mat4 u_worldViewMatrix;                    // Matrix to tranform a position to world-view space.
uniform vec3 u_cameraPosition;                     // Position of the camera in world-view space.

// Varyings
varying vec3 v_normalVector;                       // Normal vector in view space
varying vec3 v_eyeVector;                          // Eye vector in world-view space

void main()
{
    // Transform position to clip space.
    gl_Position = u_worldViewProjectionMatrix * a_position;

    // Transform normal to view space.
    v_normalVector = normalize((u_normalMatrix * vec4(a_normal.x, a_normal.y, a_normal.z, 0)).xyz);

    // Transform the camera to world-view space
    vec4 vert_pos = u_worldViewMatrix * a_position;
    v_eyeVector = u_cameraPosition - vert_pos.xyz;
}