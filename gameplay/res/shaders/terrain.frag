#ifdef OPENGL_ES
precision highp float;
#endif

// Inputs
#if defined(NORMAL_MAP)
vec3 v_normalVector;                            // Normal vector variable (from normal map)
#else
varying vec3 v_normalVector;					// Normal vector from vertex shader
#endif
varying vec2 v_texCoord0;

// Uniforms
uniform vec3 u_ambientColor;                    // Ambient color
uniform vec3 u_lightColor;                      // Light color
uniform vec3 u_lightDirection;					// Light direction

#if defined(DEBUG_PATCHES)
uniform int u_row;
uniform int u_column;
#endif

#if (LAYER_COUNT > 0)
uniform sampler2D u_samplers[SAMPLER_COUNT];
uniform int u_textureIndex[LAYER_COUNT];
uniform vec2 u_textureRepeat[LAYER_COUNT];
#if (LAYER_COUNT > 1)
uniform int u_blendIndex[LAYER_COUNT-1];
uniform int u_blendChannel[LAYER_COUNT-1];
#endif
#if defined (NORMAL_MAP)
uniform sampler2D u_normalMap;
#endif
#endif

#include "lib/lighting.frag"
#include "lib/lighting-directional.frag"

#if (LAYER_COUNT > 1)
void blendLayer(int index)
{
    vec2 uv = v_texCoord0 * u_textureRepeat[index];

    // Sample full intensity diffuse color
    vec3 diffuse = texture2D(u_samplers[u_textureIndex[index]], uv).rgb;
    
    // Sample blend map
    vec4 blend = texture2D(u_samplers[u_blendIndex[index-1]], v_texCoord0).rgba;
    float channels[4] = float[4](blend.r, blend.g, blend.b, blend.a);
    float a = channels[u_blendChannel[index-1]];

    _baseColor.rgb = _baseColor.rgb * (1.0 - a) + diffuse * a;
}
#endif

// Fragment program
void main()
{
#if (LAYER_COUNT > 0)
    // Set base diffuse color
    vec2 uvCoord = v_texCoord0 * u_textureRepeat[0];
	_baseColor = texture2D(u_samplers[u_textureIndex[0]], uvCoord);
#else
    // If no layers are defined, simple use a white color
    _baseColor = vec4(1,1,1,1);
#endif

#if (LAYER_COUNT > 1)
    blendLayer(1);
#endif

#if defined(DEBUG_PATCHES)
    // If patch debug drawing is enabled, tint patches alternate colors
    float tint = mod(u_row + mod(u_column, 2), 2);
    _baseColor.rgb = _baseColor.rgb * 0.75 + vec3(1-tint, tint, 0) * 0.25;
#endif

    // Light the pixel
#if defined(NORMAL_MAP)
    v_normalVector = normalize(texture2D(u_normalMap, v_texCoord0).xyz * 2.0f - 1.0f);
#endif
    gl_FragColor.a = _baseColor.a;
    gl_FragColor.rgb = getLitPixel();
}
