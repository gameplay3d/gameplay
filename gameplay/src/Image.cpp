#include "Base.h"
#include "Image.h"
#include "FileSystem.h"
#include "png.h"

namespace gameplay
{

Image::Image() 
    : _width(0), _height(0), _format(FORMAT_UNDEFINED), _stride(0)
{
}

Image::~Image()
{
}

static void readStream(png_structp png, png_bytep data, png_size_t length)
{
    // Callback for reading a png image using Stream
    Stream* stream = reinterpret_cast<Stream*>(png_get_io_ptr(png));
    if (stream == nullptr || stream->read(data, 1, length) != length)
    {
        png_error(png, "Error reading PNG.");
    }
}

std::shared_ptr<Image> Image::create(size_t width, size_t height, Image::Format format, unsigned char* pixelData)
{
    GP_ASSERT(width > 0 && height > 0);
    GP_ASSERT(format >= FORMAT_RGB && format <= FORMAT_RGBA);

    std::shared_ptr<Image> image = std::make_shared<Image>();
    image->_width = width;
    image->_height = height;
    image->_format = format;
    image->_stride = 0;
    switch(format)
    {
    case Image::FORMAT_RGB:
        image->_stride = 3;
        break;
    case Image::FORMAT_RGBA:
        image->_stride = 4;
        break;
    }
    size_t pixelDataSize = width * height * image->_stride;
    image->_pixelData.resize(pixelDataSize);

    if (pixelData)
        memcpy(image->_pixelData.data(), pixelData, pixelDataSize);

    return image;
}

std::shared_ptr<Image> Image::create(const std::string& url)
{
    // Open the file.
    std::unique_ptr<Stream> stream(FileSystem::open(url));
    if (stream.get() == nullptr || !stream->canRead())
    {
        GP_ERROR("Failed to open image file '%s'.", url.c_str());
        return nullptr;
    }

    // Verify PNG signature.
    unsigned char sig[8];
    if (stream->read(sig, 1, 8) != 8 || png_sig_cmp(sig, 0, 8) != 0)
    {
        GP_ERROR("Failed to load file '%s'; not a valid PNG.", url.c_str());
        return nullptr;
    }

    // Initialize png read struct (last three parameters use stderr+longjump if nullptr).
    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (png == nullptr)
    {
        GP_ERROR("Failed to create PNG structure for reading PNG file '%s'.", url.c_str());
        return nullptr;
    }

    // Initialize info struct.
    png_infop info = png_create_info_struct(png);
    if (info == nullptr)
    {
        GP_ERROR("Failed to create PNG info structure for PNG file '%s'.", url.c_str());
        png_destroy_read_struct(&png, nullptr, nullptr);
        return nullptr;
    }

    // Set up error handling (required without using custom error handlers above).
    if (setjmp(png_jmpbuf(png)))
    {
        GP_ERROR("Failed to set up error handling for reading PNG file '%s'.", url.c_str());
        png_destroy_read_struct(&png, &info, nullptr);
        return nullptr;
    }

    // Initialize file io.
    png_set_read_fn(png, stream.get(), readStream);

    // Indicate that we already read the first 8 bytes (signature).
    png_set_sig_bytes(png, 8);

    // Read the entire image into memory.
    png_read_png(png, info, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND | PNG_TRANSFORM_GRAY_TO_RGB, NULL);

    std::shared_ptr<Image> image = std::make_shared<Image>();
    image->_width = png_get_image_width(png, info);
    image->_height = png_get_image_height(png, info);

    png_byte colorType = png_get_color_type(png, info);
    switch (colorType)
    {
    case PNG_COLOR_TYPE_RGBA:
        image->_format = Image::FORMAT_RGBA;
        break;

    case PNG_COLOR_TYPE_RGB:
        image->_format = Image::FORMAT_RGB;
        break;

    default:
        GP_ERROR("Unsupported PNG color type (%d) for image file '%s'.", (int)colorType, url.c_str());
        png_destroy_read_struct(&png, &info, nullptr);
        return nullptr;
    }

    size_t stride = png_get_rowbytes(png, info);

    // Allocate image data.
    image->_pixelData.resize(stride * image->_height);

    // Read rows into image data.
    png_bytepp rows = png_get_rows(png, info);
    for (size_t i = 0; i < image->_height; ++i)
    {
        unsigned char* dst = image->_pixelData.data();
        memcpy((void*)dst[stride * (image->_height-1-i)], rows[i], stride);
    }

    // Clean up.
    png_destroy_read_struct(&png, &info, nullptr);

    return image;
}

size_t Image::getHeight() const
{
    return _height;
}
        
size_t Image::getWidth() const
{
    return _width;
}

Image::Format Image::getFormat() const
{
    return _format;
}

size_t Image::getStride() const
{
    return _stride;
}

std::vector<unsigned char> Image::getPixelData() const
{
    return _pixelData;
}

}
