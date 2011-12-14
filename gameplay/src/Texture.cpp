#include "Base.h"
#include "Image.h"
#include "Texture.h"

namespace gameplay
{

static std::vector<Texture*> __textureCache;

Texture::Texture() : _handle(0), _mipmapped(false), _cached(false)
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
        std::vector<Texture*>::iterator itr = find(__textureCache.begin(), __textureCache.end(), this);
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
            // If 'generateMipmaps' is true, call Texture::generateMipamps() to force the 
            // texture to generate its mipmap chain if it hasn't already done so.
            if (generateMipmaps)
            {
                t->generateMipmaps();
            }

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
                Image* image = Image::create(path);
                texture = create(image, generateMipmaps);
                SAFE_RELEASE(image);
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

    LOG_ERROR_VARG("Failed to load texture: %s", path);
    return NULL;
}

Texture* Texture::create(Image* image, bool generateMipmaps)
{
    return create((Texture::Format)image->getFormat(), image->getWidth(), image->getHeight(), image->getData(), generateMipmaps);
}

Texture* Texture::create(Format format, unsigned int width, unsigned int height, unsigned char* data, bool generateMipmaps)
{
    // Load our texture.
    GLuint textureId;
    GL_ASSERT( glGenTextures(1, &textureId) );
    GL_ASSERT( glBindTexture(GL_TEXTURE_2D, textureId) );

    if (format == DEPTH)
    {
        // <type> must be UNSIGNED_SHORT or UNSIGNED_INT for a format of DEPTH_COMPONENT.
        GL_ASSERT( glTexImage2D(GL_TEXTURE_2D, 0, (GLenum)format, width, height, 0, (GLenum)format, GL_UNSIGNED_INT, data) );
    }
    else
    {
        GL_ASSERT( glTexImage2D(GL_TEXTURE_2D, 0, (GLenum)format, width, height, 0, (GLenum)format, GL_UNSIGNED_BYTE, data) );
    }

    // Set initial minification filter based on whether or not mipmaping was enabled
    GL_ASSERT( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, generateMipmaps ? GL_NEAREST_MIPMAP_LINEAR : GL_LINEAR) );

    Texture* texture = new Texture();
    texture->_handle = textureId;
    texture->_width = width;
    texture->_height = height;

    if (generateMipmaps)
    {
        texture->generateMipmaps();
    }

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

void Texture::setWrapMode(Wrap wrapS, Wrap wrapT)
{
    GLint currentTextureId;
    GL_ASSERT( glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTextureId) );
    GL_ASSERT( glBindTexture(GL_TEXTURE_2D, _handle) );
    GL_ASSERT( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLenum)wrapS) );
    GL_ASSERT( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLenum)wrapT) );
    GL_ASSERT( glBindTexture(GL_TEXTURE_2D, (GLuint)currentTextureId) );
}

void Texture::setFilterMode(Filter minificationFilter, Filter magnificationFilter)
{
    GLint currentTextureId;
    GL_ASSERT( glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTextureId) );
    GL_ASSERT( glBindTexture(GL_TEXTURE_2D, _handle) );
    GL_ASSERT( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLenum)minificationFilter) );
    GL_ASSERT( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLenum)magnificationFilter) );
    GL_ASSERT( glBindTexture(GL_TEXTURE_2D, (GLuint)currentTextureId) );
}

void Texture::generateMipmaps()
{
    if (!_mipmapped)
    {
        GLint currentTextureId;
        GL_ASSERT( glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTextureId) );
        GL_ASSERT( glBindTexture(GL_TEXTURE_2D, _handle) );
        GL_ASSERT( glGenerateMipmap(GL_TEXTURE_2D) );
        GL_ASSERT( glBindTexture(GL_TEXTURE_2D, (GLuint)currentTextureId) );

        _mipmapped = true;
    }
}

bool Texture::isMipmapped() const
{
    return _mipmapped;
}

Texture::Sampler::Sampler(Texture* texture)
    : _texture(texture), _wrapS(Texture::REPEAT), _wrapT(Texture::REPEAT), _magFilter(Texture::LINEAR)
{
    _minFilter = texture->isMipmapped() ? Texture::NEAREST_MIPMAP_LINEAR : Texture::LINEAR;
}

Texture::Sampler::~Sampler()
{
    SAFE_RELEASE(_texture);
}

Texture::Sampler* Texture::Sampler::create(Texture* texture)
{
    assert(texture != NULL);

    texture->addRef();
    return new Sampler(texture);
}

Texture::Sampler* Texture::Sampler::create(const char* path, bool generateMipmaps)
{
    Texture* texture = Texture::create(path, generateMipmaps);
    return texture ? new Sampler(texture) : NULL;
}

void Texture::Sampler::setWrapMode(Wrap wrapS, Wrap wrapT)
{
    _wrapS = wrapS;
    _wrapT = wrapT;
}

void Texture::Sampler::setFilterMode(Filter minificationFilter, Filter magnificationFilter)
{
    _minFilter = minificationFilter;
    _magFilter = magnificationFilter;
}

Texture* Texture::Sampler::getTexture() const
{
    return _texture;
}

void Texture::Sampler::bind()
{
    GL_ASSERT( glBindTexture(GL_TEXTURE_2D, _texture->_handle) );
    GL_ASSERT( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLenum)_wrapS) );
    GL_ASSERT( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLenum)_wrapT) );
    GL_ASSERT( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLenum)_minFilter) );
    GL_ASSERT( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLenum)_magFilter) );
}

}
