#pragma once

#include "gameplay.h"

namespace gameplay
{

/**
 * Main application and game class.
 */
class App : public Game
{
public:

	/**
	 * Constructor.
	 */
	App();

	/**
	 * Destructor.
	 */
	~App();

	/**
	 * @see Game::onInitialize
	 */
	void onInitialize();

	/**
	 * @see Game::onFinalize
	 */
	void onFinalize();

	/**
	 * @see Game::onResize
	 */
    void onResize(size_t width, size_t height);

	/**
	 * @see Game::onUpdate
	 */
    void onUpdate(float elapsedTime);

	std::shared_ptr<Shader> _vertShader;
	std::shared_ptr<Shader> _fragShader;
	std::shared_ptr<DescriptorSet> _descriptorSet;
	std::shared_ptr<Buffer> _vertexBuffer;
	std::shared_ptr<Buffer> _indexBuffer;
	std::shared_ptr<Texture> _texture;
	std::shared_ptr<Sampler> _sampler;
	std::shared_ptr<RenderPipeline> _renderPipeline;
	std::shared_ptr<RenderPass> _renderPass;
};

}
