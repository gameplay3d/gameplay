#ifndef BASE_H_
#define BASE_H_

// C/C++
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
#include <vector>
#include <list>
#include <set>
#include <stack>
#include <map>
#include <queue>
#include <algorithm>
#include <limits>
#include <functional>
#include <bitset>
#include <typeinfo>
#include "Logger.h"

// Bring common functions from C into global namespace
using std::memcpy;
using std::fabs;
using std::sqrt;
using std::cos;
using std::sin;
using std::tan;
using std::isspace;
using std::isdigit;
using std::toupper;
using std::tolower;
using std::size_t;
using std::min;
using std::max;
using std::modf;
using std::atoi;

// Common
#ifndef NULL
#define NULL     0
#endif

namespace gameplay
{
/**
 * Print logging (implemented per platform).
 * @script{ignore}
 */
extern void print(const char* format, ...);
}

// Current function macro.
#ifdef WIN32
#define __current__func__ __FUNCTION__
#else
#define __current__func__ __func__
#endif

// Assert macros.
#ifdef _DEBUG
#define GP_ASSERT(expression) assert(expression)
#else
#define GP_ASSERT(expression)
#endif

// Error macro.
#ifdef GP_ERRORS_AS_WARNINGS
#define GP_ERROR GP_WARN
#else
#define GP_ERROR(...) do \
    { \
        gameplay::Logger::log(gameplay::Logger::LEVEL_ERROR, "%s -- ", __current__func__); \
        gameplay::Logger::log(gameplay::Logger::LEVEL_ERROR, __VA_ARGS__); \
        gameplay::Logger::log(gameplay::Logger::LEVEL_ERROR, "\n"); \
        assert(0); \
        std::exit(-1); \
    } while (0)
#endif

// Warning macro.
#define GP_WARN(...) do \
    { \
        gameplay::Logger::log(gameplay::Logger::LEVEL_WARN, "%s -- ", __current__func__); \
        gameplay::Logger::log(gameplay::Logger::LEVEL_WARN, __VA_ARGS__); \
        gameplay::Logger::log(gameplay::Logger::LEVEL_WARN, "\n"); \
    } while (0)

#if defined(WIN32)
    #pragma warning( disable : 4005 )
    #pragma warning( disable : 4172 )
    #pragma warning( disable : 4244 )
    #pragma warning( disable : 4267 )
    #pragma warning( disable : 4311 )
	#pragma warning( disable : 4316 )
    #pragma warning( disable : 4390 )
    #pragma warning( disable : 4800 )
    #pragma warning( disable : 4996 )
#endif

// Bullet Physics
#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#define BV(v) (btVector3((v).x, (v).y, (v).z))
#define BQ(q) (btQuaternion((q).x, (q).y, (q).z, (q).w))

// Debug new for memory leak detection
#include "DebugNew.h"

// Object deletion macro
#define SAFE_DELETE(x) \
    { \
        delete x; \
        x = NULL; \
    }

// Array deletion macro
#define SAFE_DELETE_ARRAY(x) \
    { \
        delete[] x; \
        x = NULL; \
    }

// Ref cleanup macro
#define SAFE_RELEASE(x) \
    if (x) \
    { \
        (x)->release(); \
        x = NULL; \
    }

// Math
#define MATH_DEG_TO_RAD(x)          ((x) * 0.0174532925f)
#define MATH_RAD_TO_DEG(x)          ((x)* 57.29577951f)
#define MATH_RANDOM_MINUS1_1()      ((2.0f*((float)rand()/RAND_MAX))-1.0f)      // Returns a random float between -1 and 1.
#define MATH_RANDOM_0_1()           ((float)rand()/RAND_MAX)                    // Returns a random float between 0 and 1.
#define MATH_FLOAT_SMALL            1.0e-37f
#define MATH_TOLERANCE              2e-37f
#define MATH_E                      2.71828182845904523536f
#define MATH_LOG10E                 0.4342944819032518f
#define MATH_LOG2E                  1.442695040888963387f
#define MATH_PI                     3.14159265358979323846f
#define MATH_PIOVER2                1.57079632679489661923f
#define MATH_PIOVER4                0.785398163397448309616f
#define MATH_PIX2                   6.28318530717958647693f
#define MATH_EPSILON                0.000001f
#define MATH_CLAMP(x, lo, hi)       ((x < lo) ? lo : ((x > hi) ? hi : x))
#ifndef M_1_PI
#define M_1_PI                      0.31830988618379067154
#endif

