precision highp float;

varying vec3 vnormal;
varying vec2 vtexCoord;

uniform sampler2D texture;
uniform vec2 textureRepeat;
uniform vec2 textureTransform;

void main()
{
    gl_FragColor = texture2D(texture, vtexCoord * textureRepeat + textureTransform);
}
