#ifndef MATHUTIL_H_
#define MATHUTIL_H_

namespace gameplay
{
class MathUtil
{
	friend class Matrix;
	friend class Vector3;

private:

	/** Matrix **/
	inline static void addMatrix(const float* m, float scalar, float* dst);
	inline static void addMatrix(const float* m1, const float* m2, float* dst);
	inline static void multiplyMatrix(const float* m, float scalar, float* dst);
	inline static void multiplyMatrix(const float* m1, const float* m2, float* dst);
	inline static void negateMatrix(const float* m, float* dst);
	inline static void subtractMatrix(const float* m1, const float* m2, float* dst);
	inline static void transformVectorMatrix(const float* m, float x, float y, float z, float w, float* dst);
	inline static void transformVectorMatrix(const float* m, const float* v, float* dst);
	inline static void transpose(const float* m, float* dst);

	/** Vector3 **/
	inline static void crossVector3(const float* v1, const float* v2, float* dst);

	MathUtil();
};
}

#ifdef USE_NEON
#include "MathUtilNeon.inl"
#else
#include "MathUtil.inl"
#endif

#endif /* MATHUTIL_H_ */
