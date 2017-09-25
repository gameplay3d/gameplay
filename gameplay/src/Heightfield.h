
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
     * Creates a Heightfield from the specified heightfield png image.
     *
     * The specified image path must refer to a valid heightfield png image. 
     *
     * The minHeight and maxHeight parameters provides a mapping from heightfield pixel
     * intensity to height values. The minHeight parameter is mapped to zero intensity
     * pixel, while maxHeight maxHeight is mapped to full intensity pixels.
     *
     * @param path Path to a heightfield image.
     * @param heightMin Minimum height value for a zero intensity pixel.
     * @param heightMax Maximum height value for a full intensity heightfield pixel (must be >= minHeight).
     *
     * @return The new Heightfield.
     */
    static std::shared_ptr<Heightfield> createFromPNG(const std::string& path, float heightMin = 0, float heightMax = 1);

    /**
     * Creates a Heightfield from the specified RAW8 or RAW16 file.
     *
     * RAW files are header-less files containing intensity values, either in 8-bit (RAW8)
     * or 16-bit (RAW16) format. RAW16 files must have little endian (PC) byte ordering. Since
     * RAW files have no header, you must specify the dimensions of the data in the file.
     * This method automatically determines (based on file size) whether the input file
     * is RAW8 or RAW16. RAW files must have a .raw or .r16 file extension.
     *
     * RAW files are commonly used in software that produces heightmap images. Using RAW16 is
     * preferred or any 8-bit heightfield source since it allows greater precision, resulting in
     * smoother height transitions.
     *
     * The minHeight and maxHeight parameters provides a mapping from heightfield pixel
     * intensity to height values. The minHeight parameter is mapped to zero intensity
     * pixel, while maxHeight maxHeight is mapped to full intensity pixels.
     *
     * @param path Path to the RAW file (must end in a .raw or .r16 file extension).
     * @param width Width of the RAW data.
     * @param height Height of the RAW data.
     * @param heightMin Minimum height value for a zero intensity pixel.
     * @param heightMax Maximum height value for a full intensity heightfield pixel (must be >= minHeight).
     *
     * @return The new Heightfield.
     */
    static std::shared_ptr<Heightfield> createFromRAW(const std::string& path, size_t width, size_t height, float heightMin = 0, float heightMax = 1);

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

    static std::shared_ptr<Heightfield> create(const std::string& path, size_t width, size_t height, float heightMin, float heightMax);
    static float getNormalizedHeightPacked(float r, float g, float b);

    size_t _columns;
    size_t _rows;
    std::vector<float> _heightData;
};

}
