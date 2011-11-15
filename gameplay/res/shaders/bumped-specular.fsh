#ifdef OPENGL_ES
precision highp float;
#endif

// Uniforms
uniform vec3 u_lightColor;                  // Light color.
uniform vec3 u_ambientColor;                // Ambient color.
uniform float u_specularExponent;           // Specular exponent or shininess property.
uniform sampler2D u_diffuseTexture;         // Diffuse texture.
uniform sampler2D u_normalMapTexture;       // Normal map texture.

// Inputs
varying vec2 v_texCoord;                    // Texture Coordinate.
varying vec3 v_cameraDirection;             // Direction the camera is looking at in tangent space.

// Global variables
vec4 _baseColor;                            // Base color
vec3 _ambientColor;                         // Ambient Color
vec3 _diffuseColor;                         // Diffuse Color
vec3 _specularColor;                        // Specular color

void lighting(vec3 normalVector, vec3 cameraDirection, vec3 lightDirection, float attenuation)
{
    // Ambient
    _ambientColor = _baseColor.rgb * u_ambientColor;

    // Diffuse
    float diffuseIntensity = attenuation * max(0.0, dot(normalVector, lightDirection));
    diffuseIntensity = max(0.0, diffuseIntensity);
    _diffuseColor = u_lightColor * _baseColor.rgb * diffuseIntensity;

    // Specular
    vec3 halfVector = normalize(cameraDirection + lightDirection);
    float specularIntensity = attenuation * max(0.0, pow(dot(normalVector, halfVector), u_specularExponent));
    specularIntensity = max(0.0, specularIntensity);
    _specularColor = u_lightColor * _baseColor.rgb * specularIntensity;
}

#if defined(POINT_LIGHT)

varying vec3 v_vertexToPointLightDirection;   // Light direction w.r.t current vertex in tangent space.
varying float v_pointLightAttenuation;        // Attenuation of point light.

void applyLight()
{
    // Normalize the vectors.
    // Fetch normals from the normal map.
    vec3 normalVector = normalize(texture2D(u_normalMapTexture, v_texCoord).rgb * 2.0 - 1.0);
    vec3 cameraDirection = normalize(v_cameraDirection);
    vec3 vertexToPointLightDirection = normalize(v_vertexToPointLightDirection);
    
    float pointLightAttenuation = clamp(v_pointLightAttenuation, 0.0, 1.0);
    
    lighting(normalVector, cameraDirection, vertexToPointLightDirection, pointLightAttenuation);
}

#elif defined(SPOT_LIGHT)

uniform float u_spotLightInnerAngleCos;       // The bright spot [0.0 - 1.0]
uniform float u_spotLightOuterAngleCos;       // The soft outer part [0.0 - 1.0]
varying vec3 v_spotLightDirection;            // Direction of spot light in tangent space.
varying vec3 v_vertexToSpotLightDirection;    // Direction of the spot light w.r.t current vertex in tangent space.
varying float v_spotLightAttenuation;         // Attenuation of spot light.

float lerpstep( float lower, float upper, float s)
{
    return clamp( ( s - lower ) / ( upper - lower ), 0.0, 1.0 );
}

void applyLight()
{
    // Fetch normals from the normal map.
    vec3 normalVector = normalize(texture2D(u_normalMapTexture, v_texCoord).rgb * 2.0 - 1.0);
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

    lighting(normalVector, cameraDirection, vertexToSpotLightDirection, spotLightAttenuation);
}

#else

varying vec3 v_lightDirection;                 // Direction of light in tangent space.

void applyLight()
{
    // Fetch normals from the normal map
    vec3 normalVector = normalize(texture2D(u_normalMapTexture, v_texCoord).rgb * 2.0 - 1.0);
    vec3 cameraDirection = normalize(v_cameraDirection);
    vec3 lightDirection = normalize(v_lightDirection);

    lighting(normalVector, cameraDirection, -lightDirection, 1.0);
}
#endif

void main()
{
    // Fetch diffuse color from texture.
    _baseColor = texture2D(u_diffuseTexture, v_texCoord);

    // Apply light
    applyLight();

    // Light the pixel
    gl_FragColor.a = _baseColor.a;
    gl_FragColor.rgb = _ambientColor + _diffuseColor + _specularColor;
}
