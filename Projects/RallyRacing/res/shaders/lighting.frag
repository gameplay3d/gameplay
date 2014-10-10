
vec3 computeLighting(vec3 normalVector, vec3 lightDirection, vec3 lightColor, float attenuation)
{
    float diffuse = max(dot(normalVector, lightDirection), 0.0);
     vec3 diffuseColor = lightColor * _baseColor.rgb * diffuse * attenuation;

    #if defined(SPECULAR)

	// Phong shading
    //vec3 vertexToEye = normalize(v_cameraDirection);
    //vec3 specularAngle = normalize(normalVector * diffuse * 2.0 - lightDirection);  
    //vec3 specularColor = vec3(pow(clamp(dot(specularAngle, vertexToEye), 0.0, 1.0), u_specularExponent)); 

    // Blinn-Phong shading
    vec3 vertexToEye = normalize(v_cameraDirection);
    vec3 halfVector = normalize(lightDirection + vertexToEye);
    float specularAngle = clamp(dot(normalVector, halfVector), 0.0, 1.0);
    vec3 specularColor = vec3(pow(specularAngle, u_specularExponent)) * attenuation;

    return diffuseColor + specularColor;

    #else
    
    return diffuseColor;
    
    #endif
}

vec3 getLitPixel()
{
    #if defined(BUMPED)
    
    vec3 normalVector = normalize(texture2D(u_normalmapTexture, v_texCoord).rgb * 2.0 - 1.0);
    
    #else
    
    vec3 normalVector = normalize(v_normalVector);
    
    #endif
    
    vec3 ambientColor = _baseColor.rgb * u_ambientColor;
    vec3 combinedColor = ambientColor;

    // Directional light contribution
    #if (DIRECTIONAL_LIGHT_COUNT > 0)
    for (int i = 0; i < DIRECTIONAL_LIGHT_COUNT; ++i)
    {
        #if defined(BUMPED)
        vec3 lightDirection = normalize(v_directionalLightDirection[i] * 2.0);
        #else
        vec3 lightDirection = normalize(u_directionalLightDirection[i] * 2.0);
        #endif 
        combinedColor += computeLighting(normalVector, -lightDirection, u_directionalLightColor[i], 1.0);
    }
    #endif

    // Point light contribution
    #if (POINT_LIGHT_COUNT > 0)
    for (int i = 0; i < POINT_LIGHT_COUNT; ++i)
    {
        vec3 ldir = v_vertexToPointLightDirection[i] * u_pointLightRangeInverse[i];
        float attenuation = clamp(1.0 - dot(ldir, ldir), 0.0, 1.0);
        combinedColor += computeLighting(normalVector, normalize(v_vertexToPointLightDirection[i]), u_pointLightColor[i], attenuation);
    }
    #endif

    // Spot light contribution
    #if (SPOT_LIGHT_COUNT > 0)
    for (int i = 0; i < SPOT_LIGHT_COUNT; ++i)
    {
        // Compute range attenuation
        vec3 ldir = v_vertexToSpotLightDirection[i] * u_spotLightRangeInverse[i];
        float attenuation = clamp(1.0 - dot(ldir, ldir), 0.0, 1.0);
        vec3 vertexToSpotLightDirection = normalize(v_vertexToSpotLightDirection[i]);

        #if defined(BUMPED)
            vec3 spotLightDirection = normalize(v_spotLightDirection[i] * 2.0);
        #else
            vec3 spotLightDirection = normalize(u_spotLightDirection[i] * 2.0);
        #endif

        // "-lightDirection" is used because light direction points in opposite direction to spot direction.
        float spotCurrentAngleCos = dot(spotLightDirection, -vertexToSpotLightDirection);

		// Apply spot attenuation
        attenuation *= smoothstep(u_spotLightOuterAngleCos[i], u_spotLightInnerAngleCos[i], spotCurrentAngleCos);
        combinedColor += computeLighting(normalVector, vertexToSpotLightDirection, u_spotLightColor[i], attenuation);
    }
    #endif

    return combinedColor;
}
