#ifdef OPENGL_ES
precision highp float;
#endif

// Uniforms
uniform sampler2D u_texture;

// Varyings
varying vec2 v_texCoord;


void main()
{
    gl_FragColor = texture2D(u_texture, v_texCoord);
}
