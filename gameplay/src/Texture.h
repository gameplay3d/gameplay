/*
 * Texture.h
 */

#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "Ref.h"

namespace gameplay
{

/**
 * Represents a texture.
 */
class Texture : public Ref
{
    friend class AssetManager;

public:

    /**
     * Defines the set of supported texture formats.
     */
    enum Format
    {
        ALPHA = GL_ALPHA,
        RGB888 = GL_RGB,
        RGBA8888 = GL_RGBA,
        LUMINANCE = GL_LUMINANCE,
        LUMINANCE_ALPHA = GL_LUMINANCE_ALPHA
    };

    /**
     * Creates a texture from the given image resource.
     *
     * @param path The image resource path.
     * @param generateMipmaps true to auto-generate a full mipmap chain, false otherwise.
     * 
     * @return The new texture, or NULL if the texture could not be loaded/created.
     */
    static Texture* create(const char* path, bool generateMipmaps = false);

    /**
     * Creates a texture from the given texture data.
     */
    static Texture* create(Format format, unsigned int width, unsigned int height, unsigned char* data, bool generateMipmaps = false);

    /**
     * Returns the texture width.
     */
    unsigned int getWidth() const;

    /**
     * Returns the texture height.
     */
    unsigned int getHeight() const;

    /**
     * Sets the wrap mode for this texture.
     *
     * @param repeatS True if the texture should be repeated horizontally, or false if it should be clamped.
     * @param repeatT True if the texture should be repeated vertically, or false if it should be clamped.
     */
    void setWrapMode(bool repeatS, bool repeatT);

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

    static Texture* loadPNG(const char* path, bool generateMipmaps);

    std::string _path;
    TextureHandle _handle;
    unsigned int _width;
    unsigned int _height;
    bool _cached;
};

}

#endif
