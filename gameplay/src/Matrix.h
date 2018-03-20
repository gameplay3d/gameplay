#pragma once

#include "Vector3.h"
#include "Vector4.h"

namespace gameplay
{

class Plane;
class Quaternion;

/**
 * Defines a 4 x 4 floating point matrix representing a 3D transformation.
 *
 * Vectors are treated as columns, resulting in a matrix that is represented as follows,
 * where x, y and z are the translation components of the matrix:
 *
 * 1  0  0  x
 * 0  1  0  y
 * 0  0  1  z
 * 0  0  0  1
 *
 * This matrix class is directly compatible with OpenGL since its elements are
 * laid out in memory exactly as they are expected by OpenGL.
 * The matrix uses column-major format such that array indices increase down column first.
 * Since matrix multiplication is not commutative, multiplication must be done in the
 * correct order when combining transformations. Suppose we have a translation
 * matrix T and a rotation matrix R. To first rotate an object around the origin
 * and then translate it, you would multiply the two matrices as TR.
 *
 * Likewise, to first translate the object and then rotate it, you would do RT.
 * So generally, matrices must be multiplied in the reverse order in which you
 * want the transformations to take place (this also applies to
 * the scale, rotate, and translate methods below; these methods are convenience
 * methods for post-multiplying by a matrix representing a scale, rotation, or translation).
 *
 * In the case of repeated local transformations (i.e. rotate around the Z-axis by 0.76 radians,
 * then translate by 2.1 along the X-axis, then ...), it is better to use the Transform class
 * (which is optimized for that kind of usage).
 *
 * @see Transform
 */
class Matrix
{
public:

    /**
     * The column major values of this 4x4 matrix.
     * */
    float m[16];

    /**
     * Constructor.
     *
     * The matrix is initialized to the identity matrix:
     *
     * 1  0  0  0
     * 0  1  0  0
     * 0  0  1  0
     * 0  0  0  1
     */
    Matrix();

    /**
     * Constructor.
     *
     * @param m11 The first element of the first row.
     * @param m12 The second element of the first row.
     * @param m13 The third element of the first row.
     * @param m14 The fourth element of the first row.
     * @param m21 The first element of the second row.
     * @param m22 The second element of the second row.
     * @param m23 The third element of the second row.
     * @param m24 The fourth element of the second row.
     * @param m31 The first element of the third row.
     * @param m32 The second element of the third row.
     * @param m33 The third element of the third row.
     * @param m34 The fourth element of the third row.
     * @param m41 The first element of the fourth row.
     * @param m42 The second element of the fourth row.
     * @param m43 The third element of the fourth row.
     * @param m44 The fourth element of the fourth row.
     */
    Matrix(float m11, float m12, float m13, float m14, 
           float m21, float m22, float m23, float m24,
           float m31, float m32, float m33, float m34, 
           float m41, float m42, float m43, float m44);

    /**
     * Constructor.
     *
     * The passed-in array is in column-major order, so the memory layout of the array is as follows:
     *
     *     0   4   8   12
     *     1   5   9   13
     *     2   6   10  14
     *     3   7   11  15
     *
     * @param m An array containing 16 elements in column-major order.
     */
    Matrix(const float* m);

    /**
     * Constructor.
     *
     * @param copy The matrix to copy.
     */
    Matrix(const Matrix& copy);

    /**
     * Destructor.
     */
    ~Matrix();

    /**
     * Gets the identity matrix:
     *
     * 1  0  0  0
     * 0  1  0  0
     * 0  0  1  0
     * 0  0  0  1
     *
     * @return The identity matrix.
     */
    static const Matrix& identity();

    /**
     * Gets the matrix with all zeros.
     *
     * @return The matrix with all zeros.
     */
    static const Matrix& zero();

    /**
     * Creates a view matrix based on the specified input parameters.
     *
     * @param eyePosition The eye position.
     * @param targetPosition The target's center position.
     * @param up The up vector.
     * @param dst A matrix to store the result in.
     */
    static void createLookAt(const Vector3& eyePosition, const Vector3& targetPosition, const Vector3& up, Matrix* dst);

