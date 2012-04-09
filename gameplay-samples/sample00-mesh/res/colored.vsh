// Uniforms
uniform mat4 u_worldViewProjectionMatrix;

// Attributes
attribute vec4 a_position;
attribute vec4 a_color;

varying vec4 v_color;

void main()
{
    // Transform position to clip space.
    gl_Position = u_worldViewProjectionMatrix * a_position;

	v_color = a_color;
}