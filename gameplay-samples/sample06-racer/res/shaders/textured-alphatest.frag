#ifdef OPENGL_ES
precision highp float;
#endif

// Uniforms
uniform sampler2D u_diffuseTexture;     	// Diffuse texture

// Inputs
varying vec2 v_texCoord0;                	// Texture coordinate(u, v)

// Fragment Program
void main()
{
    // Sample the texture for the color
    gl_FragColor = texture2D(u_diffuseTexture, v_texCoord0);

    if (gl_FragColor.a < 0.5)
        discard;
}
