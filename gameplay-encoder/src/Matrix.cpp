#include "Base.h"
#include "Matrix.h"

namespace gameplay
{

Matrix::Matrix(void)
{
    setIdentity(m);
}

Matrix::Matrix(float m0, float m1, float m2, float m3,
               float m4, float m5, float m6, float m7,
               float m8, float m9, float m10, float m11,
               float m12, float m13, float m14, float m15)
{
    m[0] = m0;
    m[1] = m1;
    m[2] = m2;
    m[3] = m3;
    m[4] = m4;
    m[5] = m5;
    m[6] = m6;
    m[7] = m7;
    m[8] = m8;
    m[9] = m9;
    m[10] = m10;
    m[11] = m11;
    m[12] = m12;
    m[13] = m13;
    m[14] = m14;
    m[15] = m15;
}

Matrix::~Matrix(void)
{
}

void Matrix::setIdentity(float* matrix)
{
    memcpy(matrix, MATRIX4F_IDENTITY, MATRIX4F_SIZE);
}

void Matrix::createRotation(const Quaternion& q, float* dst)
{
    assert(dst);

    float x2 = q.x + q.x;
    float y2 = q.y + q.y;
    float z2 = q.z + q.z;

    float xx2 = q.x * x2;
    float yy2 = q.y * y2;
    float zz2 = q.z * z2;
    float xy2 = q.x * y2;
    float xz2 = q.x * z2;
    float yz2 = q.y * z2;
    float wx2 = q.w * x2;
    float wy2 = q.w * y2;
    float wz2 = q.w * z2;

    dst[0] = 1.0f - yy2 - zz2;
    dst[1] = xy2 + wz2;
    dst[2] = xz2 - wy2;
    dst[3] = 0.0f;

    dst[4] = xy2 - wz2;
    dst[5] = 1.0f - xx2 - zz2;
    dst[6] = yz2 + wx2;
    dst[7] = 0.0f;

    dst[8] = xz2 + wy2;
    dst[9] = yz2 - wx2;
    dst[10] = 1.0f - xx2 - yy2;
    dst[11] = 0.0f;

    dst[12] = 0.0f;
    dst[13] = 0.0f;
    dst[14] = 0.0f;
    dst[15] = 1.0f;
}

void Matrix::createRotation(float x, float y, float z, float angle, float* dst)
{
    // Make sure the input axis is normalized
    float n = x*x + y*y + z*z;
    if (n != 1.0f)
    {
        // Not normalized
        n = sqrt(n);
        if (n > 0.000001f) // prevent divide too close to zero
        {
            n = 1.0f / n;
            x *= n;
            y *= n;
            z *= n;
        }
    }

    float c = cos(angle);
    float s = sin(angle);

    float t = 1.0f - c;
    float tx = t * x;
    float ty = t * y;
    float tz = t * z;
    float txy = tx * y;
    float txz = tx * z;
    float tyz = ty * z;
    float sx = s * x;
    float sy = s * y;
    float sz = s * z;
    
    dst[0] = c + tx*x;
    dst[1] = txy + sz;
    dst[2] = txz - sy;
    dst[3] = 0.0f;

    dst[4] = txy - sz;
    dst[5] = c + ty*y;
    dst[6] = tyz + sx;
    dst[7] = 0.0f;

    dst[8] = txz + sy;
    dst[9] = tyz - sx;
    dst[10] = c + tz*z;
    dst[11] = 0.0f;

    dst[12] = 0.0f;
    dst[13] = 0.0f;
    dst[14] = 0.0f;
    dst[15] = 1.0f;
}

void Matrix::createRotationX(float angle, float* dst)
{
    setIdentity(dst);

    float c = cos(angle);
    float s = sin(angle);

    dst[5]  = c;
    dst[6]  = s;
    dst[9]  = -s;
    dst[10] = c;
}

void Matrix::createRotationY(float angle, float* dst)
{
    setIdentity(dst);

    float c = cos(angle);
    float s = sin(angle);

    dst[0]  = c;
    dst[2]  = -s;
    dst[8]  = s;
    dst[10] = c;
}

void Matrix::createRotationZ(float angle, float* dst)
{
    setIdentity(dst);

    float c = cos(angle);
    float s = sin(angle);

    dst[0] = c;
    dst[1] = s;
    dst[4] = -s;
    dst[5] = c;
}

void Matrix::createTranslation(float x, float y, float z, float* dst)
{
    setIdentity(dst);
    dst[12] = x;
    dst[13] = y;
    dst[14] = z;
}

void Matrix::createScale(float x, float y, float z, float* dst)
{
    setIdentity(dst);
    dst[0]  = x;
    dst[5]  = y;
    dst[10] = z;
}

float* Matrix::getArray()
{
    return m;
}

void Matrix::translate(float x, float y, float z)
{
    float t[16];
    createTranslation(x, y, z, t);
    multiply(m, t, m);
}

void Matrix::scale(float x, float y, float z)
{
    float s[16];
    createScale(x, y, z, s);
    multiply(m, s, m);
}

void Matrix::rotate(const Quaternion& q)
{
    float r[16];
    createRotation(q, r);
    multiply(m, r, m);
}

void Matrix::rotate(float x, float y, float z, float angle)
{
    float r[16];
    createRotation(x, y, z, angle, r);
    multiply(m, r, m);
}

void Matrix::rotateX(float angle)
{
    float r[16];
    createRotationX(angle, r);
    multiply(m, r, m);
}

void Matrix::rotateY(float angle)
{
    float r[16];
    createRotationY(angle, r);
    multiply(m, r, m);
}

void Matrix::rotateZ(float angle)
{
    float r[16];
    createRotationZ(angle, r);
    multiply(m, r, m);
}

void Matrix::multiply(const float* m1, const float* m2, float* dst)
{
    // Support the case where m1 or m2 is the same array as dst.
    float product[16];
    product[0]  = m1[0] * m2[0]  + m1[4] * m2[1] + m1[8]   * m2[2]  + m1[12] * m2[3];
    product[1]  = m1[1] * m2[0]  + m1[5] * m2[1] + m1[9]   * m2[2]  + m1[13] * m2[3];
    product[2]  = m1[2] * m2[0]  + m1[6] * m2[1] + m1[10]  * m2[2]  + m1[14] * m2[3];
    product[3]  = m1[3] * m2[0]  + m1[7] * m2[1] + m1[11]  * m2[2]  + m1[15] * m2[3];

    product[4]  = m1[0] * m2[4]  + m1[4] * m2[5] + m1[8]   * m2[6]  + m1[12] * m2[7];
    product[5]  = m1[1] * m2[4]  + m1[5] * m2[5] + m1[9]   * m2[6]  + m1[13] * m2[7];
    product[6]  = m1[2] * m2[4]  + m1[6] * m2[5] + m1[10]  * m2[6]  + m1[14] * m2[7];
    product[7]  = m1[3] * m2[4]  + m1[7] * m2[5] + m1[11]  * m2[6]  + m1[15] * m2[7];

    product[8]  = m1[0] * m2[8]  + m1[4] * m2[9] + m1[8]   * m2[10] + m1[12] * m2[11];
    product[9]  = m1[1] * m2[8]  + m1[5] * m2[9] + m1[9]   * m2[10] + m1[13] * m2[11];
    product[10] = m1[2] * m2[8]  + m1[6] * m2[9] + m1[10]  * m2[10] + m1[14] * m2[11];
    product[11] = m1[3] * m2[8]  + m1[7] * m2[9] + m1[11]  * m2[10] + m1[15] * m2[11];

    product[12] = m1[0] * m2[12] + m1[4] * m2[13] + m1[8]  * m2[14] + m1[12] * m2[15];
    product[13] = m1[1] * m2[12] + m1[5] * m2[13] + m1[9]  * m2[14] + m1[13] * m2[15];
    product[14] = m1[2] * m2[12] + m1[6] * m2[13] + m1[10] * m2[14] + m1[14] * m2[15];
    product[15] = m1[3] * m2[12] + m1[7] * m2[13] + m1[11] * m2[14] + m1[15] * m2[15];
    memcpy(dst, product, MATRIX4F_SIZE);
}

bool Matrix::decompose(Vector3* scale, Quaternion* rotation, Vector3* translation) const
{
    if (translation)
    {
        // Extract the translation
        translation->x = m[12];
        translation->y = m[13];
        translation->z = m[14];
    }

    // nothing left to do
    if (scale == NULL && rotation == NULL)
    {
        return true;
    }

    // Extract the scale.
    // This is simply the length of each axis (row/column) in the matrix.
    Vector3 xaxis(m[0], m[1], m[2]);
    float scaleX = xaxis.length();

    Vector3 yaxis(m[4], m[5], m[6]);
    float scaleY = yaxis.length();

    Vector3 zaxis(m[8], m[9], m[10]);
    float scaleZ = zaxis.length();

    // Determine if we have a negative scale (true if determinant is less than zero).
    // In this case, we simply negate a single axis of the scale.
    float det = determinant();
    if (det < 0)
    {
        scaleZ = -scaleZ;
    }

    if (scale)
    {
        scale->x = scaleX;
        scale->y = scaleY;
        scale->z = scaleZ;
    }

    // nothing left to do
    if (rotation == NULL)
        return true;

    // scale too close to zero, can't decompose rotation
    if (scaleX < MATH_TOLERANCE || scaleY < MATH_TOLERANCE || fabs(scaleZ) < MATH_TOLERANCE)
        return false;

    float rn;

    // Factor the scale out of the matrix axes
    rn = 1.0f / scaleX;
    xaxis.x *= rn;
    xaxis.y *= rn;
    xaxis.z *= rn;

    rn = 1.0f / scaleY;
    yaxis.x *= rn;
    yaxis.y *= rn;
    yaxis.z *= rn;

    rn = 1.0f / scaleZ;
    zaxis.x *= rn;
    zaxis.y *= rn;
    zaxis.z *= rn;

    // Now calculate the rotation from the resulting matrix (axes)
    float trace = xaxis.x + yaxis.y + zaxis.z + 1.0f;

    if (trace > MATH_TOLERANCE)
    {
        float s = 0.5f / sqrt(trace);
        rotation->w = 0.25f / s;
        rotation->x = ( yaxis.z - zaxis.y ) * s;
        rotation->y = ( zaxis.x - xaxis.z ) * s;
        rotation->z = ( xaxis.y - yaxis.x ) * s;
    }
    else
    {
        if (xaxis.x > yaxis.y && xaxis.x > zaxis.z)
        {
            float s = 2.0f * sqrt(1.0f + xaxis.x - yaxis.y - zaxis.z);
            rotation->w = (yaxis.z - zaxis.y ) / s;
            rotation->x = 0.25f * s;
            rotation->y = (yaxis.x + xaxis.y ) / s;
            rotation->z = (zaxis.x + xaxis.z ) / s;
        }
        else if (yaxis.y > zaxis.z)
        {
            float s = 2.0f * sqrt(1.0f + yaxis.y - xaxis.x - zaxis.z);
            rotation->w = (zaxis.x - xaxis.z ) / s;
            rotation->x = (yaxis.x + xaxis.y ) / s;
            rotation->y = 0.25f * s;
            rotation->z = (zaxis.y + yaxis.z ) / s;
        }
        else
        {
            float s = 2.0f * sqrt(1.0f + zaxis.z - xaxis.x - yaxis.y );
            rotation->w = (xaxis.y - yaxis.x ) / s;
            rotation->x = (zaxis.x + xaxis.z ) / s;
            rotation->y = (zaxis.y + yaxis.z ) / s;
            rotation->z = 0.25f * s;
        }
    }

    return true;
}

float Matrix::determinant() const
{
    float a0 = m[0] * m[5] - m[1] * m[4];
    float a1 = m[0] * m[6] - m[2] * m[4];
    float a2 = m[0] * m[7] - m[3] * m[4];
    float a3 = m[1] * m[6] - m[2] * m[5];
    float a4 = m[1] * m[7] - m[3] * m[5];
    float a5 = m[2] * m[7] - m[3] * m[6];
    float b0 = m[8] * m[13] - m[9] * m[12];
    float b1 = m[8] * m[14] - m[10] * m[12];
    float b2 = m[8] * m[15] - m[11] * m[12];
    float b3 = m[9] * m[14] - m[10] * m[13];
    float b4 = m[9] * m[15] - m[11] * m[13];
    float b5 = m[10] * m[15] - m[11] * m[14];

    // Calculate the determinant
    return (a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0);
}

void Matrix::transformPoint(const Vector3& p, Vector3* dst) const
{
    dst->set(
        p.x * m[0] + p.y * m[4] + p.z * m[8] +  m[12],
        p.x * m[1] + p.y * m[5] + p.z * m[9] +  m[13],
        p.x * m[2] + p.y * m[6] + p.z * m[10] + m[14] );
}

}
