#pragma once

namespace gameplay
{

/**
 * Defines the clear value for clearing either 
 * color or depth/stencil values.
 */
class ClearValue
{
public:
	struct Color
    {
        float red;
        float green;
        float blue;
        float alpha;
    };
    struct DepthStencil
    {
        float depth;
        unsigned int stencil;
    };

    union 
    {
		Color color;
		DepthStencil depthStencil;
    };
};

}
