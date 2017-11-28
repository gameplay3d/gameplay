#include "Base.h"
#include "ClearValue.h"

namespace gameplay
{

ClearValue::ClearValue()
{
}

ClearValue::ClearValue(float red, float green, float blue, float alpha) :
	color(ClearValue::Color(red, green, blue, alpha))
{
}

ClearValue::ClearValue(float depth, uint32_t stencil) :
	depthStencil(ClearValue::DepthStencil(depth, stencil))
{
}

ClearValue::Color::Color(float red, float green, float blue, float alpha) :
	red(red), green(green), blue(blue), alpha(alpha)
{
}

ClearValue::DepthStencil::DepthStencil(float depth, uint32_t stencil) :
	depth(depth), stencil(stencil)
{
}

}
