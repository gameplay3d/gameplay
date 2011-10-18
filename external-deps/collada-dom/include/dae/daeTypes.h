/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 

#ifndef __DAE_TYPES_H__
#define __DAE_TYPES_H__

#include <dae/daePlatform.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <wchar.h>
#include <string.h>

#include <dae/daeError.h>

#define daeOffsetOf(class, member) \
 ((size_t)&(((class*)0x0100)->member) - (size_t)0x0100)
 
typedef PLATFORM_INT8		daeChar;
typedef PLATFORM_INT16		daeShort;
typedef PLATFORM_INT32		daeInt;
typedef PLATFORM_INT64		daeLong;
typedef PLATFORM_UINT8		daeUChar;
typedef PLATFORM_UINT16		daeUShort;
typedef PLATFORM_UINT32		daeUInt;
typedef PLATFORM_UINT64		daeULong;
typedef PLATFORM_FLOAT32	daeFloat;
typedef PLATFORM_FLOAT64	daeDouble;

// base types

typedef const char*			daeString;
typedef bool				daeBool;
typedef const void*			daeConstRawRef;
typedef void*				daeRawRef;
typedef daeInt				daeEnum;
typedef daeChar*			daeMemoryRef;

typedef daeChar				daeFixedName[512];

#include <dae/daeArray.h>
#include <dae/daeArrayTypes.h>

#endif //__DAE_TYPES_H__
