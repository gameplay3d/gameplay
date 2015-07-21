#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "Ref.h"
#include "Stream.h"

namespace gameplay
{

class Image;

/**
 * Defines a standard texture.
 */
class Texture : public Ref
{
    friend class Sampler;

public:

    /**
     * Defines the set of supported texture formats.
     */
    enum Format
    {
        UNKNOWN = 0,
        RGB,
        RGB888 = RGB,
        RGB565,
        RGBA,
        RGBA8888 = RGBA,
        RGBA4444,
        RGBA5551,
        ALPHA,
        DEPTH,
    };

    /**
     * Defines the set of supported texture filters.
     */
    enum Filter
    {
        NEAREST = GL_NEAREST,
        LINEAR = GL_LINEAR,
        NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
        LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
        NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
        LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR
    };

    /**
     * Defines the set of supported texture wrapping modes.
     */
    enum Wrap
    {
        REPEAT = GL_REPEAT,
        CLAMP = GL_CLAMP_TO_EDGE
    };

    /**
     * Defines the type of Texture in use.
     */
    enum Type
    {
        TEXTURE_2D = GL_TEXTURE_2D,
        TEXTURE_CUBE = GL_TEXTURE_CUBE_MAP
    };

    /**
     * Defines a face of a Texture of Type: cube.
     */
    enum CubeFace
    {
        POSITIVE_X,
        NEGATIVE_X,
        POSITIVE_Y,
        NEGATIVE_Y,
        POSITIVE_Z,
        NEGATIVE_Z
    };
    
    /**
     * Defines a texture sampler.
     *
     * A texture sampler is basically an instance of a texture that can be
     * used to sample a texture from a material. In addition to the texture
     * itself, a sampler stores per-instance texture state information, such
     * as wrap and filter modes.
     */
    class Sampler : public Ref
    {
        friend class Texture;

    public:

        /**
         * Destructor.
         */
        virtual ~Sampler();

        /**
         * Creates a sampler for the specified texture.
         *
         * @param texture The texture.
         *
         * @return The new sampler.
         * @script{create}
         */
        static Sampler* create(Texture* texture);

        /**
         * Creates a sampler for the specified texture.
         *
         * @param path Path to the texture to create a sampler for.
         * @param generateMipmaps True to force a full mipmap chain to be generated for the texture, false otherwise.
         *
         * @return The new sampler.
         * @script{create}
         */
        static Sampler* create(const char* path, bool generateMipmaps = false);

        /**
         * Sets the wrap mode for this sampler.
         *
         * @param wrapS The horizontal wrap mode.
         * @param wrapT The vertical wrap mode.
         * @param wrapR The depth wrap mode.
         */
        void setWrapMode(Wrap wrapS, Wrap wrapT, Wrap wrapR = REPEAT);

        /**
         * Sets the texture filter modes for this sampler.
         *
         * @param minificationFilter The texture minification filter.
         * @param magnificationFilter The texture magnification filter.
         */
        void setFilterMode(Filter minificationFilter, Filter magnificationFilter);

        /**
         * Gets the texture for this sampler.
         *
         * @return The texture for this sampler.
         */
        Texture* getTexture() const;

        /**
         * Binds the texture of this sampler to the renderer and applies the sampler state.
         */
        void bind();

    private:

        /**
         * Constructor.
         */
        Sampler(Texture* texture);

        /**
         * Hidden copy assignment operator.
         */
        Sampler& operator=(const Sampler&);

        Texture* _texture;
        Wrap _wrapS;
        Wrap _wrapT;
        Wrap _wrapR;
        Filter _minFilter;
        Filter _magFilter;
    };

    /**
     * Creates a texture from the given image resource.
     *
     * Note that for textures that include mipmap data in the source data (such as most compressed textures),
     * the generateMipmaps flags should NOT be set to true.
     *
     * @param path The image resource path.
     * @param generateMipmaps true to auto-generate a full mipmap chain, false otherwise.
     * 
     * @return The new texture, or NULL if the texture could not be loaded/created.
     * @script{create}
     */
    static Texture* create(const char* path, bool generateMipmaps = false);

    /**
     * Creates a texture from the given image.
     *
     * @param image The image containing the texture data.
     * @param generateMipmaps True to generate a full mipmap chain, false otherwise.
     *
     * @return The new texture, or NULL if the image is not of a supported texture format.
     * @script{create}
     */
    static Texture* create(Image* image, bool generateMipmaps = false);

