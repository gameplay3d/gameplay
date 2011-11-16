/** 
 * BoundingBox.inl
 */

#include "BoundingBox.h"

namespace gameplay
{

inline BoundingBox& BoundingBox::operator*=(const Matrix& matrix)
{
    transform(matrix);
    return *this;
}

inline BoundingBox operator*(const Matrix& matrix, const BoundingBox& box)
{
    BoundingBox b(box);
    b.transform(matrix);
    return b;
}

}
