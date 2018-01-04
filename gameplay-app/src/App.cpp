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

    // Testing immediate mode rendering
	Graphics* graphics = Graphics::getGraphics();
	
	// Create the vertex and fragment shaders
	_vertShader = graphics->createShader("nuklear.vert");
	_fragShader = graphics->createShader("nuklear.frag");

	// Create the vertex layout
	std::vector<VertexLayout::Attribute> attributes(2);	
	attributes[0] = VertexLayout::Attribute(VertexLayout::SEMANTIC_POSITION, Format::FORMAT_R32G32B32A32_FLOAT, 0, 0, 0);
	attributes[1] = VertexLayout::Attribute(VertexLayout::SEMANTIC_TEXCOORD0, Format::FORMAT_R32G32_FLOAT, 0, 1, VertexLayout::toStride(attributes[0].format));
	VertexLayout vertexLayout(attributes.data(), attributes.size());
	
	// Create the vertex buffer
	std::vector<float> vertices = 
	{
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f,
         0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f,
         0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f,
    };
	size_t vertexDataSize = sizeof(float) * vertices.size();
	size_t vertexStride = sizeof(float) * 6;
	_vertexBuffer = graphics->createVertexBuffer(vertexDataSize, vertexStride, true, vertices.data());

	// Create the index buffer
	std::vector<uint16_t> indices = 
	{
        0, 1, 2,
        0, 2, 3
    };
	size_t indexDataSize = sizeof(uint16_t) * indices.size();
	_indexBuffer = graphics->createIndexBuffer(indexDataSize, sizeof(uint32_t), true, indices.data());

	// Read an image, texture then upload
	std::shared_ptr<Image> image = Image::create("res/images/logo_white.png");
	_texture = graphics->createTexture2d(image->getWidth(), image->getHeight(), 1, 
										 Format::FORMAT_R8G8B8A8_UNORM, 
										 Texture::USAGE_SAMPLED_IMAGE, 
									     Texture::SAMPLE_COUNT_1X, true, image->getData());

	_sampler = graphics->createSampler(Sampler::FILTER_LINEAR, Sampler::FILTER_LINEAR, Sampler::FILTER_LINEAR,
									   Sampler::ADDRESS_MODE_WRAP, Sampler::ADDRESS_MODE_WRAP, Sampler::ADDRESS_MODE_WRAP,
									   Sampler::BORDER_COLOR_BLACK_OPAQUE, false, Sampler::COMPARE_FUNC_NEVER,
									   false, 1.0f, 0.0f, 0.0f, 0.0f);

	// Acquired the initial render pass
	_renderPass = graphics->acquireNextSwapchainImage();

	// Create the descriptor set
	std::vector<DescriptorSet::Descriptor> descriptors(2);
	descriptors[0].type = DescriptorSet::Descriptor::TYPE_TEXTURE;
	descriptors[0].count = 1;
	descriptors[0].binding = 0;
	descriptors[0].shaderStages = DescriptorSet::Descriptor::SHADER_STAGE_FRAG;
	descriptors[0].textures[0] = _texture;
	descriptors[1].type = DescriptorSet::Descriptor::TYPE_SAMPLER;
	descriptors[1].count = 1;
	descriptors[1].binding = 1;
	descriptors[1].shaderStages = DescriptorSet::Descriptor::SHADER_STAGE_FRAG;
	descriptors[1].samplers[0] = _sampler;
	_descriptorSet = graphics->createDescriptorSet(descriptors.data(), descriptors.size());

	// Create the render pipeline
	RasterizerState rasterizerState;
	ColorBlendState colorBlendState;
	DepthStencilState depthStencilState;
	_renderPipeline = graphics->createRenderPipeline(RenderPipeline::PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, vertexLayout, 
												     rasterizerState, colorBlendState, depthStencilState, 
												     _renderPass, _descriptorSet,
												     _vertShader, nullptr, nullptr, nullptr, _fragShader);
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

	Graphics* graphics = Graphics::getGraphics();

	_renderPass = graphics->acquireNextSwapchainImage();

	std::shared_ptr<CommandBuffer> commandBuffer = graphics->beginCommands();
	graphics->cmdTransitionImage(commandBuffer, _renderPass->getColorAttachment(0), Texture::USAGE_PRESENT, Texture::USAGE_COLOR_ATTACHMENT);
	graphics->cmdSetViewport(commandBuffer, 0, 0, graphics->getWidth(), graphics->getHeight(), 0.0f, 1.0f);
	graphics->cmdSetScissor(commandBuffer, 0, 0, graphics->getWidth(), graphics->getHeight());
	graphics->cmdBeginRenderPass(commandBuffer, _renderPass);
	graphics->cmdClearColor(commandBuffer, 0.0f, 0.0f, 0.0f, 1.0f, 0);
	graphics->cmdBindRenderPipeline(commandBuffer, _renderPipeline);
	graphics->cmdBindIndexBuffer(commandBuffer, _indexBuffer);
	graphics->cmdBindVertexBuffer(commandBuffer, _vertexBuffer);
	graphics->cmdBindDescriptorSet(commandBuffer, _renderPipeline, _descriptorSet);
	graphics->cmdDrawIndexed(commandBuffer, 6, 0);
	graphics->cmdEndRenderPass(commandBuffer);
	graphics->endCommands();

	graphics->submit(commandBuffer);
	graphics->present();
}

}
