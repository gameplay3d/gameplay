uniform float u_spotLightInnerAngleCos;       // The bright spot [0.0 - 1.0]
uniform float u_spotLightOuterAngleCos;       // The soft outer part [0.0 - 1.0]

varying vec3 v_spotLightDirection;            // Direction of spot light in tangent space.
varying vec3 v_vertexToSpotLightDirection;    // Direction of the spot light w.r.t current vertex in tangent space.
varying float v_spotLightAttenuation;         // Attenuation of spot light.

float lerpstep( float lower, float upper, float s)
{
    return clamp( ( s - lower ) / ( upper - lower ), 0.0, 1.0 );
}

#if defined(BUMPED)

vec3 getLitPixel()
{
    // Fetch normals from the normal map.
    vec3 normalVector = normalize(texture2D(u_textureNormal, v_texCoord).rgb * 2.0 - 1.0);
    vec3 spotLightDirection = normalize(v_spotLightDirection);
    vec3 vertexToSpotLightDirection = normalize(v_vertexToSpotLightDirection);
    
    // "-lightDirection" because light direction points in opposite direction to
    // to spot direction.
    // Calculate spot light effect.
    float spotCurrentAngleCos = max(0.0, dot(spotLightDirection, -vertexToSpotLightDirection));
    
    // Intensity of spot depends on the spot light attenuation and the 
    // part of the cone vertexToSpotLightDirection points to (inner or outer).
    float spotLightAttenuation = clamp(v_spotLightAttenuation, 0.0, 1.0);
    spotLightAttenuation *= lerpstep(u_spotLightOuterAngleCos, u_spotLightInnerAngleCos, spotCurrentAngleCos);

    #if defined(SPECULAR)
    
    vec3 cameraDirection = normalize(v_cameraDirection);
    return computeLighting(normalVector, vertexToSpotLightDirection, spotLightAttenuation, cameraDirection);
    
    #else
    
    return computeLighting(normalVector, vertexToSpotLightDirection, spotLightAttenuation);
    
    #endif
}

#else

vec3 getLitPixel()
{
    // Normalize the vectors.
    vec3 normalVector = normalize(v_normalVector);
    vec3 spotLightDirection = normalize(u_spotLightDirection); 
    vec3 vertexToSpotLightDirection = normalize(v_vertexToSpotLightDirection);

    // "-lightDirection" is used because light direction points in opposite direction to to spot direction.
    // Calculate spot light effect.
    float spotCurrentAngleCos = max(0.0, dot(spotLightDirection, -vertexToSpotLightDirection));
    
    // Intensity of spot depends on the spot light attenuation and the 
    // part of the cone vertexToSpotLightDirection points to (inner or outer).
    float spotLightAttenuation = clamp(v_spotLightAttenuation, 0.0, 1.0);
    spotLightAttenuation *= lerpstep(u_spotLightOuterAngleCos, u_spotLightInnerAngleCos, spotCurrentAngleCos);

    #if defined(SPECULAR)
    
    vec3 cameraDirection = normalize(v_cameraDirection);
    return computeLighting(normalVector, vertexToSpotLightDirection, spotLightAttenuation, cameraDirection);
    
    #else
    
    return computeLighting(normalVector, vertexToSpotLightDirection, spotLightAttenuation);
    
    #endif
}

#endif