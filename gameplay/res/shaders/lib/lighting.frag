vec4 _baseColor;
vec3 _ambientColor;
vec3 _diffuseColor;

#if defined(SPECULAR)

vec3 _specularColor;

vec3 computeLighting(vec3 normalVector, vec3 lightDirection, float attenuation, vec3 cameraDirection)
{
    // Ambient
    _ambientColor = _baseColor.rgb * u_ambientColor;

    // Diffuse
    float ddot = dot(normalVector, lightDirection);
    float diffuseIntensity = attenuation * ddot;
    diffuseIntensity = max(0.0, diffuseIntensity);
    _diffuseColor = u_lightColor * _baseColor.rgb * diffuseIntensity;

    // Specular
    vec3 halfVector = normalize(lightDirection + cameraDirection);
    float specularIntensity = attenuation * max(0.0, pow(dot(normalVector, halfVector), u_specularExponent));
    specularIntensity = max(0.0, specularIntensity);
    _specularColor = u_lightColor * _baseColor.rgb * specularIntensity;
	
	return _ambientColor + _diffuseColor + _specularColor;
}

#else

vec3 computeLighting(vec3 normalVector, vec3 lightDirection, float attenuation)
{
    // Ambient
    _ambientColor = _baseColor.rgb * u_ambientColor;

    // Diffuse
	float ddot = dot(normalVector, lightDirection);
    float diffuseIntensity = attenuation * ddot;
    diffuseIntensity = max(0.0, diffuseIntensity);
    _diffuseColor = u_lightColor * _baseColor.rgb * diffuseIntensity;
	
	return _ambientColor + _diffuseColor;
}

#endif

