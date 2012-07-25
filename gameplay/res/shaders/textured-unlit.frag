#ifdef OPENGL_ES
precision highp float;
#endif

// Uniforms
uniform sampler2D u_diffuseTexture;     	// Diffuse texture
#if defined(TEXTURE_LIGHTMAP)
uniform sampler2D u_lightmapTexture;     	// Lightmap texture
#endif
#if defined(MODULATE_COLOR)
uniform vec4 u_modulateColor;               // Modulation color
#endif
#if defined(MODULATE_ALPHA)
uniform float u_modulateAlpha;              // Modulation alpha
#endif

// Inputs
varying vec2 v_texCoord;                	// Texture coordinate(u, v)

// Fragment Program
void main()
{
    // Sample the texture for the color
    gl_FragColor = texture2D(u_diffuseTexture, v_texCoord);
	#if defined(TEXTURE_LIGHTMAP)
	vec4 lightColor = texture2D(u_lightmapTexture, v_texCoord);
	gl_FragColor.a *= lightColor.a;
	#endif
	// Global color modulation
	#if defined(MODULATE_COLOR)
	gl_FragColor *= u_modulateColor;
	#endif
	#if defined(MODULATE_ALPHA)
    gl_FragColor.a *= u_modulateAlpha;
    #endif
}
