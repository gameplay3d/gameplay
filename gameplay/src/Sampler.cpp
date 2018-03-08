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
    _lodMipBias(0.0f),
	_sampler(VK_NULL_HANDLE)
{
}

Sampler::~Sampler()
{
}

Sampler::Filter Sampler::getFilterMin() const
{
	return _filterMin;
}

Sampler::Filter Sampler::getFilterMag() const
{
	return _filterMag;
}

Sampler::Filter Sampler::getFilterMip() const
{
	return _filterMip;
}

Sampler::AddressMode Sampler::getAddressModeU() const
{
	return _addressModeU;
}

Sampler::AddressMode Sampler::getAddressModeV() const
{
	return _addressModeV;
}

Sampler::AddressMode Sampler::getAddressModeW() const
{
	return _addressModeW;
}

Sampler::BorderColor Sampler::getBorderColor() const
{
	return _borderColor;
}

bool Sampler::isCompareEnabled() const
{
	return _compareEnabled;
}

Sampler::CompareFunc Sampler::getCompareFunc() const
{
	return _compareFunc;
}

bool Sampler::isAnisotropyEnabled() const
{
	return _anisotropyEnabled;
}

float Sampler::getAnisotropyMax() const
{
	return _anisotropyMax;
}

float Sampler::getLodMin() const
{
	return _lodMin;
}

float Sampler::getLodMax() const
{
	return _lodMax;
}

float Sampler::getLodMipBias() const
{
	return _lodMipBias;
}

}