#pragma once

namespace gameplay
{

/**
 * Defines a math utility class.
 *
 * This is primarily used for optimized internal math operations.
 */
class MathUtil
{
    friend class Matrix;
    friend class Vector3;

public:

	/**
	 * Linear interpolation function.
	 */
	static float lerp(float s, float from, float to);

	/**
	 * Bezier interpolation function.
	 */
	static float bezier(float eq0, float eq1, float eq2, float eq3, float from, float out, float to, float in);

	/**
	 * Bspline interpolation function.
	 */
	static float bspline(float eq0, float eq1, float eq2, float eq3, float c0, float c1, float c2, float c3);

	/**
	 * Hermite interpolation function.
	 */
	static float hermite(float h00, float h01, float h10, float h11, float from, float out, float to, float in);

	/**
	 * Hermite flat interpolation function.
	 */
	static float hermiteFlat(float h00, float h01, float from, float to);

	/**
	 * Hermite smooth interpolation function.
	 */
	static float hermiteSmooth(float h00, float h01, float h10, float h11, float from, float out, float to, float in);

	/**
	* Updates the given scalar towards the given target using a smoothing function.
	* The given response time determines the amount of smoothing (lag). A longer
	* response time yields a smoother result and more lag. To force the scalar to
	* follow the target closely, provide a response time that is very small relative
	* to the given elapsed time.
	*
	* @param x the scalar to update.
	* @param target target value.
	* @param elapsedTime elapsed time between calls.
	* @param responseTime response time (in the same units as elapsedTime).
	*/
	static void smooth(float* x, float target, float elapsedTime, float responseTime);

	/**
	* Updates the given scalar towards the given target using a smoothing function.
	* The given rise and fall times determine the amount of smoothing (lag). Longer
	* rise and fall times yield a smoother result and more lag. To force the scalar to
	* follow the target closely, provide rise and fall times that are very small relative
	* to the given elapsed time.
	*
	* @param x the scalar to update.
	* @param target target value.
	* @param elapsedTime elapsed time between calls.
	* @param riseTime response time for rising slope (in the same units as elapsedTime).
	* @param fallTime response time for falling slope (in the same units as elapsedTime).
	*/
	static void smooth(float* x, float target, float elapsedTime, float riseTime, float fallTime);
private:

    MathUtil();
};

}