    /**
     * Creates a view matrix based on the specified input parameters.
     *
     * @param eyePositionX The eye x-coordinate position.
     * @param eyePositionY The eye y-coordinate position.
     * @param eyePositionZ The eye z-coordinate position.
     * @param targetCenterX The target's center x-coordinate position.
     * @param targetCenterY The target's center y-coordinate position.
     * @param targetCenterZ The target's center z-coordinate position.
     * @param upX The up vector x-coordinate value.
     * @param upY The up vector y-coordinate value.
     * @param upZ The up vector z-coordinate value.
     * @param dst A matrix to store the result in.
     */
    static void createLookAt(float eyePositionX, float eyePositionY, float eyePositionZ,
                             float targetCenterX, float targetCenterY, float targetCenterZ,
                             float upX, float upY, float upZ, Matrix* dst);

    /**
     * Builds a perspective projection matrix based on a field of view and returns by value.
     *
     * Projection space refers to the space after applying projection transformation from view space.
     * After the projection transformation, visible content has x- and y-coordinates ranging from -1 to 1,
     * and a z-coordinate ranging from 0 to 1. To obtain the viewable area (in world space) of a scene,
     * create a BoundingFrustum and pass the combined view and projection matrix to the constructor.
     *
     * @param fieldOfView The field of view in the y direction (in degrees).
     * @param aspectRatio The aspect ratio, defined as view space width divided by height.
     * @param zNearPlane The distance to the near view plane.
     * @param zFarPlane The distance to the far view plane.
     * @param dst A matrix to store the result in.
     */
    static void createPerspective(float fieldOfView, float aspectRatio, float zNearPlane, float zFarPlane, Matrix* dst);

    /**
     * Creates an orthographic projection matrix.
     *
     * @param width The width of the view.
     * @param height The height of the view.
     * @param zNearPlane The minimum z-value of the view volume.
     * @param zFarPlane The maximum z-value of the view volume.
     * @param dst A matrix to store the result in.
     */
    static void createOrthographic(float width, float height, float zNearPlane, float zFarPlane, Matrix* dst);

    /**
     * Creates an orthographic projection matrix.
     *
     * Projection space refers to the space after applying
     * projection transformation from view space. After the
     * projection transformation, visible content has
     * x and y coordinates ranging from -1 to 1, and z coordinates
     * ranging from 0 to 1.
     *
     * Unlike perspective projection, in orthographic projection
     * there is no perspective foreshortening.
     *
     * The viewable area of this orthographic projection extends
     * from left to right on the x-axis, bottom to top on the y-axis,
     * and zNearPlane to zFarPlane on the z-axis. These values are
     * relative to the position and x, y, and z-axes of the view.
     * To obtain the viewable area (in world space) of a scene,
     * create a BoundingFrustum and pass the combined view and
     * projection matrix to the constructor.
     *
     * @param left The minimum x-value of the view volume.
     * @param right The maximum x-value of the view volume.
     * @param bottom The minimum y-value of the view volume.
     * @param top The maximum y-value of the view volume.
     * @param zNearPlane The minimum z-value of the view volume.
     * @param zFarPlane The maximum z-value of the view volume.
     * @param dst A matrix to store the result in.
     */
    static void createOrthographicOffCenter(float left, float right, float bottom, float top,
                                            float zNearPlane, float zFarPlane, Matrix* dst);

    /**
     * Creates a spherical billboard that rotates around a specified object position.
     *
     * This method computes the facing direction of the billboard from the object position
     * and camera position. When the object and camera positions are too close, the matrix
     * will not be accurate. To avoid this problem, this method defaults to the identity
     * rotation if the positions are too close. (See the other overload of createBillboard
     * for an alternative approach).
     *
     * @param objectPosition The position of the object the billboard will rotate around.
     * @param cameraPosition The position of the camera.
     * @param cameraUpVector The up vector of the camera.
     * @param dst A matrix to store the result in.
     */
    static void createBillboard(const Vector3& objectPosition, const Vector3& cameraPosition,
                                const Vector3& cameraUpVector, Matrix* dst);

