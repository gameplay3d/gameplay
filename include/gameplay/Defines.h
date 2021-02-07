#pragma once

#include <cassert>
#include <cinttypes>
#include <climits>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#ifndef GP_NO_MALLOC_FREE
#   include <cstring>
#else
#   include <cstddef>
#endif
#include <exception>
#include <type_traits>
#include <mutex>

#ifdef NDEBUG
#   define GP_DEBUG 0
#else
#   define GP_DEBUG 1
#endif

#ifdef _WIN32
#   define GP_PLATFORM_WINDOWS 1
#   define GP_PLATFORM_LINUX 0
#   define GP_PLATFORM_MACOS 0
#   define GP_PLATFORM_IOS 0
#   define GP_PLATFORM_ANDROID 0
#elif defined(__APPLE__)
#   include "TargetConditionals.h"
#   if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
#       define GP_PLATFORM_WINDOWS 0
#       define GP_PLATFORM_LINUX 0
#       define GP_PLATFORM_MACOS 0
#       define GP_PLATFORM_IOS 1
#       define GP_PLATFORM_ANDROID 0
#   elif TARGET_OS_MAC
#       define GP_PLATFORM_WINDOWS 0
#       define GP_PLATFORM_LINUX 0
#       define GP_PLATFORM_MACOS 1
#       define GP_PLATFORM_IOS 0
#       define GP_PLATFORM_ANDROID 0
#   endif
#elif defined(__ANDROID__)
#   define GP_PLATFORM_WINDOWS 0
#   define GP_PLATFORM_LINUX 0
#   define GP_PLATFORM_MACOS 0
#   define GP_PLATFORM_IOS 0
#   define GP_PLATFORM_ANDROID 1
#elif defined(__linux__)
#   define GP_PLATFORM_WINDOWS 0
#   define GP_PLATFORM_LINUX 1
#   define GP_PLATFORM_MACOS 0
#   define GP_PLATFORM_IOS 0
#   define GP_PLATFORM_ANDROID 0
#else
#   error "Unsupported platform"
#endif

#if GP_PLATFORM_WINDOWS
#   ifndef GP_NO_MALLOC_FREE
#       include "malloc.h"
#   endif
#   include <intrin.h>
#elif GP_PLATFORM_LINUX
#   include <alloca.h>
#   include <signal.h>
#   define _alloca alloca
#endif

#if defined(__aarch64__)
#   define GP_ARCH_AARCH64 1
#   define GP_ARCH_X86_64 0
#elif defined(__x86_64__) || defined(_M_X64)
#   define GP_ARCH__X86_64 1
#   define GP_ARCH_AARCH64 0
#endif

#if defined(_MSC_VER)
#   define GP_COMPILER_MSC 1
#   define GP_COMPILER_GNUC 0
#   define GP_ASAN_ENABLED 0
#endif
#if defined(__GNUC__)
#   define GP_COMPILER_MSC 0
#   define GP_COMPILER_GNUC 1
#   ifdef __SANITIZE_ADDRESS__
#       define GP_ASAN_ENABLED __SANITIZE_ADDRESS__
#   else
#       define GP_ASAN_ENABLED 0
#   endif
#endif

#if GP_COMPILER_MSC
#   define GP_PRETTY_FUNCTION __FUNCSIG__
#   define GP_ATTRIBUTE(...)
#   define GP_DECLSPEC(...) __declspec(__VA_ARGS__)
#   define GP_IGNOREWARNING_MSC_PUSH __pragma(warning(push))
#   define GP_IGNOREWARNING_MSC_POP __pragma(warning(pop))
#   define GP_IGNOREWARNING_MSC(w) __pragma(warning(disable : w))
#   define GP_IGNOREWARNING_MSC_WITH_PUSH(w)                                                                          \
       GP_IGNOREWARNING_MSC_PUSH                                                                                      \
       GP_IGNOREWARNING_MSC(w)
