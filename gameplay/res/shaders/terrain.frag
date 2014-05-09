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

///////////////////////////////////////////////////////////
// Uniforms
uniform vec3 u_ambientColor; 

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

#if defined (NORMAL_MAP)
uniform sampler2D u_normalMap;
uniform mat4 u_normalMatrix;
#endif

#endif

#if defined(DEBUG_PATCHES)
uniform float u_row;
uniform float u_column;
#endif

#if (LAYER_COUNT > 0)
uniform sampler2D u_surfaceLayerMaps[SAMPLER_COUNT];
#endif

///////////////////////////////////////////////////////////
// Variables
vec4 _baseColor;

///////////////////////////////////////////////////////////
// Varyings
#if defined(LIGHTING)
#if !defined(NORMAL_MAP)
varying vec3 v_normalVector;
#else
vec3 v_normalVector;
#endif
#endif

varying vec2 v_texCoord0;

#if (LAYER_COUNT > 0)
varying vec2 v_texCoordLayer0;
#endif
#if (LAYER_COUNT > 1)
varying vec2 v_texCoordLayer1;
#endif
#if (LAYER_COUNT > 2)
varying vec2 v_texCoordLayer2;
#endif
#if (LAYER_COUNT > 1)
void blendLayer(sampler2D textureMap, vec2 texCoord, float alphaBlend)
{
    vec3 diffuse = texture2D(textureMap,  mod(texCoord, vec2(1,1))).rgb;
    _baseColor.rgb = _baseColor.rgb * (1.0 - alphaBlend) + diffuse * alphaBlend;
}
#endif

#if defined(LIGHTING)
#include "lighting.frag"
#endif


void main()
{
    #if (LAYER_COUNT > 0)
    // Sample base texture
	_baseColor.rgb = texture2D(u_surfaceLayerMaps[TEXTURE_INDEX_0], mod(v_texCoordLayer0, vec2(1,1))).rgb;
    _baseColor.a = 1.0;
    #else
    // If no layers are defined, simply use a white color
    _baseColor = vec4(1, 1, 1, 1);
    #endif

    #if (LAYER_COUNT > 1)
    blendLayer(u_surfaceLayerMaps[TEXTURE_INDEX_1], v_texCoordLayer1, texture2D(u_surfaceLayerMaps[BLEND_INDEX_1], v_texCoord0)[BLEND_CHANNEL_1]);
    #endif
    #if (LAYER_COUNT > 2)
    blendLayer(u_surfaceLayerMaps[TEXTURE_INDEX_2], v_texCoordLayer2, texture2D(u_surfaceLayerMaps[BLEND_INDEX_2], v_texCoord0)[BLEND_CHANNEL_2]);
    #endif

    #if defined(DEBUG_PATCHES)
    float tint = mod(u_row + mod(u_column, 2.0), 2.0);
    _baseColor.rgb = _baseColor.rgb * 0.75 + vec3(1.0-tint, tint, 0) * 0.25;
    #endif

    #if defined(LIGHTING)

    #if defined(NORMAL_MAP)
    v_normalVector = texture2D(u_normalMap, v_texCoord0).xyz * 2.0 - 1.0;
    v_normalVector = (u_normalMatrix * vec4(v_normalVector.x, v_normalVector.y, v_normalVector.z, 0)).xyz;
    #endif

    gl_FragColor.a = _baseColor.a;
    gl_FragColor.rgb = getLitPixel();

    #else

    gl_FragColor.rgb = _baseColor.rgb;

    #endif
}
