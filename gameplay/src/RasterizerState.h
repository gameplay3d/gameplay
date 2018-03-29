#pragma once

#include "Texture.h"

namespace gameplay
{

/**
 * Defines the state which a primitive is rasterized
 * to a two-dimensional image in a render pipeline.
 */
class RasterizerState
{
public:

    /**
     * Defines the mode in which polygons are filled.
     */
    enum class FillMode
    {
        eSolid,
        eWireframe
    };

    /**
     * Defines the triangle culling mode.
     */
    enum class CullMode
    {
        eNone,
        eBack,
        eFront
    };

    /**
     * Define the polygon front-facing orientation.
     */
    enum class FrontFace
    {
        eCCW,
        eCW
    };

    /**
     * The polygon fill mode.
     */
    FillMode fillMode = FillMode::eSolid;

    /**
     * The triangle culling mode.
     */
    CullMode cullMode = CullMode::eNone;

    /**
     * The polygon front-facing orientation.
     */
    FrontFace frontFace = FrontFace::eCCW;

    /**
     * Whether to bias fragment depth values.
     */
    uint32_t depthBias = 0;

    /**
     * The maximum (or minimum) depth bias of a fragment.
     */
    float depthBiasClamp = 0.0f;

    /**
     * Whether to clip the fragment's depth values instead of clipping primitives to the z planes of the frustum.
     */
    bool depthClipEnabled = false;

    /**
     * The width of rasterized line segments.
     */
    float lineWidth = 1.0f;

    /**
     * Whether multi-sampling is enabled.
     */
    bool multiSampleEnabled = false;

    /**
     * The texture multi-sampling count.
     */
    Texture::SampleCount multiSampleCount = Texture::SampleCount::e1X;
};

}
