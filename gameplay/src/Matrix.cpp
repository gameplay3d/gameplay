#include "Base.h"
#include "Matrix.h"
#include "Plane.h"
#include "Quaternion.h"


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

Matrix::Matrix(float m11, float m12, float m13, float m14, 
               float m21, float m22, float m23, float m24,
               float m31, float m32, float m33, float m34, 
               float m41, float m42, float m43, float m44)
{
    set(m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44);
}

Matrix::Matrix(const float* m)
{
    set(m);
}

Matrix::Matrix(const Matrix& copy)
{
    std::memcpy(m, copy.m, GP_MATH_MATRIX_SIZE);
}

Matrix::~Matrix()
{
}

const Matrix& Matrix::identity()
{
    static Matrix m(1, 0, 0, 0,
					0, 1, 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1 );
    return m;
}

bool Matrix::isIdentity() const
{
    return (memcmp(m, MATRIX_IDENTITY, GP_MATH_MATRIX_SIZE) == 0);
}

const Matrix& Matrix::zero()
{
    static Matrix m(0, 0, 0, 0,
					0, 0, 0, 0,
					0, 0, 0, 0,
					0, 0, 0, 0 );
    return m;
}

void Matrix::createLookAt(const Vector3& eyePosition, const Vector3& targetPosition, const Vector3& up, Matrix* dst)
{
    createLookAt(eyePosition.x, eyePosition.y, eyePosition.z, 
		         targetPosition.x, targetPosition.y, targetPosition.z,
                 up.x, up.y, up.z, dst);
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
    float theta = GP_MATH_DEG_TO_RAD(fieldOfView) * 0.5f;
    if (std::fabs(std::fmod(theta, GP_MATH_PIOVER2)) < GP_MATH_EPSILON)
    {
        GP_ERROR("Invalid field of view value (%d) causes attempted calculation tan(%d), which is undefined.", fieldOfView, theta);
        return;
    }
    float divisor = std::tan(theta);
    GP_ASSERT(divisor);
    float factor = 1.0f / divisor;

    memset(dst, 0, GP_MATH_MATRIX_SIZE);

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

    memset(dst, 0, GP_MATH_MATRIX_SIZE);
    dst->m[0] = 2 / (right - left);
    dst->m[5] = 2 / (top - bottom);
    dst->m[12] = (left + right) / (left - right);
    dst->m[10] = 1 / (zNearPlane - zFarPlane);
    dst->m[13] = (top + bottom) / (bottom - top);
    dst->m[14] = zNearPlane / (zNearPlane - zFarPlane);
    dst->m[15] = 1;
}
    
void Matrix::createBillboard(const Vector3& objectPosition, const Vector3& cameraPosition,
                             const Vector3& cameraUpVector, Matrix* dst)
{
    createBillboardHelper(objectPosition, cameraPosition, cameraUpVector, nullptr, dst);
}

void Matrix::createBillboard(const Vector3& objectPosition, const Vector3& cameraPosition,
                             const Vector3& cameraUpVector, const Vector3& cameraForwardVector,
                             Matrix* dst)
{
    createBillboardHelper(objectPosition, cameraPosition, cameraUpVector, &cameraForwardVector, dst);
}
    
void Matrix::createReflection(const Plane& plane, Matrix* dst)
{
    Vector3 normal(plane.getNormal());
    float k = -2.0f * plane.getDistance();

    dst->setIdentity();
    dst->m[0] -= 2.0f * normal.x * normal.x;
    dst->m[5] -= 2.0f * normal.y * normal.y;
    dst->m[10] -= 2.0f * normal.z * normal.z;
    dst->m[1] = dst->m[4] = -2.0f * normal.x * normal.y;
    dst->m[2] = dst->m[8] = -2.0f * normal.x * normal.z;
    dst->m[6] = dst->m[9] = -2.0f * normal.y * normal.z;
    
    dst->m[3] = k * normal.x;
    dst->m[7] = k * normal.y;
    dst->m[11] = k * normal.z;
}

