#if defined(BUMPED)

void applyLight(mat3 tangentSpaceTransformMatrix)
{
    vec4 positionWorldViewSpace = u_worldViewMatrix * a_position;

    // Transform spot light direction to tangent space.
    v_spotLightDirection = tangentSpaceTransformMatrix * u_spotLightDirection;

    // Compute the light direction with light position and the vertex position.
    vec3 lightDirection = u_spotLightPosition - positionWorldViewSpace.xyz;
    
    // Transform current light direction to tangent space.
    lightDirection = tangentSpaceTransformMatrix * lightDirection;
    
    // Attenuation
    v_spotLightAttenuation = 1.0 - dot(lightDirection * u_spotLightRangeInverse, lightDirection * u_spotLightRangeInverse);

    // Output light direction.
    v_vertexToSpotLightDirection = lightDirection;
    
    #if defined(SPECULAR)
    
    // Compute camera direction and transform it to tangent space.
    v_cameraDirection = tangentSpaceTransformMatrix * (u_cameraPosition - positionWorldViewSpace.xyz);
    
    #endif
}

#else

void applyLight(vec4 position)
{
    // World space position.
    vec4 positionWorldViewSpace = u_worldViewMatrix * position;

    // Compute the light direction with light position and the vertex position.
    vec3 lightDirection = u_spotLightPosition - positionWorldViewSpace.xyz;

    // Attenuation
    v_spotLightAttenuation = 1.0 - dot(lightDirection * u_spotLightRangeInverse, lightDirection * u_spotLightRangeInverse);

    // Output light direction.
    v_vertexToSpotLightDirection = lightDirection;
    
    #if defined(SPECULAR)
    
    // Compute camera direction and transform it to tangent space.
    v_cameraDirection = tangentSpaceTransformMatrix * (u_cameraPosition - positionWorldViewSpace.xyz);
    
    #endif
}

#endif