/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 

#ifndef __DAE_MEMORY_SYSTEM_H__
#define __DAE_MEMORY_SYSTEM_H__

#include <dae/daeTypes.h>

/**
 * The @c daeMemorySystem class is a simple wrapper for memory operations.
 * Every allocation passes a string pool name such that
 * in the future different pools can be used based on allocation type.
 * Currently the system is just a pass-through to system @c malloc.
 */
class daeMemorySystem
{
public:
	/**
	 * Provides a wrapper malloc with pool field.
	 * @param pool String name of the pool to use for this allocation.
	 * @param n Number of bytes to allocate.
	 * @return Returns the memory allocated if successful, or NULL if not.
	 */
	static DLLSPEC daeRawRef alloc(daeString pool, size_t n);

	/**
	 * Provides a wrapper free with pool argument.
	 * @param pool Pool the memory should be freed from.
	 * @param mem Memory to be freed.
	 */
	static DLLSPEC void dealloc(daeString pool, daeRawRef mem);
};

// (steveT) These new/delete overrides aren't complete. What about new[] and delete[]?
// Standard new should throw a bad_alloc exception, and a nothrow new should be provided
// that returns null instead of throwing bad_alloc. Because of these problems, plus the
// fact that we currently don't benefit in any way from overriding new and delete, this
// code is currently disabled.

#if 0
#define DAE_ALLOC \
	/* Standard new/delete */ \
	inline void* operator new(size_t size) { return daeMemorySystem::alloc("meta", size); } \
	inline void operator delete(void* p) { daeMemorySystem::dealloc("meta", p); } \
	/* Placement new/delete */ \
	inline void* operator new(size_t, void* p) { return p; } \
	inline void operator delete(void*, void*) { }
#endif

#define DAE_ALLOC

#endif // __DAE_MEMORY_H__
