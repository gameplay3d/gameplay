// Attributes
attribute vec4 a_position;									// Vertex Position							(x, y, z, w)
#ifndef NORMAL_MAP
attribute vec3 a_normal;									// Vertex Normal							(x, y, z)
#endif
attribute vec2 a_texCoord0;

// Uniforms
uniform mat4 u_worldViewProjectionMatrix;					// World view projection matrix
#ifndef NORMAL_MAP
uniform mat4 u_normalMatrix;					            // Matrix used for normal vector transformation
#endif
uniform vec3 u_lightDirection;								// Direction of light

// Varyings
#ifndef NORMAL_MAP
varying vec3 v_normalVector;								// Normal vector out
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

#ifndef NORMAL_MAP
    // Pass normal to fragment shader
    v_normalVector = (u_normalMatrix * vec4(a_normal.x, a_normal.y, a_normal.z, 0)).xyz;
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
