
#pragma once

namespace gameplay
{

/**
 * Defines a heightfield used to store height data representing elevation.
 *
 * Heightfields can be used to construct both terrains as well as heightfield collision shapes.
 * Heightfields can be populated manually, or loaded from images and RAW files.
 */
class Heightfield
{
public:

    /**
     * Constructor.
     */
    Heightfield();
    
    /**
     * Destructor.
     */
    ~Heightfield();

    /**
     * Creates a new Heightfield from the given dimensions, with uninitialized height data.
     *
     * @param columns Number of columns in the height field.
     * @param rows Number of rows in the height field.
     *
     * @return The new Heightfield.
     */
    static std::shared_ptr<Heightfield> create(size_t columns, size_t rows);

    /**
     * Gets the number of columns in the heightfield.
     *
     * @return The column count.
     */
    size_t getColumnCount() const;

    /**
     * Gets the number of rows in the heightfield.
     *
     * @return The number of rows.
     */
    size_t getRowCount() const;

    /**
     * Gets the underlying height data.
     *
     * The data is packed in row major order, 
     * meaning that the data is aligned in rows,
     * from top left to bottom right.
     *
     * @return The underlying height data.
     */
    std::vector<float> getHeightData() const;

    /**
     * Gets the height at the specified row and column.
     *
     * The specified row and column are specified as floating point numbers so that values
     * between points can be specified. In this case, a height value is calculated that is
     * interpolated between neighboring height values.
     *
     * If the specified point lies outside the heightfield, it is clamped to the boundary
     * of the heightfield.
     *
     * @param column The column of the height value to query.
     * @param row The row of the height value to query.
     *
     * @return The height value.
     */
    float getHeight(float column, float row) const;

private:

    static float getNormalizedHeightPacked(float r, float g, float b);

    size_t _columns;
    size_t _rows;
    std::vector<float> _heightData;
};

}
