#ifndef IMAGE_H__
#define IMAGE_H__

namespace gameplay
{

/**
 * Represents an image (currently only supports PNG files).
 */
class Image
{
public:

    /**
     * Defines the set of supported image formats.
     */
    enum Format
    {
        LUMINANCE,
        RGB,
        RGBA
    };

    /**
     * Destructor.
     */
    ~Image();

    /**
     * Creates an image from the image file at the given path.
     * 
     * @param path The path to the image file.
     * @return The newly created image.
     */
    static Image* create(const char* path);

    /**
     * Creates a new empty image of the given format and size.
     *
     * @param format Image format.
     * @param width Image width.
     * @param height Image height.
     * @return The newly created image.
     */
    static Image* create(Format format, unsigned int width, unsigned int height);

    /**
     * Gets the image's raw pixel data.
     * 
     * @return The image's pixel data.
     */
    void* getData() const;

    /** 
     * Sets the image's raw pixel data.
     *
     * The passed in data MUST be at least width*height*bpp
     * bytes of data.
     */
    void setData(void* data);

    /**
     * Gets the image's format.
     * 
     * @return The image's format.
     */
    Format getFormat() const;

    /**
     * Gets the height of the image.
     * 
     * @return The height of the image.
     */
    unsigned int getHeight() const;
        
    /**
     * Gets the width of the image.
     * 
     * @return The width of the image.
     */
    unsigned int getWidth() const;

    /**
     * Returns the number of bytes per pixel for this image.
     *
     * @return The number of bytes per pixel.
     */
    unsigned int getBpp() const;

    /**
     * Saves the contents of the image as a PNG to the specified location.
     *
     * @param path Path to save to.
     */
    void save(const char* path);

private:

    /**
     * Hidden constructor.
     */
    Image();

    /**
     * Hidden copy constructor.
     */
    Image(const Image&);

    /**
     * Hidden copy assignment operator.
     */
    Image& operator=(const Image&);

    unsigned char* _data;
    Format _format;
    unsigned int _height;
    unsigned int _width;
    unsigned int _bpp;
};

}

#endif
