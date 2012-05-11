#include "Base.h"
#include "Image.h"
#include "Texture.h"
#include "FileSystem.h"

// PVRTC (GL_IMG_texture_compression_pvrtc) : Imagination based gpus
#ifndef GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG
#define GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG 0x8C01
#endif
#ifndef GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG
#define GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG 0x8C03
#endif
#ifndef GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG
#define GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG 0x8C00
#endif
#ifndef GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG
#define GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG 0x8C02
#endif

// S3TC/DXT (GL_EXT_texture_compression_s3tc) : Most desktop/console gpus
#ifndef GL_COMPRESSED_RGBA_S3TC_DXT1_EXT
#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT 0x83F1
#endif
#ifndef GL_COMPRESSED_RGBA_S3TC_DXT3_EXT
#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT 0x83F2
#endif
#ifndef GL_COMPRESSED_RGBA_S3TC_DXT5_EXT
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT 0x83F3
#endif

// ATC (GL_AMD_compressed_ATC_texture) : Qualcomm/Adreno based gpus
#ifndef ATC_RGB_AMD
#define ATC_RGB_AMD 0x8C92
#endif
#ifndef ATC_RGBA_EXPLICIT_ALPHA_AMD
#define ATC_RGBA_EXPLICIT_ALPHA_AMD 0x8C93
#endif
#ifndef ATC_RGBA_INTERPOLATED_ALPHA_AMD
#define ATC_RGBA_INTERPOLATED_ALPHA_AMD 0x87EE
#endif

namespace gameplay
{

static std::vector<Texture*> __textureCache;

Texture::Texture() : _handle(0), _mipmapped(false), _cached(false), _compressed(false)
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
    const char* ext = strrchr(FileSystem::resolvePath(path), '.');
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
                // PowerVR Compressed Texture RGBA
                texture = createCompressedPVRTC(path);
            }
            else if (tolower(ext[1]) == 'd' && tolower(ext[2]) == 'd' && tolower(ext[3]) == 's')
            {
                // DDS file format (DXT/S3TC) compressed textures
                texture = createCompressedDDS(path);
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

// Computes the size of a PVRTC data chunk for a mipmap level of the given size.
unsigned int computePVRTCDataSize(int width, int height, int bpp)
{
    int blockSize;
    int widthBlocks;
    int heightBlocks;

    if (bpp == 4)
    {
        blockSize = 4 * 4; // Pixel by pixel block size for 4bpp
        widthBlocks = std::max(width >> 2, 2);
        heightBlocks = std::max(height >> 2, 2);
    }
    else
    {
        blockSize = 8 * 4; // Pixel by pixel block size for 2bpp
        widthBlocks = std::max(width >> 3, 2);
        heightBlocks = std::max(height >> 2, 2);
    }

    return widthBlocks * heightBlocks * ((blockSize  * bpp) >> 3);
}

Texture* Texture::createCompressedPVRTC(const char* path)
{
    FILE* file = FileSystem::openFile(path, "rb");
    if (file == NULL)
    {
        LOG_ERROR_VARG("Failed to load file: %s", path);
        return NULL;
    }

    // Read first 4 bytes to determine PVRTC format
    unsigned int read;
    unsigned int version;
    read = fread(&version, sizeof(unsigned int), 1, file);
    assert(read == 1);

    // Rewind to start of header
    fseek(file, 0, SEEK_SET);

    // Read texture data
    GLsizei width, height;
    GLenum format;
    GLubyte* data = NULL;
    unsigned int mipMapCount;

    if (version == 0x03525650)
    {
    	// Modern PVR file format.
    	data = readCompressedPVRTC(path, file, &width, &height, &format, &mipMapCount);
    }
    else
    {
    	// Legacy PVR file format.
    	data = readCompressedPVRTCLegacy(path, file, &width, &height, &format, &mipMapCount);
    }

    fclose(file);

    if (data == NULL)
    {
    	LOG_ERROR_VARG("Failed to read PVR file: %s", path);
    	return NULL;
    }

    int bpp = (format == GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG || format == GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG) ? 2 : 4;

    // Generate our texture.
	GLuint textureId;
	GL_ASSERT( glGenTextures(1, &textureId) );
	GL_ASSERT( glBindTexture(GL_TEXTURE_2D, textureId) );
	GL_ASSERT( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipMapCount > 1 ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR) );

	Texture* texture = new Texture();
	texture->_handle = textureId;
	texture->_width = width;
	texture->_height = height;
	texture->_mipmapped = mipMapCount > 1;
	texture->_compressed = true;

	// Load the data for each level
	GLubyte* ptr = data;
	for (unsigned int level = 0; level < mipMapCount; ++level)
	{
		unsigned int dataSize = computePVRTCDataSize(width, height, bpp);

		// Upload data to GL
		GL_ASSERT( glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height, 0, dataSize, ptr) );

		width = std::max(width >> 1, 1);
		height = std::max(height >> 1, 1);
		ptr += dataSize;
	}

	// Free data
	SAFE_DELETE_ARRAY(data);

    return texture;
}

