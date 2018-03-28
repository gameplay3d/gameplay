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
    enum class BlendOp
    {
        eAdd,
        eSubtract,
        eReverseSubstact,
        eMin,
        eMax
    };

    /**
     * Defines the framebuffer blending factors.
     */
    enum class BlendFactor
    {
        eZero,
        eOne,
        eSrcColor,
        eOneMinusSrcColor,
        eDstColor,
        eOneMinusDstColor,
        eSrcAlpha,
        eOneMinuseSrcAlpha,
        eDstAlpha,
        eOneMinuseDstAlpha,
        eSrcAlphaSaturate,
        eSrc1Color,
        eOneMinusSrc1Color,
        eSrc1Alpha,
        eOneMinuseSrc1Alpha
    };

    /**
     * Defines whether the final color values R, G, B and A
     * are written to the framebuffer attachment(s).
     */
    enum class WriteMask : uint32_t
    {
        eRed    = 0x00000001,
        eGreen  = 0x00000002,
        eBlue   = 0x00000004,
        eAlpha  = 0x00000008
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
    BlendFactor colorBlendSrc = BlendFactor::eOne;

    /**
     * Selects which blend factor is used to determine the destination factors (Dr,Dg,Db).
     */
    BlendFactor colorBlendDst = BlendFactor::eZero;

    /**
     * Selects which blend operation is used to calculate the RGB values to write to the color attachment.
     */
    BlendOp colorBlendOp = BlendOp::eAdd;

    /**
     * The WriteMask specifying which of the R, G, B, and/or A components are enabled for writing.
     */
    uint32_t colorWriteMask = 0xf;

    /**
     * Selects which blend factor is used to determine the source factor Sa.
     */
    BlendFactor alphaBlendSrc = BlendFactor::eOne;

    /**
     * Selects which blend factor is used to determine the destination factor Da.
     */
    BlendFactor alphaBlendDst = BlendFactor::eZero;

    /**
     * Selects which blend operation is use to calculate the alpha values to write to the color attachment.
     */
    BlendOp alphaBlendOp = BlendOp::eAdd;
};

}

GP_ENABLE_BITWISE_OPERATORS(gameplay::ColorBlendState::WriteMask);
