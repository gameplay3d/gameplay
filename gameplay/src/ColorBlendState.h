#pragma once

namespace gameplay
{

/**
 * Defines the blending state across color attachments
 * for a render pipeline.
 *
 * @see RenderPipeline
 */
class ColorBlendState
{
public:

    /**
     * Defines the framebuffer blending operations.
     */
    enum BlendOp
    {
        BLEND_OP_ADD,
        BLEND_OP_SUBSTRACT,
        BLEND_OP_REVERSE_SUBTRACT,
        BLEND_OP_MIN,
        BLEND_OP_MAX
    };

    /**
     * Defines the framebuffer blending factors.
     */
    enum BlendFactor
    {
        BLEND_FACTOR_ZERO,
        BLEND_FACTOR_ONE,
        BLEND_FACTOR_SRC_COLOR,
        BLEND_FACTOR_ONE_MINUS_SRC_COLOR,
        BLEND_FACTOR_DST_COLOR,
        BLEND_FACTOR_ONE_MINUS_DST_COLOR,
        BLEND_FACTOR_SRC_ALPHA,
        BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
        BLEND_FACTOR_DST_ALPHA,
        BLEND_FACTOR_ONE_MINUS_DST_ALPHA,
        BLEND_FACTOR_SRC_ALPHA_SATURATE,
        BLEND_FACTOR_SRC1_COLOR,
        BLEND_FACTOR_ONE_MINUS_SRC1_COLOR,
        BLEND_FACTOR_SRC1_ALPHA,
        BLEND_FACTOR_ONE_MINUS_SRC1_ALPHA
    };

    /**
     * Defines whether the final color values R, G, B and A
     * are written to the framebuffer attachment(s).
     */
    enum WriteMask : uint32_t
    {
        WRITE_MASK_RED = 0x00000001,
        WRITE_MASK_GREEN = 0x00000002,
        WRITE_MASK_BLUE = 0x00000004,
        WRITE_MASK_ALPHA = 0x00000008
    };

   /**
    * Whether blending is enabled for the corresponding color attachment.
    * If blending is not enabled, the source fragment's color for that
    * attachment is passed through unmodified.
    */
    bool blendEnabled = false;

    /**
     * Selects which blend factor is used to determine the source factors (Sr,Sg,Sb).
     */
    BlendFactor colorBlendSrc = BLEND_FACTOR_ONE;

    /**
     * Selects which blend factor is used to determine the destination factors (Dr,Dg,Db).
     */
    BlendFactor colorBlendDst = BLEND_FACTOR_ZERO;

    /**
     * Selects which blend operation is used to calculate the RGB values to write to the color attachment.
     */
    BlendOp colorBlendOp = BLEND_OP_ADD;

    /**
     * The WriteMask specifying which of the R, G, B, and/or A components are enabled for writing.
     */
    uint32_t colorWriteMask = 0xf;

    /**
     * Selects which blend factor is used to determine the source factor Sa.
     */
    BlendFactor alphaBlendSrc = BLEND_FACTOR_ONE;

    /**
     * Selects which blend factor is used to determine the destination factor Da.
     */
    BlendFactor alphaBlendDst = BLEND_FACTOR_ZERO;

    /**
     * Selects which blend operation is use to calculate the alpha values to write to the color attachment.
     */
    BlendOp alphaBlendOp = BLEND_OP_ADD;
};

}