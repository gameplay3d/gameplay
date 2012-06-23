#ifdef OPENGL_ES
precision highp float;
#endif

// Uniforms
uniform vec3 u_lightColor;                  // Light color
uniform vec3 u_ambientColor;                // Ambient color

uniform vec4 u_baseColor;               	// Modulation color

#if defined(MODULATE_COLOR)
uniform vec4 u_modulateColor;               // Modulation color
#endif
#if defined(MODULATE_ALPHA)
uniform float u_tintAlpha;              	// Tint alpha
#endif

// Inputs
varying vec3 v_normalVector;                // Normal vector in view space
#if defined(VERTEX_COLOR)
varying vec3 v_color;						// Vertex color
#endif

// Lighting
#include "lib/lighting.fsh"
#if defined(POINT_LIGHT)
#include "lib/lighting-point.fsh"
#elif defined(SPOT_LIGHT)
#include "lib/lighting-spot.fsh"
#else
#include "lib/lighting-directional.fsh"
#endif

void main()
{
    // Set base diffuse color
    #if defined(VERTEX_COLOR)
	_baseColor.rgb = v_color;
	#else
	_baseColor = u_baseColor;
	#endif

    // Light the pixel
    gl_FragColor.a = _baseColor.a;
    gl_FragColor.rgb = getLitPixel();
    
	#if defined(MODULATE_COLOR)
    gl_FragColor.a *= u_modulateColor;
    #endif
	#if defined(MODULATE_ALPHA)
    gl_FragColor.a *= u_modulateAlpha;
    #endif
}
