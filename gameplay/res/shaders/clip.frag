#if defined(CLIP_PLANE)
varying float v_clipDistance;

void clipFrag()
{
    if(v_clipDistance < 0.0) discard;
}
#endif