/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 

#ifndef __DAE_WIN32_PLATFORM_H__
#define __DAE_WIN32_PLATFORM_H__

#define PLATFORM_INT8	__int8
#define PLATFORM_INT16	__int16
#define PLATFORM_INT32	__int32
#define PLATFORM_INT64	__int64
#define PLATFORM_UINT8	unsigned __int8
#define PLATFORM_UINT16 unsigned __int16
#define PLATFORM_UINT32 unsigned __int32
#define PLATFORM_UINT64 unsigned __int64
#define PLATFORM_FLOAT32 float
#define PLATFORM_FLOAT64 double

#if _MSC_VER <= 1200
typedef int intptr_t;
#endif

#ifdef DOM_DYNAMIC

#ifdef DOM_EXPORT
#define DLLSPEC __declspec( dllexport )
#else
#define DLLSPEC __declspec( dllimport )
#endif

#else
#define DLLSPEC
#endif

// GCC doesn't understand "#pragma warning"
#ifdef _MSC_VER
// class 'std::auto_ptr<_Ty>' needs to have dll-interface to be used by clients of class 'daeErrorHandler'
#pragma warning(disable: 4251)
// warning C4100: 'profile' : unreferenced formal parameter
#pragma warning(disable: 4100)
// warning C4355: 'this' : used in base member initializer list
#pragma warning(disable: 4355)
// warning C4512: 'daeDatabase' : assignment operator could not be generated
#pragma warning(disable: 4512)
// warning LNK4099: Missing pdb file for PCRE
#pragma warning(disable: 4099)
#endif

#endif
