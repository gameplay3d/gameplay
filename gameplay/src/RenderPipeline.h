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
        PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP
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

protected:

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
};

}