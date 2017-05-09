#include "Base.h"
#include "Quaternion.h"

namespace gameplay
{

Quaternion::Quaternion()
{
}

Quaternion::Quaternion(float x, float y, float z, float w) : 
    x(x),
    y(y),
    z(z),
    w(w)
{
}

Quaternion::Quaternion(const float* array)
{
    set(array);
}

Quaternion::Quaternion(const Matrix& m)
{
    set(m);
}

Quaternion::Quaternion(const Vector3& axis, float angle)
{
    set(axis, angle);
}

Quaternion::Quaternion(const Vector3& euler)
{
    set(euler);
}

Quaternion::Quaternion(const Quaternion& copy)
{
    set(copy);
}

Quaternion::~Quaternion()
{
}

const Quaternion& Quaternion::identity()
{
    static Quaternion value(0.0f, 0.0f, 0.0f, 1.0f);
    return value;
}

const Quaternion& Quaternion::zero()
{
    static Quaternion value(0.0f, 0.0f, 0.0f, 0.0f);
    return value;
}

bool Quaternion::isIdentity() const
{
    return x == 0.0f && y == 0.0f && z == 0.0f && w == 1.0f;
}

bool Quaternion::isZero() const
{
    return x == 0.0f && y == 0.0f && z == 0.0f && w == 0.0f;
}

void Quaternion::createFromEulerAngles(const Vector3& eulerAngles, Quaternion* dst)
{
    GP_ASSERT(dst);

	float halfToRad = 0.5f * GP_MATH_PIOVER180;
	float ex = eulerAngles.x * halfToRad;
    float ey = eulerAngles.y * halfToRad;
    float ez = eulerAngles.z * halfToRad;

    float sx = std::sin(ex);
    float cx = std::cos(ex);
    float sy = std::sin(ey);
    float cy = std::cos(ey);
    float sz = std::sin(ez);
    float cz = std::cos(ez);

    dst->x = sx * cy * cz - cx * sy * sz;
    dst->y = cx * sy * cz + sx * cy * sz;
    dst->z = cx * cy * sz - sx * sy * cz;
    dst->w = cx * cy * cz + sx * sy * sz;
}

void Quaternion::createFromAxisAngle(const Vector3& axis, float angle, Quaternion* dst)
{
    GP_ASSERT(dst);

    float halfAngle = GP_MATH_DEG_TO_RAD(angle) * 0.5f;
    float sinHalfAngle = sinf(halfAngle);

    Vector3 normal(axis);
    normal.normalize();
    dst->x = normal.x * sinHalfAngle;
    dst->y = normal.y * sinHalfAngle;
    dst->z = normal.z * sinHalfAngle;
    dst->w = std::cos(halfAngle);
}

void Quaternion::createFromRotationMatrix(const Matrix& m, Quaternion* dst)
{
	GP_ASSERT(dst);

    float m00 = m.m[0];
    float m01 = m.m[1];
    float m02 = m.m[2];
    float m10 = m.m[4];
    float m11 = m.m[5];
    float m12 = m.m[6];
    float m20 = m.m[8];
    float m21 = m.m[9];
    float m22 = m.m[10];

    float lx = 1 / std::sqrt(m00 * m00 + m01 * m01 + m02 * m02);
    float ly = 1 / std::sqrt(m10 * m10 + m11 * m11 + m12 * m12);
    float lz = 1 / std::sqrt(m20 * m20 + m21 * m21 + m22 * m22);

    m00 *= lx;
    m01 *= lx;
    m02 *= lx;
    m10 *= ly;
    m11 *= ly;
    m12 *= ly;
    m20 *= lz;
    m21 *= lz;
    m22 *= lz;

	float s, rs;
    float tr = m00 + m11 + m22;
    if (tr >= 0) 
	{
        s = std::sqrt(tr + 1);
        dst->w = s * 0.5f;
        s = 0.5f / s;
        dst->x = (m12 - m21) * s;
        dst->y = (m20 - m02) * s;
        dst->z = (m01 - m10) * s;
    } 
	else 
	{
        if (m00 > m11) 
		{
            if (m00 > m22) 
			{
                rs = (m00 - (m11 + m22)) + 1;
                rs = std::sqrt(rs);
                dst->x = rs * 0.5f;
                rs = 0.5f / rs;
                dst->w = (m12 - m21) * rs;
                dst->y = (m01 + m10) * rs;
                dst->z = (m02 + m20) * rs;
            } 
			else 
			{
                rs = (m22 - (m00 + m11)) + 1;
                rs = std::sqrt(rs);
                dst->z = rs * 0.5f;
                rs = 0.5f / rs;
                dst->w = (m01 - m10) * rs;
                dst->x = (m20 + m02) * rs;
                dst->y = (m21 + m12) * rs;
            }
        } 
		else if (m11 > m22) 
		{
            rs = (m11 - (m22 + m00)) + 1;
            rs = std::sqrt(rs);
            dst->y = rs * 0.5f;
            rs = 0.5f / rs;
            dst->w = (m20 - m02) * rs;
            dst->z = (m12 + m21) * rs;
            dst->x = (m10 + m01) * rs;
        } 
		else 
		{
            rs = (m22 - (m00 + m11)) + 1;
            rs = std::sqrt(rs);
            dst->z = rs * 0.5f;
            rs = 0.5f / rs;
            dst->w = (m01 - m10) * rs;
            dst->x = (m20 + m02) * rs;
            dst->y = (m21 + m12) * rs;
        }
    }
}

void Quaternion::conjugate()
{
    conjugate(this);
}

void Quaternion::conjugate(Quaternion* dst) const
{
    GP_ASSERT(dst);

    dst->x = -x;
    dst->y = -y;
    dst->z = -z;
    dst->w =  w;
}

bool Quaternion::inverse()
{
    return inverse(this);
}

bool Quaternion::inverse(Quaternion* dst) const
{
    GP_ASSERT(dst);

    float n = x * x + y * y + z * z + w * w;
    if (n == 1.0f)
    {
        dst->x = -x;
        dst->y = -y;
        dst->z = -z;
        dst->w = w;

        return true;
    }

    // Too close to zero.
    if (n < 0.000001f)
        return false;

    n = 1.0f / n;
    dst->x = -x * n;
    dst->y = -y * n;
    dst->z = -z * n;
    dst->w = w * n;

    return true;
}

void Quaternion::multiply(const Quaternion& q)
{
    multiply(*this, q, this);
}

void Quaternion::multiply(const Quaternion& q1, const Quaternion& q2, Quaternion* dst)
{
    GP_ASSERT(dst);
	
	dst->w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;
	dst->x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y;
    dst->y = q1.w * q2.y + q1.y * q2.w + q1.z * q2.x - q1.x * q2.z;
    dst->z = q1.w * q2.z + q1.z * q2.w + q1.x * q2.y - q1.y * q2.x;

}

void Quaternion::normalize()
{
    normalize(this);
}

void Quaternion::normalize(Quaternion* dst) const
{
    GP_ASSERT(dst);

    if (this != dst)
    {
        dst->x = x;
        dst->y = y;
        dst->z = z;
        dst->w = w;
    }

    float n = x * x + y * y + z * z + w * w;

    // Already normalized.
    if (n == 1.0f)
        return;

    n = std::sqrt(n);
    // Too close to zero.
    if (n < 0.000001f)
        return;

    n = 1.0f / n;
    dst->x *= n;
    dst->y *= n;
    dst->z *= n;
    dst->w *= n;
}

void Quaternion::rotatePoint(const Vector3& point, Vector3* dst) const
{
	Quaternion vecQuat;
	Quaternion resQuat;
	vecQuat.x = point.x;
	vecQuat.y = point.y;
	vecQuat.z = point.z;
	vecQuat.w = 0.0f;

	Quaternion conQuat;
	this->conjugate(&conQuat);

	resQuat = vecQuat * conQuat;
	resQuat = (*this) * resQuat;

	dst->set(resQuat.x, resQuat.y, resQuat.z);
}

void Quaternion::set(float x, float y, float z, float w)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

void Quaternion::set(const float* array)
{
    GP_ASSERT(array);

    x = array[0];
    y = array[1];
    z = array[2];
    w = array[3];
}

void Quaternion::set(const Vector3& eulerAngles)
{
    Quaternion::createFromEulerAngles(eulerAngles, this);
}

void Quaternion::set(const Vector3& axis, float angle)
{
    Quaternion::createFromAxisAngle(axis, angle, this);
}

void Quaternion::set(const Matrix& m)
{
    Quaternion::createFromRotationMatrix(m, this);
}

void Quaternion::set(const Quaternion& q)
{
    this->x = q.x;
    this->y = q.y;
    this->z = q.z;
    this->w = q.w;
}

void Quaternion::setIdentity()
{
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
    w = 1.0f;
}

void Quaternion::toEulerAngles(Vector3* eulerAngles) const
{
	GP_ASSERT(eulerAngles);

    float a2 = 2 * (w * y - x * z);
    if (a2 <= -0.99999f) 
	{
        eulerAngles->x = 2 * std::atan2(x, w);
		eulerAngles->y = -GP_MATH_PIOVER2;
        eulerAngles->z = 0;
    } 
	else if (a2 >= 0.99999f) 
	{
		eulerAngles->x = 2 * std::atan2(x, w);
        eulerAngles->y = GP_MATH_PIOVER2;
        eulerAngles->z = 0;
    } 
	else 
	{
		eulerAngles->x = std::atan2(2 * (w * x + y * z), 1 - 2 * (x * x + y * y));
        eulerAngles->y = std::asin(a2);
        eulerAngles->z = std::atan2(2 * (w * z + x * y), 1 - 2 * (y * y + z * z));
    }
    eulerAngles->scale(GP_MATH_180OVERPI);

	// Make positive
	/*
	float num = -0.005729578f;
	float num2 = 360.0f + num;
	if (eulerAngles->x < num)
	{
		eulerAngles->x += 360.0f;
	}
	else if( eulerAngles->x > num2)
	{
		eulerAngles->x -= 360.0f;
	}
	if (eulerAngles->y < num)
	{
		eulerAngles->y += 360.0f;
	}
	else if (eulerAngles->y > num2)
	{
		eulerAngles->y -= 360.0f;
	}
	if (eulerAngles->z < num)
	{
		eulerAngles->z += 360.0f;
	}
	else if (eulerAngles->z > num2)
	{
		eulerAngles->z -= 360.0f;
	}*/
}

void Quaternion::transformVector(const Vector3& v, Vector3* dst)
{
	GP_ASSERT(dst);

	float x = v.x;
	float y = v.y;
	float z = v.z;
	float qx = this->x;
	float qy = this->y;
	float qz = this->z;
	float qw = this->w;
    float ix = qw * x + qy * z - qz * y;
    float iy = qw * y + qz * x - qx * z;
    float iz = qw * z + qx * y - qy * x;
    float iw = -qx * x - qy * y - qz * z;
    
    dst->x = ix * qw + iw * -qx + iy * -qz - iz * -qy;
    dst->y = iy * qw + iw * -qy + iz * -qx - ix * -qz;
    dst->z = iz * qw + iw * -qz + ix * -qy - iy * -qx;
}

float Quaternion::toAxisAngle(Vector3* axis) const
{
    Quaternion q(x, y, z, w);
    q.normalize();
    if (axis)
    {
        axis->x = q.x;
        axis->y = q.y;
        axis->z = q.z;
        axis->normalize();
    }
    return (2.0f * acos(q.w));
}

void Quaternion::lerp(const Quaternion& q1, const Quaternion& q2, float t, Quaternion* dst)
{
    GP_ASSERT(dst);
    GP_ASSERT(!(t < 0.0f || t > 1.0f));

    if (t == 0.0f)
    {
        std::memcpy(dst, &q1, sizeof(float) * 4);
        return;
    }
    else if (t == 1.0f)
    {
        std::memcpy(dst, &q2, sizeof(float) * 4);
        return;
    }

    float t1 = 1.0f - t;

    dst->x = t1 * q1.x + t * q2.x;
    dst->y = t1 * q1.y + t * q2.y;
    dst->z = t1 * q1.z + t * q2.z;
    dst->w = t1 * q1.w + t * q2.w;
}

void Quaternion::slerp(const Quaternion& q1, const Quaternion& q2, float t, Quaternion* dst)
{
    GP_ASSERT(dst);
    slerp(q1.x, q1.y, q1.z, q1.w, q2.x, q2.y, q2.z, q2.w, t, &dst->x, &dst->y, &dst->z, &dst->w);
}

void Quaternion::squad(const Quaternion& q1, const Quaternion& q2, const Quaternion& s1, const Quaternion& s2, float t, Quaternion* dst)
{
    GP_ASSERT(!(t < 0.0f || t > 1.0f));

    Quaternion dstQ(0.0f, 0.0f, 0.0f, 1.0f);
    Quaternion dstS(0.0f, 0.0f, 0.0f, 1.0f);

    slerpForSquad(q1, q2, t, &dstQ);
    slerpForSquad(s1, s2, t, &dstS);
    slerpForSquad(dstQ, dstS, 2.0f * t * (1.0f - t), dst);
}

void Quaternion::slerp(float q1x, float q1y, float q1z, float q1w, float q2x, float q2y, float q2z, float q2w, float t, float* dstx, float* dsty, float* dstz, float* dstw)
{
    // Fast slerp implementation by kwhatmough:
    // It contains no division operations, no trig, no inverse trig
    // and no sqrt. Not only does this code tolerate small constraint
    // errors in the input quaternions, it actually corrects for them.
    GP_ASSERT(dstx && dsty && dstz && dstw);
    GP_ASSERT(!(t < 0.0f || t > 1.0f));

    if (t == 0.0f)
    {
        *dstx = q1x;
        *dsty = q1y;
        *dstz = q1z;
        *dstw = q1w;
        return;
    }
    else if (t == 1.0f)
    {
        *dstx = q2x;
        *dsty = q2y;
        *dstz = q2z;
        *dstw = q2w;
        return;
    }
    if (q1x == q2x && q1y == q2y && q1z == q2z && q1w == q2w)
    {
        *dstx = q1x;
        *dsty = q1y;
        *dstz = q1z;
        *dstw = q1w;
        return;
    }

    float halfY, alpha, beta;
    float u, f1, f2a, f2b;
    float ratio1, ratio2;
    float halfSecHalfTheta, versHalfTheta;
    float sqNotU, sqU;
    float cosTheta = q1w * q2w + q1x * q2x + q1y * q2y + q1z * q2z;

    // As usual in all slerp implementations, we fold theta.
    alpha = cosTheta >= 0 ? 1.0f : -1.0f;
    halfY = 1.0f + alpha * cosTheta;

    // Here we bisect the interval, so we need to fold t as well.
    f2b = t - 0.5f;
    u = f2b >= 0 ? f2b : -f2b;
    f2a = u - f2b;
    f2b += u;
    u += u;
    f1 = 1.0f - u;

    // One iteration of Newton to get 1-cos(theta / 2) to good accuracy.
    halfSecHalfTheta = 1.09f - (0.476537f - 0.0903321f * halfY) * halfY;
    halfSecHalfTheta *= 1.5f - halfY * halfSecHalfTheta * halfSecHalfTheta;
    versHalfTheta = 1.0f - halfY * halfSecHalfTheta;

    // Evaluate series expansions of the coefficients.
    sqNotU = f1 * f1;
    ratio2 = 0.0000440917108f * versHalfTheta;
    ratio1 = -0.00158730159f + (sqNotU - 16.0f) * ratio2;
    ratio1 = 0.0333333333f + ratio1 * (sqNotU - 9.0f) * versHalfTheta;
    ratio1 = -0.333333333f + ratio1 * (sqNotU - 4.0f) * versHalfTheta;
    ratio1 = 1.0f + ratio1 * (sqNotU - 1.0f) * versHalfTheta;

    sqU = u * u;
    ratio2 = -0.00158730159f + (sqU - 16.0f) * ratio2;
    ratio2 = 0.0333333333f + ratio2 * (sqU - 9.0f) * versHalfTheta;
    ratio2 = -0.333333333f + ratio2 * (sqU - 4.0f) * versHalfTheta;
    ratio2 = 1.0f + ratio2 * (sqU - 1.0f) * versHalfTheta;

    // Perform the bisection and resolve the folding done earlier.
    f1 *= ratio1 * halfSecHalfTheta;
    f2a *= ratio2;
    f2b *= ratio2;
    alpha *= f1 + f2a;
    beta = f1 + f2b;

    // Apply final coefficients to a and b as usual.
    float w = alpha * q1w + beta * q2w;
    float x = alpha * q1x + beta * q2x;
    float y = alpha * q1y + beta * q2y;
    float z = alpha * q1z + beta * q2z;

    // This final adjustment to the quaternion's length corrects for
    // any small constraint error in the inputs q1 and q2 But as you
    // can see, it comes at the cost of 9 additional multiplication
    // operations. If this error-correcting feature is not required,
    // the following code may be removed.
    f1 = 1.5f - 0.5f * (w * w + x * x + y * y + z * z);
    *dstw = w * f1;
    *dstx = x * f1;
    *dsty = y * f1;
    *dstz = z * f1;
}

void Quaternion::slerpForSquad(const Quaternion& q1, const Quaternion& q2, float t, Quaternion* dst)
{
    GP_ASSERT(dst);

    // cos(omega) = q1 * q2;
    // slerp(q1, q2, t) = (q1*sin((1-t)*omega) + q2*sin(t*omega))/sin(omega);
    // q1 = +- q2, slerp(q1,q2,t) = q1.
    // This is a straight-forward implementation of the formula of slerp. It does not do any sign switching.
    float c = q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;

    if (std::fabs(c) >= 1.0f)
    {
        dst->x = q1.x;
        dst->y = q1.y;
        dst->z = q1.z;
        dst->w = q1.w;
        return;
    }

    float omega = std::acos(c);
    float s = std::sqrt(1.0f - c * c);
    if (std::fabs(s) <= 0.00001f)
    {
        dst->x = q1.x;
        dst->y = q1.y;
        dst->z = q1.z;
        dst->w = q1.w;
        return;
    }

    float r1 = std::sin((1 - t) * omega) / s;
    float r2 = std::sin(t * omega) / s;
    dst->x = (q1.x * r1 + q2.x * r2);
    dst->y = (q1.y * r1 + q2.y * r2);
    dst->z = (q1.z * r1 + q2.z * r2);
    dst->w = (q1.w * r1 + q2.w * r2);
}

Quaternion& Quaternion::operator=(const Quaternion& q)
{
    if(&q == this)
        return *this;

    x = q.x;
    y = q.y;
    z = q.z;
    w = q.w;

    return *this;
}

const Quaternion Quaternion::operator*(const Quaternion& q) const
{
    Quaternion result(*this);
    result.multiply(q);
    return result;
}

Quaternion& Quaternion::operator*=(const Quaternion& q)
{
    multiply(q);
    return *this;
}

}
