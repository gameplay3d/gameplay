#pragma once

namespace gameplay
{

/**
 * Defines data formats.
 */
enum class Format
{
    eUndefined,
    eR8Unorm,
    eR16Unorm,
    eR16Float,
    eR32Uint,
    eR32Float,
    eR8G8Unorm,
    eR16G16Unorm,
    eR16G16Float,
    eR32G32Uint,
    eR32G32Float,
    eR32G32B32Uint,
    eR32G32B32Float,
    eB8G8R8A8Unorm,
    eR8G8B8A8Unorm,
    eR16G16B16A16Unorm,
    eR16G16B16A16Float,
    eR32G32B32A32Uint,
    eR32G32B32A32Float,
    eD16Unorm,
    eX8D24UnormPack32,
    eD32Float,
    eS8Uint,
    eD24UnormS8Uint,
    eD32FloatS8Uint
};

}
