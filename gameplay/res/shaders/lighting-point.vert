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
    // World view space position.
	vec4 positionWorldViewSpace = u_worldViewMatrix * position;
    
    // Compute the light direction with light position and the vertex position.
	v_vertexToPointLightDirection = u_pointLightPosition - positionWorldViewSpace.xyz;
   
    // Attenuation
    v_pointLightAttenuation = 1.0 - dot(v_vertexToPointLightDirection * u_pointLightRangeInverse, v_vertexToPointLightDirection * u_pointLightRangeInverse);

    // Output light direction.
    //v_vertexToPointLightDirection = lightDirection;
   
    #if defined (SPECULAR)  

	v_cameraDirection = u_cameraPosition - positionWorldViewSpace.xyz;
	
    #endif
}

#endif