    /**
     * Creates a spherical billboard that rotates around a specified object position with
     * provision for a safe default orientation.
     *
     * This method computes the facing direction of the billboard from the object position
     * and camera position. When the object and camera positions are too close, the matrix
     * will not be accurate. To avoid this problem, this method uses the specified camera
     * forward vector if the positions are too close. (See the other overload of createBillboard
     * for an alternative approach).
     *
     * @param objectPosition The position of the object the billboard will rotate around.
     * @param cameraPosition The position of the camera.
     * @param cameraUpVector The up vector of the camera.
     * @param cameraForwardVector The forward vector of the camera, used if the positions are too close.
     * @param dst A matrix to store the result in.
     */
    static void createBillboard(const Vector3& objectPosition, const Vector3& cameraPosition,
                                const Vector3& cameraUpVector, const Vector3& cameraForwardVector,
                                Matrix* dst);

    /**
     * Creates a reflection matrix so that it reflects the coordinate system about a specified Plane.
     *
     * @param plane The Plane about which to create a reflection.
     * @param dst A matrix to store the result in.
     */
    static void createReflection(const Plane& plane, Matrix* dst);

    /**
     * Creates a scale matrix.
     *
     * @param scale The mount to scale along all axis.  
     * @param dst A matrix to store the result in.
     */
    static void createScale(const Vector3& scale, Matrix* dst);

    /**
     * Creates a rotation matrix from the specified quaternion.
     *
     * @param q A quaternion describing a 3D orientation.
     * @param dst A matrix to store the result in.
     */
    static void createRotation(const Quaternion& q, Matrix* dst);

    /**
     * Creates a translation matrix.
     *
     * @param translation The translation.
     * @param dst A matrix to store the result in.
     */
    static void createTranslation(const Vector3& translation, Matrix* dst);

    /**
     * Gets the scalar component of this matrix.
     *
     * If the scalar component of this matrix has negative parts,
     * it is not possible to always extract the exact scalar component;
     * instead, a scale vector that is mathematically equivalent to the
     * original scale vector is extracted and returned.
     *
     * @return The scalar component of this matrix
     */
    Vector3 getScale() const;

    /**
     * Gets the Euler angles equivalent to the rotational portion
     * of the specified matrix. 
     *
     * The returned Euler angles are in XYZ order an in degrees.
     *
     * @return The rotational component of this matrix
     */
    Vector3 getEulerAngles() const;

    /**
     * Gets the translational component of this matrix.
     *
     * @return Gets the scalar component of this matrix
     */
    Vector3 getTranslation() const;

    /**
     * Gets the x-axis from the specified 4x4 matrix.
     *
     * @return The x-axis from the specified 4x4 matrix.
     */
    Vector3 getX() const;

    /**
     * Gets the y-axis from the specified 4x4 matrix.
     *
     * @return The y-axis from the specified 4x4 matrix.
     */
    Vector3 getY() const;

    /**
     * Gets the z-axis from the specified 4x4 matrix.
     *
     * @return The z-axis from the specified 4x4 matrix.
     */
    Vector3 getZ() const;

    /**
     * Computes the determinant of this matrix.
     *
     * @return The determinant.
     */
    float determinant() const;

    /**
     * Inverts this matrix.
     *
     * @return true if the the matrix can be inverted, false otherwise.
     */
    bool invert();

    /**
     * Stores the inverse of this matrix in the specified matrix.
     *
     * @param dst A matrix to store the invert of this matrix in.
     * @return true if the the matrix can be inverted, false otherwise.
     */
    bool invert(Matrix* dst) const;

    /**
     * Multiplies the components of this matrix by the specified scalar.
     *
     * @param scalar The scalar value.
     */
    void multiply(float scalar);

