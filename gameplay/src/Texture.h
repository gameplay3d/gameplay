#pragma once

#include "Format.h"

#include <vulkan/vulkan.h>

namespace gameplay
{

/**
 * Defines an allocation of formatted image data 
 * with a specified texture type and pixel format. 
 *
 * Texture objects are used as source textures for 
 * vertex, fragment, or compute functions, 
 * as well as to store graphics rendering output.
 */
class Texture
{
    friend class Graphics;
    friend class Activator;

public:

    /**
     * Defines the type of texture.
     */
    enum class Type
    {
        e1D = 0,
        e2D,
        e3D
    };

    /**
     * Defines the usage for a texture.
     */
    enum class Usage : uint32_t
    {
        eNone                   = 0x00000000,
        eTransferSrc            = 0x00000001,
        eTransferDst            = 0x00000002,
        eSampledImage           = 0x00000004,
        eStorage                = 0x00000008,
        eColorAttachment        = 0x00000010,
        eDepthStencilAttachment = 0x00000020,
        eResolveSrc             = 0x00000040,
        eResolveDst             = 0x00000080,
        ePresent                = 0x00000100
    };


    /**
     * Defines the supported sample counts for texture and used for storage operations.
     */
    enum class SampleCount : uint32_t
    {
        e1X     = 1,
        e2X     = 2,
        e4X     = 4,
        e8X     = 8,
        e16X    = 16
    };

    /**
     * Constructor.
     */
    Texture();

    /**
     * Destructor.
     */
    ~Texture();

    /**
     * Gets the type of texture.
     *
     * @return The type of texture.
     */
    Type getType() const;

     /**
      * Gets the width of the the texture.
      *
      * @return The width of the the texture.
      */
    size_t getWidth() const;

     /**
      * Gets the height of the texture.
      *
      * @return The height of the texture.
      */
    size_t getHeight() const;

     /**
      * Gets the depth of the texture.
      *
      * @return The depth of the texture.
      */
    size_t getDepth() const;

    /**
     * Gets the number of mip levels.
     * 
     * @return The number of mip levels.
     */
    size_t getMipLevels() const;

    /**
     * Gets the pixel format.
     *
     * @return the pixel format.
     */
    Format getPixelFormat() const;

    /**
     * Gets the usage for the texture.
     *
     * @return The usage for the texture.
     */
    Usage getUsage() const;

    /**
     * Gets the number of samples.
     *
     * @return The number of samples.
     */
    Texture::SampleCount getSampleCount() const;

    /**
     * Deterines if the buffer has visible access to host memory.
     *
     * @return true if the buffer has visible access to host memory, false if static
     */
    bool isHostVisible() const;

    /**
     * Gets the pointer to the mapped host memory.
     *
     * @return The pointer to the mapped host memory.
     */
    void* getHostMemory() const;

    /**
     * Is the texture owned by the host.
     *
     * @return true if host owns the texture and memory, false if swapchain owned.
     */
    bool isHostOwned() const;

private:

    Type _type;
    size_t _width;
    size_t _height;
    size_t _depth;
    size_t _mipLevels;
    Format _pixelFormat;
    Usage _usage;
    Texture::SampleCount _sampleCount;
    bool _hostVisible;
    void* _hostMemory;
    bool _hostOwned;
    VkDeviceMemory _deviceMemory;
    VkImage _image;
    VkImageView  _imageView;
    VkImageAspectFlags  _imageAspectFlags;
    VkDescriptorImageInfo _imageViewInfo;
};

}

GP_ENABLE_BITWISE_OPERATORS(gameplay::Texture::Usage);
