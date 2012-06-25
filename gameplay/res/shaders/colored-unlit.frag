#ifdef OPENGL_ES
precision highp float;
#endif

// Inputs
#if defined(VERTEX_COLOR)
varying vec3 v_color;						// Input Vertex color ( r g b )
#endif

uniform vec4 u_baseColor;               	// Base color

// Uniforms
#if defined(MODULATE_COLOR)
uniform vec4 u_modulateColor;               // Modulation color
#endif
#if defined(MODULTE_ALPHA)
uniform float u_modulateAlpha;              // Modulation alpha
#endif

// Fragment program
void main()
{
    // Set base diffuse color
    #if defined(VERTEX_COLOR)
	gl_FragColor.rgb = v_color;
	#else
	gl_FragColor = u_baseColor;
    #endif
	
	// Global color modulation
	#if defined(MODULATE_COLOR)
	gl_FragColor *= u_modulateColor;
	#endif
	#if defined(MODULTE_ALPHA)
    gl_FragColor.a *= u_modulateAlpha;
    #endif
}