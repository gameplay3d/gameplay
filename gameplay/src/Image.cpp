#include "Base.h"
#include "FileSystem.h"
#include "Image.h"

namespace gameplay
{

Image* Image::create(const char* path)
{
    // Open the file.
    FILE* fp = FileSystem::openFile(path, "rb");
    if (fp == NULL)
    {
        return NULL;
    }

    // Verify PNG signature.
    unsigned char sig[8];
    if (fread(sig, 1, 8, fp) != 8 || png_sig_cmp(sig, 0, 8) != 0)
    {
        LOG_ERROR_VARG("Texture is not a valid PNG: %s", path);
        fclose(fp);
        return NULL;
    }

    // Initialize png read struct (last three parameters use stderr+longjump if NULL).
    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png == NULL)
    {
        fclose(fp);
        return NULL;
    }

    // Initialize info struct.
    png_infop info = png_create_info_struct(png);
    if (info == NULL)
    {
        fclose(fp);
        png_destroy_read_struct(&png, NULL, NULL);
        return NULL;
    }

    // Set up error handling (required without using custom error handlers above).
    if (setjmp(png_jmpbuf(png)))
    {
        fclose(fp);
        png_destroy_read_struct(&png, &info, NULL);
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
    case PNG_COLOR_TYPE_RGBA:
        image->_format = RGBA;
        break;

    case PNG_COLOR_TYPE_RGB:
        image->_format = RGB;
        break;

    default:
        LOG_ERROR_VARG("Unsupported PNG color type (%d) for texture: %s", (int)colorType, path);
        fclose(fp);
        png_destroy_read_struct(&png, &info, NULL);
        return NULL;
    }

    unsigned int stride = png_get_rowbytes(png, info);

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
    fclose(fp);

    return image;
}

Image::Image()
{
    // Unused
}

Image::~Image()
{
    SAFE_DELETE_ARRAY(_data);
}

}
