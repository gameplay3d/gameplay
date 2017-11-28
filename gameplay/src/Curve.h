#pragma once

namespace gameplay
{

/**
 * Defines an n-dimensional curve.
 */
class Curve
{
public:

    /**
     * Types of interpolation.
     *
     * Defines how the points in the curve are connected.
     * Interpolation::INTERPOLATION_BEZIER uses control points and InterpolationType::HERMITE uses tangents.
     */
    enum Interpolation
    {
		INTERPOLATION_LINEAR,
        INTERPOLATION_BEZIER,
        INTERPOLATION_BSPLINE,
        INTERPOLATION_FLAT,
        INTERPOLATION_HERMITE,
        INTERPOLATION_SMOOTH,
        INTERPOLATION_STEP,
        INTERPOLATION_QUADRATIC_IN,
        INTERPOLATION_QUADRATIC_OUT,
        INTERPOLATION_QUADRATIC_IN_OUT,
        INTERPOLATION_QUADRATIC_OUT_IN,
        INTERPOLATION_CUBIC_IN,
        INTERPOLATION_CUBIC_OUT,
        INTERPOLATION_CUBIC_IN_OUT,
        INTERPOLATION_CUBIC_OUT_IN,
        INTERPOLATION_QUARTIC_IN,
        INTERPOLATION_QUARTIC_OUT,
        INTERPOLATION_QUARTIC_IN_OUT,
        INTERPOLATION_QUARTIC_OUT_IN,
        INTERPOLATION_QUINTIC_IN,
        INTERPOLATION_QUINTIC_OUT,
        INTERPOLATION_QUINTIC_IN_OUT,
        INTERPOLATION_QUINTIC_OUT_IN,
        INTERPOLATION_SINE_IN,
        INTERPOLATION_SINE_OUT,
        INTERPOLATION_SINE_IN_OUT,
        INTERPOLATION_SINE_OUT_IN,
        INTERPOLATION_EXPONENTIAL_IN,
        INTERPOLATION_EXPONENTIAL_OUT,
        INTERPOLATION_EXPONENTIAL_IN_OUT,
        INTERPOLATION_EXPONENTIAL_OUT_IN,
        INTERPOLATION_CIRCULAR_IN,
        INTERPOLATION_CIRCULAR_OUT,
        INTERPOLATION_CIRCULAR_IN_OUT,
        INTERPOLATION_CIRCULAR_OUT_IN,
        INTERPOLATION_ELASTIC_IN,
        INTERPOLATION_ELASTIC_OUT,
        INTERPOLATION_ELASTIC_IN_OUT,
        INTERPOLATION_ELASTIC_OUT_IN,
        INTERPOLATION_OVERSHOOT_IN,
        INTERPOLATION_OVERSHOOT_OUT,
        INTERPOLATION_OVERSHOOT_IN_OUT,
        INTERPOLATION_OVERSHOOT_OUT_IN,
        INTERPOLATION_BOUNCE_IN,
        INTERPOLATION_BOUNCE_OUT,
        INTERPOLATION_BOUNCE_IN_OUT,
        INTERPOLATION_BOUNCE_OUT_IN
    };

    /**
     * Creates a new curve.
     *
     * @param pointCount The number of points in the curve.
     * @param componentCount The number of float component values per key value.
     */
    static Curve* create(size_t pointCount, size_t componentCount);

    /**
     * Gets the number of points in the curve.
     *
     * @return The number of points in the curve.
     */
	size_t getPointCount() const;

    /**
     * Gets the number of float component values per points.
     *
     * @return The number of float component values per point.
     */
	size_t getComponentCount() const;

    /**
     * Gets the start time for the curve.
     *
     * @return The curve's start time.
     */
    float getStartTime() const;

    /**
     * Gets the end time for the curve.
     *
     * @return The curve's end time.
     */
    float getEndTime() const;

    /**
     * Sets the given point values on the curve.
     *
     * @param index The index of the point.
     * @param time The time for the key.
     * @param value The point to add.
     * @param interpolation The  interpolation.
     */
    void setPoint(size_t index, float time, float* value, Interpolation interpolation);

    /**
     * Sets the given point on the curve for the specified index and the specified parameters.
     *
     * @param index The index of the point.
     * @param time The time of the point within the curve.
     * @param value The value of the point to copy the data from.
     * @param interpolation The interpolation.
     * @param inValue The tangent approaching the point.
     * @param outValue The tangent leaving the point.
     */
    void setPoint(size_t index, float time, float* value, Interpolation interpolation,
                  float* inValue, float* outValue);