    /**
     * Multiplies the components of this matrix by a scalar and stores the result in dst.
     *
     * @param scalar The scalar value.
     * @param dst A matrix to store the result in.
     */
    void multiply(float scalar, Matrix* dst) const;

    /**
     * Multiplies the components of the specified matrix by a scalar and stores the result in dst.
     *
     * @param matrix The matrix.
     * @param scalar The scalar value.
     * @param dst A matrix to store the result in.
     */
    static void multiply(const Matrix& matrix, float scalar, Matrix* dst);

    /**
     * Multiplies this matrix by the specified one.
     *
     * @param m The matrix to multiply.
     */
    void multiply(const Matrix& m);

    /**
     * Multiplies m1 by m2 and stores the result in dst.
     *
     * @param m1 The first matrix to multiply.
     * @param m2 The second matrix to multiply.
     * @param dst A matrix to store the result in.
     */
    static void multiply(const Matrix& m1, const Matrix& m2, Matrix* dst);

    /**
     * Adds m1 by m2 and stores the result in dst.
     *
     * @param m1 The first matrix to add.
     * @param m2 The second matrix to add.
     * @param dst A matrix to store the result in.
     */
    static void add(const Matrix& m1, const Matrix& m2, Matrix* dst);

    /**
     * Subtracts m1 from m2 and stores the result in dst.
     *
     * @param m1 The first matrix to add.
     * @param m2 The second matrix to add.
     * @param dst A matrix to store the result in.
     */
    static void subtract(const Matrix& m1, const Matrix& m2, Matrix* dst);

    /**
     * Post-multiplies this matrix by the matrix corresponding to the
     * specified quaternion rotation.
     *
     * @param q The quaternion to rotate by.
     */
    void rotate(const Quaternion& q);

 /**
     * Post-multiplies this matrix by the matrix corresponding to the
     * specified translation.
     *
     * @param t The translation values along the x, y and z axes.
     */
    void translate(const Vector3& t);

    /**
     * Post-multiplies this matrix by the matrix corresponding to the
     * specified translation and stores the result in dst.
     *
     * @param t The translation values along the x, y and z axes.
     * @param dst A matrix to store the result in.
     */
    void translate(const Vector3& t, Matrix* dst) const;

    /**
     * Post-multiplies this matrix by the matrix corresponding to the
     * specified quaternion rotation and stores the result in dst.
     *
     * @param r The quaternion to rotate by.
     * @param dst A matrix to store the result in.
     */
    void rotate(const Quaternion& r, Matrix* dst) const;

  /**
     * Post-multiplies this matrix by the matrix corresponding to the
     * specified scale transformation.
     *
     * @param s The scale values along the x, y and z axes.
     */
    void scale(const Vector3& s);

    /**
     * Post-multiplies this matrix by the matrix corresponding to the
     * specified scale transformation and stores the result in dst.
     *
     * @param s The scale values along the x, y and z axes.
     * @param dst A matrix to store the result in.
     */
    void scale(const Vector3& s, Matrix* dst) const;

    /**
     * Negates this matrix.
     */
    void negate();

    /**
     * Negates this matrix and stores the result in dst.
     *
     * @param dst A matrix to store the result in.
     */
    void negate(Matrix* dst) const;

    /**
     * Transposes this matrix.
     */
    void transpose();

    /**
     * Transposes this matrix and stores the result in dst.
     *
     * @param dst A matrix to store the result in.
     */
    void transpose(Matrix* dst) const;

    /**
     * Sets the values of this matrix.
     *
     * @param m11 The first element of the first row.
     * @param m12 The second element of the first row.
     * @param m13 The third element of the first row.
     * @param m14 The fourth element of the first row.
     * @param m21 The first element of the second row.
     * @param m22 The second element of the second row.
     * @param m23 The third element of the second row.
     * @param m24 The fourth element of the second row.
     * @param m31 The first element of the third row.
     * @param m32 The second element of the third row.
     * @param m33 The third element of the third row.
     * @param m34 The fourth element of the third row.
     * @param m41 The first element of the fourth row.
     * @param m42 The second element of the fourth row.
     * @param m43 The third element of the fourth row.
     * @param m44 The fourth element of the fourth row.
     */
    void set(float m11, float m12, float m13, float m14, 
             float m21, float m22, float m23, float m24,
             float m31, float m32, float m33, float m34, 
             float m41, float m42, float m43, float m44);

