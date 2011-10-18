uniform mat4 worldViewProjection;

attribute vec3 a_position;
attribute vec3 a_normal;
attribute vec2 a_texCoord;

varying vec3 vnormal;
varying vec2 vtexCoord;

void main()
{
    gl_Position = worldViewProjection * vec4(a_position, 1);
    vnormal = a_normal;
    vtexCoord = a_texCoord;
}
