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
// Attributes
attribute vec4 a_position;
#if !defined(NORMAL_MAP) && defined(LIGHTING)
attribute vec3 a_normal;
#endif
attribute vec2 a_texCoord0;

///////////////////////////////////////////////////////////
// Uniforms
uniform mat4 u_worldViewProjectionMatrix;
#if !defined(NORMAL_MAP) && defined(LIGHTING)
uniform mat4 u_normalMatrix;
#endif

#if defined(LIGHTING)

uniform mat4 u_inverseTransposeWorldViewMatrix;

#if (POINT_LIGHT_COUNT > 0) || (SPOT_LIGHT_COUNT > 0)
uniform mat4 u_worldViewMatrix;
#endif

#if (DIRECTIONAL_LIGHT_COUNT > 0)
uniform vec3 u_directionalLightDirection[DIRECTIONAL_LIGHT_COUNT];
#endif

#if (POINT_LIGHT_COUNT > 0) 
uniform vec3 u_pointLightPosition[POINT_LIGHT_COUNT];
#endif

#if (SPOT_LIGHT_COUNT > 0)
uniform vec3 u_spotLightPosition[SPOT_LIGHT_COUNT];
uniform vec3 u_spotLightDirection[SPOT_LIGHT_COUNT];
#endif

#endif

///////////////////////////////////////////////////////////
// Varyings

#if defined(LIGHTING)
varying vec3 v_normalVector;

#if (DIRECTIONAL_LIGHT_COUNT > 0) 
varying vec3 v_lightDirection[DIRECTIONAL_LIGHT_COUNT];
#endif

#if (POINT_LIGHT_COUNT > 0)
varying vec3 v_vertexToPointLightDirection[POINT_LIGHT_COUNT];
#endif

#if (SPOT_LIGHT_COUNT > 0)
varying vec3 v_vertexToSpotLightDirection[SPOT_LIGHT_COUNT];
#endif

#include "lighting.vert"

#endif

varying vec2 v_texCoord0;
#if LAYER_COUNT > 0
varying vec2 v_texCoordLayer0;
#endif
#if LAYER_COUNT > 1
varying vec2 v_texCoordLayer1;
#endif
#if LAYER_COUNT > 2
varying vec2 v_texCoordLayer2;
#endif


void main()
{
    // Transform position to clip space.
    gl_Position = u_worldViewProjectionMatrix * a_position;

    #if defined(LIGHTING)

    #if !defined(NORMAL_MAP) 
    v_normalVector = normalize((u_normalMatrix * vec4(a_normal.x, a_normal.y, a_normal.z, 0)).xyz);
    #endif

    applyLight(a_position);

    #endif

    // Pass base texture coord
    v_texCoord0 = a_texCoord0;

    // Pass repeated texture coordinates for each layer
    #if LAYER_COUNT > 0
    v_texCoordLayer0 = a_texCoord0 * TEXTURE_REPEAT_0;
    #endif
    #if LAYER_COUNT > 1
    v_texCoordLayer1 = a_texCoord0 * TEXTURE_REPEAT_1;
    #endif
    #if LAYER_COUNT > 2
    v_texCoordLayer2 = a_texCoord0 * TEXTURE_REPEAT_2;
    #endif
}