    /**
     * Sets the values of this matrix to those in the specified column-major array.
     *
     * @param m An array containing 16 elements in column-major format.
     */
    void set(const float* m);

    /**
     * Sets the values of this matrix to those of the specified matrix.
     *
     * @param m The source matrix.
     */
    void set(const Matrix& m);

    /**
     * Sets this matrix to the identity matrix.
     */
    void setIdentity();

    /**
     * Determines if this matrix is equal to the identity matrix.
     *
     * @return true if the matrix is an identity matrix, false otherwise.
     */
    bool isIdentity() const;

    /**
     * Sets all elements of the current matrix to zero.
     */
    void setZero();

    /**
     * Set the matrix by applying translation, rotation and scale.
     *
     * @param t The translation to be applied.
     * @param r The rotation to be applied.
     * @param s The scale to be applied.
     */
    void set(const Vector3& t, const Quaternion& r, const Vector3& s);

    /**
     * Transforms the specified point by this matrix.
     *
     * The result of the transformation is stored directly into point.
     *
     * @param point The point to transform and also a vector to hold the result in.
     */
    void transformPoint(Vector3* point) const;

    /**
     * Transforms the specified point by this matrix.
     *
     * @param p The point to transform.
     * @param dst A vector to store the transformed point in.
     */
    void transformPoint(const Vector3& p, Vector3* dst) const;

    /**
     * Transforms the specified vector by this matrix by
     * treating the fourth (w) coordinate as zero.
     *
     * The result of the transformation is stored directly into vector.
     *
     * @param vector The vector to transform and also a vector to hold the result in.
     */
    void transformVector(Vector3* vector) const;

    /**
     * Transforms the specified vector by this matrix.
     *
     * @param v The vector to transform.
     * @param dst A vector to store the transformed vector in.
     */
    void transformVector(const Vector3& v, Vector3* dst) const;

    /**
     * Transforms the specified vector by this matrix.
     *
     * @param v The vector to transform.
     * @param dst A vector to store the transformed vector in.
     */
    void transformVector(const Vector4& v, Vector4* dst) const;

    /**
     * operator =
     */
    Matrix& operator=(const Matrix& m);

    /**
     * Determines if this matrix is equal to the given matrix.
     *
     * @param m The matrix to compare against.
     *
     * @return True if this matrix is equal to the given matrix, false otherwise.
     */
    bool operator==(const Matrix& m) const;

    /**
     * Determines if this vector is not equal to the given vector.
     *
     * @param m The matrix to compare against.
     * @return True if this matrix is not equal to the given matrix, false otherwise.
     */
    bool operator!=(const Matrix& m) const;

    /**
     * Calculates the negation of this matrix.
     * 
     * This does not modify this matrix.
     * 
     * @return The negation of this matrix.
     */
    const Matrix operator-() const;

    /**
     * Calculates the matrix product of this matrix with the given matrix.
     * 
     * This does not modify this matrix.
     * 
     * @param m The matrix to multiply by.
     * @return The matrix product.
     */
    const Matrix operator*(const Matrix& m) const;

    /**
     * Right-multiplies this matrix by the given matrix.
     * 
     * @param m The matrix to multiply by.
     * @return This matrix, after the multiplication occurs.
     */
    Matrix& operator*=(const Matrix& m);
    
private:

    static void createBillboardHelper(const Vector3& objectPosition, const Vector3& cameraPosition,
                                      const Vector3& cameraUpVector, const Vector3* cameraForwardVector,
                                      Matrix* dst);
};

}
