#include "Base.h"
#include "Matrix.h"
#include "Quaternion.h"

#define MATRIX_SIZE     ( sizeof(float) * 16 )

namespace gameplay
{

static const float MATRIX_IDENTITY[16] =
{
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
};

Matrix::Matrix()
{
    *this = Matrix::identity();
}

Matrix::Matrix(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24, float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44)
{
    set(m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44);
}

Matrix::Matrix(const float* m)
{
    set(m);
}

Matrix::Matrix(const Matrix& copy)
{
    memcpy(m, copy.m, MATRIX_SIZE);
}

Matrix::~Matrix()
{
}

const Matrix& Matrix::identity()
{
    static Matrix m(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1 );
    return m;
}

const Matrix& Matrix::zero()
{
    static Matrix m(
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0 );
    return m;
}

void Matrix::createLookAt(const Vector3& eyePosition, const Vector3& targetPosition, const Vector3& up, Matrix* dst)
{
    createLookAt(eyePosition.x, eyePosition.y, eyePosition.z, targetPosition.x, targetPosition.y, targetPosition.z, up.x, up.y, up.z, dst);
}

void Matrix::createLookAt(float eyePositionX, float eyePositionY, float eyePositionZ,
                          float targetPositionX, float targetPositionY, float targetPositionZ,
                          float upX, float upY, float upZ, Matrix* dst)
{
    GP_ASSERT(dst);

    Vector3 eye(eyePositionX, eyePositionY, eyePositionZ);
    Vector3 target(targetPositionX, targetPositionY, targetPositionZ);
    Vector3 up(upX, upY, upZ);
    up.normalize();

    Vector3 zaxis;
    Vector3::subtract(eye, target, &zaxis);
    zaxis.normalize();

    Vector3 xaxis;
    Vector3::cross(up, zaxis, &xaxis);
    xaxis.normalize();

    Vector3 yaxis;
    Vector3::cross(zaxis, xaxis, &yaxis);
    yaxis.normalize();

    dst->m[0] = xaxis.x;
    dst->m[1] = yaxis.x;
    dst->m[2] = zaxis.x;
    dst->m[3] = 0.0f;

    dst->m[4] = xaxis.y;
    dst->m[5] = yaxis.y;
    dst->m[6] = zaxis.y;
    dst->m[7] = 0.0f;

    dst->m[8] = xaxis.z;
    dst->m[9] = yaxis.z;
    dst->m[10] = zaxis.z;
    dst->m[11] = 0.0f;

    dst->m[12] = -Vector3::dot(xaxis, eye);
    dst->m[13] = -Vector3::dot(yaxis, eye);
    dst->m[14] = -Vector3::dot(zaxis, eye);
    dst->m[15] = 1.0f;
}

void Matrix::createPerspective(float fieldOfView, float aspectRatio,
                                     float zNearPlane, float zFarPlane, Matrix* dst)
{
    GP_ASSERT(dst);
    GP_ASSERT(zFarPlane != zNearPlane);

    float f_n = 1.0f / (zFarPlane - zNearPlane);
    float theta = MATH_DEG_TO_RAD(fieldOfView) * 0.5f;
    if (fabs(fmod(theta, MATH_PIOVER2)) < MATH_EPSILON)
    {
        GP_ERROR("Invalid field of view value (%d) causes attempted calculation tan(%d), which is undefined.", fieldOfView, theta);
        return;
    }
    float divisor = tan(theta);
    GP_ASSERT(divisor);
    float factor = 1.0f / divisor;

    memset(dst, 0, MATRIX_SIZE);

    GP_ASSERT(aspectRatio);
    dst->m[0] = (1.0f / aspectRatio) * factor;
    dst->m[5] = factor;
    dst->m[10] = (-(zFarPlane + zNearPlane)) * f_n;
    dst->m[11] = -1.0f;
    dst->m[14] = -2.0f * zFarPlane * zNearPlane * f_n;
}

void Matrix::createOrthographic(float width, float height, float zNearPlane, float zFarPlane, Matrix* dst)
{
    float halfWidth = width / 2.0f;
    float halfHeight = height / 2.0f;
    createOrthographicOffCenter(-halfWidth, halfWidth, -halfHeight, halfHeight, zNearPlane, zFarPlane, dst);
}

void Matrix::createOrthographicOffCenter(float left, float right, float bottom, float top,
                                         float zNearPlane, float zFarPlane, Matrix* dst)
{
    GP_ASSERT(dst);
    GP_ASSERT(right != left);
    GP_ASSERT(top != bottom);
    GP_ASSERT(zFarPlane != zNearPlane);

    float r_l = 1.0f / (right - left);
    float t_b = 1.0f / (top - bottom);
    float f_n = 1.0f / (zFarPlane - zNearPlane);

    memset(dst, 0, MATRIX_SIZE);
    dst->m[0] = 2.0f * r_l;
    dst->m[5] = 2.0f * t_b;
    dst->m[10] = -2.0f * f_n;
    dst->m[12] = (-(right + left)) * r_l;
    dst->m[13] = (-(top + bottom)) * t_b;
    dst->m[14] = (-(zFarPlane + zNearPlane)) * f_n;
    dst->m[15] = 1.0f;
}

void Matrix::createScale(const Vector3& scale, Matrix* dst)
{
    GP_ASSERT(dst);

    memcpy(dst, MATRIX_IDENTITY, MATRIX_SIZE);

    dst->m[0] = scale.x;
    dst->m[5] = scale.y;
    dst->m[10] = scale.z;
}

void Matrix::createScale(float xScale, float yScale, float zScale, Matrix* dst)
{
    GP_ASSERT(dst);

    memcpy(dst, MATRIX_IDENTITY, MATRIX_SIZE);

    dst->m[0] = xScale;
    dst->m[5] = yScale;
    dst->m[10] = zScale;
}


void Matrix::createRotation(const Quaternion& q, Matrix* dst)
{
    GP_ASSERT(dst);

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

    dst->m[0] = 1.0f - yy2 - zz2;
    dst->m[1] = xy2 + wz2;
    dst->m[2] = xz2 - wy2;
    dst->m[3] = 0.0f;

    dst->m[4] = xy2 - wz2;
    dst->m[5] = 1.0f - xx2 - zz2;
    dst->m[6] = yz2 + wx2;
    dst->m[7] = 0.0f;

    dst->m[8] = xz2 + wy2;
    dst->m[9] = yz2 - wx2;
    dst->m[10] = 1.0f - xx2 - yy2;
    dst->m[11] = 0.0f;

    dst->m[12] = 0.0f;
    dst->m[13] = 0.0f;
    dst->m[14] = 0.0f;
    dst->m[15] = 1.0f;
}

void Matrix::createRotation(const Vector3& axis, float angle, Matrix* dst)
{
    GP_ASSERT(dst);

    float x = axis.x;
    float y = axis.y;
    float z = axis.z;

    // Make sure the input axis is normalized.
    float n = x*x + y*y + z*z;
    if (n != 1.0f)
    {
        // Not normalized.
        n = sqrt(n);
        // Prevent divide too close to zero.
        if (n > 0.000001f)
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

    dst->m[0] = c + tx*x;
    dst->m[1] = txy + sz;
    dst->m[2] = txz - sy;
    dst->m[3] = 0.0f;

    dst->m[4] = txy - sz;
    dst->m[5] = c + ty*y;
    dst->m[6] = tyz + sx;
    dst->m[7] = 0.0f;

    dst->m[8] = txz + sy;
    dst->m[9] = tyz - sx;
    dst->m[10] = c + tz*z;
    dst->m[11] = 0.0f;

    dst->m[12] = 0.0f;
    dst->m[13] = 0.0f;
    dst->m[14] = 0.0f;
    dst->m[15] = 1.0f;
}

void Matrix::createRotationX(float angle, Matrix* dst)
{
    GP_ASSERT(dst);

    memcpy(dst, MATRIX_IDENTITY, MATRIX_SIZE);

    float c = cos(angle);
    float s = sin(angle);

    dst->m[5]  = c;
    dst->m[6]  = s;
    dst->m[9]  = -s;
    dst->m[10] = c;
}

void Matrix::createRotationY(float angle, Matrix* dst)
{
    GP_ASSERT(dst);

    memcpy(dst, MATRIX_IDENTITY, MATRIX_SIZE);

    float c = cos(angle);
    float s = sin(angle);

    dst->m[0]  = c;
    dst->m[2]  = -s;
    dst->m[8]  = s;
    dst->m[10] = c;
}

void Matrix::createRotationZ(float angle, Matrix* dst)
{
    GP_ASSERT(dst);

    memcpy(dst, MATRIX_IDENTITY, MATRIX_SIZE);

    float c = cos(angle);
    float s = sin(angle);

    dst->m[0] = c;
    dst->m[1] = s;
    dst->m[4] = -s;
    dst->m[5] = c;
}

void Matrix::createTranslation(const Vector3& translation, Matrix* dst)
{
    GP_ASSERT(dst);

    memcpy(dst, MATRIX_IDENTITY, MATRIX_SIZE);

    dst->m[12] = translation.x;
    dst->m[13] = translation.y;
    dst->m[14] = translation.z;
}

void Matrix::createTranslation(float xTranslation, float yTranslation, float zTranslation, Matrix* dst)
{
    GP_ASSERT(dst);

    memcpy(dst, MATRIX_IDENTITY, MATRIX_SIZE);

    dst->m[12] = xTranslation;
    dst->m[13] = yTranslation;
    dst->m[14] = zTranslation;
}

void Matrix::add(float scalar)
{
    add(scalar, this);
}

void Matrix::add(float scalar, Matrix* dst)
{
    GP_ASSERT(dst);

    dst->m[0]  = m[0]  + scalar;
    dst->m[1]  = m[1]  + scalar;
    dst->m[2]  = m[2]  + scalar;
    dst->m[3]  = m[3]  + scalar;
    dst->m[4]  = m[4]  + scalar;
    dst->m[5]  = m[5]  + scalar;
    dst->m[6]  = m[6]  + scalar;
    dst->m[7]  = m[7]  + scalar;
    dst->m[8]  = m[8]  + scalar;
    dst->m[9]  = m[9]  + scalar;
    dst->m[10] = m[10] + scalar;
    dst->m[11] = m[11] + scalar;
    dst->m[12] = m[12] + scalar;
    dst->m[13] = m[13] + scalar;
    dst->m[14] = m[14] + scalar;
    dst->m[15] = m[15] + scalar;
}

void Matrix::add(const Matrix& m)
{
    add(*this, m, this);
}

void Matrix::add(const Matrix& m1, const Matrix& m2, Matrix* dst)
{
    GP_ASSERT(dst);

    dst->m[0]  = m1.m[0]  + m2.m[0];
    dst->m[1]  = m1.m[1]  + m2.m[1];
    dst->m[2]  = m1.m[2]  + m2.m[2];
    dst->m[3]  = m1.m[3]  + m2.m[3];
    dst->m[4]  = m1.m[4]  + m2.m[4];
    dst->m[5]  = m1.m[5]  + m2.m[5];
    dst->m[6]  = m1.m[6]  + m2.m[6];
    dst->m[7]  = m1.m[7]  + m2.m[7];
    dst->m[8]  = m1.m[8]  + m2.m[8];
    dst->m[9]  = m1.m[9]  + m2.m[9];
    dst->m[10] = m1.m[10] + m2.m[10];
    dst->m[11] = m1.m[11] + m2.m[11];
    dst->m[12] = m1.m[12] + m2.m[12];
    dst->m[13] = m1.m[13] + m2.m[13];
    dst->m[14] = m1.m[14] + m2.m[14];
    dst->m[15] = m1.m[15] + m2.m[15];
}

bool Matrix::decompose(Vector3* scale, Quaternion* rotation, Vector3* translation) const
{
    if (translation)
    {
        // Extract the translation.
        translation->x = m[12];
        translation->y = m[13];
        translation->z = m[14];
    }

    // Nothing left to do.
    if (scale == NULL && rotation == NULL)
        return true;

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
        scaleZ = -scaleZ;

    if (scale)
    {
        scale->x = scaleX;
        scale->y = scaleY;
        scale->z = scaleZ;
    }

    // Nothing left to do.
    if (rotation == NULL)
        return true;

    // Scale too close to zero, can't decompose rotation.
    if (scaleX < MATH_TOLERANCE || scaleY < MATH_TOLERANCE || fabs(scaleZ) < MATH_TOLERANCE)
        return false;

    float rn;

    // Factor the scale out of the matrix axes.
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

    // Now calculate the rotation from the resulting matrix (axes).
    float trace = xaxis.x + yaxis.y + zaxis.z + 1.0f;

    if (trace > MATH_EPSILON)
    {
        float s = 0.5f / sqrt(trace);
        rotation->w = 0.25f / s;
        rotation->x = (yaxis.z - zaxis.y) * s;
        rotation->y = (zaxis.x - xaxis.z) * s;
        rotation->z = (xaxis.y - yaxis.x) * s;
    }
    else
    {
        // Note: since xaxis, yaxis, and zaxis are normalized, 
        // we will never divide by zero in the code below.
        if (xaxis.x > yaxis.y && xaxis.x > zaxis.z)
        {
            float s = 0.5f / sqrt(1.0f + xaxis.x - yaxis.y - zaxis.z);
            rotation->w = (yaxis.z - zaxis.y) * s;
            rotation->x = 0.25f / s;
            rotation->y = (yaxis.x + xaxis.y) * s;
            rotation->z = (zaxis.x + xaxis.z) * s;
        }
        else if (yaxis.y > zaxis.z)
        {
            float s = 0.5f / sqrt(1.0f + yaxis.y - xaxis.x - zaxis.z);
            rotation->w = (zaxis.x - xaxis.z) * s;
            rotation->x = (yaxis.x + xaxis.y) * s;
            rotation->y = 0.25f / s;
            rotation->z = (zaxis.y + yaxis.z) * s;
        }
        else
        {
            float s = 0.5f / sqrt(1.0f + zaxis.z - xaxis.x - yaxis.y );
            rotation->w = (xaxis.y - yaxis.x ) * s;
            rotation->x = (zaxis.x + xaxis.z ) * s;
            rotation->y = (zaxis.y + yaxis.z ) * s;
            rotation->z = 0.25f / s;
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

    // Calculate the determinant.
    return (a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0);
}

void Matrix::getScale(Vector3* scale) const
{
    decompose(scale, NULL, NULL);
}

bool Matrix::getRotation(Quaternion* rotation) const
{
    return decompose(NULL, rotation, NULL);
}

void Matrix::getTranslation(Vector3* translation) const
{
    decompose(NULL, NULL, translation);
}

void Matrix::getUpVector(Vector3* dst) const
{
    GP_ASSERT(dst);

    dst->x = m[4];
    dst->y = m[5];
    dst->z = m[6];
}

void Matrix::getDownVector(Vector3* dst) const
{
    GP_ASSERT(dst);
    
    dst->x = -m[4];
    dst->y = -m[5];
    dst->z = -m[6];
}

void Matrix::getLeftVector(Vector3* dst) const
{
    GP_ASSERT(dst);

    dst->x = -m[0];
    dst->y = -m[1];
    dst->z = -m[2];
}

void Matrix::getRightVector(Vector3* dst) const
{
    GP_ASSERT(dst);

    dst->x = m[0];
    dst->y = m[1];
    dst->z = m[2];
}

void Matrix::getForwardVector(Vector3* dst) const
{
    GP_ASSERT(dst);

    dst->x = -m[8];
    dst->y = -m[9];
    dst->z = -m[10];
}

void Matrix::getBackVector(Vector3* dst) const
{
    GP_ASSERT(dst);

    dst->x = m[8];
    dst->y = m[9];
    dst->z = m[10];
}

bool Matrix::invert()
{
    return invert(this);
}

bool Matrix::invert(Matrix* dst) const
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

    // Calculate the determinant.
    float det = a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0;

    // Close to zero, can't invert.
    if (fabs(det) <= MATH_TOLERANCE)
        return false;

    // Support the case where m == dst.
    Matrix inverse;
    inverse.m[0]  = m[5] * b5 - m[6] * b4 + m[7] * b3;
    inverse.m[1]  = -m[1] * b5 + m[2] * b4 - m[3] * b3;
    inverse.m[2]  = m[13] * a5 - m[14] * a4 + m[15] * a3;
    inverse.m[3]  = -m[9] * a5 + m[10] * a4 - m[11] * a3;

    inverse.m[4]  = -m[4] * b5 + m[6] * b2 - m[7] * b1;
    inverse.m[5]  = m[0] * b5 - m[2] * b2 + m[3] * b1;
    inverse.m[6]  = -m[12] * a5 + m[14] * a2 - m[15] * a1;
    inverse.m[7]  = m[8] * a5 - m[10] * a2 + m[11] * a1;

    inverse.m[8]  = m[4] * b4 - m[5] * b2 + m[7] * b0;
    inverse.m[9]  = -m[0] * b4 + m[1] * b2 - m[3] * b0;
    inverse.m[10] = m[12] * a4 - m[13] * a2 + m[15] * a0;
    inverse.m[11] = -m[8] * a4 + m[9] * a2 - m[11] * a0;

    inverse.m[12] = -m[4] * b3 + m[5] * b1 - m[6] * b0;
    inverse.m[13] = m[0] * b3 - m[1] * b1 + m[2] * b0;
    inverse.m[14] = -m[12] * a3 + m[13] * a1 - m[14] * a0;
    inverse.m[15] = m[8] * a3 - m[9] * a1 + m[10] * a0;

    multiply(inverse, 1.0f / det, dst);

    return true;
}

bool Matrix::isIdentity() const
{
    return (memcmp(m, MATRIX_IDENTITY, MATRIX_SIZE) == 0);
}

void Matrix::multiply(float scalar)
{
    multiply(scalar, this);
}

void Matrix::multiply(float scalar, Matrix* dst) const
{
    multiply(*this, scalar, dst);
}

void Matrix::multiply(const Matrix& m, float scalar, Matrix* dst)
{
    GP_ASSERT(dst);

    dst->m[0]  = m.m[0]  * scalar;
    dst->m[1]  = m.m[1]  * scalar;
    dst->m[2]  = m.m[2]  * scalar;
    dst->m[3]  = m.m[3]  * scalar;
    dst->m[4]  = m.m[4]  * scalar;
    dst->m[5]  = m.m[5]  * scalar;
    dst->m[6]  = m.m[6]  * scalar;
    dst->m[7]  = m.m[7]  * scalar;
    dst->m[8]  = m.m[8]  * scalar;
    dst->m[9]  = m.m[9]  * scalar;
    dst->m[10] = m.m[10] * scalar;
    dst->m[11] = m.m[11] * scalar;
    dst->m[12] = m.m[12] * scalar;
    dst->m[13] = m.m[13] * scalar;
    dst->m[14] = m.m[14] * scalar;
    dst->m[15] = m.m[15] * scalar;
}

void Matrix::multiply(const Matrix& m)
{
    multiply(*this, m, this);
}

void Matrix::multiply(const Matrix& m1, const Matrix& m2, Matrix* dst)
{
	GP_ASSERT(dst);

#ifdef USE_NEON // if set, neon unit is present.

    asm volatile
    (
        "vld1.32	 {d16 - d19}, [%1]!	\n\t"         // load first eight elements of matrix 0
		"vld1.32     {d20 - d23}, [%1]!   \n\t"         // load second eight elements of matrix 0
		"vld1.32     {d0 - d3}, [%2]!     \n\t"         // load first eight elements of matrix 1
		"vld1.32     {d4 - d7}, [%2]!     \n\t"         // load second eight elements of matrix 1

		"vmul.f32    q12, q8, d0[0]     \n\t"         // rslt col0  = (mat0 col0) * (mat1 col0 elt0)
		"vmul.f32    q13, q8, d2[0]     \n\t"         // rslt col1  = (mat0 col0) * (mat1 col1 elt0)
		"vmul.f32    q14, q8, d4[0]     \n\t"         // rslt col2  = (mat0 col0) * (mat1 col2 elt0)
		"vmul.f32    q15, q8, d6[0]     \n\t"         // rslt col3  = (mat0 col0) * (mat1 col3 elt0)

		"vmla.f32    q12, q9, d0[1]     \n\t"         // rslt col0 += (mat0 col1) * (mat1 col0 elt1)
		"vmla.f32    q13, q9, d2[1]     \n\t"         // rslt col1 += (mat0 col1) * (mat1 col1 elt1)
		"vmla.f32    q14, q9, d4[1]     \n\t"         // rslt col2 += (mat0 col1) * (mat1 col2 elt1)
		"vmla.f32    q15, q9, d6[1]     \n\t"         // rslt col3 += (mat0 col1) * (mat1 col3 elt1)

		"vmla.f32    q12, q10, d1[0]    \n\t"         // rslt col0 += (mat0 col2) * (mat1 col0 elt2)
		"vmla.f32    q13, q10, d3[0]    \n\t"         // rslt col1 += (mat0 col2) * (mat1 col1 elt2)
		"vmla.f32    q14, q10, d5[0]    \n\t"         // rslt col2 += (mat0 col2) * (mat1 col2 elt2)
		"vmla.f32    q15, q10, d7[0]    \n\t"         // rslt col3 += (mat0 col2) * (mat1 col2 elt2)

		"vmla.f32    q12, q11, d1[1]    \n\t"         // rslt col0 += (mat0 col3) * (mat1 col0 elt3)
		"vmla.f32    q13, q11, d3[1]    \n\t"         // rslt col1 += (mat0 col3) * (mat1 col1 elt3)
		"vmla.f32    q14, q11, d5[1]    \n\t"         // rslt col2 += (mat0 col3) * (mat1 col2 elt3)
		"vmla.f32    q15, q11, d7[1]    \n\t"         // rslt col3 += (mat0 col3) * (mat1 col3 elt3)

		"vst1.32    {d24 - d27}, [%0]!    \n\t"         // store first eight elements of result
		"vst1.32    {d28 - d31}, [%0]!    \n\t"         // store second eight elements of result
        
        : // output
        : "r"(dst->m), "r"(m1.m), "r"(m2.m) // input - note *value* of pointer doesn't change.
        : "memory", "q0", "q1", "q2", "q3", "q8", "q9", "q10", "q11", "q12", "q13", "q14", "q15"
	);

#else

    // Support the case where m1 or m2 is the same array as dst.
    float product[16];

    product[0]  = m1.m[0] * m2.m[0]  + m1.m[4] * m2.m[1] + m1.m[8]   * m2.m[2]  + m1.m[12] * m2.m[3];
    product[1]  = m1.m[1] * m2.m[0]  + m1.m[5] * m2.m[1] + m1.m[9]   * m2.m[2]  + m1.m[13] * m2.m[3];
    product[2]  = m1.m[2] * m2.m[0]  + m1.m[6] * m2.m[1] + m1.m[10]  * m2.m[2]  + m1.m[14] * m2.m[3];
    product[3]  = m1.m[3] * m2.m[0]  + m1.m[7] * m2.m[1] + m1.m[11]  * m2.m[2]  + m1.m[15] * m2.m[3];

    product[4]  = m1.m[0] * m2.m[4]  + m1.m[4] * m2.m[5] + m1.m[8]   * m2.m[6]  + m1.m[12] * m2.m[7];
    product[5]  = m1.m[1] * m2.m[4]  + m1.m[5] * m2.m[5] + m1.m[9]   * m2.m[6]  + m1.m[13] * m2.m[7];
    product[6]  = m1.m[2] * m2.m[4]  + m1.m[6] * m2.m[5] + m1.m[10]  * m2.m[6]  + m1.m[14] * m2.m[7];
    product[7]  = m1.m[3] * m2.m[4]  + m1.m[7] * m2.m[5] + m1.m[11]  * m2.m[6]  + m1.m[15] * m2.m[7];

    product[8]  = m1.m[0] * m2.m[8]  + m1.m[4] * m2.m[9] + m1.m[8]   * m2.m[10] + m1.m[12] * m2.m[11];
    product[9]  = m1.m[1] * m2.m[8]  + m1.m[5] * m2.m[9] + m1.m[9]   * m2.m[10] + m1.m[13] * m2.m[11];
    product[10] = m1.m[2] * m2.m[8]  + m1.m[6] * m2.m[9] + m1.m[10]  * m2.m[10] + m1.m[14] * m2.m[11];
    product[11] = m1.m[3] * m2.m[8]  + m1.m[7] * m2.m[9] + m1.m[11]  * m2.m[10] + m1.m[15] * m2.m[11];

    product[12] = m1.m[0] * m2.m[12] + m1.m[4] * m2.m[13] + m1.m[8]  * m2.m[14] + m1.m[12] * m2.m[15];
    product[13] = m1.m[1] * m2.m[12] + m1.m[5] * m2.m[13] + m1.m[9]  * m2.m[14] + m1.m[13] * m2.m[15];
    product[14] = m1.m[2] * m2.m[12] + m1.m[6] * m2.m[13] + m1.m[10] * m2.m[14] + m1.m[14] * m2.m[15];
    product[15] = m1.m[3] * m2.m[12] + m1.m[7] * m2.m[13] + m1.m[11] * m2.m[14] + m1.m[15] * m2.m[15];

    memcpy(dst->m, product, MATRIX_SIZE);

#endif
}

void Matrix::negate()
{
    negate(this);
}

void Matrix::negate(Matrix* dst) const
{
    GP_ASSERT(dst);

    dst->m[0]  = -m[0];
    dst->m[1]  = -m[1];
    dst->m[2]  = -m[2];
    dst->m[3]  = -m[3];
    dst->m[4]  = -m[4];
    dst->m[5]  = -m[5];
    dst->m[6]  = -m[6];
    dst->m[7]  = -m[7];
    dst->m[8]  = -m[8];
    dst->m[9]  = -m[9];
    dst->m[10] = -m[10];
    dst->m[11] = -m[11];
    dst->m[12] = -m[12];
    dst->m[13] = -m[13];
    dst->m[14] = -m[14];
    dst->m[15] = -m[15];
}

void Matrix::rotate(const Quaternion& q)
{
    rotate(q, this);
}

void Matrix::rotate(const Quaternion& q, Matrix* dst) const
{
    Matrix r;
    createRotation(q, &r);
    multiply(*this, r, dst);
}

void Matrix::rotate(const Vector3& axis, float angle)
{
    rotate(axis, angle, this);
}

void Matrix::rotate(const Vector3& axis, float angle, Matrix* dst) const
{
    Matrix r;
    createRotation(axis, angle, &r);
    multiply(*this, r, dst);
}

void Matrix::rotateX(float angle)
{
    rotateX(angle, this);
}

void Matrix::rotateX(float angle, Matrix* dst) const
{
    Matrix r;
    createRotationX(angle, &r);
    multiply(*this, r, dst);
}

void Matrix::rotateY(float angle)
{
    rotateY(angle, this);
}

void Matrix::rotateY(float angle, Matrix* dst) const
{
    Matrix r;
    createRotationY(angle, &r);
    multiply(*this, r, dst);
}

void Matrix::rotateZ(float angle)
{
    rotateZ(angle, this);
}

void Matrix::rotateZ(float angle, Matrix* dst) const
{
    Matrix r;
    createRotationZ(angle, &r);
    multiply(*this, r, dst);
}

void Matrix::scale(float value)
{
    scale(value, this);
}

void Matrix::scale(float value, Matrix* dst) const
{
    scale(value, value, value, dst);
}

void Matrix::scale(float xScale, float yScale, float zScale)
{
    scale(xScale, yScale, zScale, this);
}

void Matrix::scale(float xScale, float yScale, float zScale, Matrix* dst) const
{
    Matrix s;
    createScale(xScale, yScale, zScale, &s);
    multiply(*this, s, dst);
}

void Matrix::scale(const Vector3& s)
{
    scale(s.x, s.y, s.z, this);
}

void Matrix::scale(const Vector3& s, Matrix* dst) const
{
    scale(s.x, s.y, s.z, dst);
}

void Matrix::set(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24, float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44)
{
    m[0]  = m11;
    m[1]  = m21;
    m[2]  = m31;
    m[3]  = m41;
    m[4]  = m12;
    m[5]  = m22;
    m[6]  = m32;
    m[7]  = m42;
    m[8]  = m13;
    m[9]  = m23;
    m[10] = m33;
    m[11] = m43;
    m[12] = m14;
    m[13] = m24;
    m[14] = m34;
    m[15] = m44;
}

void Matrix::set(const float* m)
{
    GP_ASSERT(m);
    memcpy(this->m, m, MATRIX_SIZE);
}

void Matrix::set(const Matrix& m)
{
    memcpy(this->m, m.m, MATRIX_SIZE);
}

void Matrix::setIdentity()
{
    memcpy(m, MATRIX_IDENTITY, MATRIX_SIZE);
}

void Matrix::setZero()
{
    memset(m, 0, MATRIX_SIZE);
}

void Matrix::subtract(const Matrix& m)
{
    subtract(*this, m, this);
}

void Matrix::subtract(const Matrix& m1, const Matrix& m2, Matrix* dst)
{
    GP_ASSERT(dst);

    dst->m[0]  = m1.m[0]  - m2.m[0];
    dst->m[1]  = m1.m[1]  - m2.m[1];
    dst->m[2]  = m1.m[2]  - m2.m[2];
    dst->m[3]  = m1.m[3]  - m2.m[3];
    dst->m[4]  = m1.m[4]  - m2.m[4];
    dst->m[5]  = m1.m[5]  - m2.m[5];
    dst->m[6]  = m1.m[6]  - m2.m[6];
    dst->m[7]  = m1.m[7]  - m2.m[7];
    dst->m[8]  = m1.m[8]  - m2.m[8];
    dst->m[9]  = m1.m[9]  - m2.m[9];
    dst->m[10] = m1.m[10] - m2.m[10];
    dst->m[11] = m1.m[11] - m2.m[11];
    dst->m[12] = m1.m[12] - m2.m[12];
    dst->m[13] = m1.m[13] - m2.m[13];
    dst->m[14] = m1.m[14] - m2.m[14];
    dst->m[15] = m1.m[15] - m2.m[15];
}

void Matrix::transformPoint(Vector3* point) const
{
    GP_ASSERT(point);
    transformVector(point->x, point->y, point->z, 1.0f, point);
}

void Matrix::transformPoint(const Vector3& point, Vector3* dst) const
{
    transformVector(point.x, point.y, point.z, 1.0f, dst);
}

void Matrix::transformVector(Vector3* vector) const
{
    GP_ASSERT(vector);
    transformVector(vector->x, vector->y, vector->z, 0.0f, vector);
}

void Matrix::transformVector(const Vector3& vector, Vector3* dst) const
{
    transformVector(vector.x, vector.y, vector.z, 0.0f, dst);
}

void Matrix::transformVector(float x, float y, float z, float w, Vector3* dst) const
{
    GP_ASSERT(dst);

#ifdef USE_NEON

    asm volatile(
    	"vld1.32	{d0[0]},		[%0]	\n\t"	// load x
		"vld1.32	{d0[1]},    	[%1]	\n\t"	// load y
		"vld1.32	{d1[0]},		[%2]	\n\t"	// load z
		"vld1.32	{d1[1]},		[%3]	\n\t"	// load w
		"vld1.32	{d18 - d21},	[%4]!	\n\t"	// load first 8 elements of matrix m0-m7
		"vld1.32	{d22 - d25},	[%4]!	\n\t"	// load second 8 elements of matrix m8-m15

    	"vmul.f32 q13,  q9, d0[0]			\n\t"	// Q5 =  (m0-m3)*x
    	"vmla.f32 q13, q10, d0[1]      		\n\t"	// Q5 +=  (m4-m7)*y
    	"vmla.f32 q13, q11, d1[0]      		\n\t"	// Q5 +=  (m8-m11)*z
		"vmla.f32 q13, q12, d1[1]      		\n\t"	// Q5 +=  (m12-m15)*w

    	"vst1.32 {d26[0]}, [%5]!        	\n\t"	// store dst->x
		"vst1.32 {d26[1]}, [%5]!        	\n\t"	// store dst->y
		"vst1.32 {d27[0]}, [%5]!        	\n\t"	// store dst->z
		:
    	: "r"(&x), "r"(&y), "r"(&z), "r"(&w), "r"(m), "r"(dst)
        : "q0", "q9", "q10","q11", "q12", "q13", "memory"
    );

#else

    dst->set(
        x * m[0] + y * m[4] + z * m[8] + w * m[12],
        x * m[1] + y * m[5] + z * m[9] + w * m[13],
        x * m[2] + y * m[6] + z * m[10] + w * m[14]);

#endif
}

void Matrix::transformVector(Vector4* vector) const
{
    GP_ASSERT(vector);
    transformVector(*vector, vector);
}

void Matrix::transformVector(const Vector4& vector, Vector4* dst) const
{
    GP_ASSERT(dst);

#ifdef USE_NEON

    asm volatile
    (
    		"vld1.32	{d0, d1}, [%1]		\n\t"   //Q0 = v (x, y, z, w)
    		"vld1.32    {d18 - d21}, [%0]!  \n\t"   //Q1 = M (m0-m7)
    		"vld1.32    {d22 - d25}, [%0]!  \n\t"   //Q2 = M (m8-m15)

    		"vmul.f32   q13, q9, d0[0]      \n\t"   //Q5 =  Q0*Q0[0]
    		"vmla.f32   q13, q10, d0[1]     \n\t"   //Q5 += Q1*Q0[1]
    		"vmla.f32   q13, q11, d1[0]     \n\t"   //Q5 += Q2*Q0[2]
    		"vmla.f32   q13, q12, d1[1]     \n\t"   //Q5 += Q3*Q0[3]
    		"vst1.32    {d26, d27}, [%2]    \n\t"   //Q4 = m+12
    		:
    		: "r"(m), "r"(&vector), "r"(dst)
    		: "q0", "q9", "q10","q11", "q12", "q13", "memory"
    );

#else

    dst->set(
        vector.x * m[0] + vector.y * m[4] + vector.z * m[8] + vector.w * m[12],
        vector.x * m[1] + vector.y * m[5] + vector.z * m[9] + vector.w * m[13],
        vector.x * m[2] + vector.y * m[6] + vector.z * m[10] + vector.w * m[14],
        vector.x * m[3] + vector.y * m[7] + vector.z * m[11] + vector.w * m[15]);

#endif
}

void Matrix::translate(float x, float y, float z)
{
    translate(x, y, z, this);
}

void Matrix::translate(float x, float y, float z, Matrix* dst) const
{
    Matrix t;
    createTranslation(x, y, z, &t);
    multiply(*this, t, dst);
}

void Matrix::translate(const Vector3& t)
{
    translate(t.x, t.y, t.z, this);
}

void Matrix::translate(const Vector3& t, Matrix* dst) const
{
    translate(t.x, t.y, t.z, dst);
}

void Matrix::transpose()
{
    transpose(this);
}

void Matrix::transpose(Matrix* dst) const
{
    GP_ASSERT(dst);
    
    float t[16] = {
        m[0], m[4], m[8], m[12],
        m[1], m[5], m[9], m[13],
        m[2], m[6], m[10], m[14],
        m[3], m[7], m[11], m[15]
    };
    memcpy(dst->m, t, MATRIX_SIZE);
}

}