GLubyte* Texture::readCompressedPVRTC(const char* path, FILE* file, GLsizei* width, GLsizei* height, GLenum* format, unsigned int* mipMapCount)
{
    struct pvrtc_file_header
    {
        unsigned int version;
        unsigned int flags;
        unsigned int pixelFormat[2];
        unsigned int colorSpace;
        unsigned int channelType;
        unsigned int height;
        unsigned int width;
        unsigned int depth;
        unsigned int surfaceCount;
        unsigned int faceCount;
        unsigned int mipMapCount;
        unsigned int metaDataSize;
    };

    unsigned int read;

    // Read header data
    pvrtc_file_header header;
    read = fread(&header, sizeof(pvrtc_file_header), 1, file);
    assert(read == 1);

    if (header.pixelFormat[1] != 0)
    {
        // Unsupported pixel format
        LOG_ERROR_VARG("Unsupported pixel format in PVR file (MSB == %d != 0): %s", header.pixelFormat[1], path);
        return NULL;
    }

    int bpp;
    switch (header.pixelFormat[0])
    {
    case 0:
        *format = GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG;
        bpp = 2;
        break;
    case 1:
        *format = GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG;
        bpp = 2;
        break;
    case 2:
        *format = GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG;
        bpp = 4;
        break;
    case 3:
        *format = GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG;
        bpp = 4;
        break;
    default:
        // Unsupported format
        LOG_ERROR_VARG("Unsupported pixel format value (%d) in PVR file: %s", header.pixelFormat[0], path);
        return NULL;
    }

    *width = (GLsizei)header.width;
    *height = (GLsizei)header.height;
    *mipMapCount = header.mipMapCount;

    // Skip meta-data
    assert( fseek(file, header.metaDataSize, SEEK_CUR) == 0 );

    // Compute total size of data to be read.
    int w = *width;
    int h = *height;
    unsigned int dataSize = 0;
    for (unsigned int level = 0; level < header.mipMapCount; ++level)
    {
        dataSize += computePVRTCDataSize(w, h, bpp);
    	w = std::max(w>>1, 1);
    	h = std::max(h>>1, 1);
    }

    // Read data
    GLubyte* data = new GLubyte[dataSize];
    read = fread(data, 1, dataSize, file);
    assert(read == dataSize);

    return data;
}

