
#if defined(BUMPED)
void applyLight(vec4 position, mat3 tangentSpaceTransformMatrix)
{
    #if (defined(SPECULAR) || (POINT_LIGHT_COUNT > 0) || (SPOT_LIGHT_COUNT > 0))
    vec4 positionWorldViewSpace = u_worldViewMatrix * position;
    #endif
    
    #if (DIRECTIONAL_LIGHT_COUNT > 0)
    for (int i = 0; i < DIRECTIONAL_LIGHT_COUNT; ++i)
    {
        // Transform light direction to tangent space
        v_directionalLightDirection[i] = tangentSpaceTransformMatrix * u_directionalLightDirection[i];
    }
    #endif
    
    #if (POINT_LIGHT_COUNT > 0)
    for (int i = 0; i < POINT_LIGHT_COUNT; ++i)
    {
        // Compute the vertex to light direction, in tangent space
        v_vertexToPointLightDirection[i] = tangentSpaceTransformMatrix * (u_pointLightPosition[i] - positionWorldViewSpace.xyz);
    }
    #endif
    
    #if (SPOT_LIGHT_COUNT > 0)
    for (int i = 0; i < SPOT_LIGHT_COUNT; ++i)
    {
        // Compute the vertex to light direction, in tangent space
	    v_vertexToSpotLightDirection[i] = tangentSpaceTransformMatrix * (u_spotLightPosition[i] - positionWorldViewSpace.xyz);
        v_spotLightDirection[i] = tangentSpaceTransformMatrix * u_spotLightDirection[i];
    }
    #endif
    
    #if defined(SPECULAR)
    // Compute camera direction and transform it to tangent space.
    v_cameraDirection = tangentSpaceTransformMatrix * (u_cameraPosition - positionWorldViewSpace.xyz);
    #endif
}
#else
void applyLight(vec4 position)
{
    #if defined(SPECULAR) || (POINT_LIGHT_COUNT > 0) || (SPOT_LIGHT_COUNT > 0)
	vec4 positionWorldViewSpace = u_worldViewMatrix * position;
    #endif

    #if (POINT_LIGHT_COUNT > 0)
    for (int i = 0; i < POINT_LIGHT_COUNT; ++i)
    {
        // Compute the light direction with light position and the vertex position.
        v_vertexToPointLightDirection[i] = u_pointLightPosition[i] - positionWorldViewSpace.xyz;
    }
    #endif

    #if (SPOT_LIGHT_COUNT > 0)
    for (int i = 0; i < SPOT_LIGHT_COUNT; ++i)
    {
        // Compute the light direction with light position and the vertex position.
	    v_vertexToSpotLightDirection[i] = u_spotLightPosition[i] - positionWorldViewSpace.xyz;
    }
    #endif

    #if defined(SPECULAR)  
	v_cameraDirection = u_cameraPosition - positionWorldViewSpace.xyz;
    #endif
}

#endif