    /**
     * Sets the tangents for a point on the curve specified by the index.
     *
     * @param index The index of the point.
     * @param interpolation The interpolation.
     * @param inValue The tangent approaching the point.
     * @param outValue The tangent leaving the point.
     */
    void setTangent(size_t index, Interpolation interpolation, float* inValue, float* outValue);
    
    /**
     * Gets the time at a specified point.
     *
     * @param index The index of the point.
     * @return The time for a key point.
     */
    float getPointTime(size_t index) const;
    
    /**
     * Gets the interpolation at the specified point
     *
     * @param index The index of the point.
     * @return The interpolation type at the specified index.
     */
    Interpolation getPointInterpolation(size_t index) const;
    
    /**
     * Gets the values and in/out tangent value at a spedified point.
     *
     * @param index The index of the point.
     * @param value The value at the specified index. Ignored if nullptr.
     * @param inValue The tangent inValue at the specified index. Ignored if nullptr.
     * @param outValue The tangent outValue at the specified index. Ignored if nullptr.
     */
    void getPointValues(size_t index, float* value, float* inValue, float* outValue) const;

    /**
     * Evaluates the curve at the given position value.
     *
     * Time should generally be specified as a value between 0.0 - 1.0, inclusive.
     * A value outside this range can also be specified to perform an interpolation
     * between the two end points of the curve. This can be useful for smoothly
     * interpolating a repeat of the curve.
     *
     * @param time The position to evaluate the curve at.
     * @param dst The evaluated value of the curve at the given time.
     */
    void evaluate(float time, float* dst) const;

    /**
     * Evaluates the curve at the given position value (between 0.0 and 1.0 inclusive)
     * within the specified subregion of the curve.
     *
     * This method is useful for evaluating sub sections of the curve. A common use for
     * this is when evaluating individual animation clips that are positioned within a
     * larger animation curve. This method also allows looping to occur between the
     * end points of curve sub regions, with optional blending/interpolation between
     * the end points (using the loopBlendTime parameter).
     *
     * Time should generally be specified as a value between 0.0 - 1.0, inclusive.
     * A value outside this range can also be specified to perform an interpolation
     * between the two end points of the curve. This can be useful for smoothly
     * interpolating a repeat of the curve.
     *
     * @param time The position within the subregion of the curve to evaluate the curve at.
     *      A time of zero represents the start of the subregion, with a time of one
     *      representing the end of the subregion.
     * @param startTime Start time for the subregion (between 0.0 - 1.0).
     * @param endTime End time for the subregion (between 0.0 - 1.0).
     * @param loopBlendTime Time (in milliseconds) to blend between the end points of the curve
     *      for looping purposes when time is outside the range 0-1. A value of zero here
     *      disables curve looping.
     * @param dst The evaluated value of the curve at the given time.
     */
    void evaluate(float time, float startTime, float endTime, float loopBlendTime, float* dst) const;

private:

    class Point
    {
    public:

        /** 
		 * The time of the point within the curve.
		 */
        float time;
        /** 
		 * The value of the point.
		 */
        float* value;
        /** 
		 * The value of the tangent when approaching this point (from the previous point in the curve).
		 */
        float* inValue;
        /** 
		 * The value of the tangent when leaving this point (towards the next point in the curve).
		 */
        float* outValue;
        /** 
		 * The interpolation to use between this point and the next point.
		 */
        Interpolation interpolation;

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
     *
     * @param pointCount The number of points in the curve.
     * @param componentCount The number of float component values per key value.
     */
    Curve(size_t pointCount, size_t componentCount);

    /**
     * Constructor.
     */
    Curve(const Curve& copy);

    /**
     * Destructor.
     */
    ~Curve();

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
    void interpolateHermiteSmooth(float s, size_t index, Point* from, Point* to, float* dst) const;

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
    int determineIndex(float time, uint32_t min, uint32_t max) const;

    /**
     * Sets the offset for the beginning of a Quaternion piece of data within the curve's value span at the specified
     * index. The next four components of data starting at the given index will be interpolated as a Quaternion.
     * This function will assert an error if the given index is greater than the component size subtracted by the four components required
     * to store a quaternion.
     *
     * @param index The index of the Quaternion rotation data.
     */
    void setQuaternionOffset(size_t index);

    /**
     * Gets the InterpolationType value for the given string ID
     *
     * @param interpolationStr The string representation of the InterpolationType
     * @return the InterpolationType value; -1 if the string does not represent an InterpolationType.
     */
    static Interpolation getInterpolation(const std::string& interpolationStr);

    size_t _pointCount;
	size_t _componentCount;
	size_t _componentSize;
	size_t* _quaternionOffset;
    Point* _points;
};

}
