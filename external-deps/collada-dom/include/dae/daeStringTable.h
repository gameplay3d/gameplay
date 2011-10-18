/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 

#ifndef __DAE_STRING_TABLE_H__
#define __DAE_STRING_TABLE_H__
#include <dae/daeTypes.h>
#include <dae/daeMemorySystem.h>

/**
 * The @c daeStringTable is a simple string table class to hold a float list of strings
 * without a lot of allocations.
 */
class daeStringTable
{
public: // allocate/construct/destruct/deallocate
	/**
	 * Macro that defines new and delete overrides for this class
	 */
	DAE_ALLOC
	/**
	 * Constructor which specifies fixed buffer size.
	 * @param stringBufferSize The size of the buffer to create for string allocation.
	 */
	DLLSPEC daeStringTable(int stringBufferSize = 1024*1024);

	/**
	 * Destructor.
	 */
	~daeStringTable() { clear(); }

public: // INTERFACE
	/**
	 * Allocates a string from the table.
	 * @param string <tt> const char * </tt> to copy into the table.
	 * @return Returns an allocated string.
	 */
	DLLSPEC daeString allocString(daeString string);

	/**
	 * Clears the storage.
	 */
	DLLSPEC void clear();

private: // MEMBERS
	size_t _stringBufferSize;
	size_t _stringBufferIndex;
	daeStringArray _stringBuffersList;

	daeString allocateBuffer();

	daeString _empty;
};

#endif //__DAE_STRING_TABLE_H__
