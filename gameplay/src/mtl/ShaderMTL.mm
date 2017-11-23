#include "Base.h"
#include "ShaderMTL.h"

namespace gameplay
{

ShaderMTL::ShaderMTL() :
	Shader(),
	_device(nullptr),
    _library(nullptr),
	_function(nullptr)
{
}

ShaderMTL::ShaderMTL(id<MTLDevice> device, id<MTLLibrary> library, id<MTLFunction> function) :
	Shader(),
	_device(device),
	_library(library),
    _function(function)
{
}

ShaderMTL::~ShaderMTL()
{
}

}