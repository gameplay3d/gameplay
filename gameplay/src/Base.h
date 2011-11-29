/*
 * Base.h
 */
#ifndef BASE_H_
#define BASE_H_

// C/C++
#include <new>
#include <cstdio>
#include <cassert>
#include <memory>
#include <iostream>
#include <string>
#include <cwchar>
#include <cwctype>
#include <cctype>
#include <cmath>
#include <vector>
#include <list>
#include <stack>
#include <map>
#include <algorithm>
#include <ctime>
#include <limits>
#include <functional>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <cstdarg>

// Common
#ifndef NULL
#define NULL     0
#endif

// Print logging (implemented per platform)
namespace gameplay
{
extern void printError(const char* format, ...);
}

// System Errors
#define LOG_ERROR(x) \
    { \
        printError(x); \
        assert(#x == 0); \
    }
#define LOG_ERROR_VARG(x, ...) \
    { \
        printError(x, __VA_ARGS__); \
        assert(#x == 0); \
    }

// Warning macro
#ifdef WARN
#undef WARN
#endif
#define WARN(x) printError(x)
#define WARN_VARG(x, ...) printError(x, __VA_ARGS__)

// Bullet Physics
#include <btBulletDynamicsCommon.h>

// Since Bullet overrides new, we have to allocate objects manually using its
// aligned allocation function when we turn on memory leak detection in GamePlay.
#ifdef GAMEPLAY_MEM_LEAK_DETECTION
#define BULLET_NEW(type, name, ...) \
    type* name = (type*)btAlignedAlloc(sizeof(type), 16); \
    type __##name##_tmp (__VA_ARGS__); \
    memcpy(name, &__##name##_tmp, sizeof(type))

#define BULLET_DELETE(name) \
    if (name) \
    { \
        btAlignedFree(name); \
        name = NULL; \
    }

#else
#define BULLET_NEW(type, name, ...) \
    type* name = new type(__VA_ARGS__)

#define BULLET_DELETE(name) SAFE_DELETE(name)
#endif


// Debug new for memory leak detection
#ifdef GAMEPLAY_MEM_LEAK_DETECTION
#include "DebugNew.h"
#endif

// Object deletion macro
#define SAFE_DELETE(x) \
    if (x) \
    { \
        delete x; \
        x = NULL; \
    }

// Array deletion macro
#define SAFE_DELETE_ARRAY(x) \
    if (x) \
    { \
        delete[] x; \
        x = NULL; \
    }

// Ref cleanup macro
#define SAFE_RELEASE(x) \
    if (x) \
    { \
        x->release(); \
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
#define MATH_PIOVER4                M_PI_4
#define MATH_PIX2                   6.28318530717958647693f
#define MATH_EPSILON                0.000001f
#ifndef M_1_PI
#define M_1_PI                      0.31830988618379067154
#endif

// Audio (OpenAL)
#ifdef __QNX__
#include <AL/al.h>
#include <AL/alc.h>
#elif WIN32
#include <al.h>
#include <alc.h>
#elif __APPLE__
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#endif

// Screen/Window
#define WINDOW_WIDTH        1024
#define WINDOW_HEIGHT       600
#define WINDOW_VSYNC        1
#define WINDOW_FULLSCREEN   0

// Image
#include <png.h>

// Graphics (OpenGL)
#ifdef __QNX__
    #include <EGL/egl.h>
    #include <GLES2/gl2.h>
    #include <GLES2/gl2ext.h>
    extern PFNGLBINDVERTEXARRAYOESPROC glBindVertexArray;
    extern PFNGLDELETEVERTEXARRAYSOESPROC glDeleteVertexArrays;
    extern PFNGLGENVERTEXARRAYSOESPROC glGenVertexArrays;
    extern PFNGLISVERTEXARRAYOESPROC glIsVertexArray;
    #define glClearDepth glClearDepthf
   #define OPENGL_ES
#elif WIN32
    #define WIN32_LEAN_AND_MEAN
    #include <GL/glew.h>
    #include <GL/wglew.h>
#elif __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#define glBindVertexArray glBindVertexArrayAPPLE
#define glDeleteVertexArrays glDeleteVertexArraysAPPLE
#define glGenVertexArrays glGenVertexArraysAPPLE
#define glIsVertexArray glIsVertexArrayAPPLE
#endif

// Graphics (GLSL)
#define VERTEX_ATTRIBUTE_POSITION_NAME              "a_position"
#define VERTEX_ATTRIBUTE_NORMAL_NAME                "a_normal"
#define VERTEX_ATTRIBUTE_COLOR_NAME                 "a_color"
#define VERTEX_ATTRIBUTE_TANGENT_NAME               "a_tangent"
#define VERTEX_ATTRIBUTE_BINORMAL_NAME              "a_binormal"
#define VERTEX_ATTRIBUTE_BLENDWEIGHTS_NAME          "a_blendWeights"
#define VERTEX_ATTRIBUTE_BLENDINDICES_NAME          "a_blendIndices"
#define VERTEX_ATTRIBUTE_TEXCOORD_PREFIX            "a_texCoord"

// Hardware Resources
namespace gameplay
{
typedef GLint VertexAttribute;
typedef GLuint VertexBuffer;
typedef GLuint IndexBuffer;
typedef GLuint TextureHandle;
typedef GLuint FrameBufferHandle;
typedef GLuint RenderBufferHandle;
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
#define GL_ASSERT( gl_code ) \
    { \
        gl_code; \
        __gl_error_code = glGetError(); \
        if (__gl_error_code != GL_NO_ERROR) \
        { \
            LOG_ERROR_VARG(#gl_code ": %d", (int)__gl_error_code); \
        } \
        assert(__gl_error_code == GL_NO_ERROR); \
    }
#endif

/**
 * Executes the specified GL code and checks the GL error afterwards
 * to ensure it succeeded.
 *
 * This macro should be used instead of GL_ASSERT for code that must
 * be checked in both debug and release builds. The GL_LAST_ERROR
 * macro can be used afterwards to check whether a GL error was
 * encountered executing the specified code.
 */
#define GL_CHECK( gl_code ) \
    { \
        while (glGetError() != GL_NO_ERROR) ; \
        gl_code; \
        __gl_error_code = glGetError(); \
        if (__gl_error_code != GL_NO_ERROR) \
        { \
            LOG_ERROR_VARG(#gl_code ": %d", (int)__gl_error_code); \
        } \
    }

// Global variable to hold GL errors
extern GLenum __gl_error_code;

/**
 * Accesses the most recently set global GL error.
 */
#define GL_LAST_ERROR() __gl_error_code


#if defined(WIN32) || defined(__APPLE__)

    inline float fminf(float a, float b)
    {
      return a < b ? a : b;
    }
    inline float fmin(float a, float b)
    {
      return a < b ? a : b;
    }
    inline float fmaxf(float a, float b)
    {
      return a > b ? a : b;
    }
    inline float fmax(float a, float b)
    {
      return a > b ? a : b;
    }
        
    #pragma warning( disable : 4172 )
    #pragma warning( disable : 4244 )
    #pragma warning( disable : 4311 )
    #pragma warning( disable : 4390 )
    #pragma warning( disable : 4800 )
    #pragma warning( disable : 4996 )
#endif

#endif
