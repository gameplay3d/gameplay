#ifdef OPENGL_ES
precision highp float;
#endif

// Uniforms
uniform vec3 u_lightColor;                      // Light color.
uniform vec3 u_ambientColor;                    // Ambient color.
uniform sampler2D u_diffuseTexture;             // Diffuse texture.
uniform sampler2D u_bumpMapTexture;             // Height map texture
uniform sampler2D u_normalMapTexture;           // Normal map texture.
uniform float u_parallaxHeight;                 // Parallax height
#if defined(GLOBAL_ALPHA)
uniform float u_globalAlpha;                    // Global alpha value
#endif

// Inputs
varying vec2 v_texCoord;                        // Texture Coordinate.
varying vec3 v_cameraDirection;                 // Direction the camera is looking at in tangent space.

// Global variables
vec4 _baseColor;                                // Base color
vec3 _ambientColor;                             // Ambient Color
vec3 _diffuseColor;                             // Diffuse Color

void lighting(vec3 normalVector, vec3 lightDirection, float attenuation)
{
    // Ambient
    _ambientColor = _baseColor.rgb * u_ambientColor;

    // Diffuse
    float diffuseIntensity = attenuation * max(0.0, dot(normalVector, lightDirection));
    diffuseIntensity = max(0.0, diffuseIntensity);
    _diffuseColor = u_lightColor * _baseColor.rgb * diffuseIntensity;
}

#if defined(POINT_LIGHT)

varying vec3 v_vertexToPointLightDirection;     // Light direction w.r.t current vertex in tangent space.
varying float v_pointLightAttenuation;          // Attenuation of point light.

void applyLight()
{
    // Normalize the vectors.
    vec3 cameraDirection = normalize(v_cameraDirection);
    vec3 vertexToPointLightDirection = normalize(v_vertexToPointLightDirection);
    
    float pointLightAttenuation = clamp(v_pointLightAttenuation, 0.0, 1.0);

    // Get the height in the bumpmap texture.
    float height = texture2D(u_bumpMapTexture, v_texCoord).r;

    // Compute offset within the range (-parallax height to +parallax height).
    float offset = u_parallaxHeight * (2.0 * height - 1.0);
    vec2 parallaxTexCoord = v_texCoord + (cameraDirection.xy * offset);

    // Compute offset again with the new texture coordinates to get better precision.
    // Find out the mean height.
    height += texture2D(u_bumpMapTexture, parallaxTexCoord).r;
    offset = u_parallaxHeight * (height - 1.0);
    parallaxTexCoord = v_texCoord + ( cameraDirection.xy * offset);

    // Fetch normals from the normal map with the modified texture coordinates.
    vec3 normalVector = normalize(texture2D(u_normalMapTexture, parallaxTexCoord).xyz * 2.0 - 1.0);

    // Fetch diffuse color from texture.
    _baseColor = texture2D(u_diffuseTexture, parallaxTexCoord);
    
    lighting(normalVector, vertexToPointLightDirection, pointLightAttenuation);
}

#elif defined(SPOT_LIGHT)

uniform float u_spotLightInnerAngleCos;         // The bright spot [0.0 - 1.0]
uniform float u_spotLightOuterAngleCos;         // The soft outer part [0.0 - 1.0]
varying vec3 v_spotLightDirection;              // Direction of spot light in tangent space.
varying vec3 v_vertexToSpotLightDirection;      // Direction of the spot light w.r.t current vertex in tangent space.
varying float v_spotLightAttenuation;           // Attenuation of spot light.

float lerpstep( float lower, float upper, float s)
{
    return clamp( ( s - lower ) / ( upper - lower ), 0.0, 1.0 );
}

void applyLight()
{
    vec3 cameraDirection = normalize(v_cameraDirection);
    vec3 spotLightDirection = normalize(v_spotLightDirection);
    vec3 vertexToSpotLightDirection = normalize(v_vertexToSpotLightDirection);

    // "-lightDirection" because light direction points in opposite direction to
    // to spot direction.
    // Calculate spot light effect.
    float spotCurrentAngleCos = max(0.0, dot(spotLightDirection, -vertexToSpotLightDirection));
    
    // Intensity of spot depends on the spot light attenuation and the 
    // part of the cone vertexToSpotLightDirection points to (inner or outer).
    float spotLightAttenuation = clamp(v_spotLightAttenuation, 0.0, 1.0);
    spotLightAttenuation *= lerpstep(u_spotLightOuterAngleCos, u_spotLightInnerAngleCos, spotCurrentAngleCos);

    // Get the height in the bumpmap texture.
    float height = texture2D(u_bumpMapTexture, v_texCoord).r;

    // Compute offset within the range (-parallax height to +parallax height).
    float offset = u_parallaxHeight * (2.0 * height - 1.0);
    vec2 parallaxTexCoord = v_texCoord + (cameraDirection.xy * offset);

    // Compute offset again with the new texture coordinates to get better precision.
    // Find out the mean height.
    height += texture2D(u_bumpMapTexture, parallaxTexCoord).r;
    offset = u_parallaxHeight * (height - 1.0);
    parallaxTexCoord = v_texCoord + ( cameraDirection.xy * offset);

    // Fetch normals from the normal map with the modified texture coordinates.
    vec3 normalVector = normalize(texture2D(u_normalMapTexture, parallaxTexCoord).xyz * 2.0 - 1.0);

    // Fetch diffuse color from texture.
    _baseColor = texture2D(u_diffuseTexture, parallaxTexCoord);

    lighting(normalVector, vertexToSpotLightDirection, spotLightAttenuation);
}

#else

varying vec3 v_lightDirection;                      // Direction of light in tangent space.

void applyLight()
{
    // Normalize vectors.
    vec3 cameraDirection = normalize(v_cameraDirection);
    vec3 lightDirection = normalize(v_lightDirection);

    // Get the height in the bumpmap texture.
    float height = texture2D(u_bumpMapTexture, v_texCoord).r;

    // Compute offset within the range (-parallax height to +parallax height).
    float offset = u_parallaxHeight * (2.0 * height - 1.0);
    vec2 parallaxTexCoord = v_texCoord + (cameraDirection.xy * offset);

    // Compute offset again with the new texture coordinates to get better precision.
    // Find out the mean height.
    height += texture2D(u_bumpMapTexture, parallaxTexCoord).r;
    offset = u_parallaxHeight * (height - 1.0);
    parallaxTexCoord = v_texCoord + ( cameraDirection.xy * offset);

    // Fetch normals from the normal map with the modified texture coordinates.
    vec3 normalVector = normalize(texture2D(u_normalMapTexture, parallaxTexCoord).xyz * 2.0 - 1.0);

    // Fetch diffuse color from texture.
    _baseColor = texture2D(u_diffuseTexture, parallaxTexCoord);

    lighting(normalVector, -lightDirection, 1.0);
}
#endif

void main()
{
    // Apply light
    applyLight();

    // Light the pixel
    gl_FragColor.a = _baseColor.a;
    gl_FragColor.rgb = _ambientColor + _diffuseColor;

#if defined(GLOBAL_ALPHA)
    gl_FragColor.a *= u_globalAlpha;
#endif
}