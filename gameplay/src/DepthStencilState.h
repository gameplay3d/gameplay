#pragma once

#include "Sampler.h"

namespace gameplay
{

/**
 * Defines the depth stencil state for the render pipeline.
 *
 * @see RenderPipeline
 */
class DepthStencilState
{
public:

    /**
     * Defines the operation to the stored stencil value
     * if this or certain subsequent tests fail or pass,
     */
    enum StencilOp
    {
        STENCIL_OP_KEEP,
        STENCIL_OP_ZERO,
        STENCIL_OP_REPLACE,
        STENCIL_OP_INCREMENT_AND_CLAMP,
        STENCIL_OP_DECREMENT_AND_CLAMP,
        STENCIL_OP_INVERT,
        STENCIL_OP_INCREMENT_AND_WRAP,
        STENCIL_OP_DECREMENT_AND_WRAP
    };

    /**
     * Defines the stencil tests.
     */
	class StencilOpState
    {
    public:

        /**
         * The action performed on samples that fail the stencil test.
         */
        StencilOp failOp = STENCIL_OP_KEEP;

        /**
         * The action performed on samples that pass the stencil test and fail the depth test.
         */
        StencilOp passOp = STENCIL_OP_KEEP;

		/**
         * The action performed on samples that fail both the depth and stencil tests.
         */
        StencilOp depthFailOp = STENCIL_OP_KEEP;

		/**
		 * The depth compare function.
		 */
		Sampler::CompareFunc compareFunc = Sampler::COMPARE_FUNC_NEVER;

		 /**
		  * Selects the bits of the stencil values participating 
		  * in the stencil test.
		  */
		unsigned char compareMask = 0xff;

		/**
		 * Selects the bits of the uint32_t stencil values updated
		 * by the stencil test in the stencil framebuffer attachment
		 */
		unsigned char writeMask = 0xff;
        
    };

    /**
     * Determines if depth testing is enabled.
     */
    bool depthEnabled = true;

    /**
     * The depth write mask.
     */
    bool  depthWrite = true;

    /**
     * The comparison function used in the depth test sampling.
     */
    Sampler::CompareFunc depthFunc = Sampler::COMPARE_FUNC_LESS_OR_EQUAL;

    /**
     * Determines if stencil testing is enabled.
     */
    bool stencilEnabled = false;

    /**
     * Stencil state of the front stencil test.
     */
    StencilOpState stencilOpStateFront;

    /**
     * Stencil state of the back stencil test.
     */
    StencilOpState stencilOpStateBack;
};

}