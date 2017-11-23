#include "Base.h"
#include "Sampler.h"

namespace gameplay
{

Sampler::Sampler() :
	_filterMag(FILTER_NEAREST),
	_filterMin(FILTER_NEAREST),
	_filterMip(FILTER_NEAREST),
	_addressModeU(ADDRESS_MODE_WRAP),
	_addressModeV(ADDRESS_MODE_WRAP),
	_addressModeW(ADDRESS_MODE_WRAP),
	_borderColor(BORDER_COLOR_BLACK_OPAQUE),
	_compareEnabled(true),
	_compareFunc(COMPARE_FUNC_NEVER),
	_anisotropyEnabled(false),
	_anisotropyMax(0.0f),
	_lodMin(0.0f),
	_lodMax(0.0f),
	_lodMipBias(0.0f)
{
}

Sampler::Sampler(Filter filterMag,
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
				 float lodMipBias) :
	_filterMag(filterMag),
	_filterMin(filterMin),
	_filterMip(filterMip),
	_addressModeU(addressModeU),
	_addressModeV(addressModeV),
	_addressModeW(addressModeW),
	_borderColor(borderColor),
	_compareEnabled(compareEnabled),
	_compareFunc(compareFunc),
	_anisotropyEnabled(anisotropyEnabled),
	_anisotropyMax(anisotropyMax),
	_lodMin(lodMin),
	_lodMax(lodMax),
	_lodMipBias(lodMipBias)
{
}

Sampler::~Sampler()
{
}

}