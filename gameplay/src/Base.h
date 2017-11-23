#pragma once

// C++
#include <new>
#include <memory>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cwchar>
#include <cwctype>
#include <cctype>
#include <cmath>
#include <cstdarg>
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <array>
#include <vector>
#include <list>
#include <set>
#include <stack>
#include <map>
#include <unordered_map>
#include <queue>
#include <bitset>
#include <algorithm>
#include <utility>
#include <limits>
#include <functional>
#include <typeinfo>
#include <thread>
#include <mutex>
#include <chrono>
#include <typeindex>

// Function
#ifdef _WINDOWS
#define __current__func__ __FUNCTION__
#else
#define __current__func__ __func__
#endif

// Asserts
#ifdef _DEBUG
#define GP_ASSERT(expression) assert(expression)
#else
#define GP_ASSERT(expression)
#endif	

// Debugging
#if defined(_WINDOWS) && defined(_MSC_VER)
#define GP_DEBUG_BREAK() __debugbreak()
#else
#define GP_DEBUG_BREAK()
#endif

// Logging
#include "Logger.h"
#ifdef GP_ERRORS_AS_WARNINGS
#define GP_ERROR GP_WARN
#else
#define GP_ERROR(...) do \
{ \
    gameplay::Logger::log(gameplay::Logger::LEVEL_ERROR, "%s -- ", __current__func__); \
    gameplay::Logger::log(gameplay::Logger::LEVEL_ERROR, __VA_ARGS__); \
    gameplay::Logger::log(gameplay::Logger::LEVEL_ERROR, "\n"); \
    GP_DEBUG_BREAK(); \
    assert(0); \
    std::exit(-1); \
} while (0)
#endif
#define GP_WARN(...) do \
{ \
    gameplay::Logger::log(gameplay::Logger::LEVEL_WARN, "%s -- ", __current__func__); \
    gameplay::Logger::log(gameplay::Logger::LEVEL_WARN, __VA_ARGS__); \
    gameplay::Logger::log(gameplay::Logger::LEVEL_WARN, "\n"); \
} while (0)
#define GP_INFO(...) do \
{ \
    gameplay::Logger::log(gameplay::Logger::LEVEL_INFO, "%s -- ", __current__func__); \
    gameplay::Logger::log(gameplay::Logger::LEVEL_INFO, __VA_ARGS__); \
    gameplay::Logger::log(gameplay::Logger::LEVEL_INFO, "\n"); \
} while (0)

// Memory
#define GP_SAFE_FREE(x) \
{ \
	if(x) \
	{ \
		free(x); \
		x = nullptr; \
	} \
}
#define GP_SAFE_DELETE(x) \
{ \
    delete x; \
    x = nullptr; \
}
#define GP_SAFE_DELETE_ARRAY(x) \
{ \
    delete[] x; \
    x = nullptr; \
}

#if defined(_WINDOWS) && defined(_MSC_VER)
#define GP_SAFE_RELEASE(x) \
   if(x != nullptr)	\
   { \
      x->Release(); \
      x = nullptr; \
   }
#endif

// Engine
#define GP_ENGINE_NAME					"gameplay"
#define GP_ENGINE_VERSION_MAJOR			4
#define GP_ENGINE_VERSION_MINOR			0
#define GP_ENGINE_HOME_PATH				"./"
#define GP_ENGINE_CONFIG				"game.config"

