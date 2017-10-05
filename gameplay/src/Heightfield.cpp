#include "Base.h"
#include "Heightfield.h"
#include "Image.h"
#include "FileSystem.h"

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

std::shared_ptr<Heightfield> Heightfield::createFromPNG(const std::string& path, float heightMin, float heightMax)
{
    return create(path, 0, 0, heightMin, heightMax);
}

std::shared_ptr<Heightfield> Heightfield::createFromRAW(const std::string& path, size_t width, size_t height, float heightMin, float heightMax)
{
    return create(path, width, height, heightMin, heightMax);
}

std::shared_ptr<Heightfield> Heightfield::create(const std::string& path, size_t width, size_t height, float heightMin, float heightMax)
{
    GP_ASSERT(heightMax >= heightMin);

    float heightScale = heightMax - heightMin;

    std::shared_ptr<Heightfield> heightfield = nullptr;

    // Load height data from image
    std::string ext = FileSystem::getExtension(path);
    if (ext == ".PNG")
    {
        //  Image file
        std::shared_ptr<Image> image = Image::create(path);
        if (!image)
            return nullptr;

        size_t stride = 0;
        switch (image->getFormat())
        {
            case Image::FORMAT_RGB:
                stride = 3;
                break;
            case Image::FORMAT_RGBA:
                stride = 4;
                break;
            default:
                GP_WARN("Unsupported pixel format for heightfield image: %s.", path.c_str());
                return nullptr;
        }

        // Calculate the heights for each pixel.
        heightfield = std::make_shared<Heightfield>();
        std::vector<float> heights = heightfield->getHeightData();
        unsigned char* data = image->getPixelData().data();
        size_t idx;
        for (int y = (int)image->getHeight() - 1, i = 0; y >= 0; --y)
        {
            for (size_t x = 0, w = image->getWidth(); x < w; ++x)
            {
                idx = (y*w + x) * stride;
                heights[i++] = heightMin + getNormalizedHeightPacked(data[idx], data[idx + 1], data[idx + 2]) * heightScale;
            }
        }
    }
    else if (ext == ".RAW" || ext == ".R16")
    {
        // RAW image (headerless)
        if (width < 2 || height < 2 || heightMax < 0)
        {
            GP_WARN("Invalid 'width', 'height' or 'heightMax' parameter for RAW heightfield image: %s.", path.c_str());
            return nullptr;
        }

        // Load raw bytes
        int fileSize = 0;
        unsigned char* bytes = (unsigned char*)FileSystem::readAll(path).c_str();
        if (bytes == nullptr)
        {
            GP_WARN("Falied to read bytes from RAW heightfield image: %s.", path.c_str());
            return nullptr;
        }

        // Determine if the RAW file is 8-bit or 16-bit based on file size.
        int bits = (fileSize / (width * height)) * 8;
        if (bits != 8 && bits != 16)
        {
            GP_WARN("Invalid RAW file - must be 8-bit or 16-bit, but found neither: %s.", path.c_str());
            GP_SAFE_DELETE_ARRAY(bytes);
            return nullptr;
        }

        heightfield = Heightfield::create(width, height);
        std::vector<float> heights = heightfield->getHeightData();

        if (bits == 16)
        {
            // 16-bit (0-65535)
            unsigned int idx;
            for (unsigned int y = 0, i = 0; y < height; ++y)
            {
                for (unsigned int x = 0; x < width; ++x, ++i)
                {
                    idx = ((y * (int)width + x) << 1);
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
        GP_SAFE_DELETE_ARRAY(bytes);
    }
    else
    {
        GP_WARN("Unsupported heightfield image format: %s.", path.c_str());
    }

    return heightfield;
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

    unsigned int x1 = column;
    unsigned int y1 = row;
    unsigned int x2 = x1 + 1;
    unsigned int y2 = y1 + 1;
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
