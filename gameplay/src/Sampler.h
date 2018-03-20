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