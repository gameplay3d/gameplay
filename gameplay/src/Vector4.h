#pragma once

namespace gameplay
{

class Matrix;

/**
 * Defines 4-element floating point vector.
 */
class Vector4
{
public:

    /**
     * The x-coordinate.
     */
    float x{ 0.0f };

    /**
     * The y-coordinate.
     */
    float y{ 0.0f };

    /**
     * The z-coordinate.
     */
    float z{ 0.0f };

    /**
     * The w-coordinate.
     */
    float w{ 0.0f };

    /**
     * Constructor
	 *
	 * The x, y, z, w coordinate values are all set to zero.
     */
    Vector4();

    /**
     * Constructor.
     *
     * @param x The x coordinate.
     * @param y The y coordinate.
     * @param z The z coordinate.
     * @param w The w coordinate.
     */
    Vector4(float x, float y, float z, float w);

    /**
     * Constructor.
     *
     * @param array An array containing the elements of the vector in the order x, y, z, w.
     */
    Vector4(const float* array);

    /**
     * Constructor.
     *
     * @param p1 The first point.
     * @param p2 The second point.
     */
    Vector4(const Vector4& p1, const Vector4& p2);

    /**
     * Constructor.
     *
     * Creates a new vector that is a copy of the specified vector.
     *
     * @param copy The vector to copy.
     */
    Vector4(const Vector4& copy);

    /**
     * Destructor.
     */
    ~Vector4();

    /**
     * Creates a new vector from an packed unsigned integer interpreted as an RGBA value.
     * Ex. 0xff0000ff represents opaque red or the vector (1, 0, 0, 1).
     *
     * @param color The integer to interpret as an RGBA value.
     * @return A vector corresponding to the interpreted RGBA color.
     */
    static Vector4 fromColor(unsigned int color);

    /**
     * Creates a new vector from a hex formatted string interpreted as an RGBA value.
     * Ex. "#ff0000ff" represents opaque red or the vector (1, 0, 0, 1).
     *
     * @param str The hex string to interpret as an RGBA value.
     * @return A vector corresponding to the interpreted RGBA color.
     */
    static Vector4 fromColorString(const char* str);

    /**
     * Gets the vector as a packed unsigned integer represented as RGBA value.
     * Ex. 0xff0000ff represents opaque red or the vector (1, 0, 0, 1).
     *
     * @return The vector as an unsigned integer color value.
     */
    unsigned int toColor() const;

    /**
     * Gets the zero vector.
     *
     * @return The 4-element vector of 0s.
     */
    static const Vector4& zero();

    /**
     * Gets the one vector.
     *
     * @return The 4-element vector of 1s.
     */
    static const Vector4& one();

    /**
     * Determines whether this vector contains all zeros.
     *
     * @return true if this vector contains all zeros, false otherwise.
     */
    bool isZero() const;

    /**
     * Determines whether this vector contains all ones.
     *
     * @return true if this vector contains all ones, false otherwise.
     */
    bool isOne() const;

    /**
     * Gets the computed angle (in radians) between the specified vectors.
     *
     * @param v1 The first vector.
     * @param v2 The second vector.
     * @return The angle between the two vectors (in radians).
     */
    static float angle(const Vector4& v1, const Vector4& v2);

    /**
     * Adds the elements of the specified vector to this one.
     *
     * @param v The vector to add.
     */
    void add(const Vector4& v);

    /**
     * Adds the specified vectors and stores the result in dst.
     *
     * @param v1 The first vector.
     * @param v2 The second vector.
     * @param dst A vector to store the result in.
     */
    static void add(const Vector4& v1, const Vector4& v2, Vector4* dst);

    /**
     * Clamps this vector within the specified range.
     *
     * @param min The minimum value.
     * @param max The maximum value.
     */
    void clamp(const Vector4& min, const Vector4& max);

    /**
     * Clamps the specified vector within the specified range and returns it in dst.
     *
     * @param v The vector to clamp.
     * @param min The minimum value.
     * @param max The maximum value.
     * @param dst A vector to store the result in.
     */
    static void clamp(const Vector4& v, const Vector4& min, const Vector4& max, Vector4* dst);

    /**
     * Gets the distance computed between this vector and v.
     *
     * @param v The other vector.
     * @return The distance between this vector and v.
     * @see distanceSquared
     */
    float distance(const Vector4& v) const;

    /**
     * Gets the squared distance computed between this vector and v.
     *
     * When it is not necessary to get the exact distance between
     * two vectors (for example, when simply comparing the
     * distance between different vectors), it is advised to use
     * this method instead of distance.
     *
     * @param v The other vector.
     * @return The squared distance between this vector and v.
	 *
     * @see distance
     */
    float distanceSquared(const Vector4& v) const;

    /**
     * Gets the dot product computed for this vector and the specified vector.
     *
     * @param v The vector to compute the dot product with.
     * @return The dot product.
     */
    float dot(const Vector4& v) const;

    /**
     * Gets the dot product computed between the specified vectors.
     *
     * @param v1 The first vector.
     * @param v2 The second vector.
     * @return The dot product between the vectors.
     */
    static float dot(const Vector4& v1, const Vector4& v2);

