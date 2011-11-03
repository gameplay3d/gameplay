uniform mat4 u_worldViewProjectionMatrix;

attribute vec3 a_position;
attribute vec3 a_normal;
attribute vec2 a_texCoord;

varying vec3 v_normal;
varying vec2 v_texCoord;

void main()
{
    gl_Position = u_worldViewProjectionMatrix * vec4(a_position, 1);
    v_normal = a_normal;
    v_texCoord = a_texCoord;
}