    /**
     * Creates a texture from the given texture data.
     *
     * The data in the texture is expected to be tightly packed (no padding at the end of rows).
     *
     * @param format Format of the texture data.
     * @param width Width of the texture data. If type is TEX_CUBE, then this is the cube face width.
     * @param height Height of the texture data. If type is TEX_CUBE, then this is the cube face height.
     * @param data Raw texture data (expected to be tightly packed). If the type parameter is set 
     *   to TEXTURE_CUBE, then data is expected to be each face stored back contiguously within the
     *   array.
     * @param generateMipmaps True to generate a full mipmap chain, false otherwise.
     * @param type What type of Texture should be created.
     *
     * @return The new texture.
     * @script{create}
     */
    static Texture* create(Format format, unsigned int width, unsigned int height, const unsigned char* data, bool generateMipmaps = false, Type type = TEXTURE_2D);

    /**
     * Creates a texture object to wrap the specified pre-created native texture handle.
     *
     * The specified TextureHandle must represent a valid texture that has been created
     * on the underlying renderer and it should not be referenced by any other Texture
     * object. When the returned Texture object is destroyed, the passed in TextureHandle
     * will also be destroyed.
     *
     * @param handle Native texture handle.
     * @param width The width of the texture represented by 'handle'.
     * @param height The height of the texture represented by 'handle'.
     * @param format Optionally, the format of the texture represented by 'handle'.
     *      If the format cannot be represented by any of the Texture::Format values,
     *      use a value of UNKNOWN.
     *
     * @return The new texture.
     * @script{create}
     */
    static Texture* create(TextureHandle handle, int width, int height, Format format = UNKNOWN);

    /**
     * Set texture data to replace current texture image.
     * 
     * @param data Raw texture data (expected to be tightly packed). If the type parameter is set 
     *   to TEXTURE_CUBE, then data is expected to be each face stored back contiguously within the
     *   array.
     */
    void setData(const unsigned char* data);

    /**
     * Returns the path that the texture was originally loaded from (if applicable).
     *
     * @return The texture path, or an empty string if the texture was not loaded from file.
     */
    const char* getPath() const;

    /**
     * Gets the format of the texture.
     *
     * @return The texture format.
     */
    Format getFormat() const;

    /**
     * Gets the texture type.
     *
     * @return The texture type.
     */
    Type getType() const;

    /**
     * Gets the texture width.
     *
     * @return The texture width.
     */
    unsigned int getWidth() const;

    /**
     * Gets the texture height.
     *
     * @return The texture height.
     */
    unsigned int getHeight() const;

    /**
     * Generates a full mipmap chain for this texture if it isn't already mipmapped.
     */
    void generateMipmaps();

    /**
     * Determines if this texture currently contains a full mipmap chain.
     *
     * @return True if this texture is currently mipmapped, false otherwise.
     */
    bool isMipmapped() const;

    /**
     * Determines if this texture is a compressed texture.
     */
    bool isCompressed() const;

    /**
     * Returns the texture handle.
     *
     * @return The texture handle.
     */
    TextureHandle getHandle() const;

private:

    /**
     * Constructor.
     */
    Texture();

    /**
     * Copy constructor.
     */
    Texture(const Texture& copy);

    /**
     * Destructor.
     */
    virtual ~Texture();

    /**
     * Hidden copy assignment operator.
     */
    Texture& operator=(const Texture&);

    static Texture* createCompressedPVRTC(const char* path);

    static Texture* createCompressedDDS(const char* path);

    static GLubyte* readCompressedPVRTC(const char* path, Stream* stream, GLsizei* width, GLsizei* height, GLenum* format, unsigned int* mipMapCount, unsigned int* faceCount, GLenum faces[6]);

    static GLubyte* readCompressedPVRTCLegacy(const char* path, Stream* stream, GLsizei* width, GLsizei* height, GLenum* format, unsigned int* mipMapCount, unsigned int* faceCount, GLenum faces[6]);

    static int getMaskByteIndex(unsigned int mask);
    static GLint getFormatInternal(Format format);
    static GLenum getFormatTexel(Format format);
    static size_t getFormatBPP(Format format);

    std::string _path;
    TextureHandle _handle;
    Format _format;
    Type _type;
    unsigned int _width;
    unsigned int _height;
    bool _mipmapped;
    bool _cached;
    bool _compressed;
    Wrap _wrapS;
    Wrap _wrapT;
    Wrap _wrapR;
    Filter _minFilter;
    Filter _magFilter;

    GLint _internalFormat;
    GLenum _texelType;
    size_t _bpp;
};

}

#endif
