#pragma once

#include <vulkan/vulkan.h>

namespace gameplay
{
    class Buffer;
    class Texture;
    class Sampler;

/**
 * Defines a set of resource descriptors to be bound to a RenderPipeline
 */
class DescriptorSet
{
    friend class Graphics;

public:

    /**
     * Defines an opaque resource descriptor.
     */
    class Descriptor
    {
    public:

        /**
         * Constructor.
         */
        Descriptor();

        /**
         * Destructor.
         */
        ~Descriptor();

        /**
         * Defines the type of resource.
         */
        enum class Type
        {
            eUndefined,
            eUniform,
            eTexture,
            eSampler
        };

        /**
         * Defines the shader stages.
         */
        enum class ShaderStages : uint32_t
        {
            eNone = 0x00000000,
            eVert = 0x00000001,
            eTesc = 0x00000002,
            eTese = 0x00000004,
            eGeom = 0x00000008,
            eFrag = 0x00000010
        };

        /**
         * The type of descriptor.
         */
        Type type = Type::eUndefined;

        /**
         * The descriptor binding index.
         */
        uint32_t binding = 0;

        /**
         * The number of resources.
         */
        size_t count = 0;

        /**
         * The stage in the shader that the resource is accessible from.
         */
        ShaderStages shaderStages = ShaderStages::eNone;

        std::shared_ptr<Buffer> uniforms[GP_GRAPHICS_DESCRIPTOR_ENTRIES_MAX];
        std::shared_ptr<Texture> textures[GP_GRAPHICS_DESCRIPTOR_ENTRIES_MAX];
        std::shared_ptr<Sampler>  samplers[GP_GRAPHICS_DESCRIPTOR_ENTRIES_MAX];
    
    };

    /**
     * Constructor.
     */
    DescriptorSet();

    /**
     * Constructor.
     */
    DescriptorSet(const Descriptor* descriptors, size_t descriptorCount);

    /**
     * Destructor.
     */
    ~DescriptorSet();

    /**
     * Gets the number of descriptors in the set.
     *
     * @return The number of descriptors in the set.
     */
    size_t getDescriptorCount() const;

    /**
     * Gets the descriptor at the specified index.
     *
     * @param index The index to access.
     */
    DescriptorSet::Descriptor getDescriptor(size_t index) const;

private:

    std::vector<Descriptor> _descriptors;
    VkDescriptorPool _descriptorPool;
    VkDescriptorSetLayout _descriptorSetLayout;
    VkDescriptorSet _descriptorSet;
};

}

GP_ENABLE_BITWISE_OPERATORS(gameplay::DescriptorSet::Descriptor::ShaderStages);
