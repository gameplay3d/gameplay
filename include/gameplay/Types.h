#pragma once

#include <cstddef>
#include <cstdint>

namespace gameplay
{
struct Float2
{
    float x;
    float y;
};

struct Float3
{
    float x;
    float y;
    float z;
};

struct Float4
{
    float x;
    float y;
    float z;
    float w;
};

struct Double2
{
    double x;
    double y;
};

struct Double3
{
    double x;
    double y;
    double z;
};

struct Double4
{
    double x;
    double y;
    double z;
    double w;
};

struct Int2
{
    int32_t x;
    int32_t y;
};

struct Int3
{
    int32_t x;
    int32_t y;
    int32_t z;
};

struct Int4
{
    int32_t x;
    int32_t y;
    int32_t z;
    int32_t w;
};

struct Uint2
{
    uint32_t x;
    uint32_t y;
};

struct Uint3
{
    uint32_t x;
    uint32_t y;
    uint32_t z;
};

struct Uint4
{
    uint32_t x;
    uint32_t y;
    uint32_t z;
    uint32_t w;
};

struct Pixmap
{
    int32_t width;
    int32_t height;
    uint8_t* pixels;
};
}
