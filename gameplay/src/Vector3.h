#pragma once

namespace gameplay
{

class Matrix;

/**
 * Defines a 3-element floating point vector.
 *
 * When using a vector to represent a surface normal, 
 * the vector should typically be normalized.
 * Other uses of directional vectors may wish to leave
 * the magnitude of the vector intact. When used as a point,
 * the elements of the vector represent a position in 3D space.
 */
class Vector3
{
public:

    /**
     * The x-coordinate.
     */
    float x = 0.0f;

    /**
     * The y-coordinate.
     */
    float y = 0.0f;

    /**
     * The z-coordinate.
     */
    float z = 0.0f;

    /**
     * Constructor.
	 * 
	 * The values x, y and z coordinate values are set to zero.
     */
    Vector3();

    /**
     * Constructor.
     *
     * @param x The x coordinate.
     * @param y The y coordinate.
     * @param z The z coordinate.
     */
    Vector3(float x, float y, float z);

    /**
     * Constructor.
     *
     * @param array An array containing the elements of the vector in the order x, y, z.
     */
    Vector3(const float* array);

    /**
     * Constructor.
     *
     * @param p1 The first point.
     * @param p2 The second point.
     */
    Vector3(const Vector3& p1, const Vector3& p2);

    /**
     * Constructor.
     *
     * @param copy The vector to copy.
     */
    Vector3(const Vector3& copy);

    /**
     * Destructor.
     */
    ~Vector3();

    /**
     * Creates a new vector from an integer interpreted as an RGB value.
     * Ex. 0xff0000 represents red or the vector (1, 0, 0).
     *
     * @param color The integer to interpret as an RGB value.
     * @return A vector corresponding to the interpreted RGB color.
     */
    static Vector3 fromColor(unsigned int color);

    /**
     * Creates a new vector from a hex formatted string interpreted as an RGB value.
     * Ex. "#ff0000" represents opaque red or the vector (1, 0, 0).
     *
     * @param str The hex string to interpret as an RGB value.
     * @return A vector corresponding to the interpreted RGB color.
     */
    static Vector3 fromColorString(const char* str);

    /**
     * Gets the vector as a packed unsigned integer represented as RGB value.
     * Ex. 0xff0000 represents opaque red or the vector (1, 0, 0).
     *
     * @return The vector as an unsigned integer color value.
     */
    unsigned int toColor() const;

    /**
     * Gets the zero vector.
     *
     * @return The 3-element vector of 0s.
     */
    static const Vector3& zero();

    /**
     * Gets the one vector.
     *
     * @return The 3-element vector of 1s.
     */
    static const Vector3& one();

    /**
     * Gets the up vector. Equal to Vector3(0, 1, 0).
     *
     * @return The up vector. Equal to Vector3(0, 1, 0).
     */
    static const Vector3& up();

    /**
     * Gets the down vector. Equal to Vector3(0, -1, 0).
     *
     * @return The down vector. Equal to Vector3(0, -1, 0).
     */
    static const Vector3& down();

    /**
     * Gets the right vector. Equal to Vector3(1, 0, 0).
     *
     * @return The right vector. Equal to Vector3(1, 0, 0).
     */
    static const Vector3& right();

    /**
     * Gets the left vector. Equal to Vector3(-1, 0, 0).
     *
     * @return The left vector. Equal to Vector3(-1, 0, 0).
     */
    static const Vector3& left();

    /**
     * Gets the forward vector. Equal to Vector3(0, 0, 1).
     *
     * @return The forward vector. Equal to Vector3(0, 0, 1).
     */
    static const Vector3& forward();

    /**
     * Gets the back vector. Equal to Vector3(0, 0, -1).
     *
     * @return The back vector. Equal to Vector3(0, 0, -1).
     */
    static const Vector3& back();

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
     * Gets the angle (in radians) computed between the specified vectors.
     *
     * @param v1 The first vector.
     * @param v2 The second vector.
     * @return The angle between the two vectors (in radians).
     */
    static float angle(const Vector3& v1, const Vector3& v2);

    /**
     * Adds the elements of the specified vector to this one.
     *
     * @param v The vector to add.
     */
    void add(const Vector3& v);

    /**
     * Adds the specified vectors and stores the result in dst.
     *
     * @param v1 The first vector.
     * @param v2 The second vector.
     * @param dst A vector to store the result in.
     */
    static void add(const Vector3& v1, const Vector3& v2, Vector3* dst);

    /**
     * Clamps this vector within the specified range.
     *
     * @param min The minimum value.
     * @param max The maximum value.
     */
    void clamp(const Vector3& min, const Vector3& max);

    /**
     * Clamps the specified vector within the specified range and returns it in dst.
     *
     * @param v The vector to clamp.
     * @param min The minimum value.
     * @param max The maximum value.
     * @param dst A vector to store the result in.
     */
    static void clamp(const Vector3& v, const Vector3& min, const Vector3& max, Vector3* dst);

    /**
     * Sets this vector to the cross product between itself and the specified vector.
     *
     * @param v The vector to compute the cross product with.
     */
    void cross(const Vector3& v);

    /**
     * Computes the cross product of the specified vectors and stores the result in dst.
     *
     * @param v1 The first vector.
     * @param v2 The second vector.
     * @param dst A vector to store the result in.
     */
    static void cross(const Vector3& v1, const Vector3& v2, Vector3* dst);

