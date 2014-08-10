#ifdef OPENGL_ES
#ifdef GL_FRAGMENT_PRECISION_HIGH
precision highp float;
#else
precision mediump float;
#endif
#endif

#ifndef DIRECTIONAL_LIGHT_COUNT
#define DIRECTIONAL_LIGHT_COUNT 0
#endif
#ifndef SPOT_LIGHT_COUNT
#define SPOT_LIGHT_COUNT 0
#endif
#ifndef POINT_LIGHT_COUNT
#define POINT_LIGHT_COUNT 0
#endif
#if (DIRECTIONAL_LIGHT_COUNT > 0) || (POINT_LIGHT_COUNT > 0) || (SPOT_LIGHT_COUNT > 0)
#define LIGHTING
#endif
#if defined(CUBEMAP_REFLECTION)
#define CUBEMAP
#endif

///////////////////////////////////////////////////////////
// Uniforms
uniform vec3 u_ambientColor;
uniform vec4 u_diffuseColor;

#if defined(LIGHTMAP)
uniform sampler2D u_lightmapTexture;
#endif

#if defined(LIGHTING)

#if (DIRECTIONAL_LIGHT_COUNT > 0)
uniform vec3 u_directionalLightColor[DIRECTIONAL_LIGHT_COUNT];
uniform vec3 u_directionalLightDirection[DIRECTIONAL_LIGHT_COUNT];
#endif

#if (POINT_LIGHT_COUNT > 0)
uniform vec3 u_pointLightColor[POINT_LIGHT_COUNT];
uniform vec3 u_pointLightPosition[POINT_LIGHT_COUNT];
uniform float u_pointLightRangeInverse[POINT_LIGHT_COUNT];
#endif

#if (SPOT_LIGHT_COUNT > 0)
uniform vec3 u_spotLightColor[SPOT_LIGHT_COUNT];
uniform vec3 u_spotLightDirection[SPOT_LIGHT_COUNT];
uniform float u_spotLightRangeInverse[SPOT_LIGHT_COUNT];
uniform float u_spotLightInnerAngleCos[SPOT_LIGHT_COUNT];
uniform float u_spotLightOuterAngleCos[SPOT_LIGHT_COUNT];
#endif

#if defined(SPECULAR)
uniform float u_specularExponent;
#endif

#endif

#if defined(CUBEMAP)
uniform samplerCube u_cubemapTexture;
#if !defined(CUBEMAP_MIX)
uniform float u_cubemapMix;
#define CUBEMAP_MIX u_cubemapMix;
#endif
#endif

#if defined(MODULATE_COLOR)
uniform vec4 u_modulateColor;
#endif

#if defined(MODULATE_ALPHA)
uniform float u_modulateAlpha;
#endif

///////////////////////////////////////////////////////////
// Variables
vec4 _baseColor;

///////////////////////////////////////////////////////////
// Varyings
#if defined(VERTEX_COLOR)
varying vec3 v_color;
#endif

#if defined(LIGHTING) || defined(CUBEMAP)

#if defined(LIGHTING)
varying vec3 v_normalVector;
#endif

#if (POINT_LIGHT_COUNT > 0)
varying vec3 v_vertexToPointLightDirection[POINT_LIGHT_COUNT];
#endif

#if (SPOT_LIGHT_COUNT > 0)
varying vec3 v_vertexToSpotLightDirection[SPOT_LIGHT_COUNT];
#endif

#if defined(SPECULAR)
varying vec3 v_cameraDirection; 
#endif

#if defined(CUBEMAP)
varying vec3 v_normalWorldVector;
varying vec3 v_cameraWorldDirection;
#endif

#include "lighting.frag"

#endif


void main()
{
    #if defined(LIGHTING)

    #if defined(VERTEX_COLOR)
	_baseColor.rgb = v_color;
    #else
    _baseColor = u_diffuseColor;
	#endif
    
    gl_FragColor.a = _baseColor.a;
    gl_FragColor.rgb = getLitPixel();
    
    #else
    
    #if defined(VERTEX_COLOR)
    gl_FragColor.rgb = v_color;
    gl_FragColor.a = 1.0;
    #else
    gl_FragColor = u_diffuseColor;
    #endif
    
    #endif

	#if defined(LIGHTMAP)
	vec4 lightColor = texture2D(u_lightmapTexture, v_texCoord1);
	gl_FragColor.rgb *= lightColor.rgb;
	#endif

    #if defined(CUBEMAP)
    vec3 cubemapNormal = normalize(v_normalWorldVector);
    vec3 cubemapCameraDirection = normalize(v_cameraWorldDirection);
    vec3 cubemapDirection = reflect(-cubemapCameraDirection, cubemapNormal);
    
    gl_FragColor = mix(gl_FragColor, textureCube(u_cubemapTexture, cubemapDirection), CUBEMAP_MIX);
    #endif

	#if defined(MODULATE_COLOR)
    gl_FragColor *= u_modulateColor;
    #endif

	#if defined(MODULATE_ALPHA)
    gl_FragColor.a *= u_modulateAlpha;
    #endif
}
