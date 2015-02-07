#ifdef OPENGL_ES
#ifdef GL_FRAGMENT_PRECISION_HIGH
precision highp float;
#else
precision mediump float;
#endif
#endif

const float AMPLITUDE = 0.008;
const float FREQUENCY = 40.0;
const float SPEED = 0.5;

///////////////////////////////////////////////////////////
// Uniforms
uniform sampler2D u_texture;
uniform sampler2D u_texture_noise;
uniform float u_time;

///////////////////////////////////////////////////////////
// Varyings
varying vec2 v_texCoord;
varying vec4 v_color;


void main()
{
    vec2 displacement = texture2D (u_texture_noise, v_texCoord / 6.0).xy;
    float t = v_texCoord.y + displacement.y * 0.1 - 0.10 + (sin (v_texCoord.x * FREQUENCY + (u_time * SPEED)) * AMPLITUDE);
    gl_FragColor = v_color * texture2D(u_texture, vec2(v_texCoord.x, t));
}

