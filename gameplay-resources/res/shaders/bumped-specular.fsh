precision highp float;

// Uniforms
uniform vec3 u_lightColor;                  // Light color.
uniform vec3 u_ambientColor;                // Ambient color.
uniform float u_specularExponent;           // Specular exponent or shininess property.
uniform sampler2D u_diffuseTexture;         // Diffuse texture.
uniform sampler2D u_normalMapTexture;       // Normal map texture.

// Inputs
varying vec2 v_texCoord;                    // Texture Coordinate.
varying vec3 v_cameraDirection;     	    // Direction the camera is looking at in tangent space.

// Common colors
vec4 _baseColor;                            // Base color
vec3 _ambientColor;                         // Ambient Color
vec3 _diffuseColor;                         // Diffuse Color
vec3 _specularColor;                        // Specular color

void lighting(vec3 normalVector, vec3 cameraDirection, vec3 lightDirection, float attenuation)
{
    // Ambient
	_ambientColor += _baseColor.rgb * u_ambientColor;

	// Diffuse
	float diffuseIntensity = attenuation * max(0.0, dot(normalVector, lightDirection));
    diffuseIntensity = max(0.0, diffuseIntensity);
	_diffuseColor += u_lightColor * _baseColor.rgb * diffuseIntensity;

	// Specular
	vec3 halfVector = normalize(cameraDirection + lightDirection);
	float specularIntensity = attenuation * max(0.0, pow(dot(normalVector, halfVector), u_specularExponent));
    specularIntensity = max(0.0, specularIntensity);
    _specularColor += u_lightColor * _baseColor.rgb * specularIntensity;
}

#if defined(POINT_LIGHT)

varying vec3 v_pointLightDirectionCurrent;  // Light direction w.r.t current vertex in tangent space.
varying float v_pointLightAttenuation;      // Attenuation of point light.

void applyLight()
{
    // Normalize the vectors.
	// Fetch normals from the normal map.
	vec3 normalVector = normalize(texture2D(u_normalMapTexture, v_texCoord).rgb * 2.0 - 1.0);
    vec3 cameraDirection = normalize(v_cameraDirection);
    
    vec3 pointLightDirectionCurrent = normalize(v_pointLightDirectionCurrent);
    
    // Fetch point light attenuation.
    lighting(normalVector, cameraDirection, pointLightDirectionCurrent, v_pointLightAttenuation);
}

#elif defined(SPOT_LIGHT)

uniform float u_spotLightInnerCosAngle;     // The bright spot [0.5 - 1.0]
uniform float u_spotLightOuterCosAngle;     // The soft outer part [0.0 - 0.5]
varying vec3 v_spotLightDirection;          // Direction of spot light in tangent space.
varying vec3 v_spotLightDirectionCurrent;   // Direction of the spot light w.r.t current vertex in tangent space.

float lerpstep( float lower, float upper, float s)
{
    return saturate( ( s - lower ) / ( upper - lower ) );
}

void applyLight()
{
    // Normalize the vectors.
	// Fetch normals from the normal map.
	vec3 normalVector = normalize(texture2D(u_normalMapTexture, v_texCoord).rgb * 2.0 - 1.0);
    vec3 cameraDirection = normalize(v_cameraDirection);
    vec3 spotLightDirection =normalize(v_spotLightDirection);
    vec3 spotLightDirectionCurrent = normalize(v_spotLightDirectionCurrent);
    
    // "-lightDirection" because light direction points in opposite direction to
    // to spot direction.
    // Calculate spot light effect.
	float spotCurrentCosAngle = max(0.0, dot(spotLightDirection, -spotLightDirectionCurrent));
	
    // Intensity of spot depends on the part of the cone the light direction points to (inner or outer).
    float spotLightAttenuation = lerpstep(u_spotLightOuterCosAngle, u_spotLightInnerCosAngle, spotCurrentCosAngle);

    lighting(normalVector, cameraDirection, spotLightDirectionCurrent, spotLightAttenuation);
}

#else

varying vec3 v_directionalLightDirection;    // Direction of light in tangent space.

void applyLight()
{
    // Normalize vectors.
	// Fetch normals from the normal map
	vec3 normalVector = normalize(texture2D(u_normalMapTexture, v_texCoord).rgb * 2.0 - 1.0);
    vec3 cameraDirection = normalize(v_cameraDirection);
    vec3 lightDirection = normalize(v_directionalLightDirection);

    lighting(normalVector, cameraDirection, lightDirection, 1.0);
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