// NOMINMAX makes sure that windef.h doesn't add macros min and max
#ifdef WIN32
    #define NOMINMAX
#endif

// Audio (OpenAL/Vorbis)
#ifdef __QNX__
#include <AL/al.h>
#include <AL/alc.h>
#elif __ANDROID__
#include <AL/al.h>
#include <AL/alc.h>
#elif __linux__
#include <AL/al.h>
#include <AL/alc.h>
#elif WIN32
#include <al.h>
#include <alc.h>
#elif __APPLE__
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#endif
#include <vorbis/vorbisfile.h>

// Image
#include <png.h>

// Scripting
using std::va_list;
#include <lua.hpp>

#define WINDOW_VSYNC        1

// Graphics (OpenGL)
#ifdef __QNX__
    #include <EGL/egl.h>
    #include <GLES2/gl2.h>
    #include <GLES2/gl2ext.h>
    #include <screen/screen.h>
    extern PFNGLBINDVERTEXARRAYOESPROC glBindVertexArray;
    extern PFNGLDELETEVERTEXARRAYSOESPROC glDeleteVertexArrays;
    extern PFNGLGENVERTEXARRAYSOESPROC glGenVertexArrays;
    extern PFNGLISVERTEXARRAYOESPROC glIsVertexArray;
    #define GL_DEPTH24_STENCIL8 GL_DEPTH24_STENCIL8_OES
    #define glClearDepth glClearDepthf
    #define OPENGL_ES
    #define USE_PVRTC
    #ifdef __arm__
        #define USE_NEON
    #endif
#elif __ANDROID__
    #include <EGL/egl.h>
    #include <GLES2/gl2.h>
    #include <GLES2/gl2ext.h>
    extern PFNGLBINDVERTEXARRAYOESPROC glBindVertexArray;
    extern PFNGLDELETEVERTEXARRAYSOESPROC glDeleteVertexArrays;
    extern PFNGLGENVERTEXARRAYSOESPROC glGenVertexArrays;
    extern PFNGLISVERTEXARRAYOESPROC glIsVertexArray;
    #define GL_DEPTH24_STENCIL8 GL_DEPTH24_STENCIL8_OES
    #define glClearDepth glClearDepthf
    #define OPENGL_ES
#elif WIN32
    #define WIN32_LEAN_AND_MEAN
    #define GLEW_STATIC
    #include <GL/glew.h>
    #define USE_VAO
#elif __linux__
        #define GLEW_STATIC
        #include <GL/glew.h>
        #define USE_VAO
#elif __APPLE__
    #include "TargetConditionals.h"
    #if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
        #include <OpenGLES/ES2/gl.h>
        #include <OpenGLES/ES2/glext.h>
        #define glBindVertexArray glBindVertexArrayOES
        #define glDeleteVertexArrays glDeleteVertexArraysOES
        #define glGenVertexArrays glGenVertexArraysOES
        #define glIsVertexArray glIsVertexArrayOES
        #define GL_DEPTH24_STENCIL8 GL_DEPTH24_STENCIL8_OES
        #define glClearDepth glClearDepthf
        #define OPENGL_ES
        #define USE_VAO
        #ifdef __arm__
            #define USE_NEON
        #endif
    #elif TARGET_OS_MAC
        #include <OpenGL/gl.h>
        #include <OpenGL/glext.h>
        #define glBindVertexArray glBindVertexArrayAPPLE
        #define glDeleteVertexArrays glDeleteVertexArraysAPPLE
        #define glGenVertexArrays glGenVertexArraysAPPLE
        #define glIsVertexArray glIsVertexArrayAPPLE
        #define USE_VAO
    #else
        #error "Unsupported Apple Device"
    #endif
