#ifdef OPENGL_ES
precision highp float;
#endif

// Uniforms
uniform sampler2D u_textureDiffuse;     	// Diffuse texture

#if defined(MODULATE_COLOR)
uniform vec4 u_modulateColor;               // Modulation color
#endif
#if defined(MODULATE_ALPHA)
uniform float u_modulateAlpha;              // Modulation alpha
#endif

// Inputs
varying vec2 v_texCoord;                	// Texture coordinate(u, v)

void main()
{
    // Sample the texture for the color
    gl_FragColor = texture2D(u_textureDiffuse, v_texCoord);

	// Global color modulation
	#if defined(MODULATE_COLOR)
	gl_FragColor *= u_modulateColor;
	#endif
	#if defined(MODULATE_ALPHA)
    gl_FragColor.a *= u_modulateAlpha;
    #endif
}
