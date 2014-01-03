#include "Base.h"
#include "FileSystem.h"
#include "Image.h"

#define uint unsigned int
#define byte unsigned char

// We are using png explicitly, should be defined in build cfg
#define GP_USE_PNG

namespace gameplay
{

#ifdef GP_USE_PNG
#include <png.h>
// Callback for reading a png image using Stream
static void readPNGStream(png_structp png, png_bytep data, png_size_t length)
{
    Stream* stream = reinterpret_cast<Stream*>(png_get_io_ptr(png));
    if (stream == NULL || stream->read(data, 1, length) != length)
    {
        png_error(png, "Error reading PNG.");
    }
}

static bool readPNGImage(const char* path, uint* pWidth, uint* pHeight, Image::Format* pFormat, byte** pPixels)
{
    GP_ASSERT(path);

    // Open the file.
    std::auto_ptr<Stream> stream(FileSystem::open(path));
    if (stream.get() == NULL || !stream->canRead())
    {
        GP_ERROR("Failed to open PNG file '%s'.", path);
        return false;
    }

    // Verify PNG signature.
    unsigned char sig[8];
    if (stream->read(sig, 1, 8) != 8 || png_sig_cmp(sig, 0, 8) != 0)
    {
        GP_ERROR("Failed to load file '%s'; not a valid PNG.", path);
        return false;
    }

    // Initialize png read struct (last three parameters use stderr+longjump if NULL).
    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png == NULL)
    {
        GP_ERROR("Failed to create PNG structure for reading PNG file '%s'.", path);
        return false;
    }

    // Initialize info struct.
    png_infop info = png_create_info_struct(png);
    if (info == NULL)
    {
        GP_ERROR("Failed to create PNG info structure for PNG file '%s'.", path);
        png_destroy_read_struct(&png, NULL, NULL);
        return false;
    }

    // Set up error handling (required without using custom error handlers above).
    if (setjmp(png_jmpbuf(png)))
    {
        GP_ERROR("Failed to set up error handling for reading PNG file '%s'.", path);
        png_destroy_read_struct(&png, &info, NULL);
        return false;
    }

    // Initialize file io.
    png_set_read_fn(png, stream.get(), readPNGStream);

    // Indicate that we already read the first 8 bytes (signature).
    png_set_sig_bytes(png, 8);

    // Read the entire image into memory.
    png_read_png(png, info, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, NULL);

    *pWidth = png_get_image_width(png, info);
    *pHeight = png_get_image_height(png, info);

    png_byte colorType = png_get_color_type(png, info);
    switch (colorType)
    {
        case PNG_COLOR_TYPE_RGBA:
            *pFormat = Image::RGBA;
            break;

        case PNG_COLOR_TYPE_RGB:
            *pFormat = Image::RGB;
            break;

        default:
            GP_ERROR("Unsupported PNG color type (%d) for image file '%s'.", (int)colorType, path);
            png_destroy_read_struct(&png, &info, NULL);
            return false;
    }

    size_t stride = png_get_rowbytes(png, info);

    // Allocate image data.
    *pPixels = new byte[stride * (*pHeight)];

    // Read rows into image data.
    png_bytepp rows = png_get_rows(png, info);
    for (unsigned int i = 0; i < (*pHeight); ++i)
    {
        memcpy((*pPixels)+(stride * ((*pHeight)-1-i)), rows[i], stride);
    }

    // Clean up.
    png_destroy_read_struct(&png, &info, NULL);

    return true;
}
#else
static bool readPNGImage(const char* path, uint* pWidth, uint* pHeight, Image::Format* pFormat, byte** pPixels)
{
    GP_ERROR("PNG format is unsupported!");
    return false;
}
#endif

#ifdef GP_USE_JPEG
#include <jpeglib.h>
static bool readJPGImage(const char* path, uint* pWidth, uint* pHeight, Image::Format* pFormat, byte** pPixels)
{
    int fileSize;
    byte* fileData;
    byte* pBuffer;
    struct jpeg_error_mgr jerr;
    struct jpeg_decompress_struct cinfo;

    GP_ASSERT(path);

    fileData = (byte *) FileSystem::readAll(path, &fileSize);
    if (!fileData)
    {
        GP_ERROR("Failed to open JPG file: %s", path);
        return false;
    }

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);
    jpeg_mem_src(&cinfo, fileData, fileSize);

    if (jpeg_read_header(&cinfo, (gameplay::boolean) 1) != JPEG_HEADER_OK)
    {
        GP_ERROR("Failed to read header for JPEG file '%s'.", path);
        SAFE_DELETE_ARRAY(fileData);
        return false;
    }

    if (!jpeg_start_decompress(&cinfo))
    {
        GP_ERROR("Failed to start decompression for JPEG file '%s'.", path);
        SAFE_DELETE_ARRAY(fileData);
        return false;
    }

    size_t stride = cinfo.output_width * cinfo.output_components;
    switch (cinfo.output_components) {
        case 4:
            *pFormat = Image::RGBA;
            break;
        case 3:
            *pFormat = Image::RGB;
            break;
        default:
            GP_ERROR("Unsupported JPEG color type (%d) for image file '%s'.", cinfo.output_components, path);
            SAFE_DELETE_ARRAY(fileData);
            return false;
    }
    *pPixels = new byte[cinfo.output_width * cinfo.output_height * cinfo.output_components];
    *pWidth = cinfo.output_width;
    *pHeight = cinfo.output_height;

    while (cinfo.output_scanline < cinfo.output_height) {
        // OpenGL writes from bottom to top, but libjpeg goes
        // from top to bottom, so we must flip lines.
        pBuffer = (*pPixels) + ((cinfo.output_height - 1 - cinfo.output_scanline) * stride);
        if (jpeg_read_scanlines(&cinfo, &pBuffer, 1) <= 0)
        {
            GP_ERROR("Unable to read data for JPEG file '%s'.", path);
            SAFE_DELETE_ARRAY(pPixels);
            SAFE_DELETE_ARRAY(fileData);
            return false;
        }
    }

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    SAFE_DELETE_ARRAY(fileData);

    return true;
}
#else
static bool readJPGImage(const char* path, uint* pWidth, uint* pHeight, Image::Format* pFormat, byte** pPixels)
{
    GP_ERROR("JPEG format is unsupported!");
    return false;
}
#endif

Image* Image::create(const char* path)
{
    GP_ASSERT(path);

    const char* ext = strrchr(FileSystem::resolvePath(path), '.');
    if (ext)
    {
        switch (strlen(ext))
        {
            case 4:
                if (tolower(ext[1]) == 'p' && tolower(ext[2]) == 'n' && tolower(ext[3]) == 'g')
                {
                    Image* i = new Image();
                    if (!readPNGImage(path, &i->_width, &i->_height, &i->_format, &i->_data)) {
                        delete i;
                        return NULL;
                    }
                    return i;
                }
                else if (tolower(ext[1]) == 'j' && tolower(ext[2]) == 'p' && tolower(ext[3]) == 'g')
                {
                    Image* i = new Image();
                    if (!readJPGImage(path, &i->_width, &i->_height, &i->_format, &i->_data)) {
                        delete i;
                        return NULL;
                    }
                    return i;

                }
                break;

        }
    }

    GP_ERROR("Failed to open image file '%s'.", path);
    return NULL;
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

Image::Image() : _data(NULL), _format(RGB), _width(0), _height(0)
{
}

Image::~Image()
{
    SAFE_DELETE_ARRAY(_data);
}

}
