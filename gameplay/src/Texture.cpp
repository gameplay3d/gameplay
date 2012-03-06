#include "Base.h"
#include "Image.h"
#include "Texture.h"
#include "FileSystem.h"

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
                if (image)
                    texture = create(image, generateMipmaps);
                SAFE_RELEASE(image);
            }
			else if (tolower(ext[1]) == 'p' && tolower(ext[2]) == 'v' && tolower(ext[3]) == 'r')
			{
            	// PowerVR Compressed RGBA
				texture = createCompressedPVR(path);
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
    switch (image->getFormat())
    {
    case Image::RGB:
        return create(Texture::RGB, image->getWidth(), image->getHeight(), image->getData(), generateMipmaps);
    case Image::RGBA:
        return create(Texture::RGBA, image->getWidth(), image->getHeight(), image->getData(), generateMipmaps);
    }
    
    return NULL;
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

Texture* Texture::createCompressedPVR(const char* path)
{
	char PVRTexIdentifier[] = "PVR!";

	enum
	{
	    PVRTextureFlagTypePVRTC_2 = 24,
	    PVRTextureFlagTypePVRTC_4
	};

	struct pvr_file_header
	{
		unsigned int size;          		// size of the structure
		unsigned int height;              	// height of surface to be created
		unsigned int width;               	// width of input surface
		unsigned int mipmapCount;         	// number of mip-map levels requested
		unsigned int formatflags;           // pixel format flags
		unsigned int dataSize;     			// total size in bytes
		unsigned int bpp;            		// number of bits per pixel
		unsigned int redBitMask;            // mask for red bit
		unsigned int greenBitMask;          // mask for green bits
		unsigned int blueBitMask;           // mask for blue bits
		unsigned int alphaBitMask;        	// mask for alpha channel
		unsigned int pvrTag;                // magic number identifying pvr file
		unsigned int surfaceCount;          // number of surfaces present in the pvr
	} ;

	FILE* file = FileSystem::openFile(path, "rb");
	if (file == NULL)
	{
		LOG_ERROR_VARG("Failed to load file: %s", path);
		return NULL;
	}

	// Read the file header
	unsigned int size = sizeof(pvr_file_header);
	pvr_file_header header;
	unsigned int read = (int)fread(&header, 1, size, file);
	assert(read == size);
	if (read != size)
	{
		LOG_ERROR_VARG("Read file header error for pvr file: %s (%d < %d)", path, (int)read, (int)size);
		fclose(file);
		return NULL;
	}

	// Proper file header identifier
	if (PVRTexIdentifier[0] != (char)((header.pvrTag >>  0) & 0xff) ||
		PVRTexIdentifier[1] != (char)((header.pvrTag >>  8) & 0xff) ||
	    PVRTexIdentifier[2] != (char)((header.pvrTag >> 16) & 0xff) ||
	    PVRTexIdentifier[3] != (char)((header.pvrTag >> 24) & 0xff))
	 {
		LOG_ERROR_VARG("Invalid PVR texture file: %s", path);
		fclose(file);
	    return NULL;
	}

	// Format flags for GLenum format
	GLenum format;
	unsigned int formatFlags = header.formatflags & 0xff;
	if (formatFlags == PVRTextureFlagTypePVRTC_4)
	{
		format = header.alphaBitMask ? COMPRESSED_RGBA_PVRTC_4BPP : COMPRESSED_RGB_PVRTC_4BPP;
	}
	else if (formatFlags == PVRTextureFlagTypePVRTC_2)
	{
		format = header.alphaBitMask ? COMPRESSED_RGBA_PVRTC_2BPP : COMPRESSED_RGB_PVRTC_2BPP;
	}
	else
	{
		LOG_ERROR_VARG("Invalid PVR texture format flags for file: %s", path);
		fclose(file);
		return NULL;
	}

	unsigned char* data = new unsigned char[header.dataSize];
	read = (int)fread(data, 1, header.dataSize, file);
	assert(read == header.dataSize);
	if (read != header.dataSize)
	{
		LOG_ERROR_VARG("Read file data error for pvr file: %s (%d < %d)", path, (int)read, (int)header.dataSize);
		SAFE_DELETE_ARRAY(data);
		fclose(file);
		return NULL;
	}
	// Close file
	fclose(file);

	// Load our texture.
	GLuint textureId;
	GL_ASSERT( glGenTextures(1, &textureId) );
	GL_ASSERT( glBindTexture(GL_TEXTURE_2D, textureId) );
	GL_ASSERT( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, header.mipmapCount > 0 ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR ) );

    Texture* texture = new Texture();
    texture->_handle = textureId;
    texture->_width = header.width;
    texture->_height = header.height;

	// Load the data for each level
	unsigned int width = header.width;
	unsigned int height = header.height;
	unsigned int blockSize = 0;
	unsigned int widthBlocks = 0;
	unsigned int heightBlocks = 0;
	unsigned int bpp = 0;
	unsigned int dataSize = 0;
	unsigned char* dataOffset = data;

	for (unsigned int level = 0; level <= header.mipmapCount; level++)
	{
		if (formatFlags == PVRTextureFlagTypePVRTC_4)
		{
			dataSize = ( max((int)width, 8) * max((int)height, 8) * 4 + 7) / 8;
		}
		else
		{
			dataSize = ( max((int)width, 16) * max((int)height, 8) * 2 + 7) / 8;
		}

		GL_ASSERT( glCompressedTexImage2D(GL_TEXTURE_2D, level, (GLenum)format, width, height, 0, dataSize, dataOffset) );

		dataOffset += dataSize;
		width = max((int)width >> 1, 1);
		height = max((int)height >> 1, 1);
	}

	SAFE_DELETE_ARRAY(data);

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
