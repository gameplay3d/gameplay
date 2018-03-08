#pragma once

#include "VertexLayout.h"
#include "RasterizerState.h"
#include "ColorBlendState.h"
#include "DepthStencilState.h"
#include "DescriptorSet.h"
#include "RenderPass.h"
#include "Shader.h"

namespace gameplay
{

/**
 * Defines a pipeline for processing for rendering programs
 */
class RenderPipeline
{
	friend class Graphics;

public:

	/**
	 * Defines the primitive topology.
	 */
	enum PrimitiveTopology
	{
		PRIMITIVE_TOPOLOGY_POINT_LIST,
        PRIMITIVE_TOPOLOGY_LINE_LIST,
        PRIMITIVE_TOPOLOGY_LINE_STRIP,
		PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
		PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP,
	};

	/**
	 * Constructor.
	 */
	RenderPipeline();

	/**
	 * Constructor.
	 */
	RenderPipeline(PrimitiveTopology primitiveTopology,
				   VertexLayout vertexLayout,
				   RasterizerState rasterizerState,
				   ColorBlendState colorBlendState,
				   DepthStencilState depthStencilState,
				   std::shared_ptr<RenderPass> renderPass,
				   std::shared_ptr<DescriptorSet> descriptorSet,
				   std::shared_ptr<Shader> vertShader,
				   std::shared_ptr<Shader> tescShader,
				   std::shared_ptr<Shader> teseShader,
				   std::shared_ptr<Shader> geomShader,
				   std::shared_ptr<Shader> fragShader);

	/**
	 * Destructor.
	 */
	~RenderPipeline();

	/**
	 * Gets the primitive topology.
	 *
	 * @return The primitive topology.
	 */
	PrimitiveTopology getPrimitiveTopology() const;

	/**
	 * Gets the vertex layout.
	 *
	 * @return The vertex layout.
	 */
	VertexLayout getVertexLayout() const;

	/**
	 * Gets the rasterizer state.
	 *
	 * @return The rasterizer state.
	 */
	RasterizerState getRasterizerState() const;

	/**
	 * Gets the color blend state.
	 *
	 * @return The color blend state.
	 */
	ColorBlendState getColorBlendState() const;

	/**
	 * Gets the depth stencil state.
	 *
	 * @return The depth stencil state.
	 */
	DepthStencilState getDepthStencilState() const;

	/**
	 * Gets the render pass.
	 *
	 * @return The render pass.
	 */
	std::shared_ptr<RenderPass> getRenderPass() const;

	/**
	 * Gets the descriptor set.
	 *
	 * @return The  descriptor set.
	 */
	std::shared_ptr<DescriptorSet> getDescriptorSet() const;

	/**
	 * Gets the vertex shader.
	 *
	 * @return The vertex shader.
	 */
	std::shared_ptr<Shader> getVertexShader() const;

	/**
	 * Gets the tessellation control shader.
	 *
	 * @return The tessellation control shader.
	 */
	std::shared_ptr<Shader> getTessellationControlShader() const;

	/**
	 * Gets the tessellation evaluation shader.
	 *
	 * @return The tessellation evaluation shader.
	 */
	std::shared_ptr<Shader> getTessellationEvaluationShader() const;

	/**
	 * Gets the geometry shader.
	 *
	 * @return The geometry shader.
	 */
	std::shared_ptr<Shader> getGeometryShader() const;

	/**
	 * Gets the fragment shader.
	 *
	 * @return The fragment shader.
	 */
	std::shared_ptr<Shader> getFragmentShader() const;

private:

	PrimitiveTopology _primitiveTopology;
	VertexLayout _vertexLayout;
	RasterizerState _rasterizerState;
	ColorBlendState _colorBlendState;
	DepthStencilState _depthStencilState;
	std::shared_ptr<RenderPass> _renderPass;
	std::shared_ptr<DescriptorSet> _descriptorSet;
	std::shared_ptr<Shader> _vertShader;
	std::shared_ptr<Shader> _tescShader;
	std::shared_ptr<Shader> _teseShader;
	std::shared_ptr<Shader> _geomShader;
	std::shared_ptr<Shader> _fragShader;

    VkPipelineLayout _pipelineLayout;
    VkPipeline _pipeline;
};

}
