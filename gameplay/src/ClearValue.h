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

	/**
	 * Constuctor.
	 */
	ClearValue();

	/**
	 * Constuctor.
	 */
	ClearValue(float red, float green, float blue, float alpha);
	
	/**
	 * Constuctor.
	 */
	ClearValue(float depth, uint32_t stencil);

	struct Color
    {
		Color(float red, float green, float blue, float alpha);
		float red = 0.0f;
		float green = 0.0f;
		float blue = 0.0f;
		float alpha = 1.0f;
	};
    struct DepthStencil
    {
		DepthStencil(float depth, uint32_t stencil);
		float depth = 0.0f;
		uint32_t stencil = 0;
	};

    union 
    {
		Color color;
		DepthStencil depthStencil;
    };
};

}
