#pragma once

namespace gameplay
{

/**
 * Maps gameplay::Format to DXGI_FORMAT
 */
static const DXGI_FORMAT lookupDXGI_FORMAT[] =
{
	DXGI_FORMAT_UNKNOWN,
	DXGI_FORMAT_R8_UNORM,
	DXGI_FORMAT_R16_UNORM,
	DXGI_FORMAT_R16_FLOAT,
	DXGI_FORMAT_R32_UINT,
	DXGI_FORMAT_R32_FLOAT,
	DXGI_FORMAT_R8G8_UNORM,
	DXGI_FORMAT_R16G16_UNORM,
	DXGI_FORMAT_R16G16_FLOAT,
	DXGI_FORMAT_R32G32_UINT,
	DXGI_FORMAT_R32G32_FLOAT,
	DXGI_FORMAT_R32G32B32_UINT,
	DXGI_FORMAT_R32G32B32_FLOAT,
	DXGI_FORMAT_B8G8R8A8_UNORM,
	DXGI_FORMAT_R8G8B8A8_UNORM,
	DXGI_FORMAT_R16G16B16A16_UNORM,
	DXGI_FORMAT_R16G16B16A16_FLOAT,
	DXGI_FORMAT_R32G32B32A32_UINT,
	DXGI_FORMAT_R32G32B32A32_FLOAT,
	DXGI_FORMAT_D16_UNORM,
	DXGI_FORMAT_X32_TYPELESS_G8X24_UINT,
	DXGI_FORMAT_D32_FLOAT,
	DXGI_FORMAT_R8_UINT,
	DXGI_FORMAT_D24_UNORM_S8_UINT,
	DXGI_FORMAT_D32_FLOAT_S8X24_UINT
};


/**
 * Maps gameplay::Format to DXGI_FORMAT
 */
static const LPCSTR lookupSemantic[] =
{
    "POSITION",
    "NORMAL",
    "COLOR",
	"COLOR",
	"COLOR",
	"COLOR",
	"COLOR",
	"COLOR",
    "TANGENT",
    "BITANGENT",
    "TEXCOORD",
	"TEXCOORD",
	"TEXCOORD",
	"TEXCOORD",
	"TEXCOORD",
	"TEXCOORD",
	"TEXCOORD",
	"TEXCOORD",
	"TEXCOORD",
};

static const uint32_t lookupSemanticIndex[] =
{
    0,
    0,
    0,
	0,
	1,
	2,
	3,
	4,
    0,
    0,
    0,
	0,
	1,
	2,
	3,
	4,
	5,
	6,
	7
};

/**
 * Maps gameplay::Sampler::AddressMode to D3D12_TEXTURE_ADDRESS_MODE
 */
static const D3D12_TEXTURE_ADDRESS_MODE lookupD3D12_TEXTURE_ADDRESS_MODE[] =
{
	D3D12_TEXTURE_ADDRESS_MODE_WRAP,
	D3D12_TEXTURE_ADDRESS_MODE_MIRROR,
	D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
	D3D12_TEXTURE_ADDRESS_MODE_BORDER,
	D3D12_TEXTURE_ADDRESS_MODE_MIRROR_ONCE
};

/**
 * Maps gameplay::ColorBlendState::BlendFactor to D3D12_BLEND
 */
static const D3D12_BLEND lookupD3D12_BLEND[] =
{
	D3D12_BLEND_ZERO,
    D3D12_BLEND_ONE,
    D3D12_BLEND_SRC_COLOR,
    D3D12_BLEND_INV_SRC_COLOR,
    D3D12_BLEND_DEST_COLOR,
    D3D12_BLEND_INV_DEST_COLOR,
    D3D12_BLEND_SRC_ALPHA,
    D3D12_BLEND_INV_SRC_ALPHA,
    D3D12_BLEND_DEST_ALPHA,
    D3D12_BLEND_INV_DEST_ALPHA,
    D3D12_BLEND_SRC_ALPHA_SAT,
    D3D12_BLEND_SRC1_COLOR,
    D3D12_BLEND_INV_SRC1_COLOR,
    D3D12_BLEND_SRC1_ALPHA,
    D3D12_BLEND_INV_SRC1_ALPHA
};


/**
 * Maps gameplay::ColorBlendState::BlendOp to D3D12_BLEND_OP
 */
static const D3D12_BLEND_OP lookupD3D12_BLEND_OP[] =
{
	D3D12_BLEND_OP_ADD,
    D3D12_BLEND_OP_SUBTRACT,
    D3D12_BLEND_OP_REV_SUBTRACT,
    D3D12_BLEND_OP_MIN,
    D3D12_BLEND_OP_MAX
};

/**
 * Maps gameplay::RasterizerState::FillMode to D3D12_FILL_MODE
 */
static const D3D12_FILL_MODE lookupD3D12_FILL_MODE[] =
{
	D3D12_FILL_MODE_SOLID,
    D3D12_FILL_MODE_WIREFRAME
};

/**
 * Maps gameplay::RasterizerState::CullMode to D3D12_CULL_MODE
 */
static const D3D12_CULL_MODE lookupD3D12_CULL_MODE[] =
{
	D3D12_CULL_MODE_NONE,
    D3D12_CULL_MODE_BACK,
    D3D12_CULL_MODE_FRONT
};


/**
 * Maps gameplay::Sampler::CompareFunc to D3D12_COMPARISON_FUNC
 */
static const D3D12_COMPARISON_FUNC lookupD3D12_COMPARISON_FUNC[] =
{
	D3D12_COMPARISON_FUNC_NEVER,
	D3D12_COMPARISON_FUNC_LESS,
	D3D12_COMPARISON_FUNC_EQUAL,
	D3D12_COMPARISON_FUNC_LESS_EQUAL,
	D3D12_COMPARISON_FUNC_GREATER,
	D3D12_COMPARISON_FUNC_NOT_EQUAL,
	D3D12_COMPARISON_FUNC_GREATER_EQUAL,
	D3D12_COMPARISON_FUNC_ALWAYS
};

/**
 * Maps gameplay::DepthStencilState::StencilOp to D3D12_STENCIL_OP
 */
static const D3D12_STENCIL_OP lookupD3D12_STENCIL_OP[] =
{
	D3D12_STENCIL_OP_KEEP,
    D3D12_STENCIL_OP_ZERO,
    D3D12_STENCIL_OP_REPLACE,
    D3D12_STENCIL_OP_INCR_SAT,
    D3D12_STENCIL_OP_DECR_SAT,
    D3D12_STENCIL_OP_INVERT,
    D3D12_STENCIL_OP_INCR,
    D3D12_STENCIL_OP_DECR
};

static D3D12_RESOURCE_STATES toD3D12_RESOURCE_STATES(Texture::Usage usage)
{
	D3D12_RESOURCE_STATES result = D3D12_RESOURCE_STATE_COMMON;
    if (Texture::USAGE_TRANSFER_SRC == (usage & Texture::USAGE_TRANSFER_SRC)) 
        result |= D3D12_RESOURCE_STATE_COPY_SOURCE;
    if (Texture::USAGE_TRANSFER_DST == (usage & Texture::USAGE_TRANSFER_DST)) 
        result |= D3D12_RESOURCE_STATE_COPY_DEST;
    if (Texture::USAGE_SAMPLED_IMAGE == (usage & Texture::USAGE_SAMPLED_IMAGE)) 
        result |= D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE;
    if (Texture::USAGE_STORAGE == (usage & Texture::USAGE_STORAGE)) 
        result |= D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
    if (Texture::USAGE_COLOR_ATTACHMENT == (usage & Texture::USAGE_COLOR_ATTACHMENT)) 
        result |= D3D12_RESOURCE_STATE_RENDER_TARGET;
    if (Texture::USAGE_DEPTH_STENCIL_ATTACHMENT == (usage & Texture::USAGE_DEPTH_STENCIL_ATTACHMENT)) 
        result |= D3D12_RESOURCE_STATE_DEPTH_WRITE;
    if (Texture::USAGE_RESOLVE_SRC == (usage & Texture::USAGE_RESOLVE_SRC)) 
        result |= D3D12_RESOURCE_STATE_RESOLVE_SOURCE;
    if (Texture::USAGE_RESOLVE_DST == (usage & Texture::USAGE_RESOLVE_DST))
        result |= D3D12_RESOURCE_STATE_RESOLVE_DEST;
    return result;
}

static D3D12_FILTER toD3D12_FILTER(Sampler::Filter minFilter, Sampler::Filter magFilter, Sampler::Filter mipFilter)
{
	D3D12_FILTER result = D3D12_FILTER_MIN_MAG_MIP_LINEAR;

	if ((minFilter == Sampler::FILTER_LINEAR) && 
		(magFilter == Sampler::FILTER_LINEAR) &&
		(mipFilter == Sampler::FILTER_LINEAR))
	{
		D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	}
	else if ((minFilter == Sampler::FILTER_NEAREST) && 
			 (magFilter == Sampler::FILTER_NEAREST) &&
			 (mipFilter == Sampler::FILTER_NEAREST))
	{
		result = D3D12_FILTER_MIN_MAG_MIP_POINT;
	}
	else if ((minFilter == Sampler::FILTER_NEAREST) && 
			 (magFilter == Sampler::FILTER_NEAREST) &&
			 (mipFilter == Sampler::FILTER_LINEAR))
	{
		result = D3D12_FILTER_MIN_MAG_POINT_MIP_LINEAR;
	}
	else if ((minFilter == Sampler::FILTER_NEAREST) && 
			 (magFilter == Sampler::FILTER_LINEAR) &&
			 (mipFilter == Sampler::FILTER_NEAREST))
	{
		result = D3D12_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
	}
	else if ((minFilter == Sampler::FILTER_NEAREST) && 
			 (magFilter == Sampler::FILTER_LINEAR) &&
			 (mipFilter == Sampler::FILTER_LINEAR))
	{
		result = D3D12_FILTER_MIN_POINT_MAG_MIP_LINEAR;
	}
	else if ((minFilter == Sampler::FILTER_LINEAR) && 
			 (magFilter == Sampler::FILTER_NEAREST) &&
			 (mipFilter == Sampler::FILTER_NEAREST))
	{
		result = D3D12_FILTER_MIN_LINEAR_MAG_MIP_POINT;
	}
	else if ((minFilter == Sampler::FILTER_LINEAR) && 
			 (magFilter == Sampler::FILTER_NEAREST) &&
			 (mipFilter == Sampler::FILTER_LINEAR))
	{
		result = D3D12_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
	}
	else if ((minFilter == Sampler::FILTER_LINEAR) && 
			 (magFilter == Sampler::FILTER_LINEAR) &&
			 (mipFilter == Sampler::FILTER_NEAREST))
	{
		result = D3D12_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	}
	return result;
}

static void toD3D12_BORDER_COLOR(Sampler::BorderColor borderColor, FLOAT d3dBorder[4])
{
	switch (borderColor)
	{
	case Sampler::BORDER_COLOR_BLACK_TRANSPARENT:
		d3dBorder[0] = 0;
		d3dBorder[1] = 0;
		d3dBorder[2] = 0;
		d3dBorder[3] = 0;
		break;
	case Sampler::BORDER_COLOR_BLACK_OPAQUE:
		d3dBorder[0] = 0;
		d3dBorder[1] = 0;
		d3dBorder[2] = 0;
		d3dBorder[3] = 1;
		break;
	case Sampler::BORDER_COLOR_WHITE_OPAQUE:
		d3dBorder[0] = 1;
		d3dBorder[1] = 1;
		d3dBorder[2] = 1;
		d3dBorder[3] = 1;
		break;
	}
}

PFN_D3D12_CREATE_ROOT_SIGNATURE_DESERIALIZER D3D12CreateRootSignatureDeserializer;
PFN_D3D12_SERIALIZE_VERSIONED_ROOT_SIGNATURE  D3D12SerializeVersionedRootSignature;
PFN_D3D12_CREATE_VERSIONED_ROOT_SIGNATURE_DESERIALIZER D3D12CreateVersionedRootSignatureDeserializer;

}