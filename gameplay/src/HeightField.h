#ifndef HEIGHTFIELD_H_
#define HEIGHTFIELD_H_

#include "Ref.h"

namespace gameplay
{

    /**
     * Defines height data used to store values representing elevation.
     *
     * Heightfields can be used to construct both Terrain objects as well as PhysicsCollisionShape
     * heightfield defintions, which are used in heightfield rigid body creation. Heightfields can
     * be populated manually, or loaded from images and RAW files.
     */
    class HeightField : public Ref
    {
    public:

        /**
         * Creates a new HeightField of the given dimensions, with uninitialized height data.
         *
         * @param rows Number of rows in the height field.
         * @param columns Number of columns in the height field.
         *
         * @return The new HeightField.
         */
        static HeightField* create(unsigned int rows, unsigned int columns);

        /**
         * Creates a HeightField from the specified heightfield image.
         *
         * The specified image path must refer to a valid heightfield image. Supported images are
         * the same as those supported by the Image class (i.e. PNG).
         *
         * The minHeight and maxHeight parameters provides a mapping from heightfield pixel
         * intensity to height values. The minHeight parameter is mapped to zero intensity
         * pixel, while maxHeight maxHeight is mapped to full intensity pixels.
         *
         * @param path Path to a heightfield image.
         * @param heightMin Minimum height value for a zero intensity pixel.
         * @param heightMax Maximum height value for a full intensity heightfield pixel (must be >= minHeight).
         *
         * @return The new HeightField.
         */
        static HeightField* createFromImage(const char* path, float heightMin = 0, float heightMax = 1);

        /**
         * Creates a HeightField from the specified RAW8 or RAW16 file.
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
         * @return The new HeightField.
         */
        static HeightField* createFromRAW(const char* path, unsigned int width, unsigned int height, float heightMin = 0, float heightMax = 1);

        /**
         * Returns a pointer to the underlying height array.
         *
         * The array is packed in row major order, meaning that the data is aligned in rows,
         * from top left to bottom right.
         *
         * @return The underlying height array.
         */
        float* getArray() const;

        /**
         * Returns the height at the specified row and column.
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

        /**
         * Returns the number of rows in the heightfield.
         *
         * @return The number of rows.
         */
        unsigned int getRowCount() const;

        /**
         * Returns the number of columns in the heightfield.
         *
         * @return The column count.
         */
        unsigned int getColumnCount() const;

    private:

        /**
         * Hidden constructor.
         */
        HeightField(unsigned int columns, unsigned int rows);

        /**
         * Hidden destructor (use Ref::release()).
         */
        ~HeightField();

        /**
         * Internal method for creating a HeightField.
         */
        static HeightField* create(const char* path, unsigned int width, unsigned int height, float heightMin, float heightMax);

        float* _array;
        unsigned int _cols;
        unsigned int _rows;
    };

}

#endif
