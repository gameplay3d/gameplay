#pragma once

namespace gameplay
{

/**
 * Defines a sampler used to read image data
 * and apply filtering and other transformations 
 * for the shader.
 */
class Sampler
{
public:

	/**
	 * Defines a sampling filters for mag, min and mip.
	 */
	enum Filter : uint32_t
	{
		FILTER_NEAREST,
		FILTER_LINEAR
	};

	/**
	 * Defines the address mode use for U, V, W.
	 */
	enum AddressMode : uint32_t
	{
		ADDRESS_MODE_WRAP,
		ADDRESS_MODE_MIRROR,
		ADDRESS_MODE_CLAMP_EDGE,
		ADDRESS_MODE_CLAMP_BORDER,
		ADDRESS_MODE_MIRROR_ONCE
	};

	/**
	 * Defines a comparison function.
	 */
	enum CompareFunc : uint32_t
	{
		COMPARE_FUNC_NEVER,
		COMPARE_FUNC_LESS,
		COMPARE_FUNC_EQUAL,
		COMPARE_FUNC_LESS_OR_EQUAL,
		COMPARE_FUNC_GREATER,
		COMPARE_FUNC_NOT_EQUAL,
		COMPARE_FUNC_GREATER_OR_EQUAL,
		COMPARE_FUNC_ALWAYS
	};

	/**
	 * Defines the border color used for texture lookups.
	 */
	enum BorderColor : uint32_t
	{
		BORDER_COLOR_BLACK_TRANSPARENT,
		BORDER_COLOR_BLACK_OPAQUE,
		BORDER_COLOR_WHITE_OPAQUE
	};

	/**
	 * Constructor.
	 */
	Sampler();

	/**
	 * Constructor.
	 */
	Sampler(Filter filterMag,
			Filter filterMin,
			Filter filterMip,
			AddressMode addressModeU,
			AddressMode addressModeV,
			AddressMode addressModeW,
			BorderColor borderColor,
			bool compareEnabled,
			CompareFunc compareFunc,
			bool anisotropyEnabled,
			float anisotropyMax,
			float lodMin,
			float lodMax,
			float lodMipBias);

	/**
	 * Destructor.
	 */
	~Sampler();

protected:

	Filter _filterMag;
	Filter _filterMin;
	Filter _filterMip;
	AddressMode _addressModeU;
	AddressMode _addressModeV;
	AddressMode _addressModeW;
	BorderColor _borderColor;
	bool _compareEnabled;
	CompareFunc _compareFunc;
	bool _anisotropyEnabled;
	float _anisotropyMax;
	float _lodMin;
	float _lodMax;
	float _lodMipBias;
};

}