GLubyte* Texture::readCompressedPVRTCLegacy(const char* path, FILE* file, GLsizei* width, GLsizei* height, GLenum* format, unsigned int* mipMapCount)
{
    char PVRTCIdentifier[] = "PVR!";

    struct pvrtc_file_header_legacy
    {
        unsigned int size;                  // size of the structure
        unsigned int height;                // height of surface to be created
        unsigned int width;                 // width of input surface
        unsigned int mipmapCount;           // number of mip-map levels requested
        unsigned int formatflags;           // pixel format flags
        unsigned int dataSize;              // total size in bytes
        unsigned int bpp;                   // number of bits per pixel
        unsigned int redBitMask;            // mask for red bit
        unsigned int greenBitMask;          // mask for green bits
        unsigned int blueBitMask;           // mask for blue bits
        unsigned int alphaBitMask;          // mask for alpha channel
        unsigned int pvrtcTag;              // magic number identifying pvrtc file
        unsigned int surfaceCount;          // number of surfaces present in the pvrtc
    };

    // Read the file header
    unsigned int size = sizeof(pvrtc_file_header_legacy);
    pvrtc_file_header_legacy header;
    unsigned int read = (int)fread(&header, 1, size, file);
    assert(read == size);
    if (read != size)
    {
        LOG_ERROR_VARG("Read file header error for pvrtc file: %s (%d < %d)", path, (int)read, (int)size);
        return NULL;
    }

    // Proper file header identifier
    if (PVRTCIdentifier[0] != (char)((header.pvrtcTag >>  0) & 0xff) ||
        PVRTCIdentifier[1] != (char)((header.pvrtcTag >>  8) & 0xff) ||
        PVRTCIdentifier[2] != (char)((header.pvrtcTag >> 16) & 0xff) ||
        PVRTCIdentifier[3] != (char)((header.pvrtcTag >> 24) & 0xff))
     {
        LOG_ERROR_VARG("Invalid PVRTC compressed texture file: %s", path);
        return NULL;
    }

    // Format flags for GLenum format
    if (header.bpp == 4)
    {
        *format = header.alphaBitMask ? GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG : GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG;
    }
    else if (header.bpp == 2)
    {
        *format = header.alphaBitMask ? GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG : GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG;
    }
    else
    {
        LOG_ERROR_VARG("Invalid PVRTC compressed texture format flags for file: %s", path);
        return NULL;
    }

    *width = (GLsizei)header.width;
    *height = (GLsizei)header.height;
    *mipMapCount = header.mipmapCount + 1; // +1 because mipmapCount does not include the base level

    GLubyte* data = new GLubyte[header.dataSize];
    read = (int)fread(data, 1, header.dataSize, file);
    assert(read == header.dataSize);
    if (read != header.dataSize)
    {
        LOG_ERROR_VARG("Read file data error for pvrtc file: %s (%d < %d)", path, (int)read, (int)header.dataSize);
        SAFE_DELETE_ARRAY(data);
        return NULL;
    }

    return data;
}

