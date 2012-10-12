#ifdef OPENGL_ES
precision highp float;
#endif

// Inputs
#if defined(VERTEX_COLOR)
varying vec3 v_color;						// Input Vertex color ( r g b )
#endif
#if defined(TEXTURE_LIGHTMAP)
varying vec2 v_texCoord;
#endif

// Uniforms
uniform vec4 u_diffuseColor;               	// Diffuse color
#if defined(TEXTURE_LIGHTMAP)
uniform sampler2D u_lightmapTexture;     	// Lightmap texture
#endif
#if defined(MODULATE_COLOR)
uniform vec4 u_modulateColor;               // Modulation color
#endif
#if defined(MODULATE_ALPHA)
uniform float u_modulateAlpha;              // Modulation alpha
#endif

// Fragment program
void main()
{
    // Set base diffuse color
    #if defined(VERTEX_COLOR)
	gl_FragColor.rgb = v_color;
	#else
	gl_FragColor = u_diffuseColor;
    #endif
	#if defined(TEXTURE_LIGHTMAP)
	vec4 lightColor = texture2D(u_lightmapTexture, v_texCoord);
	gl_FragColor.rgb *= lightColor.rgb;
	#endif
	// Global color modulation
	#if defined(MODULATE_COLOR)
	gl_FragColor *= u_modulateColor;
	#endif
	#if defined(MODULATE_ALPHA)
    gl_FragColor.a *= u_modulateAlpha;
    #endif
}
