#pragma once

#include "Shader.h"
#include <dxgi1_5.h>
#include <d3d12.h>
#include <d3dcompiler.h>

namespace gameplay
{

/**
 * Direct3D 12 impl of a Shader.
 */
class ShaderD3D12 : public Shader
{
public:

    /**
     * Constructor.
     */
	ShaderD3D12();

    /**
     * Constructor.
     */
    ShaderD3D12(ID3D12Device* device, ID3DBlob* shader);

    /**
	 * Destructor.
     */
    ~ShaderD3D12();

	ID3D12Device* _device;
	ID3DBlob* _shader;
};

}