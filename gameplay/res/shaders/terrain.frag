#ifdef OPENGL_ES
precision highp float;
#endif

// Uniforms
uniform vec3 u_ambientColor;                    // Ambient color
uniform vec3 u_lightColor;                      // Light color
uniform vec3 u_lightDirection;					// Light direction
#if defined(DEBUG_PATCHES)
uniform float u_row;                            // Patch row
uniform float u_column;                         // Patch column
#endif
#if (LAYER_COUNT > 0)
uniform sampler2D u_samplers[SAMPLER_COUNT];    // Surface layer samplers
#endif
#if defined (NORMAL_MAP)
uniform sampler2D u_normalMap;                  // Normal map
#endif

// Varyings
#if defined(NORMAL_MAP)
vec3 v_normalVector;                            // Normal vector variable (from normal map)
#else
varying vec3 v_normalVector;					// Normal vector from vertex shader
#endif
varying vec2 v_texCoord0;

// Lighting
#include "lighting.frag"
#include "lighting-directional.frag"

#if (LAYER_COUNT > 1)
void blendLayer(sampler2D textureMap, vec2 textureRepeat, float alphaBlend)
{
    vec2 uv = mod(v_texCoord0 * textureRepeat, vec2(1,1));

    // Sample full intensity diffuse color
    vec3 diffuse = texture2D(textureMap, uv).rgb;

    _baseColor.rgb = _baseColor.rgb * (1.0 - alphaBlend) + diffuse * alphaBlend;
}
#endif


void main()
{
#if (LAYER_COUNT > 0)
    // Set base diffuse color
    vec2 uvCoord = mod(v_texCoord0 * TEXTURE_REPEAT_0, vec2(1,1));
	_baseColor = texture2D(u_samplers[TEXTURE_INDEX_0], uvCoord);
#else
    // If no layers are defined, simple use a white color
    _baseColor = vec4(1,1,1,1);
#endif

#if (LAYER_COUNT > 1)
    blendLayer(u_samplers[TEXTURE_INDEX_1], TEXTURE_REPEAT_1, texture2D(u_samplers[BLEND_INDEX_1], v_texCoord0)[BLEND_CHANNEL_1]);
#endif

#if (LAYER_COUNT > 2)
    blendLayer(u_samplers[TEXTURE_INDEX_2], TEXTURE_REPEAT_2, texture2D(u_samplers[BLEND_INDEX_2], v_texCoord0)[BLEND_CHANNEL_2]);
#endif

#if defined(DEBUG_PATCHES)
    // If patch debug drawing is enabled, tint patches alternate colors
    float tint = mod(u_row + mod(u_column, 2.0), 2.0);
    _baseColor.rgb = _baseColor.rgb * 0.75 + vec3(1.0-tint, tint, 0) * 0.25;
#endif

    // Light the pixel
#if defined(NORMAL_MAP)
    v_normalVector = normalize(texture2D(u_normalMap, v_texCoord0).xyz * 2.0 - 1.0);
#endif

    gl_FragColor.a = _baseColor.a;
    gl_FragColor.rgb = getLitPixel();
}
