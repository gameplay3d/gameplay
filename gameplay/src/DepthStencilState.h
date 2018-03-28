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
    enum class StencilOp
    {
        eKeep,
        eZero,
        eReplace,
        eIncrementAndClamp,
        eDecrementAndClamp,
        eInvert,
        eIncrementAndWrap,
        eDecrementAndWrap
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
        StencilOp failOp = StencilOp::eKeep;

        /**
         * The action performed on samples that pass the stencil test and fail the depth test.
         */
        StencilOp passOp = StencilOp::eKeep;

        /**
         * The action performed on samples that fail both the depth and stencil tests.
         */
        StencilOp depthFailOp = StencilOp::eKeep;

        /**
         * The depth compare function.
         */
        Sampler::CompareFunc compareFunc = Sampler::CompareFunc::eNever;

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
    Sampler::CompareFunc depthFunc = Sampler::CompareFunc::eLessOrEqual;

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
