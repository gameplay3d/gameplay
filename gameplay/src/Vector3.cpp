#include "Base.h"
#include "Vector3.h"

namespace gameplay
{

Vector3::Vector3()
{
}

Vector3::Vector3(float x, float y, float z) : 
    x(x),
    y(y),
    z(z)
{
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

Vector3 Vector3::fromColor(uint32_t color)
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

Vector3 Vector3::fromColorString(const char* str)
{
    Vector3 value = Vector3(0, 0, 0);
    if (str[0] == '#' && strlen(str) == 7 )
    {
        uint32_t color;
        if (sscanf(str + 1, "%x", &color) == 1)
            value = Vector3::fromColor(color);
    }
    return value;
}

uint32_t Vector3::toColor() const
{
    unsigned char component;
    uint32_t value = 0;
    // Red component
    component = static_cast<unsigned char>(x * 255);
    value += component << 16;
    // Green component
    component = static_cast<unsigned char>(y * 255);
    value += component << 8;
    // Blue component
    component = static_cast<unsigned char>(z * 255);
    value += component;
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

const Vector3& Vector3::up()
{
    static Vector3 value(0.0f, 1.0f, 0.0f);
    return value;
}

const Vector3& Vector3::down()
{
    static Vector3 value(0.0f, -1.0f, 0.0f);
    return value;
}

const Vector3& Vector3::right()
{
    static Vector3 value(1.0f, 0.0f, 0.0f);
    return value;
}

const Vector3& Vector3::left()
{
    static Vector3 value(-1.0f, 0.0f, 0.0f);
    return value;
}

const Vector3& Vector3::forward()
{
    static Vector3 value(0.0f, 0.0f, 1.0f);
    return value;
}

const Vector3& Vector3::back()
{
    static Vector3 value(0.0f, 0.0f, -1.0f);
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

    return std::atan2(std::sqrt(dx * dx + dy * dy + dz * dz) + GP_MATH_FLOAT_SMALL, dot(v1, v2));
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

    if (x < min.x)
        x = min.x;
    if (x > max.x)
        x = max.x;
    if (y < min.y)
        y = min.y;
    if (y > max.y)
        y = max.y;
    if (z < min.z)
        z = min.z;
    if (z > max.z)
        z = max.z;
}

void Vector3::clamp(const Vector3& v, const Vector3& min, const Vector3& max, Vector3* dst)
{
    GP_ASSERT(dst);
    GP_ASSERT(!(min.x > max.x || min.y > max.y || min.z > max.z));

    dst->x = v.x;
    if (dst->x < min.x)
        dst->x = min.x;
    if (dst->x > max.x)
        dst->x = max.x;
    dst->y = v.y;
    if (dst->y < min.y)
        dst->y = min.y;
    if (dst->y > max.y)
        dst->y = max.y;
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
    dst->x = (v1.y * v2.z) - (v1.z * v2.y);
    dst->y = (v1.z * v2.x) - (v1.x * v2.z);
    dst->z = (v1.x * v2.y) - (v1.y * v2.x);
}

float Vector3::distance(const Vector3& v) const
{
    float dx = v.x - x;
    float dy = v.y - y;
    float dz = v.z - z;

    return std::sqrt(dx * dx + dy * dy + dz * dz);
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
    return std::sqrt(x * x + y * y + z * z);
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

    n = std::sqrt(n);
    // Too close to zero.
    if (n < GP_MATH_TOLERANCE)
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

void Vector3::smooth(const Vector3& target, float elapsedTime, float responseTime)
{
    if (elapsedTime > 0)
    {
        *this += (target - *this) * (elapsedTime / (elapsedTime + responseTime));
    }
}

Vector3& Vector3::operator=(const Vector3& v)
{
    if(&v == this)
        return *this;

    this->x = v.x;
    this->y = v.y;
    this->z = v.z;

    return *this;
}

const Vector3 Vector3::operator+(const Vector3& v) const
{
    Vector3 result(*this);
    result.add(v);
    return result;
}

Vector3& Vector3::operator+=(const Vector3& v)
{
    add(v);
    return *this;
}

const Vector3 Vector3::operator-(const Vector3& v) const
{
    Vector3 result(*this);
    result.subtract(v);
    return result;
}

Vector3& Vector3::operator-=(const Vector3& v)
{
    subtract(v);
    return *this;
}

const Vector3 Vector3::operator-() const
{
    Vector3 result(*this);
    result.negate();
    return result;
}

const Vector3 Vector3::operator*(float x) const
{
    Vector3 result(*this);
    result.scale(x);
    return result;
}

Vector3& Vector3::operator*=(float x)
{
    scale(x);
    return *this;
}

const Vector3 Vector3::operator/(const float x) const
{
    return Vector3(this->x / x, this->y / x, this->z / x);
}

bool Vector3::operator<(const Vector3& v) const
{
    if (x == v.x)
    {
        if (y == v.y)
        {
            return z < v.z;
        }
        return y < v.y;
    }
    return x < v.x;
}

bool Vector3::operator==(const Vector3& v) const
{
    return x == v.x && y == v.y && z == v.z;
}

bool Vector3::operator!=(const Vector3& v) const
{
    return x != v.x || y != v.y || z != v.z;
}

const Vector3 operator*(float x, const Vector3& v)
{
    Vector3 result(v);
    result.scale(x);
    return result;
}

}
