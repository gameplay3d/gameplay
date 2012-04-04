

namespace gameplay
{

inline float bezier(float eq0, float eq1, float eq2, float eq3, float from, float out, float to, float in)
{
    return from * eq0 + out * eq1 + in * eq2 + to * eq3;
}

inline float bspline(float eq0, float eq1, float eq2, float eq3, float c0, float c1, float c2, float c3)
{
    return c0 * eq0 + c1 * eq1 + c2 * eq2 + c3 * eq3;
}

inline float hermite(float h00, float h01, float h10, float h11, float from, float out, float to, float in)
{
    return h00 * from + h01 * to + h10 * out + h11 * in;
}

inline float hermiteFlat(float h00, float h01, float from, float to)
{
    return h00 * from + h01 * to;
}

inline float hermiteSmooth(float h00, float h01, float h10, float h11, float from, float out, float to, float in)
{
    return h00 * from + h01 * to + h10 * out + h11 * in;
}

inline float lerpInl(float s, float from, float to)
{
    return from + (to - from) * s;
}

}