    /**
     * Gets the length computed for this vector.
     *
     * @return The length of the vector.
	 *
     * @see lengthSquared
     */
    float length() const;

    /**
     * Gets the squared length computed for this vector.
     *
     * When it is not necessary to get the exact length of a
     * vector (for example, when simply comparing the lengths of
     * different vectors), it is advised to use this method
     * instead of length.
     *
     * @return The squared length of the vector.
	 *
     * @see length
     */
    float lengthSquared() const;

    /**
     * Negates this vector.
     */
    void negate();

    /**
     * Normalizes this vector.
     *
     * This method normalizes this Vector4 so that it is of
     * unit length (in other words, the length of the vector
     * after calling this method will be 1.0f). If the vector
     * already has unit length or if the length of the vector
     * is zero, this method does nothing.
     * 
     * @return This vector, after the normalization occurs.
     */
    Vector4& normalize();

    /**
     * Normalizes this vector and stores the result in dst.
     *
     * If the vector already has unit length or if the length
     * of the vector is zero, this method simply copies the
     * current vector into dst.
     *
     * @param dst The destination vector.
     */
    void normalize(Vector4* dst) const;

    /**
     * Scales all elements of this vector by the specified value.
     *
     * @param scalar The scalar value.
     */
    void scale(float scalar);

    /**
     * Sets the elements of this vector to the specified values.
     *
     * @param x The new x coordinate.
     * @param y The new y coordinate.
     * @param z The new z coordinate.
     * @param w The new w coordinate.
     */
    void set(float x, float y, float z, float w);

    /**
     * Sets the elements of this vector from the values in the specified array.
     *
     * @param array An array containing the elements of the vector in the order x, y, z, w.
     */
    void set(const float* array);

    /**
     * Sets the elements of this vector to those in the specified vector.
     *
     * @param v The vector to copy.
     */
    void set(const Vector4& v);

    /**
     * Sets this vector to the directional vector between the specified points.
     * 
     * @param p1 The first point.
     * @param p2 The second point.
     */
    void set(const Vector4& p1, const Vector4& p2);

    /**
     * Subtracts this vector and the specified vector as (this - v)
     * and stores the result in this vector.
     *
     * @param v The vector to subtract.
     */
    void subtract(const Vector4& v);

    /**
     * Subtracts the specified vectors and stores the result in dst.
     * The resulting vector is computed as (v1 - v2).
     *
     * @param v1 The first vector.
     * @param v2 The second vector.
     * @param dst The destination vector.
     */
    static void subtract(const Vector4& v1, const Vector4& v2, Vector4* dst);

    /**
     * operator=
     */
    Vector4& operator=(const Vector4& v);

    /**
     * Computes the sum of this vector with the given vector.
     * 
     * Note: this does not modify this vector.
     * 
     * @param v The vector to add.
     * @return The vector sum.
     */
    const Vector4 operator+(const Vector4& v) const;

    /**
     * Adds the given vector to this vector.
     * 
     * @param v The vector to add.
     * @return This vector, after the addition occurs.
     */
    Vector4& operator+=(const Vector4& v);

    /**
     * Computes the difference of this vector with the given vector.
     * 
     * Note: this does not modify this vector.
     * 
     * @param v The vector to add.
     * @return The vector sum.
     */
    const Vector4 operator-(const Vector4& v) const;

    /**
     * Subtracts the given vector from this vector.
     * 
     * @param v The vector to subtract.
     * @return This vector, after the subtraction occurs.
     */
    Vector4& operator-=(const Vector4& v);

    /**
     * Calculates the negation of this vector.
     * 
     * Note: this does not modify this vector.
     * 
     * @return The negation of this vector.
     */
    const Vector4 operator-() const;

    /**
     * Calculates the scalar product of this vector with the given value.
     * 
     * Note: this does not modify this vector.
     * 
     * @param x The value to scale by.
     * @return The scaled vector.
     */
    const Vector4 operator*(float x) const;

    /**
     * Scales this vector by the given value.
     * 
     * @param x The value to scale by.
     * @return This vector, after the scale occurs.
     */
    Vector4& operator*=(float x);
    
    /**
     * Gets the components of this vector divided by the given constant
     *
     * Note: this does not modify this vector.
     *
     * @param x the constant to divide this vector with
     * @return a smaller vector
     */
    const Vector4 operator/(float x) const;

    /**
     * Determines if this vector is less than the given vector.
     * 
     * @param v The vector to compare against.
     * @return True if this vector is less than the given vector, false otherwise.
     */
    bool operator<(const Vector4& v) const;

    /**
     * Determines if this vector is equal to the given vector.
     * 
     * @param v The vector to compare against.
     * @return True if this vector is equal to the given vector, false otherwise.
     */
    bool operator==(const Vector4& v) const;

    /**
     * Determines if this vector is not equal to the given vector.
     * 
     * @param v The vector to compare against.
     * @return True if this vector is not equal to the given vector, false otherwise.
     */
    bool operator!=(const Vector4& v) const;
};

/**
 * Calculates the scalar product of the given vector with the given value.
 * 
 * @param x The value to scale by.
 * @param v The vector to scale.
 * @return The scaled vector.
 */
const Vector4 operator*(float x, const Vector4& v);

}
