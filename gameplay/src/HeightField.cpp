#include "Base.h"
#include "HeightField.h"
#include "Image.h"
#include "FileSystem.h"

namespace gameplay
{

HeightField::HeightField(unsigned int columns, unsigned int rows)
    : _array(NULL), _cols(columns), _rows(rows)
{
    _array = new float[columns * rows];
}

HeightField::~HeightField()
{
    SAFE_DELETE_ARRAY(_array);
}

HeightField* HeightField::create(unsigned int columns, unsigned int rows)
{
    return new HeightField(columns, rows);
}

/**
 * @script{ignore}
 */
float normalizedHeightPacked(float r, float g, float b)
{
    // This formula is intended for 24-bit packed heightmap images (that are generated
    // with gameplay-encoder. However, it is also compatible with normal grayscale 
    // heightmap images, with an error of approximately 0.4%. This can be seen by
    // setting r=g=b=x and comparing the grayscale height expression to the packed
    // height expression: the error is 2^-8 + 2^-16 which is just under 0.4%.
    return (256.0f*r + g + 0.00390625f*b) / 65536.0f;
}

HeightField* HeightField::createFromImage(const char* path, float heightMin, float heightMax)
{
    return create(path, 0, 0, heightMin, heightMax);
}

HeightField* HeightField::createFromRAW(const char* path, unsigned int width, unsigned int height, float heightMin, float heightMax)
{
    return create(path, width, height, heightMin, heightMax);
}

HeightField* HeightField::create(const char* path, unsigned int width, unsigned int height, float heightMin, float heightMax)
{
    GP_ASSERT(path);
    GP_ASSERT(heightMax >= heightMin);

    float heightScale = heightMax - heightMin;

    HeightField* heightfield = NULL;

    // Load height data from image
    std::string ext = FileSystem::getExtension(path);
    if (ext == ".PNG")
    {
        // Normal image
        Image* image = Image::create(path);
        if (!image)
            return NULL;

        unsigned int pixelSize = 0;
        switch (image->getFormat())
        {
            case Image::RGB:
                pixelSize = 3;
                break;
            case Image::RGBA:
                pixelSize = 4;
                break;
            default:
                SAFE_RELEASE(image);
                GP_WARN("Unsupported pixel format for heightfield image: %s.", path);
                return NULL;
        }

        // Calculate the heights for each pixel.
        heightfield = HeightField::create(image->getWidth(), image->getHeight());
        float* heights = heightfield->getArray();
        unsigned char* data = image->getData();
        int idx;
        for (int y = image->getHeight()-1, i = 0; y >= 0; --y)
        {
            for (unsigned int x = 0, w = image->getWidth(); x < w; ++x)
            {
                idx = (y*w + x) * pixelSize;
                heights[i++] = heightMin + normalizedHeightPacked(data[idx], data[idx + 1], data[idx + 2]) * heightScale;
            }
        }

        SAFE_RELEASE(image);
    }
    else if (ext == ".RAW" || ext == ".R16")
    {
        // RAW image (headerless)
        if (width < 2 || height < 2 || heightMax < 0)
        {
            GP_WARN("Invalid 'width', 'height' or 'heightMax' parameter for RAW heightfield image: %s.", path);
            return NULL;
        }

        // Load raw bytes
        int fileSize = 0;
        unsigned char* bytes = (unsigned char*)FileSystem::readAll(path, &fileSize);
        if (bytes == NULL)
        {
            GP_WARN("Falied to read bytes from RAW heightfield image: %s.", path);
            return NULL;
        }

        // Determine if the RAW file is 8-bit or 16-bit based on file size.
        int bits = (fileSize / (width * height)) * 8;
        if (bits != 8 && bits != 16)
        {
            GP_WARN("Invalid RAW file - must be 8-bit or 16-bit, but found neither: %s.", path);
            SAFE_DELETE_ARRAY(bytes);
            return NULL;
        }

        heightfield = HeightField::create(width, height);
        float* heights = heightfield->getArray();

        if (bits == 16)
        {
            // 16-bit (0-65535)
            int idx;
            for (unsigned int y = 0, i = 0; y < height; ++y)
            {
                for (unsigned int x = 0; x < width; ++x, ++i)
                {
                    idx = (y * width + x) << 1;
                    heights[i] = heightMin + ((bytes[idx] | (int)bytes[idx+1] << 8) / 65535.0f) * heightScale;
                }
            }
        }
        else
        {
            // 8-bit (0-255)
            for (unsigned int y = 0, i = 0; y < height; ++y)
            {
                for (unsigned int x = 0; x < width; ++x, ++i)
                {
                    heights[i] = heightMin + (bytes[y * width + x] / 255.0f) * heightScale;
                }
            }
        }

        SAFE_DELETE_ARRAY(bytes);
    }
    else
    {
        GP_WARN("Unsupported heightfield image format: %s.", path);
    }

    return heightfield;
}

float* HeightField::getArray() const
{
    return _array;
}

float HeightField::getHeight(float column, float row) const
{
    // Clamp to heightfield boundaries
    column = column < 0 ? 0 : (column > (_cols-1) ? (_cols-1) : column);
    row = row < 0 ? 0 : (row > (_rows-1) ? (_rows-1) : row);

    unsigned int x1 = column;
    unsigned int y1 = row;
    unsigned int x2 = x1 + 1;
    unsigned int y2 = y1 + 1;
    float tmp;
    float xFactor = modf(column, &tmp);
    float yFactor = modf(row, &tmp);
    float xFactorI = 1.0f - xFactor;
    float yFactorI = 1.0f - yFactor;

    if (x2 >= _cols && y2 >= _rows)
    {
        return _array[x1 + y1 * _cols];
    }
    else if (x2 >= _cols)
    {
        return _array[x1 + y1 * _cols] * yFactorI + _array[x1 + y2 * _cols] * yFactor;
    }
    else if (y2 >= _rows)
    {
        return _array[x1 + y1 * _cols] * xFactorI + _array[x2 + y1 * _cols] * xFactor;
    }
    else
    {
        float a = xFactorI * yFactorI;
        float b = xFactorI * yFactor;
        float c = xFactor * yFactor;
        float d = xFactor * yFactorI;
        return _array[x1 + y1 * _cols] * a + _array[x1 + y2 * _cols] * b +
            _array[x2 + y2 * _cols] * c + _array[x2 + y1 * _cols] * d;
    }
}

unsigned int HeightField::getColumnCount() const
{
    return _cols;
}

unsigned int HeightField::getRowCount() const
{
    return _rows;
}

}