    /**
     * Gets the distance computed between this vector and v.
     *
     * @param v The other vector.
     * @return The distance between this vector and v.
	 *
     * @see distanceSquared
     */
    float distance(const Vector3& v) const;

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
    float distanceSquared(const Vector3& v) const;

    /**
     * Gets the dot product computed for this vector and the specified vector.
     *
     * @param v The vector to compute the dot product with.
     * @return The dot product.
     */
    float dot(const Vector3& v) const;

    /**
     * Returns the dot product between the specified vectors.
     *
     * @param v1 The first vector.
     * @param v2 The second vector.
     * @return The dot product between the vectors.
     */
    static float dot(const Vector3& v1, const Vector3& v2);

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
     * This method normalizes this Vector3 so that it is of
     * unit length (in other words, the length of the vector
     * after calling this method will be 1.0f). If the vector
     * already has unit length or if the length of the vector
     * is zero, this method does nothing.
     * 
     * @return This vector, after the normalization occurs.
     */
    Vector3& normalize();

    /**
     * Normalizes this vector and stores the result in dst.
     *
     * If the vector already has unit length or if the length
     * of the vector is zero, this method simply copies the
     * current vector into dst.
     *
     * @param dst The destination vector.
     */
    void normalize(Vector3* dst) const;

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
     */
    void set(float x, float y, float z);

    /**
     * Sets the elements of this vector from the values in the specified array.
     *
     * @param array An array containing the elements of the vector in the order x, y, z.
     */
    void set(const float* array);

    /**
     * Sets the elements of this vector to those in the specified vector.
     *
     * @param v The vector to copy.
     */
    void set(const Vector3& v);

    /**
     * Sets this vector to the directional vector between the specified points.
	 * 
	 * @param p1 The first point.
	 * @param p2 The second point.
     */
    void set(const Vector3& p1, const Vector3& p2);

    /**
     * Subtracts this vector and the specified vector as (this - v)
     * and stores the result in this vector.
     *
     * @param v The vector to subtract.
     */
    void subtract(const Vector3& v);

    /**
     * Subtracts the specified vectors and stores the result in dst.
     * The resulting vector is computed as (v1 - v2).
     *
     * @param v1 The first vector.
     * @param v2 The second vector.
     * @param dst The destination vector.
     */
    static void subtract(const Vector3& v1, const Vector3& v2, Vector3* dst);

    /**
     * Updates this vector towards the given target using a smoothing function.
     * The given response time determines the amount of smoothing (lag). A longer
     * response time yields a smoother result and more lag. To force this vector to
     * follow the target closely, provide a response time that is very small relative
     * to the given elapsed time.
     *
     * @param target target value.
     * @param elapsedTime elapsed time between calls.
     * @param responseTime response time (in the same units as elapsedTime).
     */
    void smooth(const Vector3& target, float elapsedTime, float responseTime);

    /**
     * operator=
     */
    Vector3& operator=(const Vector3& v);

    /**
     * Calculates the sum of this vector with the given vector.
     * 
     * Note: this does not modify this vector.
     * 
     * @param v The vector to add.
     * @return The vector sum.
     */
    const Vector3 operator+(const Vector3& v) const;

    /**
     * Adds the given vector to this vector.
     * 
     * @param v The vector to add.
     * @return This vector, after the addition occurs.
     */
    Vector3& operator+=(const Vector3& v);

    /**
     * Substracts the difference of this vector with the given vector.
     * 
     * Note: this does not modify this vector.
     * 
     * @param v The vector to subtract.
     * @return The vector difference.
     */
    const Vector3 operator-(const Vector3& v) const;

    /**
     * Subtracts the given vector from this vector.
     * 
     * @param v The vector to subtract.
     * @return This vector, after the subtraction occurs.
     */
    Vector3& operator-=(const Vector3& v);

    /**
     * Computes the negation of this vector.
     * 
     * Note: this does not modify this vector.
     * 
     * @return The negation of this vector.
     */
    const Vector3 operator-() const;

    /**
     * Computes the scalar product of this vector with the given value.
     * 
     * Note: this does not modify this vector.
     * 
     * @param x The value to scale by.
     * @return The scaled vector.
     */
    const Vector3 operator*(float x) const;

    /**
     * Scales this vector by the given value.
     * 
     * @param x The value to scale by.
     * @return This vector, after the scale occurs.
     */
    Vector3& operator*=(float x);
    
    /**
     * Gets the components of this vector divided by the given constant
     *
     * Note: this does not modify this vector.
     *
     * @param x the constant to divide this vector with
     * @return a smaller vector
     */
    const Vector3 operator/(float x) const;

    /**
     * Determines if this vector is less than the given vector.
     * 
     * @param v The vector to compare against.
     * 
     * @return True if this vector is less than the given vector, false otherwise.
     */
    bool operator<(const Vector3& v) const;

    /**
     * Determines if this vector is equal to the given vector.
     * 
     * @param v The vector to compare against.
     * 
     * @return True if this vector is equal to the given vector, false otherwise.
     */
    bool operator==(const Vector3& v) const;

    /**
     * Determines if this vector is not equal to the given vector.
     * 
     * @param v The vector to compare against.
     * 
     * @return True if this vector is not equal to the given vector, false otherwise.
     */
    bool operator!=(const Vector3& v) const;
};

/**
 * Calculates the scalar product of the given vector with the given value.
 * 
 * @param x The value to scale by.
 * @param v The vector to scale.
 * @return The scaled vector.
 */
const Vector3 operator*(float x, const Vector3& v);

}
