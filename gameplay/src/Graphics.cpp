#include "Base.h"
#include "Graphics.h"

namespace gameplay
{

Graphics* Graphics::_graphics = nullptr;

Graphics::Api Graphics::getApi()
{
    return _api;
}

}
