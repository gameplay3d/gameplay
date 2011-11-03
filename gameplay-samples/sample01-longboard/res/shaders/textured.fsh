precision highp float;

varying vec3 v_normal;
varying vec2 v_texCoord;

uniform sampler2D u_texture;
uniform vec2 u_textureRepeat;
uniform vec2 u_textureTransform;

void main()
{
    gl_FragColor = texture2D(u_texture, v_texCoord * u_textureRepeat + u_textureTransform);
}
