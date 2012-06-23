varying vec3 v_lightDirection;                 // Direction of light in tangent space.

#if defined(BUMPED)

vec3 getLitPixel()
{
    // Fetch normals from the normal map
    vec3 normalVector = normalize(texture2D(u_textureNormal, v_texCoord).rgb * 2.0 - 1.0);
    vec3 lightDirection = normalize(v_lightDirection);
    
    #if defined(SPECULAR)
    
    vec3 cameraDirection = normalize(v_cameraDirection);
    return computeLighting(normalVector, -lightDirection, 1.0, cameraDirection);
    
    #else
    
    return computeLighting(normalVector, -lightDirection, 1.0);
    
    #endif
}

#else

vec3 getLitPixel()
{
    // Normalize the vectors.
    vec3 normalVector = normalize(v_normalVector);
    vec3 lightDirection = normalize(u_lightDirection);

    #if defined(SPECULAR)
    
    vec3 cameraDirection = normalize(v_cameraDirection);
    return computeLighting(normalVector, -lightDirection, 1.0, cameraDirection);
    
    #else
    
    return computeLighting(normalVector, -lightDirection, 1.0);
    
    #endif
}

#endif