#pragma once

#include <vulkan/vulkan.h>

namespace gameplay
{

/**
 * Defines an allocation of unformatted memory 
 * that can contain any type of data. 
 *
 * Buffers are often used for vertex, shader 
 * and compute state data.
 */
class Buffer
{
    friend class Graphics;
    
public:

    /**
     * Defines the usage of Buffer.
     */
    enum class Usage : uint32_t
    {
        eVertex  =  0x00000000,
        eIndex   =  0x00000001,
        eUniform =  0x00000002
    };

    /**
     * Constructor.
     */
    Buffer();

    /**
     * Destructor.
     */
    ~Buffer();

    /**
     * Gets the usage of buffer.
     *
     * @return The usage of buffer.
     */
    Usage getUsage() const;

    /**
     * Gets the buffer size (in bytes).
     *
     * @return the buffer size (in bytes).
     */
    size_t getSize() const;

    /**
     * Gets the stride of one element in the buffer.
     */
    size_t getStride() const;

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

private:

    Usage _usage;
    size_t _size;
    size_t _stride;
    bool _hostVisible;
    void* _hostMemory;
    VkDeviceMemory _deviceMemory;
    VkBuffer _buffer;
    VkDescriptorBufferInfo _bufferView;
};

}

GP_ENABLE_BITWISE_OPERATORS(gameplay::Buffer::Usage);
