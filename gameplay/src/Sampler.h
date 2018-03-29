#pragma once

#include <vulkan/vulkan.h>

namespace gameplay
{

/**
 * Defines a sampler used to read image data
 * and apply filtering and other transformations 
 * for the shader.
 */
class Sampler
{
    friend class Graphics;

public:

    /**
     * Defines a sampling filters for mag, min and mip.
     */
    enum class Filter
    {
        eNearest,
        eLinear
    };

    /**
     * Defines the address mode use for U, V, W.
     */
    enum class AddressMode
    {
        eWrap,
        eMirror,
        eClampEdge,
        eClampBorder,
        eMirrorOnce
    };

    /**
     * Defines a comparison function.
     */
    enum class CompareFunc
    {
        eNever,
        eLess,
        eEqual,
        eLessOrEqual,
        eGreater,
        eNotEqual,
        eGreaterOrEqual,
        eAlways
    };

    /**
     * Defines the border color used for texture lookups.
     */
    enum class BorderColor
    {
        eBlackTransparent,
        eBlackOpaque,
        eWhiteOpaque
    };

    /**
     * Constructor.
     */
    Sampler();

    /**
     * Destructor.
     */
    ~Sampler();

    /**
     * Gets the value specifying the minification filter to apply to lookups.
     *
     * @return The value specifying the minification filter to apply to lookups.
     */
    Filter getFilterMin() const;

    /**
     * Gets the value specifying the magnification filter to apply to lookups.
     *
     * @return The value specifying the magnification filter to apply to lookups.
     */
    Filter getFilterMag() const;

    /**
     * Gets the value specifying the mipmap filter to apply to lookups.
     *
     * @return The value specifying the mipmap filter to apply to lookups.
     */
    Filter getFilterMip() const;

    /**
     * Gets the value specifying the addressing mode for outside [0..1] range for U coordinate.
     *
     * @return The value specifying the addressing mode for outside [0..1] range for U coordinate.
     */
    AddressMode getAddressModeU() const;

    /**
     * Gets the value specifying the addressing mode for outside [0..1] range for V coordinate.
     *
     * @return The value specifying the addressing mode for outside [0..1] range for V coordinate.
     */
    AddressMode getAddressModeV() const;

    /**
     * Gets the value specifying the addressing mode for outside [0..1] range for W coordinate.
     *
     * @return The value specifying the addressing mode for outside [0..1] range for W coordinate.
     */
    AddressMode getAddressModeW() const;

    /**
     * Gets the value specifying the border color used for texture lookup.
     *
     * @return The value specifying the border color used for texture lookup.
     */
    BorderColor getBorderColor() const;

    /**
     * Determines if the comparison function to apply to fetched data before filtering. 
     *
     * @return true Determines if the comparison function to apply to fetched data before filtering.
     */
    bool isCompareEnabled() const;

    /**
     * Gets the value specifying the comparison function to apply to fetched data before filtering.
     *
     * @return The value specifying the comparison function to apply to fetched data before filtering.
     */
    CompareFunc getCompareFunc() const;

    /**
     * Determines if anisotropy if applied.
     *
     * @return true if anisotropy if applied, false if disabled.
     */
    bool isAnisotropyEnabled() const;

    /**
     * Gets The maximum anisotropy value to clamp to.
     *
     * @return The maximum anisotropy value to clamp to.
     */
    float getAnisotropyMax() const;

    /**
     * Gets the minimum value used to clamp the computed level-of-detail value.
     *
     * @return The minimum value used to clamp the computed level-of-detail value.
     */
    float getLodMin() const;

    /**
     * Gets the maximum value used to clamp the computed level-of-detail value.
     *
     * @return The maximum value used to clamp the computed level-of-detail value.
     */
    float getLodMax() const;

    /**
     * Gets the bias to be added to mipmap LOD calculation and bias provided by image sampling functions.
     *
     * @return The bias to be added to mipmap LOD calculation and bias provided by image sampling functions.
     */
    float getLodMipBias() const;

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
    VkSampler _sampler;
};

}
