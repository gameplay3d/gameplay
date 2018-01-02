#pragma once

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
		 * Constructor.
		 */
		Descriptor(const Descriptor& copy);

		/**
		 * Destructor.
		 */
		~Descriptor();

		/**
		 * Defines the type of resource.
		 */
		enum Type
		{
			TYPE_UNDEFINED,
			TYPE_UNIFORM,
			TYPE_TEXTURE,
			TYPE_SAMPLER
		};

		/**
		 * Defines the shader stages.
		 */
		enum ShaderStages : uint32_t
		{
			SHADER_STAGE_VERT,
			SHADER_STAGE_TESC,
			SHADER_STAGE_TESE,
			SHADER_STAGE_GEOM,
			SHADER_STAGE_FRAG
		};

		/**
		 * The type of descriptor.
		 */
		Type type = TYPE_UNDEFINED;

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
		ShaderStages shaderStages = SHADER_STAGE_VERT;

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
	const DescriptorSet::Descriptor& getDescriptor(size_t index) const;

protected:

	std::vector<Descriptor> _descriptors;
};

}