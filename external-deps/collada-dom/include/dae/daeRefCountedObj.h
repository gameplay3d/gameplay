/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 

#ifndef daeRefCountedObj_h
#define daeRefCountedObj_h

#include <dae/daeTypes.h>
#include <dae/daePlatform.h>

class DLLSPEC daeRefCountedObj {
protected:
	mutable daeInt _refCount;

public:
	daeRefCountedObj();
	virtual ~daeRefCountedObj();

	/**
	 * Decrements the reference count and deletes the object if reference count is zero.
	 * @note Should not be used externally if daeSmartRefs are being used, they call it
	 * automatically.
	 */
	void release() const;

	/**
	 * Increments the reference count of this element.
	 * @note Should not be used externally if daeSmartRefs are being used, they call it
	 * automatically.
	 */
	void ref() const;
};

void DLLSPEC checkedRelease(const daeRefCountedObj* obj);
void DLLSPEC checkedRef(const daeRefCountedObj* obj);

#endif
