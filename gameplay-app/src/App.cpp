#include "App.h"

namespace gameplay
{

App app;


App::App()
{
}

App::~App()
{
}

void App::onInitialize()
{
	Game::onInitialize();

	/* Testing 

    // Testing immediate mode rendering
	Graphics* graphics = Graphics::getGraphics();
	
	// Create a command pool
	_commandPool = graphics->createCommandPool();

	// Create command buffers for each swap image
	_commandBuffers.resize(GP_GRAPHICS_SWAPCHAIN_IMAGE_COUNT);
	for (size_t i = 0; i < GP_GRAPHICS_SWAPCHAIN_IMAGE_COUNT; i++)
	{
		std::shared_ptr<CommandBuffer> commandBuffer = _commandPool->createCommandBuffer();
		_commandBuffers.push_back(commandBuffer);
	}

	// Create the vertex and fragment shaders
	_vertShader = graphics->createShader("texture.vert");
	_fragShader = graphics->createShader("texture.frag");

	// Create the descriptor set
	std::vector<DescriptorSet::Descriptor> descriptors(2);
	descriptors[0].type = DescriptorSet::Descriptor::TYPE_TEXTURE;
	descriptors[0].count = 1;
	descriptors[0].binding = 0;
	descriptors[0].shaderStages = DescriptorSet::Descriptor::SHADER_STAGE_FRAG;
	descriptors[1].type = DescriptorSet::Descriptor::TYPE_SAMPLER;
	descriptors[1].count = 1;
	descriptors[1].binding = 1;
	descriptors[1].shaderStages = DescriptorSet::Descriptor::SHADER_STAGE_FRAG;
	_descriptorSet = graphics->createDescriptorSet(descriptors.data(), descriptors.size());

	// Create the vertex layout
	std::vector<VertexLayout::Attribute> attributes(2);	
	attributes[0] = VertexLayout::Attribute(VertexLayout::SEMANTIC_POSITION, Format::FORMAT_R32G32B32A32_FLOAT, 0, 0, 0);
	attributes[1] = VertexLayout::Attribute(VertexLayout::SEMANTIC_TEXCOORD0, Format::FORMAT_R32G32_FLOAT, 0, 1, VertexLayout::toStride(attributes[0].format));
	
	// Create the vertex buffer
	std::vector<float> vertexData = 
	{
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f,
         0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f,
         0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f,
    };
	size_t vertexDataSize = sizeof(float) * vertexData.size();
	size_t vertexStride = sizeof(float) * 6;
	_vertexBuffer = graphics->createVertexBuffer(vertexDataSize, vertexStride, true);
	memcpy(_vertexBuffer->getHostMemory(), vertexData.data(), vertexDataSize);

	// Create the index buffer
	std::vector<uint16_t> indexData = 
	{
        0, 1, 2,
        0, 2, 3
    };
	size_t indexDataSize = sizeof(uint16_t) * indexData.size();
	_indexBuffer = graphics->createIndexBuffer(indexDataSize, Graphics::INDEX_FORMAT_UINT32, true);
	memcpy(_indexBuffer->getHostMemory(), indexData.data(), indexDataSize);

	// Read an image, texture then upload
	std::shared_ptr<Image> image = Image::create("res/images/logo_powered_black.png");
	_texture = graphics->createTexture2d(image->getWidth(), image->getHeight(), 1, 
										 Format::FORMAT_R8G8B8A8_UNORM, 
										 Texture::USAGE_SAMPLED_IMAGE, 
									     Texture::SAMPLE_COUNT_1X, ClearValue(0, 0, 0, 1), true);
	*/

}

void App::onFinalize()
{
	Game::onFinalize();
}

void App::onResize(size_t width, size_t height)
{
	Game::onResize(width, height);
}

void App::onUpdate(float elapsedTime)
{
	Game::onUpdate(elapsedTime);
}

}
