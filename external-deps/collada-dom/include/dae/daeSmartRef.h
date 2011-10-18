/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 

#ifndef __DAE_SMARTREF_H__
#define __DAE_SMARTREF_H__

#include <assert.h>
#include <dae/daeRefCountedObj.h>

/**
 * The @c daeSmartRef template class automates reference counting for
 * objects derived from @c daeRefCountedObj.
 */
template<class T> class daeSmartRef
{
public:
	/**
	 * Constructor
	 */
	inline daeSmartRef() : _ptr(NULL) { }

	/**
	 * Destructor
	 */
	inline ~daeSmartRef() {
		checkedRelease(_ptr);
	}

	/**
	 * Copy Constructor that will convert from one template to the other.
	 * @param smartRef a daeSmartRef to the object to copy from.
	 */
	template<class U>
	inline daeSmartRef(const daeSmartRef<U>& smartRef) : _ptr(smartRef.cast()) {
		checkedRef(_ptr);
	}

	/**
	 * Function that returns a pointer to object being reference counted.
	 * @return the object being reference counted.
	 */
	inline T* cast() const { return _ptr; }

	/**
	 * Copy Constructor.
	 * @param smartRef a daeSmartRef of the same template type to copy from
	 */
	inline daeSmartRef(const daeSmartRef<T>& smartRef) : _ptr(smartRef._ptr) {
		checkedRef(_ptr);
	}

	/**
	 * Constructor
	 * @param ptr a pointer to an object of the same template type.
	 */
	inline daeSmartRef(T* ptr) : _ptr(ptr) {
		checkedRef(_ptr);
	}

	/**
	 * Overloaded assignment operator which will convert between template types.
	 * @param smartRef a daeSmartRef to the object to copy from.
	 * @return Returns a reference to this object.
	 */
	template<class U>
	inline const daeSmartRef<T>& operator=(const daeSmartRef<U>& smartRef) {
		T* ptr = smartRef.cast();
		checkedRef(ptr);
		checkedRelease(_ptr);
		_ptr = ptr;
		return *this; }

	/**
	 * Overloaded assignment operator.
	 * @param other a daeSmartRef to the object to copy from.  Must be of the same template type.
	 * @return Returns a reference to this object.
	 */
	inline const daeSmartRef<T>& operator=(const daeSmartRef<T>& other) {
		T* ptr = other._ptr;
		checkedRef(ptr);
		checkedRelease(_ptr);
		_ptr = ptr;
		return *this; }

	/**
	 * Overloaded assignment operator.
	 * @param ptr a pointer to the object to copy from.  Must be of the same template type.
	 * @return Returns a reference to this object.
	 */
	inline const daeSmartRef<T>& operator=(T* ptr) {
		checkedRef(ptr);
		checkedRelease(_ptr);
		_ptr = ptr;
		return *this; }

	/**
	 * Overloaded member selection operator.
	 * @return a pointer of the template class to the object.
	 */
	inline T* operator->() const {
		assert (_ptr != (T*)NULL); return _ptr; }

	/**
	 * Overloaded cast operator.
	 * @return a pointer of the template class to the object.
	 */
	inline operator T*() const {
		return _ptr; }
	
	/**
	 * Static cast function.
	 * @param smartRef a smartRef to cast from
	 * @return a pointer to an object of this template class
	 */
	template<class U>
	inline static T* staticCast(const daeSmartRef<U>& smartRef) {
		return static_cast<T*>(smartRef.cast()); }

private:
	/* The pointer to the element which is being reference counted */
	T* _ptr;
};

#endif // __DAE_SMARTREF_H__





