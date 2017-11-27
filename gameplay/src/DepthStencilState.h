#pragma once

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
	 * Defines a comparison function.
	 */
	enum CompareFunc
	{
		COMPARE_FUNC_NEVER,
		COMPARE_FUNC_LESS,
		COMPARE_FUNC_EQUAL,
		COMPARE_FUNC_LESS_OR_EQUAL,
		COMPARE_FUNC_GREATER,
		COMPARE_FUNC_NOT_EQUAL,
		COMPARE_FUNC_GREATER_OR_EQUAL,
		COMPARE_FUNC_ALWAYS
	};

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
		 * 
		 */
		CompareFunc compareFunc = COMPARE_FUNC_NEVER;

		 /**
		  * Selects the bits of the unsigned integer 
		  * stencil values participating in the stencil test.
		  */
		unsigned char compareMask = 0xff;

		/**
		 * Selects the bits of the unsigned integer stencil values updated
		 * by the stencil test in the stencil framebuffer attachment
		 */
		unsigned char writeMask = 0xff;
        
    };

    /**
     * Whether depth testing is enabled.
     */
    bool depthEnabled = false;

    /**
     * The depth write mask.
     */
    bool  depthWrite = true;

    /**
     * The comparison function used in the depth test sampling.
     */
    CompareFunc depthFunc = COMPARE_FUNC_LESS;

    /**
     * Whether stencil testing is enabled.
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