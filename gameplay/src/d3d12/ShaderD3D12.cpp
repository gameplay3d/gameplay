#include "Base.h"
#include "ShaderD3D12.h"

namespace gameplay
{

ShaderD3D12::ShaderD3D12() :
	Shader(),
	_device(nullptr),
	_shader(nullptr)
{
}

ShaderD3D12::ShaderD3D12(ID3D12Device* device, ID3DBlob* shader) :
	Shader(),
	_device(device),
	_shader(shader)
{
}

ShaderD3D12::~ShaderD3D12()
{
}

}