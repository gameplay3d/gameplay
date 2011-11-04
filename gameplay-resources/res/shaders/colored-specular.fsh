precision highp float;

// Uniforms
uniform vec3 u_lightColor;                      // Light color
uniform vec3 u_ambientColor;			        // Ambient color
uniform float u_specularExponent;		        // Specular exponent or shininess property.
uniform vec4 u_diffuseColor;                     // Diffuse color

// Inputs
varying vec3 v_normalVector;			        // NormalVector in view space.
varying vec3 v_cameraDirection; 		        // Camera direction

// Common colors
vec4 _baseColor;                                // Base color
vec3 _ambientColor;                             // Ambient Color
vec3 _diffuseColor;                             // Diffuse Color
vec3 _specularColor;                            // Specular color

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

varying vec4 v_pointLightDirectionCurrent;      // Light direction w.r.t current vertex.

void applyLight()
{
    // Normalize the vectors.
	vec3 normalVector = normalize(v_normalVector);
    vec3 cameraDirection = normalize(v_cameraDirection);
    
    vec3 pointLightDirectionCurrent = normalize(v_pointLightDirectionCurrent.xyz);
    
    // Fetch point light attenuation.
    float pointLightAttenuation = v_pointLightDirectionCurrent.w;
    lighting(normalVector, cameraDirection, pointLightDirectionCurrent, pointLightAttenuation);
}

#elif defined(SPOT_LIGHT)

uniform vec3 u_spotLightDirection;          // Direction of the spot light.
uniform float u_spotLightInnerCosAngle;     // The bright spot [0.5 - 1.0]
uniform float u_spotLightOuterCosAngle;     // The soft outer part [0.0 - 0.5]
varying vec3 v_spotLightDirectionCurrent;   // Light direction w.r.t current vertex.

float lerpstep( float lower, float upper, float s)
{
    return saturate( ( s - lower ) / ( upper - lower ) );
}

void applyLight()
{
    // Normalize the vectors.
	vec3 normalVector = normalize(v_normalVector);
    vec3 cameraDirection = normalize(v_cameraDirection);
    vec3 spotLightDirection =normalize(u_spotLightDirection); 
    vec3 spotLightDirectionCurrent = normalize(v_spotLightDirectionCurrent);

    // "-lightDirection" is used because light direction points in opposite direction to
    // to spot direction.
    // Calculate spot light effect.
	float spotCurrentCosAngle = max(0.0, dot(spotLightDirection, -spotLightDirectionCurrent));
	
    // Intensity of spot depends on the part of the cone the light direction points to (inner or outer).
    float spotLightAttenuation = lerpstep(u_spotLightOuterCosAngle, u_spotLightInnerCosAngle, spotCurrentCosAngle);

    lighting(normalVector, cameraDirection, spotLightDirectionCurrent, spotLightAttenuation);
}

#else

uniform vec3 u_lightDirection;       // Light direction

void applyLight()
{
    // Normalize the vectors.
	vec3 normalVector = normalize(v_normalVector);
    vec3 cameraDirection = normalize(v_cameraDirection);
    vec3 lightDirection = normalize(u_lightDirection);

    lighting(normalVector, cameraDirection, lightDirection, 1.0);
}
#endif

void main()
{
	// Fetch diffuse color from texture.
	_baseColor = u_diffuseColor;

    // Apply light
	applyLight();

	// Light the pixel
    gl_FragColor.a = _baseColor.a;
	gl_FragColor.rgb = _ambientColor + _diffuseColor + _specularColor;
}