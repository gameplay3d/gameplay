#ifdef OPENGL_ES
#ifdef OPENGL_ES_MEDIUMP
precision mediump float;
#else
precision highp float;
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
#ifdef BUMPED
#undef BUMPED
#endif

///////////////////////////////////////////////////////////
// Uniforms
uniform vec3 u_ambientColor;

uniform samplerCube u_cubeTexture;

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
uniform float u_spotLightRangeInverse[SPOT_LIGHT_COUNT];
uniform float u_spotLightInnerAngleCos[SPOT_LIGHT_COUNT];
uniform float u_spotLightOuterAngleCos[SPOT_LIGHT_COUNT];
uniform vec3 u_spotLightDirection[SPOT_LIGHT_COUNT];
#endif

#if defined(SPECULAR)
uniform float u_specularExponent;
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
varying vec3 v_normalVector;

varying vec3 v_cameraDirection;

#if defined(LIGHTING)

#if (POINT_LIGHT_COUNT > 0)
varying vec3 v_vertexToPointLightDirection[POINT_LIGHT_COUNT];
#endif

#if (SPOT_LIGHT_COUNT > 0)
varying vec3 v_vertexToSpotLightDirection[SPOT_LIGHT_COUNT];
#endif

#include "lighting.frag"

#endif

void main()
{
    vec3 N = normalize(v_normalVector);
    vec3 E = normalize(v_cameraDirection);
    vec3 R = reflect(E, N);

    _baseColor = textureCube(u_cubeTexture, R);

    gl_FragColor.a = _baseColor.a;

    #if defined(TEXTURE_DISCARD_ALPHA)
    if (gl_FragColor.a < 0.5)
        discard;
    #endif

    #if defined(LIGHTING)
    gl_FragColor.rgb = getLitPixel();
    #else
    gl_FragColor.rgb = _baseColor.rgb;
    #endif

    #if defined(MODULATE_COLOR)
    gl_FragColor *= u_modulateColor;
    #endif

    #if defined(MODULATE_ALPHA)
    gl_FragColor.a *= u_modulateAlpha;
    #endif
}