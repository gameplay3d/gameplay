#include "Base.h"
#include "FileSystem.h"
#include "Image.h"

namespace gameplay
{

// Callback for png library errors
static void pngError(png_structp png, png_const_charp data)
{
    GP_ERROR("%s", data);
}

// Callback for png library warnings
static void pngWarn(png_structp png, png_const_charp data)
{
    GP_WARN("%s", data);
}

// Callback for reading a png image using Stream
static void readStream(png_structp png, png_bytep data, png_size_t length)
{
    Stream* stream = reinterpret_cast<Stream*>(png_get_io_ptr(png));
    if (stream == NULL || stream->read(data, 1, length) != length)
    {
        png_error(png, "Error reading PNG.");
    }
}

static void writeStream(png_structp png, png_bytep data, png_size_t length)
{
    Stream* stream = reinterpret_cast<Stream*>(png_get_io_ptr(png));
    if (stream == NULL || stream->write(data, 1, length) != length)
    {
        png_error(png, "Error writing PNG.");
    }
}

static void flushStream(png_structp png)
{
    Stream* stream = reinterpret_cast<Stream*>(png_get_io_ptr(png));
    if (stream == NULL)
    {
        png_error(png, "Error flushing PNG.");
    }
    else
    {
        stream->flush();
    }
}

Image* Image::create(const char* path)
{
    GP_ASSERT(path);

    std::unique_ptr<Stream> stream(FileSystem::open(path));

    if (stream.get() == NULL || !stream->canRead())
    {
        GP_ERROR("Failed to open image %s.", stream->desc().c_str());
        return NULL;
    }

    return create(stream.get());
}

Image* Image::create(Stream* stream)
{
   // Verify PNG signature.
    unsigned char sig[8];

    if (stream->read(sig, 1, 8) != 8 || png_sig_cmp(sig, 0, 8) != 0)
    {
        GP_ERROR("Failed to load '%s'; not a valid PNG.", stream->desc().c_str());
        return NULL;
    }

    // Initialize png read struct (last three parameters use stderr+longjump if NULL).
    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, pngError, pngWarn);
    if (png == NULL)
    {
        GP_ERROR("Failed to create PNG structure for reading PNG '%s'.", stream->desc().c_str());
        return NULL;
    }

    // Initialize info struct.
    png_infop info = png_create_info_struct(png);
    if (info == NULL)
    {
        GP_ERROR("Failed to create PNG info structure for PNG '%s'.", stream->desc().c_str());
        png_destroy_read_struct(&png, NULL, NULL);
        return NULL;
    }

    // Set up error handling (required without using custom error handlers above).
    if (setjmp(png_jmpbuf(png)))
    {
        GP_ERROR("Failed to set up error handling for reading PNG '%s'.", stream->desc().c_str());
        png_destroy_read_struct(&png, &info, NULL);
        return NULL;
    }

    // Initialize file io.
    png_set_read_fn(png, stream, readStream);

    // Indicate that we already read the first 8 bytes (signature).
    png_set_sig_bytes(png, 8);

    // Read the entire image into memory.
    png_read_png(png, info, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND | PNG_TRANSFORM_GRAY_TO_RGB, NULL);

    Image* image = new Image();
    image->_width = png_get_image_width(png, info);
    image->_height = png_get_image_height(png, info);

    png_byte colorType = png_get_color_type(png, info);
    switch (colorType)
    {
    case PNG_COLOR_TYPE_RGBA:
        image->_format = Image::RGBA;
        break;

    case PNG_COLOR_TYPE_RGB:
        image->_format = Image::RGB;
        break;

    default:
        GP_ERROR("Unsupported PNG color type (%d) for image '%s'.", (int)colorType, stream->desc().c_str());
        png_destroy_read_struct(&png, &info, NULL);
        return NULL;
    }

    size_t stride = png_get_rowbytes(png, info);

    // Allocate image data.
    image->_data = new unsigned char[stride * image->_height];

    // Read rows into image data.
    png_bytepp rows = png_get_rows(png, info);
    for (unsigned int i = 0; i < image->_height; ++i)
    {
        memcpy(image->_data+(stride * (image->_height-1-i)), rows[i], stride);
    }

    // Clean up.
    png_destroy_read_struct(&png, &info, NULL);

    return image;
}

Image* Image::create(unsigned int width, unsigned int height, Image::Format format, unsigned char* data)
{
    GP_ASSERT(width > 0 && height > 0);
    GP_ASSERT(format >= RGB && format <= RGBA);

    unsigned int pixelSize = 0;
    switch(format)
    {
    case Image::RGB:
        pixelSize = 3;
        break;
    case Image::RGBA:
        pixelSize = 4;
        break;
    }

    Image* image = new Image();

    unsigned int dataSize = width * height * pixelSize;

    image->_width = width;
    image->_height = height;
    image->_format = format;
    image->_data = new unsigned char[dataSize];
    if (data)
        memcpy(image->_data, data, dataSize);

    return image;
}

bool Image::write(const char *path)
{
    GP_ASSERT(path);

    std::unique_ptr<Stream> stream(FileSystem::open(path, FileSystem::WRITE));
    if (stream.get() == NULL || !stream->canWrite())
    {
        GP_ERROR("Failed to open image '%s'.", path);
        return NULL;
    }

    return write(stream.get());
}

bool Image::write(Stream* stream)
{
    // Initialize png write struct (last three parameters use stderr+longjump if NULL).
    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, pngError, pngWarn);
    if (png == NULL)
    {
        GP_ERROR("Failed to create PNG structure for writing PNG '%s'.", stream->desc().c_str());
        return NULL;
    }

    // Initialize info struct.
    png_infop info = png_create_info_struct(png);
    if (info == NULL)
    {
        GP_ERROR("Failed to create PNG info structure for PNG '%s'.", stream->desc().c_str());
        png_destroy_write_struct(&png, NULL);
        return NULL;
    }

    // Initialize file io.
    png_set_write_fn(png, stream, writeStream, flushStream);

    int type;
    size_t stride;

    switch(_format)
    {
    case RGB:
        type = PNG_COLOR_TYPE_RGB;
        stride = 3 * _width;
        break;
    case RGBA:
        type = PNG_COLOR_TYPE_RGBA;
        stride = 4 * _width;
        break;
    }

    png_set_IHDR(
        png,
        info,
        _width, _height,
        8,
        type,
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT,
        PNG_FILTER_TYPE_DEFAULT
      );
    png_write_info(png, info);

    // To remove the alpha channel for PNG_COLOR_TYPE_RGB format,
    // Use png_set_filler().
    //png_set_filler(png, 0, PNG_FILLER_AFTER);

    png_bytepp rows = new png_bytep[_height];
    for(int i=0; i<_height;i++)
    {
        rows[i] = _data + (stride * (_height - 1 - i));
    }

    png_write_image(png, rows);
    png_write_end(png, NULL);
    delete rows;
}

Image::Image() : _data(NULL), _format(RGB), _width(0), _height(0)
{
}

Image::~Image()
{
    SAFE_DELETE_ARRAY(_data);
}

}
