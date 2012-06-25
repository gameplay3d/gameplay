#ifdef OPENGL_ES
precision highp float;
#endif

// Uniforms
uniform sampler2D u_textureDiffuse;             // Diffuse texture.

#if defined(MODULATE_COLOR)
uniform vec4 u_modulateColor;               	// Modulation color
#endif
#if defined(MODULATE_ALPHA)
uniform float u_modulateAlpha;              	// Modulation alpha
#endif

// Inputs
varying vec3 v_normalVector;                    // NormalVector in view space.
varying vec2 v_texCoord;                        // Texture coordinate (u, v).

// Lighting
#include "lib/lighting.fsh"                     // Functions for lighting
#if defined(POINT_LIGHT)
#include "lib/lighting-point.fsh"
#elif defined(SPOT_LIGHT)
#include "lib/lighting-spot.fsh"
#else
#include "lib/lighting-directional.fsh"
#endif

void main()
{
    // Sample the texture for base color
    _baseColor = texture2D(u_textureDiffuse, v_texCoord);

    // Light the pixel
    gl_FragColor.a = _baseColor.a;
    gl_FragColor.rgb = getLitPixel();
	
	// Global color modulation
	#if defined(MODULATE_COLOR)
	gl_FragColor *= u_modulateColor;
	#endif
	#if defined(MODULATE_ALPHA)
    gl_FragColor.a *= u_modulateAlpha;
    #endif
}
