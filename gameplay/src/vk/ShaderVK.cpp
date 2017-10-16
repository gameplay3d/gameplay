#include "Base.h"
#include "ShaderVK.h"

namespace gameplay
{

ShaderVK::ShaderVK() :
	Shader(),
	_device(nullptr),
	_shader(nullptr)
{
}

ShaderVK::ShaderVK(VkDevice device, VkShaderModule shader) :
	Shader(),
	_device(device),
	_shader(shader)
{
}

ShaderVK::~ShaderVK()
{
}

}