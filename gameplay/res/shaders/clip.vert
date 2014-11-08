#if defined (CLIP_PLANE)

uniform vec4 u_clipPlane;
uniform mat4 u_worldMatrix;

varying float v_clipDistance;

void clipVert(vec4 position)
{
    v_clipDistance = dot(u_worldMatrix * position, u_clipPlane);
}

#endif