Texture* Texture::createCompressedDDS(const char* path)
{
    // DDS file structures
    struct dds_pixel_format
    {
        unsigned int dwSize;
        unsigned int dwFlags;
        unsigned int dwFourCC;
        unsigned int dwRGBBitCount;
        unsigned int dwRBitMask;
        unsigned int dwGBitMask;
        unsigned int dwBBitMask;
        unsigned int dwABitMask;
    };

    struct dds_header
    {
        unsigned int     dwSize;
        unsigned int     dwFlags;
        unsigned int     dwHeight;
        unsigned int     dwWidth;
        unsigned int     dwPitchOrLinearSize;
        unsigned int     dwDepth;
        unsigned int     dwMipMapCount;
        unsigned int     dwReserved1[11];
        dds_pixel_format ddspf;
        unsigned int     dwCaps;
        unsigned int     dwCaps2;
        unsigned int     dwCaps3;
        unsigned int     dwCaps4;
        unsigned int     dwReserved2;
    };

    struct dds_mip_level
    {
        GLubyte* data;
        GLsizei width;
        GLsizei height;
        GLsizei size;
    };

    Texture* texture = NULL;

    // Read DDS file
    FILE* fp = FileSystem::openFile(path, "rb");
    if (fp == NULL)
        return NULL;

    // Validate DDS magic number
    char code[4];
    if (fread(code, 1, 4, fp) != 4 || strncmp(code, "DDS ", 4) != 0)
        return NULL; // not a valid dds file

    // Read DDS header
    dds_header header;
    if (fread(&header, sizeof(dds_header), 1, fp) != 1)
        return NULL;

    if ((header.dwFlags & 0x20000/*DDSD_MIPMAPCOUNT*/) == 0)
    {
        // Mipmap count not specified (non-mipmapped texture)
        header.dwMipMapCount = 1;
    }

    // Allocate mip level structures
    dds_mip_level* mipLevels = new dds_mip_level[header.dwMipMapCount];
    memset(mipLevels, 0, sizeof(dds_mip_level) * header.dwMipMapCount);

    GLenum format, internalFormat;
    bool compressed = false;
    GLsizei width = header.dwWidth;
    GLsizei height = header.dwHeight;
    int bytesPerBlock;

    if (header.ddspf.dwFlags & 0x4/*DDPF_FOURCC*/)
    {
        compressed = true;

        // Compressed
        switch (header.ddspf.dwFourCC)
        {
        case ('D'|('X'<<8)|('T'<<16)|('1'<<24)):
            format = internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
            bytesPerBlock = 8;
            break;
        case ('D'|('X'<<8)|('T'<<16)|('3'<<24)):
            format = internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
            bytesPerBlock = 16;
            break;
        case ('D'|('X'<<8)|('T'<<16)|('5'<<24)):
            format = internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
            bytesPerBlock = 16;
            break;
        case ('A'|('T'<<8)|('C'<<16)|(' '<<24)):
            format = internalFormat = ATC_RGB_AMD;
            bytesPerBlock = 8;
            break;
        case ('A'|('T'<<8)|('C'<<16)|('A'<<24)):
            format = internalFormat = ATC_RGBA_EXPLICIT_ALPHA_AMD;
            bytesPerBlock = 16;
            break;
        case ('A'|('T'<<8)|('C'<<16)|('I'<<24)):
            format = internalFormat = ATC_RGBA_INTERPOLATED_ALPHA_AMD;
            bytesPerBlock = 16;
            break;
        default:
            // Unsupported
            return NULL;
        }

        for (unsigned int i = 0; i < header.dwMipMapCount; ++i)
        {
            mipLevels[i].width = width;
            mipLevels[i].height = height;
            mipLevels[i].size =  std::max(1, (width+3) >> 2) * std::max(1, (height+3) >> 2) * bytesPerBlock;
            mipLevels[i].data = new GLubyte[mipLevels[i].size];

            if (fread(mipLevels[i].data, 1, mipLevels[i].size, fp) != (unsigned int)mipLevels[i].size)
            {
                LOG_ERROR_VARG("Failed to load dds compressed texture bytes for texture: %s", path);
                goto cleanup;
            }

            width  = std::max(1, width >> 1);
            height = std::max(1, height >> 1);
        }
    }
    else if (header.ddspf.dwFlags == 0x40/*DDPF_RGB*/)
    {
        // RGB (uncompressed)
        // Note: Use GL_BGR as internal format to flip bytes.
        return NULL; // unsupported
    }
    else if (header.ddspf.dwFlags == 0x41/*DDPF_RGB|DDPF_ALPHAPIXELS*/)
    {
        // RGBA (uncompressed)
        // Note: Use GL_BGRA as internal format to flip bytes.
        return NULL; // unsupported
    }
    else
    {
        // Unsupported
        return NULL;
    }

    // Generate GL texture
    GLuint textureId;
    GL_ASSERT( glGenTextures(1, &textureId) );
    GL_ASSERT( glBindTexture(GL_TEXTURE_2D, textureId) );
    GL_ASSERT( glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, header.dwMipMapCount > 1 ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR ) );

    // Create gameplay texture
    texture = new Texture();
    texture->_handle = textureId;
    texture->_width = header.dwWidth;
    texture->_height = header.dwHeight;
    texture->_compressed = compressed;
    texture->_mipmapped = header.dwMipMapCount > 1;

    // Load texture data
    for (unsigned int i = 0; i < header.dwMipMapCount; ++i)
    {
        if (compressed)
        {
            GL_ASSERT( glCompressedTexImage2D(GL_TEXTURE_2D, i, format, mipLevels[i].width, mipLevels[i].height, 0, mipLevels[i].size, mipLevels[i].data) );
        }
        else
        {
            GL_ASSERT( glTexImage2D(GL_TEXTURE_2D, i, internalFormat, mipLevels[i].width, mipLevels[i].height, 0, format, GL_UNSIGNED_INT, mipLevels[i].data) );
        }
    }

cleanup:

    // Cleanup mip data
    for (unsigned int i = 0; i < header.dwMipMapCount; ++i)
        SAFE_DELETE_ARRAY(mipLevels[i].data);
    SAFE_DELETE_ARRAY(mipLevels);

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

bool Texture::isCompressed() const
{
    return _compressed;
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
