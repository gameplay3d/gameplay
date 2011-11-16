#ifdef OPENGL_ES
precision highp float;
#endif

// Uniforms
uniform vec4 u_diffuseColor;        // Diffuse color

void main()
{
    gl_FragColor = u_diffuseColor;
}