#endif

// Graphics (GLSL)
#define VERTEX_ATTRIBUTE_POSITION_NAME              "a_position"
#define VERTEX_ATTRIBUTE_NORMAL_NAME                "a_normal"
#define VERTEX_ATTRIBUTE_COLOR_NAME                 "a_color"
#define VERTEX_ATTRIBUTE_TANGENT_NAME               "a_tangent"
#define VERTEX_ATTRIBUTE_BINORMAL_NAME              "a_binormal"
#define VERTEX_ATTRIBUTE_BLENDWEIGHTS_NAME          "a_blendWeights"
#define VERTEX_ATTRIBUTE_BLENDINDICES_NAME          "a_blendIndices"
#define VERTEX_ATTRIBUTE_TEXCOORD_PREFIX_NAME       "a_texCoord"

// Hardware buffer
namespace gameplay
{
/** Vertex attribute. */
typedef GLint VertexAttribute;
/** Vertex buffer handle. */
typedef GLuint VertexBufferHandle;
/** Index buffer handle. */
typedef GLuint IndexBufferHandle;
/** Texture handle. */
typedef GLuint TextureHandle;
/** Frame buffer handle. */
typedef GLuint FrameBufferHandle;
/** Render buffer handle. */
typedef GLuint RenderBufferHandle;

/** Gamepad handle definitions vary by platform. */
#if defined(__QNX__) && defined(GP_USE_GAMEPAD)
    typedef screen_device_t GamepadHandle;
#elif defined(WIN32)
    typedef unsigned long GamepadHandle;
#else
    typedef unsigned int GamepadHandle;
#endif

#if defined(__QNX__) && defined(GP_USE_SOCIAL)
    typedef void* SocialPlayerHandle;
    typedef void* SocialAchievementHandle;
    typedef void* SocialScoreHandle;
    typedef void* SocialChallengeHandle;
#elif defined(WIN32)
    typedef unsigned long SocialPlayerHandle;
    typedef unsigned long SocialAchievementHandle;
    typedef unsigned long SocialScoreHandle;
    typedef unsigned long SocialChallengeHandle;
#else
    typedef unsigned int SocialPlayerHandle;
    typedef unsigned int SocialAchievementHandle;
    typedef unsigned int SocialScoreHandle;
    typedef unsigned int SocialChallengeHandle;
#endif

}

/**
 * GL assertion that can be used for any OpenGL function call.
 *
 * This macro will assert if an error is detected when executing
 * the specified GL code. This macro will do nothing in release
 * mode and is therefore safe to use for realtime/per-frame GL
 * function calls.
 */
#ifdef NDEBUG
#define GL_ASSERT( gl_code ) gl_code
#else
#define GL_ASSERT( gl_code ) do \
    { \
        gl_code; \
        __gl_error_code = glGetError(); \
        GP_ASSERT(__gl_error_code == GL_NO_ERROR); \
    } while(0)
#endif

/** Global variable to hold GL errors
 * @script{ignore} */
extern GLenum __gl_error_code;

/**
 * Executes the specified AL code and checks the AL error afterwards
 * to ensure it succeeded.
 *
 * The AL_LAST_ERROR macro can be used afterwards to check whether a AL error was
 * encountered executing the specified code.
 */
#define AL_CHECK( al_code ) do \
    { \
        while (alGetError() != AL_NO_ERROR) ; \
        al_code; \
        __al_error_code = alGetError(); \
        if (__al_error_code != AL_NO_ERROR) \
        { \
            GP_ERROR(#al_code ": %d", (int)__al_error_code); \
        } \
    } while(0)

/** Global variable to hold AL errors
 * @script{ignore} */
extern ALenum __al_error_code;

/**
 * Accesses the most recently set global AL error.
 */
#define AL_LAST_ERROR() __al_error_code

#endif
