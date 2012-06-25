#if defined(BUMPED)

void applyLight(mat3 tangentSpaceTransformMatrix)
{
    // Transform light direction to tangent space
    v_lightDirection = tangentSpaceTransformMatrix * u_lightDirection;
    
    #if defined(SPECULAR)

    // Compute the camera direction for specular lighting
    vec4 positionWorldSpace = u_worldViewMatrix * a_position;
    v_cameraDirection = u_cameraPosition - positionWorldSpace.xyz;
    
    #endif
}

#else

void applyLight(vec4 position)
{
    
    #if defined(SPECULAR)

    // Compute the camera direction for specular lighting
    vec4 positionWorldSpace = u_worldViewMatrix * position;
    v_cameraDirection = u_cameraPosition - positionWorldSpace.xyz;

    #endif
}

#endif
