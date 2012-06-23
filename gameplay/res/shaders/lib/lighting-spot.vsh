#if defined(BUMPED)

uniform vec3 u_spotLightPosition;                   // Position
uniform float u_spotLightRangeInverse;              // Inverse of light range.
uniform vec3 u_spotLightDirection;                  // Direction

varying vec3 v_spotLightDirection;                  // Direction of spot light in tangent space.
varying vec3 v_vertexToSpotLightDirection;          // Direction of the spot light w.r.t current vertex in tangent space.
varying float v_spotLightAttenuation;               // Attenuation of spot light.

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

uniform vec3 u_spotLightPosition;                   // Position
uniform float u_spotLightRangeInverse;              // Inverse of light range.

varying vec3 v_vertexToSpotLightDirection;          // Light direction w.r.t current vertex.
varying float v_spotLightAttenuation;               // Attenuation of spot light.

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