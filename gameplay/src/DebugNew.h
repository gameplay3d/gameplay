#ifndef DEBUGNEW_H_
#define DEBUGNEW_H_

/**
 * Global overrides of the new and delete operators for memory tracking.
 * This file is only included when memory leak detection is explicitly
 * request via the pre-processor definition GAMEPLAY_MEM_LEAK_DETECTION.
 */
#ifdef GAMEPLAY_MEM_LEAK_DETECTION

#include <new>
#include <exception>

// Prints all heap and reference leaks to stderr.
extern void printMemoryLeaks();

// global new/delete operator overloads
#ifdef _MSC_VER
#pragma warning( disable : 4290 ) // C++ exception specification ignored.
#endif
void* operator new (std::size_t size, const char* file, int line);
void* operator new[] (std::size_t size, const char* file, int line);
void* operator new (std::size_t size) throw(std::bad_alloc);
void* operator new[] (std::size_t size) throw(std::bad_alloc);
void* operator new (std::size_t size, const std::nothrow_t&) throw();
void* operator new[] (std::size_t size, const std::nothrow_t&) throw();
void operator delete (void* p) throw();
void operator delete[] (void* p) throw();
void operator delete (void* p, const char* file, int line) throw();
void operator delete[] (void* p, const char* file, int line) throw();
#ifdef _MSC_VER
#pragma warning( default : 4290 )
#endif

// Re-define new to use versions with file and line number
#define DEBUG_NEW new (__FILE__, __LINE__)
#define new DEBUG_NEW

#endif

// Since Bullet overrides new, we define custom functions to allocate Bullet objects that undef
// 'new' before allocation and redefine it to our custom version afterwards (we support 0-2 parameter constructors).
template<typename T> T* bullet_new()
{
#ifdef GAMEPLAY_MEM_LEAK_DETECTION
#undef new
    T* t = new T();
#define new DEBUG_NEW
    return t;
#else
    return new T();
#endif
}

template<typename T, typename T1> T* bullet_new(T1 t1)
{
#ifdef GAMEPLAY_MEM_LEAK_DETECTION 
#undef new 
    T* t = new T(t1);
#define new DEBUG_NEW
    return t;
#else
    return new T(t1);
#endif
}

template<typename T, typename T1, typename T2> T* bullet_new(T1 t1, T2 t2)
{
#ifdef GAMEPLAY_MEM_LEAK_DETECTION
#undef new
    T* t = new T(t1, t2);
#define new DEBUG_NEW
    return t;
#else
    return new T(t1, t2);
#endif
}

#endif
