attribute vec2 a_position;
attribute vec2 a_texCoord;

varying vec2 vtexCoord;

void main()
{
    gl_Position = vec4(a_position, 0, 1);
    vtexCoord = a_texCoord;
}
