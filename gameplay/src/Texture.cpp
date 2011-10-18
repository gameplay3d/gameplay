/*
 * Texture.cpp
 */

#include "Base.h"
#include "Texture.h"
#include "FileSystem.h"

namespace gameplay
{

static std::vector<Texture*> __textureCache;

Texture::Texture() : _handle(0), _cached(false)
{
}

Texture::Texture(const Texture& copy)
{
}

Texture::~Texture()
{
    if (_handle)
    {
        glDeleteTextures(1, &_handle);
        _handle = 0;
    }

    // Remove ourself from the texture cache.
    if (_cached)
    {
        std::vector<Texture*>::iterator itr = std::find(__textureCache.begin(), __textureCache.end(), this);
        if (itr != __textureCache.end())
        {
            __textureCache.erase(itr);
        }
    }
}

Texture* Texture::create(const char* path, bool generateMipmaps)
{
    // Search texture cache first.
    for (unsigned int i = 0, count = __textureCache.size(); i < count; ++i)
    {
        Texture* t = __textureCache[i];
        if (t->_path == path)
        {
            // Found a match.
            t->addRef();
            return t;
        }
    }

    Texture* texture = NULL;

    // Filter loading based on file extension.
    const char* ext = strrchr(path, '.');
    if (ext)
    {
        switch (strlen(ext))
        {
        case 4:
            if (tolower(ext[1]) == 'p' && tolower(ext[2]) == 'n' && tolower(ext[3]) == 'g')
            {
                texture = loadPNG(path, generateMipmaps);
            }
            break;
        }
    }

    if (texture)
    {
        texture->_path = path;
        texture->_cached = true;

        // Add to texture cache.
        __textureCache.push_back(texture);

        return texture;
    }

    LOG_ERROR_VARG("Unrecognized file extension for texture: %s", path);
    return NULL;
}

Texture* Texture::loadPNG(const char* path, bool generateMipmaps)
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

    unsigned int width = info->width;
    unsigned int height = info->height;
    Format format;
    switch (info->color_type)
    {
    case PNG_COLOR_TYPE_RGBA:
        format = RGBA8888;
        break;

    case PNG_COLOR_TYPE_RGB:
        format = RGB888;
        break;

    default:
        LOG_ERROR_VARG("Unsupported PNG color type (%d) for texture: %s", (int)info->color_type, path);
        fclose(fp);
        png_destroy_read_struct(&png, &info, NULL);
        return NULL;
    }

    unsigned int stride = png_get_rowbytes(png, info);

    // Allocate image data.
    unsigned char* data = new unsigned char[stride * height];

    // Read rows into image data.
    png_bytepp rows = png_get_rows(png, info);
    for (unsigned int i = 0; i < height; ++i)
    {
        memcpy(data+(stride * (height-1-i)), rows[i], stride);
    }

    // Clean up.
    png_destroy_read_struct(&png, &info, NULL);
    fclose(fp);

    // Create texture.
    Texture* texture = create(format, width, height, data, generateMipmaps);

    // Free temporary data.
    delete[] data;

    return texture;
}

Texture* Texture::create(Format format, unsigned int width, unsigned int height, unsigned char* data, bool generateMipmaps)
{
    // Load our texture.
    GLuint textureId;
    GL_ASSERT( glGenTextures(1, &textureId) );
    GL_ASSERT( glBindTexture(GL_TEXTURE_2D, textureId) );
    GL_ASSERT( glTexImage2D(GL_TEXTURE_2D, 0, (GLenum)format, width, height, 0, (GLenum)format, GL_UNSIGNED_BYTE, data) );

    if (generateMipmaps)
    {
        GL_ASSERT( glGenerateMipmap(GL_TEXTURE_2D) );
    }

    // TODO: Move texture parameters into material sampler definition
    GL_ASSERT( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, generateMipmaps ? GL_NEAREST_MIPMAP_LINEAR : GL_LINEAR) );
    GL_ASSERT( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR) );
    GL_ASSERT( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT) );
    GL_ASSERT( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT) );

    Texture* texture = new Texture();
    texture->_handle = textureId;
    texture->_width = width;
    texture->_height = height;

    return texture;
}

unsigned int Texture::getWidth() const
{
    return _width;
}

unsigned int Texture::getHeight() const
{
    return _height;
}

TextureHandle Texture::getHandle() const
{
    return _handle;
}

void Texture::setWrapMode(bool repeatS, bool repeatT)
{
    GLint currentTextureId;
    GL_ASSERT( glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTextureId) );
    GL_ASSERT( glBindTexture(GL_TEXTURE_2D, _handle) );
    GL_ASSERT( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeatS ? GL_REPEAT : GL_CLAMP_TO_EDGE) );
    GL_ASSERT( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeatT ? GL_REPEAT : GL_CLAMP_TO_EDGE) );
    GL_ASSERT( glBindTexture(GL_TEXTURE_2D, (GLuint)currentTextureId) );
}

}
