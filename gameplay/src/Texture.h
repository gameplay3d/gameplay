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

public:

    /**
     * Defines the type of texture.
     */
    enum Type
    {
        TYPE_1D = 0,
        TYPE_2D,
        TYPE_3D
    };

    /**
     * Defines the usage for a texture.
     */
    enum Usage
    {
        USAGE_UNDEFINED = 0x00000000,
        USAGE_TRANSFER_SRC = 0x00000001,
        USAGE_TRANSFER_DST = 0x00000002,
        USAGE_SAMPLED_IMAGE = 0x00000004,
        USAGE_STORAGE = 0x00000008,
        USAGE_COLOR_ATTACHMENT = 0x00000010,
        USAGE_DEPTH_STENCIL_ATTACHMENT = 0x00000020,
        USAGE_RESOLVE_SRC = 0x00000040,
        USAGE_RESOLVE_DST = 0x00000080,
        USAGE_PRESENT = 0x00000100
    };

    /**
     * Defines the supported sample counts for texture and used for storage operations.
     */
    enum SampleCount : uint32_t
    {
        SAMPLE_COUNT_1X = 1,
        SAMPLE_COUNT_2X = 2,
        SAMPLE_COUNT_4X = 4,
        SAMPLE_COUNT_8X = 8,
        SAMPLE_COUNT_16X = 16
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