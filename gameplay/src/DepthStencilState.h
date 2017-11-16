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
     * Defines the depth write mask.
     */
    enum WriteMask
    {
        WRITE_MASK_ZERO,
        WRITE_MASK_ALL
    };

    /**
     * Defines the stencil tests.
     */
    class DepthStencilTest
    {
    public:

        /**
         * The action performed on samples that fail the stencil test.
         */
        StencilOp stencilFail = STENCIL_OP_KEEP;

        /**
         * The action performed on samples that fail both the depth and stencil tests.
         */
        StencilOp depthStencilFail = STENCIL_OP_KEEP;

        /**
         * The action performed on samples that pass the stencil test and fail the depth test.
         */
        StencilOp stencilPass = STENCIL_OP_KEEP;

        /**
         * The comparison function used in the stencil test.
         */
        CompareFunc stencilFunc = COMPARE_FUNC_ALWAYS;
    };

    /**
     * Whether depth testing is enabled.
     */
    bool depthEnable = false;

    /**
     * The depth write mask.
     */
    WriteMask WriteMask = WRITE_MASK_ALL;

    /**
     * The comparison function used in the depth test sampling.
     */
    CompareFunc depthFunc = COMPARE_FUNC_LESS;

    /**
     * Whether stencil testing is enabled.
     */
    bool stencilEnable = false;

    /**
     * Selects the bits of the unsigned integer 
	 * stencil values participating in the stencil test.
     */
    unsigned char stencilReadMask = 0xff;

    /**
     * Selects the bits of the unsigned integer stencil values updated
	 * by the stencil test in the stencil framebuffer attachment
     */
    unsigned char stencilWriteMask = 0xff;

    /**
     * Stencil state of the front stencil test.
     */
    StencilOp stencilStateFront = STENCIL_OP_KEEP;

    /**
     * Stencil state of the back stencil test.
     */
    StencilOp stencilStateBack = STENCIL_OP_KEEP;
};

}