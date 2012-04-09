#ifdef OPENGL_ES
precision highp float;
#endif

// Uniforms
uniform vec4 u_diffuseColor;        // Diffuse color
#if defined(GLOBAL_ALPHA)
uniform float u_globalAlpha;        // Global alpha value
#endif

void main()
{
    gl_FragColor = u_diffuseColor;

#if defined(GLOBAL_ALPHA)
    gl_FragColor.a *= u_globalAlpha;
#endif
}