// Math
#define GP_MATH_RANDOM()                ((float)rand()/RAND_MAX)
#define GP_MATH_DEG_TO_RAD(x)           ((x) * GP_MATH_PIOVER180)
#define GP_MATH_RAD_TO_DEG(x)           ((x)* GP_MATH_180OVERPI)
#define GP_MATH_FLOAT_SMALL             1.0e-37f
#define GP_MATH_TOLERANCE               2e-37f
#define GP_MATH_E                       2.71828182845904523536f
#define GP_MATH_LOG10E                  0.4342944819032518f
#define GP_MATH_LOG2E                   1.442695040888963387f
#define GP_MATH_PI                      3.14159265358979323846f
#define GP_MATH_PIOVER2                 1.57079632679489661923f
#define GP_MATH_PIOVER4                 0.785398163397448309616f
#define GP_MATH_PIOVER360				0.008726646259971647884f
#define GP_MATH_PIOVER180				0.0174532924f
#define GP_MATH_180OVERPI				57.29578f
#define GP_MATH_PIX2                    6.28318530717958647693f
#define GP_MATH_EPSILON                 0.000001f
#define GP_MATH_CLAMP(x, lo, hi)        ((x < lo) ? lo : ((x > hi) ? hi : x))
#define GP_MATH_ROUNDUP(value, to)		((value + to - 1) / to) * to
#define GP_MATH_MIN(a, b)				a < b ? a : b
#define GP_MATH_MAX(a, b)				a > b ? a : b
#define GP_MATH_1_PI                    0.31830988618379067154
#define GP_MATH_MATRIX_SIZE             (sizeof(float) * 16)

// Graphics defaults
#define GP_GRAPHICS_WIDTH					1920
#define GP_GRAPHICS_HEIGHT					1080
#define GP_GRAPHICS_FULLSCREEN				false
#define GP_GRAPHICS_VSYNC					true
#define GP_GRAPHICS_MULTISAMPLING			0
#define GP_GRAPHICS_VALIDATION				false
#define GP_GRAPHICS_SWAPCHAIN_IMAGE_COUNT	3
#define GP_GRAPHICS_VK						"vk"
#define GP_GRAPHICS_VK_SHADER_PATH			"res/shaders/glsl/"
#define GP_GRAPHICS_VK_SHADER_EXT			".spv"
#define GP_GRAPHICS_D3D12					"d3d12"
#define GP_GRAPHICS_D3D12_SHADER_PATH		"res/shaders/hlsl/"
#define GP_GRAPHICS_D3D12_SHADER_EXT		".cso"
#define GP_GRAPHICS_MTL						"mtl"
#define GP_GRAPHICS_MTL_SHADER_PATH			"res/shaders/msl/"
#define GP_GRAPHICS_MTL_SHADER_EXT			".metallib"
#define GP_GRAPHICS							GP_GRAPHICS_VK

// Graphics limits
#define GP_GRAPHICS_GPUS_MAX                        4
#define GP_GRAPHICS_INSTANCE_EXTS_MAX               256
#define GP_GRAPHICS_DEVICE_EXTS_MAX                 256
#define GP_GRAPHICS_DESCRIPTORS_MAX                 32
#define GP_GRAPHICS_DESCRIPTOR_SETS_MAX             8
#define GP_GRAPHICS_DESCRIPTOR_ENTRIES_MAX          256
#define GP_GRAPHICS_COLOR_ATTACHMENTS_MAX			8
#define GP_GRAPHICS_SUBMIT_COMMAND_BUFFERS_MAX      8
#define GP_GRAPHICS_SUBMIT_WAIT_SEMAPHORES_MAX      8
#define GP_GRAPHICS_SUBMIT_SIGNAL_SEMAPHORES_MAX    8
#define GP_GRAPHICS_PRESENT_WAIT_SEMAPHORES_MAX     8
#define GP_GRAPHICS_VERTEX_BINDINGS_MAX             15
#define GP_GRAPHICS_VERTEX_ATTRIBUTES_MAX           15
#define GP_GRAPHICS_MIP_LEVELS_MAX                  0xFFFFFFFF


namespace gameplay
{

/**
 * Print output
 *
 * @param format The string to format.
 */
extern void print(const char* format, ...);

}

// Compiler
#if defined(_WINDOWS)
#pragma warning( disable : 4005 )
#pragma warning( disable : 4172 )
#pragma warning( disable : 4244 )
#pragma warning( disable : 4267 )
#pragma warning( disable : 4302 )
#pragma warning( disable : 4311 )
#pragma warning( disable : 4312 )
#pragma warning( disable : 4316 )
#pragma warning( disable : 4390 )
#pragma warning( disable : 4800 )
#pragma warning( disable : 4996 )
#endif
