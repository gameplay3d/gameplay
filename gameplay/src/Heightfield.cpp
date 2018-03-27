#include "Base.h"
#include "Heightfield.h"

namespace gameplay
{

Heightfield::Heightfield()
    : _columns(0), _rows(0)
{
}

Heightfield::~Heightfield()
{
}

std::shared_ptr<Heightfield> Heightfield::create(size_t columns, size_t rows)
{
    std::shared_ptr<Heightfield> heightfield = std::make_shared<Heightfield>();
    heightfield->_columns = columns;
    heightfield->_rows = rows;
    return  heightfield;
}

size_t Heightfield::getColumnCount() const
{
    return _columns;
}

size_t Heightfield::getRowCount() const
{
    return _rows;
}

std::vector<float> Heightfield::getHeightData() const
{
    return _heightData;
}

float Heightfield::getHeight(float column, float row) const
{
    // Clamp to heightfield boundaries
    column = column < 0 ? 0 : (column > (_columns - 1) ? (_columns - 1) : column);
    row = row < 0 ? 0 : (row > (_rows - 1) ? (_rows - 1) : row);

    uint32_t x1 = column;
    uint32_t y1 = row;
    uint32_t x2 = x1 + 1;
    uint32_t y2 = y1 + 1;
    float tmp;
    float xFactor = std::modf(column, &tmp);
    float yFactor = std::modf(row, &tmp);
    float xFactorI = 1.0f - xFactor;
    float yFactorI = 1.0f - yFactor;

    if (x2 >= _columns && y2 >= _rows)
    {
        return _heightData[x1 + y1 * _columns];
    }
    else if (x2 >= _columns)
    {
        return _heightData[x1 + y1 * _columns] * yFactorI + _heightData[x1 + y2 * _columns] * yFactor;
    }
    else if (y2 >= _rows)
    {
        return _heightData[x1 + y1 * _columns] * xFactorI + _heightData[x2 + y1 * _columns] * xFactor;
    }
    else
    {
        float a = xFactorI * yFactorI;
        float b = xFactorI * yFactor;
        float c = xFactor * yFactor;
        float d = xFactor * yFactorI;

        return _heightData[x1 + y1 * _columns] * a + 
               _heightData[x1 + y2 * _columns] * b +
               _heightData[x2 + y2 * _columns] * c + 
               _heightData[x2 + y1 * _columns] * d;
    }
}

float Heightfield::getNormalizedHeightPacked(float r, float g, float b)
{
    // This formula is intended for 24-bit packed heightmap images.
    // It is also compatible with normal grayscale heightmap images, with an error of approximately 0.4%. 
    // This can be seen by setting r=g=b=x and comparing the grayscale height expression to the packed
    // height expression: the error is 2^-8 + 2^-16 which is just under 0.4%.
    return (256.0f*r + g + 0.00390625f*b) / 65536.0f;
}

}
