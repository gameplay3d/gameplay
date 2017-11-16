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
     * Defines the polygon rasterization mode.
     */
    enum PolygonMode
    {
        POLYGON_MODE_SOLID,
        POLYGON_MODE_WIREFRAME
    };

    /**
     * Defines the triangle culling mode.
     */
    enum CullMode
    {
        CULL_MODE_NONE,
        CULL_MODE_BACK,
        CULL_MODE_FRONT,
        CULL_MODE_BOTH
    };

    /**
     * Define the polygon front-facing orientation.
     */
    enum FrontFace
    {
        FRONT_FACE_CCW = 0,
        FRONT_FACE_CW
    };

    /**
     * The polygon rasterization mode.
     */
    PolygonMode polygonMode = POLYGON_MODE_SOLID;

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
    unsigned int depthBias = 0;

    /**
     * The maximum (or minimum) depth bias of a fragment.
     */
    float depthBiasClamp = 0.0f;

    /**
     * Whether to clip the fragment’s depth values instead of clipping primitives to the z planes of the frustum.
     */
    bool depthClipEnabled = true;

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