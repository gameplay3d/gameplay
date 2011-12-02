#ifndef MATRIX_H_
#define MATRIX_H_

#include "Vector4.h"
#include "Vector3.h"
#include "Vector2.h"
#include "Quaternion.h"
#include "FileIO.h"

#define MATRIX4F_SIZE   (sizeof(float) * 16)
#define PI 3.14159265f
#define TORADIANS(degrees) (degrees * (PI / 180.0f))
#define TODEGREES(radians) (radians * (180.0f / PI))

namespace gameplay
{

/**
 * The identify matrix.
 */
static const float MATRIX4F_IDENTITY[16] =
{
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
};

class Matrix
{
public:

    /**
     * Matrix colums.
     */
    float m[16];

    /**
     * Constructor.
     */
    Matrix(void);

    /**
     * Constructor.
     */
    Matrix(float m0, float m1, float m2, float m3,
           float m4, float m5, float m6, float m7,
           float m8, float m9, float m10, float m11,
           float m12, float m13, float m14, float m15);

    /**
     * Destructor.
     */
    ~Matrix(void);

    /**
     * Computes the determinant of this matrix.
     *
     * @return The determinant.
     */
    float determinant() const;

    /**
     * Decomposes the scale, rotation and translation components of this matrix.
     *
     * @param scale The scale.
     * @param rotation The rotation.
     * @param translation The translation.
     */
    bool decompose(Vector3* scale, Quaternion* rotation, Vector3* translation) const;

    /**
     * Sets the given matrix to be the identity matrix.
     */
    static void setIdentity(float* matrix);

    /**
     * Multiplies two matrices and stores the results in dst.
     * m1 and m2 may be the same as dst.
     */
    static void multiply(const float* m1, const float* m2, float* dst);

    /**
     * Creates a scale matrix for the givent x,y,z scale factors.
     */
    static void createScale(float x, float y, float z, float* dst);

    /**
     * Creates a rotation matrix from the given quaternion.
     */
    static void createRotation(const Quaternion& q, float* dst);

    /**
     * Creates a rotation matrix from the given axis and angle in degrees.
     */
    static void createRotation(float x, float y, float z, float angle, float* dst);
    
    /**
     * Creates a rotation matrix for the given angle in the x axis and angle in degrees
     */
    void createRotationX(float angle, float* dst);
    
    /**
     * Creates a rotation matrix for the given angle in the y axis and angle in degrees
     */
    void createRotationY(float angle, float* dst);
        
    /**
     * Creates a rotation matrix for the given angle in the z axis and angle in degrees
     */
    void createRotationZ(float angle, float* dst);
    
    /**
     * Creates a translation matrix for the given x, y ,x values.
     */
    static void createTranslation(float x, float y, float z, float* dst);

    /**
     * Returns the pointer to the float array.
     */
    float* getArray();

    /**
     * Translates the matrix by the x, y, z values.
     */
    void translate(float x, float y, float z);

    /**
     * Scales the matrix by the x, y, z factors.
     */
    void scale(float x, float y, float z);

    /**
     * Rotates the matrix by the given Quaternion.
     */
    void rotate(const Quaternion& q);

    /**
     * Rotates the matrix by the axies specified and angle.
     */
    void rotate(float x, float y, float z, float angle);

    /**
     * Rotates the matrix on the y-axis by the specified angle in degrees.
     */
    void rotateX(float angle);
    
    /**
     * Rotates the matrix on the y-axis by the specified angle in degrees.
     */
    void rotateY(float angle);

    /**
     * Rotates the matrix on the z-axis by the specified angle in degrees.
     */
    void rotateZ(float angle);

    /**
     * Transforms the specified point by this matrix.
     *
     * Note that the input vector is treated as a point and NOT a vector.
     */
    void transformPoint(const Vector3& p, Vector3* dst) const;

};

}

#endif
