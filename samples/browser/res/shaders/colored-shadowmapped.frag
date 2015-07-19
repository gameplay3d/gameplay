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

#if defined(MODULATE_COLOR)
uniform vec4 u_modulateColor;
#endif

#if defined(MODULATE_ALPHA)
uniform float u_modulateAlpha;
#endif

#if defined(SHADOW_MAPPING)
uniform float       u_pixelOffset; // 1.0 / shadow_texture_size
uniform sampler2D   u_shadowMap;
#endif

///////////////////////////////////////////////////////////
// Variables
vec4 _baseColor;

///////////////////////////////////////////////////////////
// Varyings
#if defined(VERTEX_COLOR)
varying vec3 v_color;
#endif

#if defined(LIGHTMAP)
varying vec2 v_texCoord1;
#endif

#if defined(LIGHTING)

varying vec3 v_normalVector;

#if (POINT_LIGHT_COUNT > 0)
varying vec3 v_vertexToPointLightDirection[POINT_LIGHT_COUNT];
#endif

#if (SPOT_LIGHT_COUNT > 0)
varying vec3 v_vertexToSpotLightDirection[SPOT_LIGHT_COUNT];
#endif

#if defined(SPECULAR)
varying vec3 v_cameraDirection; 
#endif

#include "lighting.frag"

#endif

#if defined(CLIP_PLANE)
varying float v_clipDistance;
#endif

#if defined(SHADOW_MAPPING)
varying vec4 v_shadowTexCoord;

// Tweakable consts
const float gradientClamp       = 0.0098;
const float gradientScaleBias   = 0.0;
const float fixedDepthBias      = 0.0005;
const vec2 texDims = vec2(512.0, 512.0);

// Ported from http://http.developer.nvidia.com/GPUGems/gpugems_ch12.html
// Unpacks a float from RGBA texture
float getDepth(vec2 uv)
{
    vec4 depthPacked = texture2D(u_shadowMap, uv);
    return depthPacked.r / 1.0 +
    depthPacked.g / 256.0 +
    depthPacked.b / 65536.0 +
    depthPacked.a / 16777216.0;
}

// Ported from: http://www.ogre3d.org/tikiwiki/tiki-index.php?page=Depth+Shadow+Mapping
float getShadowValue()
{
    float shadowTerm = 0.0;
    vec4 shadowUV = v_shadowTexCoord;
    shadowUV.xy = shadowUV.xy / shadowUV.w;
    float centerdepth = getDepth(shadowUV.xy);
    // gradient calculation
    vec4 depths = vec4( getDepth(shadowUV.xy + vec2(-u_pixelOffset, 0)),
                       getDepth(shadowUV.xy + vec2(+u_pixelOffset, 0)),
                       getDepth(shadowUV.xy + vec2(0, -u_pixelOffset)),
                       getDepth(shadowUV.xy + vec2(0, +u_pixelOffset)) );
    vec2 differences = abs( depths.yw - depths.xz );
    float gradient = min(gradientClamp, max(differences.x, differences.y));
    float gradientFactor = gradient * gradientScaleBias;
    // visibility function
    float depthAdjust = gradientFactor + (fixedDepthBias * centerdepth);
    float finalCenterDepth = centerdepth + depthAdjust;
    // PCF
    depths += depthAdjust;
    float final = step(finalCenterDepth, shadowUV.z) * 0.5 + 0.5;
    bvec4 vecCmp = greaterThan(depths, vec4(shadowUV.z));
    final += float(vecCmp.x) * 0.5 + 0.5;
    final += float(vecCmp.y) * 0.5 + 0.5;
    final += float(vecCmp.z) * 0.5 + 0.5;
    final += float(vecCmp.w) * 0.5 + 0.5;
    final *= 0.2;
    return final;
}
#endif

void main()
{
    #if defined(CLIP_PLANE)
    if(v_clipDistance < 0.0) discard;
    #endif
 
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

	#if defined(MODULATE_COLOR)
    gl_FragColor *= u_modulateColor;
    #endif

	#if defined(MODULATE_ALPHA)
    gl_FragColor.a *= u_modulateAlpha;
    #endif

    #if defined(SHADOW_MAPPING)
    gl_FragColor.rgb *= getShadowValue();
    #endif
}
