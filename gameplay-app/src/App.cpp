#include "App.h"

namespace gameplay
{

App app;


App::App() :
	_vertShader(nullptr),
	_fragShader(nullptr),
	_uniformBuffer(nullptr),
	_vertexBuffer(nullptr),
	_indexBuffer(nullptr),
	_texture(nullptr),
	_sampler(nullptr),
	_descriptorSet(nullptr),
	_renderPipeline(nullptr),
	_renderPass(nullptr)
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
	_vertShader = graphics->createShader("color.vert");
	_fragShader = graphics->createShader("color.frag");

	// Create the vertex layout // todo: improve stride calculation with special offset value of -1
	std::vector<VertexLayout::Attribute> attributes(2);	
	attributes[0] = VertexLayout::Attribute(VertexLayout::SEMANTIC_POSITION,  
											Format::FORMAT_R32G32B32A32_FLOAT, 0, 0, 0);
	attributes[1] = VertexLayout::Attribute(VertexLayout::SEMANTIC_COLOR, 
											Format::FORMAT_R32G32B32_FLOAT, 0, 1, VertexLayout::toStride(attributes[0].format));
	VertexLayout vertexLayout(attributes.data(), attributes.size());

	// Create the vertex buffer
	std::vector<float> vertices = 
	{
         0.00f, -0.25f, 0.0f,  1.0f,    1.0f, 0.0f, 0.0f,
        -0.25f,  0.25f, 0.0f, 1.0f,   0.0f, 1.0f, 0.0f,
        0.25f,  0.25f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f
    };


	size_t vertexDataSize = sizeof(float) * vertices.size();
	size_t vertexStride = sizeof(float) * 7;
	_vertexBuffer = graphics->createVertexBuffer(vertexDataSize, vertexStride, true, nullptr);
	memcpy(_vertexBuffer->getHostMemory(), vertices.data(), vertexDataSize);


	// Acquired the initial render pass
	_renderPass = graphics->acquireNextSwapchainImage();


	// Create the render pipeline
	RasterizerState rasterizerState;
	ColorBlendState colorBlendState;
	DepthStencilState depthStencilState;
	_renderPipeline = graphics->createRenderPipeline(RenderPipeline::PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, vertexLayout, 
												     rasterizerState, colorBlendState, depthStencilState, 
												     _renderPass, _descriptorSet, _vertShader, nullptr, nullptr, nullptr, _fragShader);
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
	graphics->cmdBindVertexBuffer(commandBuffer, _vertexBuffer);
	graphics->cmdDraw(commandBuffer, 3, 0);
	graphics->cmdEndRenderPass(commandBuffer);
	graphics->cmdTransitionImage(commandBuffer, _renderPass->getColorAttachment(0), Texture::USAGE_COLOR_ATTACHMENT, Texture::USAGE_PRESENT);
	graphics->endCommands();

	graphics->submit(commandBuffer);
	graphics->present();

	graphics->waitIdle();
}

}
