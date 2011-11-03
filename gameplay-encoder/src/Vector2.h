/*
 * Vector2.h
 */

#ifndef VECTOR2_H_
#define VECTOR2_H_


namespace gameplay
{
// Forward declare
class Matrix;

/**
 * Defines a 2-element floating point vector.
 */
class Vector2
{
public:

    /**
     * The x coordinate.
     */
    float x;

    /**
     * The y coordinate.
     */
    float y;

    /**
     * Constructs a new vector initialized to all zeros.
     */
    Vector2();

    /**
     * Constructs a new vector initialized to the specified values.
     *
     * @param x The x coordinate.
     * @param y The y coordinate.
     */
    Vector2(float x, float y);

    /**
     * Constructs a new vector from the values in the specified array.
     *
     * @param array An array containing the elements of the vector in the order x, y.
     */
    Vector2(float* array);

    /**
     * Constructs a vector that describes the direction between the specified points.
     *
     * @param p1 The first point.
     * @param p2 The second point.
     */
    Vector2(const Vector2& p1, const Vector2& p2);

    /**
     * Constructs a new vector that is a copy of the specified vector.
     *
     * @param copy The vector to copy.
     */
    Vector2(const Vector2& copy);

    /**
     * Destructor.
     */
    ~Vector2();

    /**
     * The zero vector
     *
     * @return The 2-element vector of 0s.
     */
    static const Vector2& zero();

    /**
     * The one vector.
     *
     * @return The 2-element vector of 1s.
     */
    static const Vector2& one();

    /**
     * The unit x vector.
     *
     * @return The 2-element unit vector along the x axis.
     */
    static const Vector2& unitX();

    /**
     * The unit y vector.
     *
     * @return The 2-element unit vector along the y axis.
     */
    static const Vector2& unitY();

    /**
     * Is this vector the all zeros.
     *
     * @return true if all zeros, false if otherwise.
     */
    bool isZero() const;

    /**
     * Is this vector all ones.
     *
     * @return true if all ones, false if otherwise.
     */
    bool isOne() const;

    /**
     * Returns the angle (in radians) between the specified vectors.
     *
     * @param v1 The first vector.
     * @param v2 The second vector.
     * 
     * @return The angle between the two vectors, in radians.
     */
    static float angle(const Vector2& v1, const Vector2& v2);

    /**
     * Adds the elements of the specified vector to this one.
     *
     * @param v The vector to add.
     */
    void add(const Vector2& v);

    /**
     * Adds the specified vectors and stores the result in dst.
     *
     * @param v1 The first vector.
     * @param v2 The second vector.
     * @param dst A vector to store the result in.
     */
    static void add(const Vector2& v1, const Vector2& v2, Vector2* dst);

    /**
     * Clamps this vector within the specified range.
     *
     * @param min The minimum value.
     * @param max The maximum value.
     */
    void clamp(const Vector2& min, const Vector2& max);

    /**
     * Clamps the specified vector within the specified range and returns it in dst.
     *
     * @param v The vector to clamp.
     * @param min The minimum value.
     * @param max The maximum value.
     * @param dst A vector to store the result in.
     */
    static void clamp(const Vector2& v, const Vector2& min, const Vector2& max, Vector2* dst);

    /**
     * Returns the distance between this vector and v.
     *
     * @param v The other vector.
     * 
     * @return The distance between this vector and v.
     * @see distanceSquared
     */
    float distance(const Vector2& v);

    /**
     * Returns the squared distance between this vector and v.
     *
     * When it is not neccessary to get the exact distance between
     * two vectors (for example, when simply comparing the
     * distance between different vectors), it is advised to use
     * this method instead of distance.
     *
     * @param v The other vector.
     * 
     * @return The squared distance between this vector and v.
     * @see distance
     */
    float distanceSquared(const Vector2& v);

    /**
     * Returns the dot product of this vector and the specified vector.
     *
     * @param v The vector to compute the dot product with.
     * 
     * @return The dot product.
     */
    float dot(const Vector2& v);

    /**
     * Returns the dot product between the specified vectors.
     *
     * @param v1 The first vector.
     * @param v2 The second vector.
     * 
     * @return The dot product between the vectors.
     */
    static float dot(const Vector2& v1, const Vector2& v2);

    /**
     * Computes the length of this vector.
     *
     * @return The length of the vector.
     * @see lengthSquared
     */
    float length();

    /**
     * Returns the squared length of this vector.
     *
     * When it is not neccessary to get the exact length of a
     * vector (for example, when simply comparing the lengths of
     * different vectors), it is advised to use this method
     * instead of length.
     *
     * @return The squared length of the vector.
     * @see length
     */
    float lengthSquared();

    /**
     * Negates this vector.
     */
    void negate();

    /**
     * Normalizes this vector.
     *
     * This method normalizes this Vector2 so that it is of
     * unit length (in other words, the length of the vector
     * after calling this method will be 1.0f). If the vector
     * already has unit length or if the length of the vector
     * is zero, this method does nothing.
     */
    void normalize();

    /**
     * Normalizes this vector and stores the result in dst.
     *
     * If the vector already has unit length or if the length
     * of the vector is zero, this method simply copies the
     * current vector into dst.
     *
     * @param dst the destination vector
     */
    void normalize(Vector2* dst);

    /**
     * Scales all elements of this vector by the specified value.
     *
     * @param scalar The scalar value.
     */
    void scale(float scalar);

    /**
     * Scales each element of this vector by the matching component of scale.
     *
     * @param scale The vector to scale by.
     */
    void scale(const Vector2& scale);

    /**
     * Rotates this vector by angle (specified in radians) around the given point.
     *
     * @param point The point to rotate around.
     * @param angle The angle to rotate by, in radians.
     */
    void rotate(const Vector2& point, float angle);

    /**
     * Sets the elements of this vector to the specified values.
     *
     * @param x The new x coordinate.
     * @param y The new y coordinate.
     */
    void set(float x, float y);

    /**
     * Sets the elements of this vector from the values in the specified array.
     *
     * @param array An array containing the elements of the vector in the order x, y.
     */
    void set(float* array);

    /**
     * Sets the elements of this vector to those in the specified vector.
     *
     * @param v The vector to copy.
     */
    void set(const Vector2& v);

    /**
     * Sets this vector to the directional vector between the specified points.
     */
    void set(const Vector2& p1, const Vector2& p2);

    /**
     * Subtracts this vector and the specified vector as (this - v)
     * and stores the result in this.
     *
     * @param v The vector to subtract.
     */
    void subtract(const Vector2& v);

    /**
     * Subtracts the specified vectors and stores the result in dst.
     * The resulting vector is computed as (v1 - v2).
     *
     * @param v1 The first vector.
     * @param v2 The second vector.
     * @param dst The destination vector.
     */
    static void subtract(const Vector2& v1, const Vector2& v2, Vector2* dst);

    inline bool operator<(const Vector2& v) const
    {
        if (x == v.x)
        {
            return y < v.y;
        }
        return x < v.x;
    }
    inline bool operator==(const Vector2& v) const
    {
        return x==v.x && y==v.y;
    }

    /**
     * Writes this vector to the binary file stream.
     */
    void writeBinary(FILE* file) const;

    /**
     * Writes this vector to a text file stream.
     */
    void writeText(FILE* file) const;
};

}

#endif
