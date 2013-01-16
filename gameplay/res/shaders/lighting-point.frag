#if defined(BUMPED)

vec3 getLitPixel()
{
    // Fetch normals from the normal map and normalize the vectors
    vec3 normalVector = normalize(texture2D(u_normalmapTexture, v_texCoord).rgb * 2.0 - 1.0);
    vec3 vertexToPointLightDirection = normalize(v_vertexToPointLightDirection);
    
    float pointLightAttenuation = clamp(v_pointLightAttenuation, 0.0, 1.0);
    
    #if defined(SPECULAR)
    
    vec3 cameraDirection = normalize(v_cameraDirection);
    return computeLighting(normalVector, vertexToPointLightDirection, pointLightAttenuation, cameraDirection);
    
    #else
    
    return computeLighting(normalVector, vertexToPointLightDirection, pointLightAttenuation);
    
    #endif
}

#else

vec3 getLitPixel()
{
    // Normalize the vectors.
    vec3 normalVector = normalize(v_normalVector);    
    vec3 vertexToPointLightDirection = normalize(v_vertexToPointLightDirection);
    
    // Fetch point light attenuation.
    float pointLightAttenuation = v_pointLightAttenuation;
    
    #if defined (SPECULAR)
    
    vec3 cameraDirection = normalize(v_cameraDirection);    
    return computeLighting(normalVector, vertexToPointLightDirection, pointLightAttenuation, cameraDirection);
    
    #else
    
    return computeLighting(normalVector, vertexToPointLightDirection, pointLightAttenuation);
    
    #endif
}

#endif