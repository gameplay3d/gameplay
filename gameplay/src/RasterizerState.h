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
    enum FillMode
    {
        FILL_MODE_SOLID,
        FILL_MODE_WIREFRAME
    };

    /**
     * Defines the triangle culling mode.
     */
    enum CullMode
    {
        CULL_MODE_NONE,
        CULL_MODE_BACK,
        CULL_MODE_FRONT
    };

    /**
     * Define the polygon front-facing orientation.
     */
    enum FrontFace : uint32_t
    {
        FRONT_FACE_CW,
        FRONT_FACE_CCW
    };

    /**
     * The polygon fill mode.
     */
    FillMode fillMode = FILL_MODE_SOLID;

    /**
     * The triangle culling mode.
     */
    CullMode cullMode = CULL_MODE_NONE;

    /**
     * The polygon front-facing orientation.
     */
    FrontFace frontFace = FRONT_FACE_CW;

    /**
     * Whether to bias fragment depth values.
     */
    uint32_t depthBias = 0;

    /**
     * The maximum (or minimum) depth bias of a fragment.
     */
    float depthBiasClamp = 0.0f;

    /**
     * Whether to clip the fragment’s depth values instead of clipping primitives to the z planes of the frustum.
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
    Texture::SampleCount multiSampleCount = Texture::SAMPLE_COUNT_1X;
};

}