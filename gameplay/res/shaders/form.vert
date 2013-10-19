///////////////////////////////////////////////////////////
// Attributes
attribute vec3 a_position;
attribute vec2 a_texCoord;

///////////////////////////////////////////////////////////
// Uniforms
uniform mat4 u_worldViewProjectionMatrix;

///////////////////////////////////////////////////////////
// Varyings
varying vec2 v_texCoord;


void main()
{
    gl_Position = u_worldViewProjectionMatrix * vec4(a_position, 1);
    v_texCoord = a_texCoord;
}
