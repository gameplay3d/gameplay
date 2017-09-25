#pragma once

namespace gameplay
{

/**
 * Defines an image buffer of RGB or RGBA color data.
 *
 * Supports loading from png files.
 */
class Image
{
public:

    /**
     * Defines the set of supported color formats.
     */
    enum Format
    {
        FORMAT_UNDEFINED,
        FORMAT_RGB,
        FORMAT_RGBA
    };

    /**
     * Constructor.
     */
    Image();

    /**
     * Destructor.
     */
    ~Image();

    /**
     * Creates a image from pixel data.
     * 
     * @param width The width of the image.
     * @param height The height of the image.
     * @param format The format of the image.
     */
    static std::shared_ptr<Image> create(size_t width, size_t height, Format format, unsigned char* pixelData = nullptr);

    /**
     * Creates a image from the specified image url.
     *
     * Currently supports loading .PNG files.
     *
     * @param url The url of the image file to be loaded.
     */
    static std::shared_ptr<Image> create(const std::string& url);

    /**
     * Gets the image height.
     *
     * @return The image height.
     */
    size_t getHeight() const;

    /**
     * Gets the image width.
     *
     * @return The image width.
     */
    size_t getWidth() const;

    /**
     * Gets the image format.
     *
     * @return The image format.
     */
    Format getFormat() const;

    /**
     * Gets the pixel stride.
     *
     * @return The pixel stride.
     */
    size_t getStride() const;

    /**
     * Gets direct access to the image pixel data.
     *
     * @return The image pixel data.
     */
    std::vector<unsigned char> getPixelData() const;

private:

    size_t _width;
    size_t _height;
    Format _format;
    size_t _stride;
    std::vector<unsigned char> _pixelData;
};

}

