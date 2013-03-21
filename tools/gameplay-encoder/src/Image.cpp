#include "Image.h"
#include "Base.h"

namespace gameplay
{

Image::Image() :
    _data(NULL), _format(RGBA), _width(0), _height(0), _bpp(0)
{
}

Image::~Image()
{
    delete[] _data;
}

Image* Image::create(const char* path)
{
    // Open the file.
    FILE* fp = fopen(path, "rb");
    if (fp == NULL)
    {
        LOG(1, "Failed to open image file '%s'.\n", path);
        return NULL;
    }

    // Verify PNG signature.
    unsigned char sig[8];
    if (fread(sig, 1, 8, fp) != 8 || png_sig_cmp(sig, 0, 8) != 0)
    {
        LOG(1, "Failed to load file '%s'; not a valid PNG.\n", path);
        if (fclose(fp) != 0)
        {
            LOG(1, "Failed to close image file '%s'.\n", path);
        }
        return NULL;
    }

    // Initialize png read struct (last three parameters use stderr+longjump if NULL).
    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png == NULL)
    {
        LOG(1, "Failed to create PNG structure for reading PNG file '%s'.\n", path);
        if (fclose(fp) != 0)
        {
            LOG(1, "Failed to close image file '%s'.\n", path);
        }
        return NULL;
    }

    // Initialize info struct.
    png_infop info = png_create_info_struct(png);
    if (info == NULL)
    {
        LOG(1, "Failed to create PNG info structure for PNG file '%s'.\n", path);
        if (fclose(fp) != 0)
        {
            LOG(1, "Failed to close image file '%s'.\n", path);
        }
        png_destroy_read_struct(&png, NULL, NULL);
        return NULL;
    }

    // Initialize file io.
    png_init_io(png, fp);

    // Indicate that we already read the first 8 bytes (signature).
    png_set_sig_bytes(png, 8);

    // Read the entire image into memory.
    png_read_png(png, info, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, NULL);

    Image* image = new Image();
    image->_width = png_get_image_width(png, info);
    image->_height = png_get_image_height(png, info);

    png_byte colorType = png_get_color_type(png, info);
    switch (colorType)
    {
    case PNG_COLOR_TYPE_GRAY:
        image->_bpp = 1;
        image->_format = Image::LUMINANCE;
        break;

    case PNG_COLOR_TYPE_RGBA:
        image->_bpp = 4;
        image->_format = Image::RGBA;
        break;

    case PNG_COLOR_TYPE_RGB:
        image->_bpp = 3;
        image->_format = Image::RGB;
        break;

    default:
        LOG(1, "Unsupported PNG color type (%d) for image file '%s'.\n", (int)colorType, path);
        if (fclose(fp) != 0)
        {
            LOG(1, "Failed to close image file '%s'.\n", path);
        }
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
        memcpy(image->_data+(stride * i), rows[i], stride);
    }

    // Clean up.
    png_destroy_read_struct(&png, &info, NULL);
    if (fclose(fp) != 0)
    {
        LOG(1, "Failed to close image file '%s'.\n", path);
    }

    return image;
}

Image* Image::create(Format format, unsigned int width, unsigned int height)
{
    unsigned int bpp;
    switch (format)
    {
    case LUMINANCE:
        bpp = 1;
        break;
    case RGB:
        bpp = 3;
        break;
    case RGBA:
        bpp = 4;
        break;
    default:
        LOG(1, "Invalid image format passed to create.\n");
        return NULL;
    }

    Image* image = new Image();
    image->_format = format;
    image->_width = width;
    image->_height = height;
    image->_bpp = bpp;
    image->_data = new unsigned char[width * height * bpp];
    memset(image->_data, 0, width * height * bpp);
    return image;
}

void* Image::getData() const
{
    return _data;
}

void Image::setData(void* data)
{
    memcpy(_data, data, _width * _height * _bpp);
}

Image::Format Image::getFormat() const
{
    return _format;
}

unsigned int Image::getHeight() const
{
    return _height;
}
        
unsigned int Image::getWidth() const
{
    return _width;
}

unsigned int Image::getBpp() const
{
    return _bpp;
}

int getPNGColorType(Image::Format format)
{
    switch (format)
    {
    case Image::LUMINANCE:
        return PNG_COLOR_TYPE_GRAY;
    case Image::RGB:
        return PNG_COLOR_TYPE_RGB;
    case Image::RGBA:
        return PNG_COLOR_TYPE_RGBA;
    }

    return PNG_COLOR_TYPE_RGBA;
}

void Image::save(const char* path)
{
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    png_bytep row = NULL;
    unsigned int stride;
    int index;

    FILE* fp = fopen(path, "wb");
    if (fp == NULL)
    {
        LOG(1, "Error: Failed to open image for writing: %s\n", path);
        goto error;
    }

    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL)
    {
        LOG(1, "Error: Write struct creation failed: %s\n", path);
        goto error;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == NULL)
    {
        LOG(1, "Error: Info struct creation failed: %s\n", path);
        goto error;
    }

    png_init_io(png_ptr, fp);

    png_set_IHDR(png_ptr, info_ptr, _width, _height, 8, getPNGColorType(_format), PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    png_write_info(png_ptr, info_ptr);

    // Allocate memory for a single row of image data
    stride = _bpp * _width * sizeof(png_byte);
    row = (png_bytep)malloc(stride);

    index = 0;
    for (unsigned int y = 0; y < _height; ++y)
    {
        for (unsigned int x = 0; x < stride; ++x)
        {
            // Write data
            row[x] = (png_byte)_data[index++];
        }
        png_write_row(png_ptr, row);
    }

    png_write_end(png_ptr, NULL);

error:
    if (fp)
        fclose(fp);
    if (row)
        free(row);
    if (info_ptr)
        png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
    if (png_ptr)
        png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
}

}