#   define GP_IGNOREWARNING_GNUC_PUSH
#   define GP_IGNOREWARNING_GNUC_POP
#   define GP_IGNOREWARNING_GNUC(w)
#   define GP_IGNOREWARNING_GNUC_WITH_PUSH(w)
#elif GP_COMPILER_GNUC
#   define GP_PRETTY_FUNCTION __PRETTY_FUNCTION__
#   define GP_ATTRIBUTE(...) __attribute__((__VA_ARGS__))
#   define GP_DECLSPEC(...)
#   define GP_IGNOREWARNING_MSC_PUSH
#   define GP_IGNOREWARNING_MSC_POP
#   define GP_IGNOREWARNING_MSC(w)
#   define GP_IGNOREWARNING_MSC_WITH_PUSH(w)
#   define GP_IGNOREWARNING_GNUC_PUSH _Pragma("GCC diagnostic push")
#   define GP_IGNOREWARNING_GNUC_POP _Pragma("GCC diagnostic pop")
#   define GP_IGNOREWARNING_GNUC_INTERNAL(str) _Pragma(#    str)
#   define GP_IGNOREWARNING_GNUC(w) GP_IGNOREWARNING_GNUC_INTERNAL(GCC diagnostic ignored w)
#   define GP_IGNOREWARNING_GNUC_WITH_PUSH(w)                                                                         \
        GCC_IGNOREWARNING_GNUC_PUSH                                                                                   \
        GCC_IGNOREWARNING_GNUC(w)
#else
#   error Unsupported compiler
#endif

#ifdef GP_EXPORT
#   define GP_API GP_DECLSPEC(dllexport) GP_ATTRIBUTE(visibility("default"))
#else
#   define GP_API GP_DECLSPEC(dllimport)
#endif


#define GP_NOINLINE GP_ATTRIBUTE(noinline) GP_DECLSPEC(noinline)
#define GP_DEPRECATED(msg) GP_ATTRIBUTE(deprecated(msg)) GP_DECLSPEC(deprecated(msg))

namespace gameplay
{

template <typename T>
constexpr T align(T x, size_t alignment)
{
    return (T)(((size_t)x + alignment - 1) / alignment * alignment);
}
template <typename T>
T* align(T* x, size_t alignment)
{
    return (T*)(((size_t)x + alignment - 1) / alignment * alignment);
}
#define GP_ALIGN(x, alignment) gameplay::align(x, alignment)

template <typename T>
constexpr T aligned_size(const T& size, uint32_t alignment)
{
    return ((size + alignment - 1) / alignment) * alignment;
}

#define GP_ALIGNED_SIZE(size, alignment) gameplay::aligned_size(size, alignment)
#define GP_ALIGN_AS(T) alignas(T)

template <typename T, size_t N>
constexpr size_t count_of(T const (&)[N])
{
    return N;
}
#define GP_COUNTOF(a) gameplay::count_of(a)

template <typename T, uint32_t N>
constexpr uint32_t count_of32(T const (&)[N])
{
    return N;
}
#define GP_COUNTOF32(a) gameplay::count_of32(a)

template <typename T, typename U>
constexpr uint32_t offset_of(U T::*member)
{
    return (uint32_t)((char*)&((T*)nullptr->*member) - (char*)nullptr);
}
#define GP_OFFSETOF(a) gameplay::offset_of(&a)

#if GP_COMPILER_MSC
#    define GP_ALIGN_OF(T) __alignof(T)
#elif GP_COMPILER_GNUC
#    define GP_ALIGN_OF(T) __alignof__(T)
#else
#   error Unsupported compiler
#endif

inline bool assert_handler(const char* condition, const char* file, const char* func, int32_t line, const char* fmt = nullptr, ...)
{
    static std::mutex m;
    std::lock_guard<std::mutex> g(m);
    if (fmt != nullptr)
    {
        fprintf(stderr, "%s:%s():%" PRId32 ": Assertion (%s) failed: ", file, func, line, condition);
        va_list args;
        va_start(args, fmt);
        vfprintf(stderr, fmt, args);
        va_end(args);
        fputc('\n', stderr);
    }
    else
    {
        fprintf(stderr, "%s:%" PRId32 ":%s(): Assertion (%s) failed.\n", file, line, func, condition);
    }
    return true;
}

#define GP_STACK_ALLOC(T, number)                                                                                     \
    gameplay::align<T>(((number) ? (T*)_alloca((number) * sizeof(T) + alignof(T)) : nullptr), alignof(T))

#define GP_MALLOC(size) std::malloc(size)
#define GP_FREE(ptr) std::free(ptr)

#define GP_STRINGIFY_INTERNAL(x) #x
#define GP_STRINGIFY(x) GP_STRINGIFY_INTERNAL(x)

constexpr uint64_t GP_FNV_BASIS = 14695981039346656037ull;
constexpr uint64_t GP_FNV_PRIME = 1099511628211ull;

constexpr uint64_t fnv1a_hash(const char* str, std::size_t n, uint64_t hash = GP_FNV_BASIS)
{
    return n > 0 ? fnv1a_hash(str + 1, n - 1, (hash ^ *str) * GP_FNV_PRIME) : hash;
}

template <std::size_t N>
constexpr uint64_t fnv1a_hash(const char (&array)[N])
{
    return fnv1aHash(&array[0], N - 1);
}

inline uint64_t hash_string(const char* str, uint64_t hash = GP_FNV_BASIS)
{
    for (uint32_t i = 0; str[i] != 0; ++i)
    {
        hash ^= static_cast<unsigned char>(str[i]);
        hash *= GP_FNV_PRIME;
    }
    return hash;
}

inline uint64_t hash_buffer(const void* buffer, size_t length, uint64_t hash = GP_FNV_BASIS)
{
    const char* ptr = static_cast<const char*>(buffer);
    for (size_t i = 0; i < length; ++i)
    {
        hash ^= static_cast<unsigned char>(ptr[i]);
        hash *= GP_FNV_PRIME;
    }
    return hash;
}

template <class T>
constexpr uint64_t hash_scalar(const T& type, uint64_t hash = GP_FNV_BASIS)
{
    static_assert(std::is_scalar<T>::value, "Unsupported type for hashing");
    return hash_buffer(reinterpret_cast<const char*>(std::addressof(type)), sizeof(type), hash);
}

#if defined(__CUDACC__)
#    define GP_HASH_STRING(str) std::integral_constant<uint64_t, gameplay::fnv1a_hash(str)>::value
#else
#    define GP_HASH_STRING(str)                                                                                       \
        GP_IGNOREWARNING_MSC_WITH_PUSH(4307)                                                                          \
        std::integral_constant<uint64_t, gameplay::fnv1a_hash(str)>::value GP_IGNOREWARNING_MSC_POP
#endif

#define GP_HASH_TYPE(T) GP_HASH_STRING(GP_STRINGIFY(T))

}

