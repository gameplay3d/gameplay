#ifndef CURVE_H_
#define CURVE_H_

namespace gameplay
{

/**
 * Represents an n-dimensional curve.
 */
class Curve
{
    friend class Animation;
    friend class AnimationClip;

public:

    /**
     * Defines the type of interpolation.
     *
     * Note: InterpolationType::BEZIER requires control points and InterpolationType::HERMITE requires tangents.
     */
    enum InterpolationType
    {
        /**
         * Bezier interpolation. Requires that two control points are set for each segment.
         */
        BEZIER,

        /**
         * B-Spline interpolation. Uses the points as control points, and the curve is guaranteed to only pass through the
         * first and last point.
         */
        BSPLINE,

        /**
         * Flat. A form of Hermite interpolation that generates flat tangents for you. The tangents have a value equal to 0.
         */
        FLAT,

        /**
         * Hermite interpolation. Requires that two tangents for each segment.
         */
        HERMITE,

        /**
         * Linear interpolation.
         */
        LINEAR,

        /** 
         * Smooth. A form of Hermite interpolation that generates tangents for each segment based on the points prior to and after the segment.
         */
        SMOOTH,

        /**
         * Discrete interpolation.
         */ 
        STEP
    };

    /**
     * Constructs a new curve and the specified parameters.
     *
     * @param pointCount The number of points in the curve.
     * @param componentCount The number of float component values per key value.
     */
    Curve(unsigned int pointCount, unsigned int componentCount);

    /**
     * Destructor.
     */
    ~Curve();

    /**
     * Gets the number of points in the curve.
     *
     * @return The number of points in the curve.
     */
    unsigned int getPointCount() const;

    /**
     * Gets the number of float component values per points.
     *
     * @return The number of float component values per point.
     */
    unsigned int getComponentCount() const;

    /**
     * Returns the start time for the curve.
     *
     * @return The curve's start time.
     */
    float getStartTime() const;

    /**
     * Returns the end time for the curve.
     *
     * @return The curve's end time.
     */
    float getEndTime() const;

    /**
     * Sets the given point values on the curve the curve.
     *
     * @param index The index of the point.
     * @param time The time for the key.
     * @param value The point to add.
     * @param type The curve interpolation type.
     */
    void setPoint(unsigned int index, float time, float* value, InterpolationType type);

    /**
     * Sets the given point on the curve for the specified index and the specified parameters.
     *
     * @param index The index of the point.
     * @param time The time of the point within the curve.
     * @param value The value of the point to copy the data from.
     * @param type The curve interpolation type.
     * @param inValue The tangent approaching the point.
     * @param outValue The tangent leaving the point.
     */
    void setPoint(unsigned int index, float time, float* value, InterpolationType type, float* inValue, float* outValue);

    /**
     * Sets the tangents for a point on the curve specified by the index.
     *
     * @param index The index of the point.
     * @param type The curve interpolation type.
     * @param inValue The tangent approaching the point.
     * @param outValue The tangent leaving the point.
     */
    void setTangent(unsigned int index, InterpolationType type, float* inValue, float* outValue);
    
    /**
     * Evaluates the curve at the given position value (between 0.0 and 1.0 inclusive).
     *
     * @param time The position to evaluate the curve at.
     * @param dst The evaluated value of the curve at the given time.
     */
    void evaluate(float time, float* dst) const;

    /**
     * Adds an offset for the beginning of a Quaternion piece of data within the curve's value span at the specified
     * index. The next four components of data starting at the given index will be interpolated as a Quaternion.
     * This function will assert an error if the given index is less than the component size less the four components required
     * to store a quaternion.
     * One can set multiple offsets within the value span. However, this function will assert an error if the index
     * are not ordered, or if the index 
     * 
     * @param index The index of the Quaternion rotation data.
     */
    void addQuaternionOffset(unsigned int index);

private:

    /**
     * Defines a single point within a curve.
     */
    class Point
    {
    public:

        /** The time of the point within the curve. */
        float time;
        /** The value of the point. */
        float* value;
        /** The value of the tangent when approaching this point (from the previous point in the curve). */
        float* inValue;
        /** The value of the tangent when leaving this point (towards the next point in the curve). */
        float* outValue;
        /** The type of interpolation to use between this point and the next point. */
        InterpolationType type;

        /**
         * Constructor.
         */
        Point();

        /**
         * Destructor.
         */
        ~Point();
    };

    /**
     * Constructor.
     */
    Curve();

    /**
     * Constructor.
     */
    Curve(const Curve& copy);

    /**
     * Bezier interpolation function.
     */
    void interpolateBezier(float s, Point* from, Point* to, float* dst) const;

    /**
     * Bspline interpolation function.
     */
    void interpolateBSpline(float s, Point* c0, Point* c1, Point* c2, Point* c3, float* dst) const;

    /**
     * Hermite interpolation function.
     */
    void interpolateHermite(float s, Point* from, Point* to, float* dst) const;

    /**
     * Hermite interpolation function.
     */
    void interpolateHermiteFlat(float s, Point* from, Point* to, float* dst) const;

    /**
     * Hermite interpolation function.
     */
    void interpolateHermiteSmooth(float s, unsigned int index, Point* from, Point* to, float* dst) const;

    /** 
     * Linear interpolation function.
     */ 
    void interpolateLinear(float s, Point* from, Point* to, float* dst) const;

    /**
     * Quaternion interpolation function.
     */
    void interpolateQuaternion(float s, float* from, float* to, float* dst) const;

    /**
     * Determines the current keyframe to interpolate from based on the specified time.
     */ 
    int determineIndex(float time) const;

    unsigned int _pointCount;           // Number of points on the curve.
    unsigned int _componentCount;       // Number of components on the curve.
    unsigned int _componentSize;        // The component size (in bytes).
    unsigned int* _quaternionOffsets;   // Offset for the rotation component.
    unsigned int _quaternionOffsetsCount;
    Point* _points;                     // The points on the curve.
};

}

#endif
