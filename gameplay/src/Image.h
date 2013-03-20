#ifndef IMAGE_H__
#define IMAGE_H__

#include "Ref.h"

namespace gameplay
{

/**
 * Represents an image (currently only supports PNG files).
 */
class Image : public Ref
{
public:

    /**
     * Defines the set of supported image formats.
     */
    enum Format
    {
        RGB,
        RGBA
    };

    /**
     * Creates an image from the image file at the given path.
     * 
     * @param path The path to the image file.
     * @return The newly created image.
     * @script{create}
     */
    static Image* create(const char* path);

    /**
     * Gets the image's raw pixel data.
     * 
     * @return The image's pixel data.
     * @script{ignore}
     */
    inline unsigned char* getData() const;

    /**
     * Gets the image's format.
     * 
     * @return The image's format.
     */
    inline Format getFormat() const;

    /**
     * Gets the height of the image.
     * 
     * @return The height of the image.
     */
    inline unsigned int getHeight() const;
        
    /**
     * Gets the width of the image.
     * 
     * @return The width of the image.
     */
    inline unsigned int getWidth() const;

private:

    /**
     * Constructor.
     */
    Image();

    /**
     * Destructor.
     */
    ~Image();

    /**
     * Hidden copy assignment operator.
     */
    Image& operator=(const Image&);

    unsigned char* _data;
    Format _format;
    unsigned int _height;
    unsigned int _width;
};

}

#include "Image.inl"

#endif
