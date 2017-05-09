#include "Base.h"
#include "MathUtil.h"

namespace gameplay
{

void MathUtil::smooth(float* x, float target, float elapsedTime, float responseTime)
{
    GP_ASSERT(x);
    if (elapsedTime > 0)
    {
        *x += (target - *x) * elapsedTime / (elapsedTime + responseTime);
    }
}

void MathUtil::smooth(float* x, float target, float elapsedTime, float riseTime, float fallTime)
{
    GP_ASSERT(x);
    if (elapsedTime > 0)
    {
        float delta = target - *x;
        *x += delta * elapsedTime / (elapsedTime + (delta > 0 ? riseTime : fallTime));
    }
}

float MathUtil::lerp(float t, float from, float to)
{
	return from + (to - from) * t;
}

float MathUtil::bezier(float eq0, float eq1, float eq2, float eq3, float from, float out, float to, float in)
{
	return from * eq0 + out * eq1 + in * eq2 + to * eq3;
}

float MathUtil::bspline(float eq0, float eq1, float eq2, float eq3, float c0, float c1, float c2, float c3)
{
	return c0 * eq0 + c1 * eq1 + c2 * eq2 + c3 * eq3;
}

float MathUtil::hermite(float h00, float h01, float h10, float h11, float from, float out, float to, float in)
{
	return h00 * from + h01 * to + h10 * out + h11 * in;
}

float MathUtil::hermiteFlat(float h00, float h01, float from, float to)
{
	return h00 * from + h01 * to;
}

float MathUtil::hermiteSmooth(float h00, float h01, float h10, float h11, float from, float out, float to, float in)
{
	return h00 * from + h01 * to + h10 * out + h11 * in;
}

}
