#include "Base.h"
#include "Graphics.h"

namespace gameplay
{

Graphics* Graphics::_graphics = nullptr;

Graphics::Api Graphics::getApi()
{
	return _api;
}

size_t Graphics::computeMipLevels(size_t width, size_t height)
{
	if (width == 0 || height == 0)
        return 0;

    size_t result = 1;
    while (width > 1 || height > 1)
    {
        width >>= 1;
        height >>= 1;
        result++;
    }
    return result;
}

}
