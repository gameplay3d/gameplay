#ifndef MATHUTIL_H_
#define MATHUTIL_H_

namespace gameplay
{
/**
 * Math utility class. Used for internal math optimizations.
 */
class MathUtil
{
    friend class Matrix;
    friend class Vector3;

private:

    inline static void addMatrix(const float* m, float scalar, float* dst);

    inline static void addMatrix(const float* m1, const float* m2, float* dst);

    inline static void subtractMatrix(const float* m1, const float* m2, float* dst);

    inline static void multiplyMatrix(const float* m, float scalar, float* dst);

    inline static void multiplyMatrix(const float* m1, const float* m2, float* dst);

    inline static void negateMatrix(const float* m, float* dst);

    inline static void transposeMatrix(const float* m, float* dst);

    inline static void transformVector4(const float* m, float x, float y, float z, float w, float* dst);

    inline static void transformVector4(const float* m, const float* v, float* dst);

    inline static void crossVector3(const float* v1, const float* v2, float* dst);

    MathUtil();
};

}

#define MATRIX_SIZE ( sizeof(float) * 16)

#ifdef USE_NEON
#include "MathUtilNeon.inl"
#else
#include "MathUtil.inl"
#endif

#endif
