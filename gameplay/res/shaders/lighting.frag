
vec3 computeLighting(vec3 normalVector, vec3 lightDirection, vec3 lightColor, float attenuation)
{
    float diffuse = clamp(dot(normalVector, lightDirection), 0.0, 1.0);
    vec3 diffuseColor = lightColor * _baseColor.rgb * diffuse;

    #if defined(SPECULAR)

    vec3 vertexToEye = normalize(v_cameraDirection);
    vec3 reflect = normalize(normalVector * diffuse * 2.0 - lightDirection);  
    float specular = pow(clamp(dot(reflect, vertexToEye), 0.0, 1.0), u_specularExponent);
    vec3 specularColor = vec3(specular, specular, specular); 
    
    return (diffuseColor + specularColor) * attenuation;

    #else
    
        return diffuseColor * attenuation;
    
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
        vec3 lightDirection = normalize(v_lightDirection);
        #else
        vec3 lightDirection = normalize(u_directionalLightDirection[i]);
        #endif 
        combinedColor += computeLighting(normalVector, -lightDirection, u_directionalLightColor[i], 1.0);
    }
    #endif

    // Point light contribution
    #if (POINT_LIGHT_COUNT > 0)
    for (int i = 0; i < POINT_LIGHT_COUNT; ++i)
    {
        // Compute attenuation
        float d = length(v_vertexToPointLightDirection[i]);
        float att = 1.0 / (u_pointLightAttenuation[i].x + u_pointLightAttenuation[i].y * d + u_pointLightAttenuation[i].z * (d*d));
        combinedColor += computeLighting(normalVector, normalize(v_vertexToPointLightDirection[i]), u_pointLightColor[i], att);
    }
    #endif

    // Spot light contribution
    #if (SPOT_LIGHT_COUNT > 0)
    for (int i = 0; i < SPOT_LIGHT_COUNT; ++i)
    {
        // Compute range attenuation
        vec3 ldir = v_vertexToSpotLightDirection[i] * u_spotLightRangeInverse[i];
        float att = clamp(1.0 - dot(ldir, ldir), 0.0, 1.0);
        vec3 vertexToSpotLightDirection = normalize(v_vertexToSpotLightDirection[i]);

        // TODO: 
        // Let app normalize this! Need Node::getForwardVectorViewNorm
        // This needs to be in TANGENT SPACE for bump mapping
        // and should always pass from vertex shader via v_spotLightDirection[i]
        vec3 spotLightDirection = normalize(u_spotLightDirection[i]);

        // "-lightDirection" is used because light direction points in opposite direction to spot direction.
        float spotCurrentAngleCos = dot(spotLightDirection, -vertexToSpotLightDirection);

		// Apply spot attenuation
        att *= smoothstep(u_spotLightOuterAngleCos[i], u_spotLightInnerAngleCos[i], spotCurrentAngleCos);
        combinedColor += computeLighting(normalVector, vertexToSpotLightDirection, u_spotLightColor[i], att);
    }
    #endif
    
    return combinedColor;
}