void Matrix::createScale(const Vector3& scale, Matrix* dst)
{
    GP_ASSERT(dst);

    std::memcpy(dst, MATRIX_IDENTITY, GP_MATH_MATRIX_SIZE);
    dst->m[0] = scale.x;
    dst->m[5] = scale.y;
    dst->m[10] = scale.z;
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

void Matrix::createTranslation(const Vector3& translation, Matrix* dst)
{
    GP_ASSERT(dst);
    std::memcpy(dst, MATRIX_IDENTITY, GP_MATH_MATRIX_SIZE);
    dst->m[12] = translation.x;
    dst->m[13] = translation.y;
    dst->m[14] = translation.z;
}

Vector3 Matrix::getScale() const
{
    float x = getX().length();
    float y = getY().length();
    float z = getZ().length();
	float det = determinant();
	if (det < 0)
		z = -z;
	return Vector3(x, y, z);
}

Vector3 Matrix::getEulerAngles() const
{
	Vector3 scale = getScale();
    float sx = scale.x;
    float sy = scale.y;
    float sz = scale.z;

	float x, y, z;
    y = std::asin(-m[2] / sx);
    if (y < GP_MATH_PIOVER2) 
	{
        if (y > -GP_MATH_PIOVER2) 
		{
            x = std::atan2(m[6] / sy, m[10] / sz);
            z = std::atan2(m[1] / sx, m[0] / sx);
        } 
		else 
		{
            z = 0;
            x = -std::atan2(m[4] / sy, m[5] / sy);
        }
    } 
	else 
	{
        z = 0;
        x = std::atan2(m[4] / sy, m[5] / sy);
    }
	Vector3 eulerAngles(x, y, z);
	eulerAngles.scale(GP_MATH_180OVERPI);
	return eulerAngles;
}

Vector3 Matrix::getTranslation() const
{
	return Vector3(m[12], m[13], m[14]);
}

Vector3 Matrix::getX() const
{
	return Vector3(m[0], m[1], m[2]);
}

Vector3 Matrix::getY() const
{
    return Vector3(m[4], m[5], m[6]);
}

Vector3 Matrix::getZ() const
{
    return Vector3(m[8], m[9], m[10]);
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

    return (a0 * b5 - a1 * b4 + a2 * b3 + a3 * b2 - a4 * b1 + a5 * b0);
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
    if (std::fabs(det) <= GP_MATH_TOLERANCE)
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

void Matrix::multiply(float scalar)
{
    multiply(scalar, this);
}

void Matrix::multiply(float scalar, Matrix* dst) const
{
    multiply(*this, scalar, dst);
}

void Matrix::multiply(const Matrix& matrix, float scalar, Matrix* dst)
{
    GP_ASSERT(dst);
    dst->m[0] = matrix.m[0] * scalar;
    dst->m[1] = matrix.m[1] * scalar;
    dst->m[2] = matrix.m[2] * scalar;
    dst->m[3] = matrix.m[3] * scalar;
    dst->m[4] = matrix.m[4] * scalar;
    dst->m[5] = matrix.m[5] * scalar;
    dst->m[6] = matrix.m[6] * scalar;
    dst->m[7] = matrix.m[7] * scalar;
    dst->m[8] = matrix.m[8] * scalar;
    dst->m[9] = matrix.m[9] * scalar;
    dst->m[10] = matrix.m[10] * scalar;
    dst->m[11] = matrix.m[11] * scalar;
    dst->m[12] = matrix.m[12] * scalar;
    dst->m[13] = matrix.m[13] * scalar;
    dst->m[14] = matrix.m[14] * scalar;
    dst->m[15] = matrix.m[15] * scalar;
}

void Matrix::multiply(const Matrix& m)
{
    multiply(*this, m, this);
}

void Matrix::multiply(const Matrix& m1, const Matrix& m2, Matrix* dst)
{
    GP_ASSERT(dst);
    dst->m[0] = m1.m[0] * m2.m[0] + m1.m[4] * m2.m[1] + m1.m[8] * m2.m[2] + m1.m[12] * m2.m[3];
    dst->m[1] = m1.m[1] * m2.m[0] + m1.m[5] * m2.m[1] + m1.m[9] * m2.m[2] + m1.m[13] * m2.m[3];
    dst->m[2] = m1.m[2] * m2.m[0] + m1.m[6] * m2.m[1] + m1.m[10] * m2.m[2] + m1.m[14] * m2.m[3];
    dst->m[3] = m1.m[3] * m2.m[0] + m1.m[7] * m2.m[1] + m1.m[11] * m2.m[2] + m1.m[15] * m2.m[3];

    dst->m[4] = m1.m[0] * m2.m[4] + m1.m[4] * m2.m[5] + m1.m[8] * m2.m[6] + m1.m[12] * m2.m[7];
    dst->m[5] = m1.m[1] * m2.m[4] + m1.m[5] * m2.m[5] + m1.m[9] * m2.m[6] + m1.m[13] * m2.m[7];
    dst->m[6] = m1.m[2] * m2.m[4] + m1.m[6] * m2.m[5] + m1.m[10] * m2.m[6] + m1.m[14] * m2.m[7];
    dst->m[7] = m1.m[3] * m2.m[4] + m1.m[7] * m2.m[5] + m1.m[11] * m2.m[6] + m1.m[15] * m2.m[7];

    dst->m[8] = m1.m[0] * m2.m[8] + m1.m[4] * m2.m[9] + m1.m[8] * m2.m[10] + m1.m[12] * m2.m[11];
    dst->m[9] = m1.m[1] * m2.m[8] + m1.m[5] * m2.m[9] + m1.m[9] * m2.m[10] + m1.m[13] * m2.m[11];
    dst->m[10] = m1.m[2] * m2.m[8] + m1.m[6] * m2.m[9] + m1.m[10] * m2.m[10] + m1.m[14] * m2.m[11];
    dst->m[11] = m1.m[3] * m2.m[8] + m1.m[7] * m2.m[9] + m1.m[11] * m2.m[10] + m1.m[15] * m2.m[11];

    dst->m[12] = m1.m[0] * m2.m[12] + m1.m[4] * m2.m[13] + m1.m[8] * m2.m[14] + m1.m[12] * m2.m[15];
    dst->m[13] = m1.m[1] * m2.m[12] + m1.m[5] * m2.m[13] + m1.m[9] * m2.m[14] + m1.m[13] * m2.m[15];
    dst->m[14] = m1.m[2] * m2.m[12] + m1.m[6] * m2.m[13] + m1.m[10] * m2.m[14] + m1.m[14] * m2.m[15];
    dst->m[15] = m1.m[3] * m2.m[12] + m1.m[7] * m2.m[13] + m1.m[11] * m2.m[14] + m1.m[15] * m2.m[15];
}

void Matrix::add(const Matrix& m1, const Matrix& m2, Matrix* dst)
{
	GP_ASSERT(dst);
    dst->m[0] = m1.m[0] + m2.m[0];
    dst->m[1] = m1.m[1] + m2.m[1];
    dst->m[2] = m1.m[2] + m2.m[2];
    dst->m[3] = m1.m[3] + m2.m[3];
    dst->m[4] = m1.m[4] + m2.m[4];
    dst->m[5] = m1.m[5] + m2.m[5];
    dst->m[6] = m1.m[6] + m2.m[6];
    dst->m[7] = m1.m[7] + m2.m[7];
    dst->m[8] = m1.m[8] + m2.m[8];
    dst->m[9] = m1.m[9] + m2.m[9];
    dst->m[10] = m1.m[10] + m2.m[10];
    dst->m[11] = m1.m[11] + m2.m[11];
    dst->m[12] = m1.m[12] + m2.m[12];
    dst->m[13] = m1.m[13] + m2.m[13];
    dst->m[14] = m1.m[14] + m2.m[14];
    dst->m[15] = m1.m[15] + m2.m[15];
}

void Matrix::subtract(const Matrix& m1, const Matrix& m2, Matrix* dst)
{
	GP_ASSERT(dst);
    dst->m[0] = m1.m[0] - m2.m[0];
    dst->m[1] = m1.m[1] - m2.m[1];
    dst->m[2] = m1.m[2] - m2.m[2];
    dst->m[3] = m1.m[3] - m2.m[3];
    dst->m[4] = m1.m[4] - m2.m[4];
    dst->m[5] = m1.m[5] - m2.m[5];
    dst->m[6] = m1.m[6] - m2.m[6];
    dst->m[7] = m1.m[7] - m2.m[7];
    dst->m[8] = m1.m[8] - m2.m[8];
    dst->m[9] = m1.m[9] - m2.m[9];
    dst->m[10] = m1.m[10] - m2.m[10];
    dst->m[11] = m1.m[11] - m2.m[11];
    dst->m[12] = m1.m[12] - m2.m[12];
    dst->m[13] = m1.m[13] - m2.m[13];
    dst->m[14] = m1.m[14] - m2.m[14];
    dst->m[15] = m1.m[15] - m2.m[15];
}

void Matrix::translate(const Vector3& t)
{
    translate(t, this);
}

void Matrix::translate(const Vector3& t, Matrix* dst) const
{
    Matrix m;
    createTranslation(t, &m);
    multiply(*this, m, dst);
}

void Matrix::rotate(const Quaternion& r)
{
    rotate(r, this);
}

void Matrix::rotate(const Quaternion& r, Matrix* dst) const
{
    Matrix m;
    createRotation(r, &m);
    multiply(*this, m, dst);
}

void Matrix::scale(const Vector3& s)
{
    scale(s, this);
}

void Matrix::scale(const Vector3& s, Matrix* dst) const
{
	Matrix m;
    createScale(s, &m);
    multiply(*this, m, dst);
}

void Matrix::negate()
{
    negate(this);
}

void Matrix::negate(Matrix* dst) const
{
    GP_ASSERT(dst);
    dst->m[0] = -m[0];
    dst->m[1] = -m[1];
    dst->m[2] = -m[2];
    dst->m[3] = -m[3];
    dst->m[4] = -m[4];
    dst->m[5] = -m[5];
    dst->m[6] = -m[6];
    dst->m[7] = -m[7];
    dst->m[8] = -m[8];
    dst->m[9] = -m[9];
    dst->m[10] = -m[10];
    dst->m[11] = -m[11];
    dst->m[12] = -m[12];
    dst->m[13] = -m[13];
    dst->m[14] = -m[14];
    dst->m[15] = -m[15];
}

void Matrix::transpose()
{
    transpose(this);
}

void Matrix::transpose(Matrix* dst) const
{
    GP_ASSERT(dst);
	dst->m[0] = m[0];
    dst->m[1] = m[4];
    dst->m[2] = m[8];
    dst->m[3] = m[12];
    dst->m[4] = m[1];
    dst->m[5] = m[5];
    dst->m[6] = m[9];
    dst->m[7] = m[13];
    dst->m[8] = m[2];
    dst->m[9] = m[6];
    dst->m[10] = m[10];
    dst->m[11] = m[14];
    dst->m[12] = m[3];
    dst->m[13] = m[7];
    dst->m[14] = m[11];
    dst->m[15] = m[15];
}

void Matrix::set(float m11, float m12, float m13, float m14, 
				 float m21, float m22, float m23, float m24,
                 float m31, float m32, float m33, float m34, 
	             float m41, float m42, float m43, float m44)
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
    std::memcpy(this->m, m, GP_MATH_MATRIX_SIZE);
}

void Matrix::set(const Matrix& m)
{
    std::memcpy(this->m, m.m, GP_MATH_MATRIX_SIZE);
}

void Matrix::setIdentity()
{
    std::memcpy(m, MATRIX_IDENTITY, GP_MATH_MATRIX_SIZE);
}

void Matrix::setZero()
{
    memset(m, 0, GP_MATH_MATRIX_SIZE);
}

void Matrix::set(const Vector3& t, const Quaternion& r, const Vector3& s)
{
	float tx, ty, tz, qx, qy, qz, qw, sx, sy, sz, x2, y2, z2, xx, xy, xz, yy, yz, zz, wx, wy, wz;

    tx = t.x;
    ty = t.y;
    tz = t.z;

    qx = r.x;
    qy = r.y;
    qz = r.z;
    qw = r.w;

    sx = s.x;
    sy = s.y;
    sz = s.z;

    x2 = qx + qx;
    y2 = qy + qy;
    z2 = qz + qz;
    xx = qx * x2;
    xy = qx * y2;
    xz = qx * z2;
    yy = qy * y2;
    yz = qy * z2;
    zz = qz * z2;
    wx = qw * x2;
    wy = qw * y2;
    wz = qw * z2;

    this->m[0] = (1 - (yy + zz)) * sx;
    this->m[1] = (xy + wz) * sx;
    this->m[2] = (xz - wy) * sx;
    this->m[3] = 0;
    this->m[4] = (xy - wz) * sy;
    this->m[5] = (1 - (xx + zz)) * sy;
    this->m[6] = (yz + wx) * sy;
    this->m[7] = 0;
    this->m[8] = (xz + wy) * sz;
    this->m[9] = (yz - wx) * sz;
    this->m[10] = (1 - (xx + yy)) * sz;
    this->m[11] = 0;
    this->m[12] = tx;
    this->m[13] = ty;
    this->m[14] = tz;
    this->m[15] = 1;
}

void Matrix::transformPoint(Vector3* point) const
{
    GP_ASSERT(point);
    transformPoint(Vector3(point->x, point->y, point->z), point);
}

void Matrix::transformPoint(const Vector3& p, Vector3* dst) const
{
	GP_ASSERT(dst);
    dst->x = p.x * m[0] + p.y * m[4] + p.z * m[8] + m[12];
	dst->y = p.x * m[1] + p.y * m[5] + p.z * m[9] + m[13];
	dst->z = p.x * m[2] + p.y * m[6] + p.z * m[10] + m[14];
}

void Matrix::transformVector(Vector3* vector) const
{
    GP_ASSERT(vector);
    transformVector(Vector3(vector->x, vector->y, vector->z), vector);
}

void Matrix::transformVector(const Vector3& v, Vector3* dst) const
{
	GP_ASSERT(dst);
	dst->x = v.x * m[0] + v.y * m[4] +  v.z * m[8];
	dst->y = v.x * m[1] + v.y * m[5] + v.z * m[9];
    dst->z = v.x * m[2] + v.y * m[6] +  v.z * m[10];
}

void Matrix::transformVector(const Vector4& v, Vector4* dst) const
{
	GP_ASSERT(dst);
	float x = v.x * m[0] + v.y * m[4] + v.z * m[8] + v.w * m[12];
    float y = v.x * m[1] + v.y * m[5] + v.z * m[9] + v.w * m[13];
    float z = v.x * m[2] + v.y * m[6] + v.z * m[10] + v.w * m[14];
    float w = v.x * m[3] + v.y * m[7] + v.z * m[11] + v.w * m[15];

    dst->x = x;
    dst->y = y;
    dst->z = z;
    dst->w = w;
}

Matrix& Matrix::operator=(const Matrix& m)
{
    if(&m == this)
        return *this;

    std::memcpy(this->m, m.m, GP_MATH_MATRIX_SIZE);

    return *this;
}

bool Matrix::operator==(const Matrix& m) const
{
    return memcmp(this->m, m.m, GP_MATH_MATRIX_SIZE) == 0;
}

bool Matrix::operator!=(const Matrix& m) const
{
    return memcmp(this->m, m.m, GP_MATH_MATRIX_SIZE) != 0;
}

const Matrix Matrix::operator-() const
{
    Matrix m(*this);
    m.negate();
    return m;
}

const Matrix Matrix::operator*(const Matrix& m) const
{
    Matrix result(*this);
    result.multiply(m);
    return result;
}

Matrix& Matrix::operator*=(const Matrix& m)
{
    multiply(m);
    return *this;
}

void Matrix::createBillboardHelper(const Vector3& objectPosition, const Vector3& cameraPosition,
                                   const Vector3& cameraUpVector, const Vector3* cameraForwardVector,
                                   Matrix* dst)
{
    Vector3 delta(objectPosition, cameraPosition);
    bool isSufficientDelta = delta.lengthSquared() > GP_MATH_EPSILON;

    dst->setIdentity();
    dst->m[3] = objectPosition.x;
    dst->m[7] = objectPosition.y;
    dst->m[11] = objectPosition.z;

    // As per the contracts for the 2 variants of createBillboard, 
	// we need either a safe default or a sufficient distance between object and camera.
    if (cameraForwardVector || isSufficientDelta)
    {
        Vector3 target = isSufficientDelta ? cameraPosition : (objectPosition - *cameraForwardVector);

        // A billboard is the inverse of a lookAt rotation
        Matrix lookAt;
        createLookAt(objectPosition, target, cameraUpVector, &lookAt);
        dst->m[0] = lookAt.m[0];
        dst->m[1] = lookAt.m[4];
        dst->m[2] = lookAt.m[8];
        dst->m[4] = lookAt.m[1];
        dst->m[5] = lookAt.m[5];
        dst->m[6] = lookAt.m[9];
        dst->m[8] = lookAt.m[2];
        dst->m[9] = lookAt.m[6];
        dst->m[10] = lookAt.m[10];
    }
}

}
