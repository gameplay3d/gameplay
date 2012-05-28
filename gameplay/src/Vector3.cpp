#include "Base.h"
#include "Vector3.h"
#include "MathUtil.h"

namespace gameplay
{

Vector3::Vector3()
    : x(0.0f), y(0.0f), z(0.0f)
{
}

Vector3::Vector3(float x, float y, float z)
{
    set(x, y, z);
}

Vector3::Vector3(const float* array)
{
    set(array);
}

Vector3::Vector3(const Vector3& p1, const Vector3& p2)
{
    set(p1, p2);
}

Vector3::Vector3(const Vector3& copy)
{
    set(copy);
}

Vector3 Vector3::fromColor(unsigned int color)
{
    float components[3];
    int componentIndex = 0;
    for (int i = 2; i >= 0; --i)
    {
        int component = (color >> i*8) & 0x0000ff;

        components[componentIndex++] = static_cast<float>(component) / 255.0f;
    }

    Vector3 value(components);
    return value;
}

Vector3::~Vector3()
{
}

const Vector3& Vector3::zero()
{
    static Vector3 value(0.0f, 0.0f, 0.0f);
    return value;
}

const Vector3& Vector3::one()
{
    static Vector3 value(1.0f, 1.0f, 1.0f);
    return value;
}

const Vector3& Vector3::unitX()
{
    static Vector3 value(1.0f, 0.0f, 0.0f);
    return value;
}

const Vector3& Vector3::unitY()
{
    static Vector3 value(0.0f, 1.0f, 0.0f);
    return value;
}

const Vector3& Vector3::unitZ()
{
    static Vector3 value(0.0f, 0.0f, 1.0f);
    return value;
}

bool Vector3::isZero() const
{
    return x == 0.0f && y == 0.0f && z == 0.0f;
}

bool Vector3::isOne() const
{
    return x == 1.0f && y == 1.0f && z == 1.0f;
}

float Vector3::angle(const Vector3& v1, const Vector3& v2)
{
    float dx = v1.y * v2.z - v1.z * v2.y;
    float dy = v1.z * v2.x - v1.x * v2.z;
    float dz = v1.x * v2.y - v1.y * v2.x;

    return atan2f(sqrt(dx * dx + dy * dy + dz * dz) + MATH_FLOAT_SMALL, dot(v1, v2));
}

void Vector3::add(const Vector3& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
}

void Vector3::add(const Vector3& v1, const Vector3& v2, Vector3* dst)
{
    GP_ASSERT(dst);

    dst->x = v1.x + v2.x;
    dst->y = v1.y + v2.y;
    dst->z = v1.z + v2.z;
}

void Vector3::clamp(const Vector3& min, const Vector3& max)
{
    GP_ASSERT(!(min.x > max.x || min.y > max.y || min.z > max.z));

    // Clamp the x value.
    if (x < min.x)
        x = min.x;
    if (x > max.x)
        x = max.x;

    // Clamp the y value.
    if (y < min.y)
        y = min.y;
    if (y > max.y)
        y = max.y;

    // Clamp the z value.
    if (z < min.z)
        z = min.z;
    if (z > max.z)
        z = max.z;
}

void Vector3::clamp(const Vector3& v, const Vector3& min, const Vector3& max, Vector3* dst)
{
    GP_ASSERT(dst);
    GP_ASSERT(!(min.x > max.x || min.y > max.y || min.z > max.z));

    // Clamp the x value.
    dst->x = v.x;
    if (dst->x < min.x)
        dst->x = min.x;
    if (dst->x > max.x)
        dst->x = max.x;

    // Clamp the y value.
    dst->y = v.y;
    if (dst->y < min.y)
        dst->y = min.y;
    if (dst->y > max.y)
        dst->y = max.y;

    // Clamp the z value.
    dst->z = v.z;
    if (dst->z < min.z)
        dst->z = min.z;
    if (dst->z > max.z)
        dst->z = max.z;
}

void Vector3::cross(const Vector3& v)
{
    cross(*this, v, this);
}

void Vector3::cross(const Vector3& v1, const Vector3& v2, Vector3* dst)
{
    GP_ASSERT(dst);
/*
#ifdef USE_NEON

    asm volatile(
		"vld1.32 {d1[1]},  [%1] 		\n\t" //
		"vld1.32 {d0},     [%2]         \n\t" //
		"vmov.f32 s2, s1                \n\t" // q0 = (v1y, v1z, v1z, v1x)

		"vld1.32 {d2[1]},  [%3]	    	\n\t" //
		"vld1.32 {d3},     [%4]         \n\t" //
		"vmov.f32 s4, s7          		\n\t" // q1 = (v2z, v2x, v2y, v2z)

		"vmul.f32 d4, d0, d2  			\n\t" // x = v1y * v2z, y = v1z * v2x
		"vmls.f32 d4, d1, d3  			\n\t" // x -= v1z * v2y, y-= v1x - v2z

		"vmul.f32 d5, d3, d1[1]			\n\t" // z = v1x * v2y
		"vmls.f32 d5, d0, d2[1]         \n\t" // z-= v1y * vx

		"vst1.32 {d4}, 	  [%0]!    		\n\t" // V[x, y]
		"vst1.32 {d5[0]}, [%0]     		\n\t" // V[z]
		:
		: "r"(dst), "r"(&v1), "r"(&(v1.y)), "r"(&v2), "r"(&(v2.y))
		: "q0", "q1", "q2", "memory"
	);

#else

    float x = (v1.y * v2.z) - (v1.z * v2.y);
    float y = (v1.z * v2.x) - (v1.x * v2.z);
    float z = (v1.x * v2.y) - (v1.y * v2.x);
    dst->x = x;
    dst->y = y;
    dst->z = z;

#endif
*/
    MathUtil::crossVector3((const float*)&v1, (const float*)&v2, (float*)dst);
}

float Vector3::distance(const Vector3& v) const
{
    float dx = v.x - x;
    float dy = v.y - y;
    float dz = v.z - z;

    return sqrt(dx * dx + dy * dy + dz * dz);
}

float Vector3::distanceSquared(const Vector3& v) const
{
    float dx = v.x - x;
    float dy = v.y - y;
    float dz = v.z - z;

    return (dx * dx + dy * dy + dz * dz);
}

float Vector3::dot(const Vector3& v) const
{
    return (x * v.x + y * v.y + z * v.z);
}

float Vector3::dot(const Vector3& v1, const Vector3& v2)
{
    return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

float Vector3::length() const
{
    return sqrt(x * x + y * y + z * z);
}

float Vector3::lengthSquared() const
{
    return (x * x + y * y + z * z);
}

void Vector3::negate()
{
    x = -x;
    y = -y;
    z = -z;
}

Vector3& Vector3::normalize()
{
    normalize(this);
    return *this;
}

void Vector3::normalize(Vector3* dst) const
{
    GP_ASSERT(dst);

    if (dst != this)
    {
        dst->x = x;
        dst->y = y;
        dst->z = z;
    }

    float n = x * x + y * y + z * z;
    // Already normalized.
    if (n == 1.0f)
        return;

    n = sqrt(n);
    // Too close to zero.
    if (n < MATH_TOLERANCE)
        return;

    n = 1.0f / n;
    dst->x *= n;
    dst->y *= n;
    dst->z *= n;
}

void Vector3::scale(float scalar)
{
    x *= scalar;
    y *= scalar;
    z *= scalar;
}

void Vector3::set(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

void Vector3::set(const float* array)
{
    GP_ASSERT(array);

    x = array[0];
    y = array[1];
    z = array[2];
}

void Vector3::set(const Vector3& v)
{
    this->x = v.x;
    this->y = v.y;
    this->z = v.z;
}

void Vector3::set(const Vector3& p1, const Vector3& p2)
{
    x = p2.x - p1.x;
    y = p2.y - p1.y;
    z = p2.z - p1.z;
}

void Vector3::subtract(const Vector3& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
}

void Vector3::subtract(const Vector3& v1, const Vector3& v2, Vector3* dst)
{
    GP_ASSERT(dst);

    dst->x = v1.x - v2.x;
    dst->y = v1.y - v2.y;
    dst->z = v1.z - v2.z;
}

}
