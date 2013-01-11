#ifdef OPENGL_ES
precision mediump float;
#endif

// Uniforms
uniform sampler2D u_texture;

// Inputs
varying vec2 v_texCoord;


float discretize(float f, float d)
{
    return floor(f*d + 0.5)/d;
}

vec2 discretize(vec2 v, float d)
{
    return vec2(discretize(v.x, d), discretize(v.y, d));
}

void main()
{
    vec2 texCoord = discretize(v_texCoord, 128.0);
    gl_FragColor = texture2D(u_texture, texCoord);
}

 
 


