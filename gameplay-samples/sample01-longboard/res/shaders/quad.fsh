precision highp float;

varying vec2 vtexCoord;

uniform sampler2D texture;

void main()
{
    gl_FragColor = texture2D(texture, vtexCoord);
}
