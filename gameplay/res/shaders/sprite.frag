#ifdef OPENGL_ES
#ifdef OPENGL_ES_MEDIUMP
precision mediump float;
#else
precision highp float;
#endif
#endif

///////////////////////////////////////////////////////////
// Uniforms
uniform sampler2D u_texture;

///////////////////////////////////////////////////////////
// Varyings
varying vec2 v_texCoord;
varying vec4 v_color;


void main()
{
    gl_FragColor = v_color * texture2D(u_texture, v_texCoord);
}