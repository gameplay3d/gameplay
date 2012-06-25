uniform vec3 u_pointLightPosition;                  // Position
uniform float u_pointLightRangeInverse;             // Inverse of light range 
varying vec3 v_vertexToPointLightDirection;         // Direction of point light w.r.t current vertex in tangent space.
varying float v_pointLightAttenuation;              // Attenuation of point light.

#if defined(BUMPED)

void applyLight(mat3 tangentSpaceTransformMatrix)
{
    vec4 positionWorldViewSpace = u_worldViewMatrix * a_position;
    
    // Compute the light direction with light position and the vertex position.
    vec3 lightDirection = u_pointLightPosition - positionWorldViewSpace.xyz;
    
    // Transform current light direction to tangent space.
    vec3 vertexToPointLightDirection = tangentSpaceTransformMatrix * lightDirection;

    // Attenuation
    v_pointLightAttenuation = 1.0 - dot(lightDirection * u_pointLightRangeInverse, lightDirection * u_pointLightRangeInverse);

    // Output light direction.
    v_vertexToPointLightDirection =  vertexToPointLightDirection;
    
    #if defined (SPECULAR)
 
    // Compute camera direction and transform it to tangent space.
    v_cameraDirection = tangentSpaceTransformMatrix * (u_cameraPosition - positionWorldViewSpace.xyz);
    
    #endif
}

#else

void applyLight(vec4 position)
{
    // World space position.
    vec4 positionWorldViewSpace = u_worldViewMatrix * position;
    
    // Compute the light direction.
    vec3 lightDirection = u_pointLightPosition - positionWorldViewSpace.xyz;
   
    vec4 vertexToPointLightDirection;
    vertexToPointLightDirection.xyz = lightDirection;
   
    // Attenuation
    v_pointLightAttenuation = 1.0 - dot(lightDirection * u_pointLightRangeInverse, lightDirection * u_pointLightRangeInverse);

    // Output light direction.
    v_vertexToPointLightDirection =  vertexToPointLightDirection;
   
    #if defined (SPECULAR)
   
    vec3 cameraDirection = normalize(v_cameraDirection);

    #endif
}

#endif