#if GP_PLATFORM_LINUX
#   define GP_BREAK_POINT() ::raise(SIGTRAP)
#elif GP_PLATFORM_WINDOWS
#   define GP_BREAK_POINT() ::__debugbreak()
#else
#   error "Unsupported platform"
#endif

#ifndef GP_CHECK
#    define GP_CHECK(cond, ...)                                                                                       \
        ![&](const char* funcname__, ...) GP_NOINLINE {                                                               \
             return ::gameplay::assert_handler(#cond, __FILE__, funcname__, __LINE__, ##__VA_ARGS__);                 \
         }
#endif

#ifndef GP_ASSERT
#   if GP_DEBUG
#       define GP_ASSERT(cond, ...) assert(cond)
#   else
#       define GP_ASSERT
#   endif
#endif

#define GP_ASSERT_STRUCTS_MATCH(A, B)                                                                                 \
    static_assert(sizeof(A) == sizeof(B), "Size mismatch between " #A " and " #B ".");                                \
    static_assert(alignof(A) == alignof(B), "Alignment mismatch between " #A " and " #B ".")

#define GP_ASSERT_MEMBERS_MATCH(A, a, B, b)                                                                           \
    static_assert(                                                                                                    \
        offsetof(A, a) == offsetof(B, b), "Offset mismatch between members " #a " of " #A " and " #b " of " #B ".")

#define GP_SAFE_DELETE(ptr)                                                                                           \
    {                                                                                                                 \
        delete ptr;                                                                                                   \
        ptr = nullptr;                                                                                                \
    }

#define GP_SAFE_DELETE_ARRAY(ptr)                                                                                     \
    {                                                                                                                 \
        delete[] ptr;                                                                                                 \
        ptr = nullptr;                                                                                                \
    }

#define GP_UINT16_MAX UINT16_MAX
#define GP_UINT32_MAX UINT32_MAX
#define GP_UINT64_MAX UINT64_MAX
#define GP_ULLONG_MAX ULLONG_MAX
#define GP_USHRT_MAX USHRT_MAX
#define GP_FLOAT_MAX 3.402823466e+38F

#define GP_MIN(a, b) (((a) < (b)) ? (a) : (b))
#define GP_MAX(a, b) (((a) > (b)) ? (a) : (b))

#define GP_CLAMP(x, lo, hi) (((x) < (lo)) ? (lo) : (((x) > (hi)) ? (hi) : (x)))
#define GP_ROUNDUP(value, to) ((((value) + (to)-1) / (to)) * (to))

template <class... Args>
void GP_UNUSED(Args&&...) {}
