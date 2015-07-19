// Inputs
attribute vec4 a_position;

// Uniforms
uniform mat4 u_worldViewProjectionMatrix;
uniform vec2 u_depthRange; // x: nearPlane; y: 1.0 / (farPlane - nearPlane);

// Outputs
varying float v_depth;

void main()
{
    vec4 position = a_position;
    gl_Position = u_worldViewProjectionMatrix * position;
    v_depth = (gl_Position.z - u_depthRange.x) * u_depthRange.y;
}