#if defined(BUMPED)

uniform vec3 u_lightDirection;                  // Direction of light
varying vec3 v_lightDirection;                  // Direction of light in tangent space.

void applyLight(mat3 tangentSpaceTransformMatrix)
{
    // Transform light direction to tangent space
    v_lightDirection = tangentSpaceTransformMatrix * u_lightDirection;
    
    // Compute the camera direction for specular lighting
    #if defined(SPECULAR)
    
    vec4 positionWorldSpace = u_worldViewMatrix * a_position;
    v_cameraDirection = u_cameraPosition - positionWorldSpace.xyz;
    
    #endif
}

#else

void applyLight(vec4 position)
{
    // Compute the camera direction for specular lighting
    #if defined(SPECULAR)
    
    vec4 positionWorldSpace = u_worldViewMatrix * position;
    v_cameraDirection = u_cameraPosition - positionWorldSpace.xyz;

    #endif
}

#endif
