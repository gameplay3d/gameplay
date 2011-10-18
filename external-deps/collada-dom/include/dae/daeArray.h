/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 

#ifndef __DAE_ARRAY_H__
#define __DAE_ARRAY_H__
#include <new>
#include <dae/daeMemorySystem.h>

class daeAtomicType;

/**
 * COLLADA C++ class that implements storage for resizable array containers.
 */
class daeArray
{
protected:
	size_t         _count;
	size_t         _capacity;
	daeMemoryRef   _data;
	size_t         _elementSize;
	daeAtomicType* _type;
public:
	/**
	 * Constructor
	 */
	DLLSPEC daeArray();
	/**
	 * Destructor
	 */
	virtual DLLSPEC ~daeArray();
	/**
	 * Clears the contents of the array. Do not use this function if the array contains @c daeSmartRef objects and the
	 * @c dom* class the array belongs to has a @c _contents member.
	 *
	 * Many @c dom* objects have a @c _contents member that stores the original creation order of the @c daeElements
	 * that are their children.  If you use @c clear() on a @c daeArray of @c daeSmartRef derived objects, these
	 * objects will not be removed from @c _contents, which can cause problems when you
	 * save the data.  We recommended that @c clear() not be used on arrays that are part of a @c dom* object.
	 */
	virtual DLLSPEC void clear() = 0;
	/**
	 * Sets the size of an element in the array. This clears and reinitializes the array.
	 * @param elementSize Size of an element in the array.
	 */
	DLLSPEC void setElementSize(size_t elementSize);
	/**
	 * Gets the size of an element in this array.
	 * @return Returns the size of an element in this array.
	 */
	size_t getElementSize() const {return _elementSize;}
	/**
	 * Grows the array to the specified size and sets the @c daeArray to that size.
	 * @param cnt Size to grow the array to.
	 */
	virtual void setCount(size_t cnt) = 0;
	/**
	 * Gets the number of items stored in this @c daeArray.
	 * @return Returns the number of items stored in this @c daeArray.
	 */
	size_t getCount() const {return _count;}
	/**
	 * Increases the capacity of the @c daeArray.
	 * @param minCapacity The minimum array capacity (the actual resulting capacity may be higher).
	 */
	virtual void grow(size_t minCapacity) = 0;
	/**
	 * Gets the current capacity of the array, the biggest it can get without incurring a realloc.
	 * @return Returns the capacity of the array.
	 */
	size_t getCapacity() const {return _capacity;}
	/**
	 * Gets a pointer to the raw memory of a particular element.
	 * @return Returns a pointer to the memory for the raw data.
	 */
	daeMemoryRef getRaw(size_t index) const {return _data + index*_elementSize;}
	/**
	 * Removes an item at a specific index in the @c daeArray. 
	 * @param index  Index number of the item to delete.
	 * @return Returns DAE_OK if success, a negative value defined in daeError.h otherwise.
	 * @note The @c daeElement objects sometimes list
	 * objects in two places, the class member and the <i> @c _contents </i> array, when you remove something from the
	 * dom, you must remove it from both places.
	 */
	virtual daeInt removeIndex(size_t index) = 0;

	// Provided for backward compatibility only. Don't use these.
	void setRawCount(size_t cnt) { setCount(cnt); } // Use setCount instead
	daeMemoryRef getRawData() const {return _data;} // Use getRaw instead
};

/**
 * COLLADA C++ templated version of @c daeArray for storing items of various types.
 */
template <class T>
class daeTArray : public daeArray
{
protected:
	T* prototype;
public:
	/**
	 *  Constructor.
	 */
	daeTArray() {
		_elementSize = sizeof( T );
		prototype = NULL;
	}
	/**
	 *  Constructor.
	 */
	explicit daeTArray(T* prototype) : prototype(prototype) {
		_elementSize = sizeof( T );
	}
	/**
	 * Copy Constructor
	 */
	daeTArray( const daeTArray<T> &cpy ) : daeArray() {
		prototype = NULL;
		*this = cpy;
	}
	/**
	 * Constructor that takes one element and turns into an array
	 */
	explicit daeTArray( const T &el ) {
		_elementSize = sizeof(T);
		prototype = NULL;
		append( el );
	}
	/**
	 *  Destructor.
	 */
	virtual ~daeTArray() {
		clear();
		delete prototype;
	}
	/**
	 * Frees the memory in this array and resets it to it's initial state.
	 */
	virtual void clear()
	{
		for(size_t i=0;i<_count;i++)
			((T*)_data + i)->~T();
		free(_data);
		_count = 0;
		_capacity = 0;
		_data = NULL;
	}
	
	/**
	 * Increases the capacity of the @c daeArray.
	 * @param minCapacity The minimum array capacity (the actual resulting capacity may be higher).
	 */
	void grow(size_t minCapacity) {
		if (minCapacity <= _capacity)
			return;

		size_t newCapacity = _capacity == 0 ? 1 : _capacity;
		while(newCapacity < minCapacity)
			newCapacity *= 2;
	
		T* newData = (T*)malloc(newCapacity*_elementSize);
		for (size_t i = 0; i < _count; i++) {
			new (&newData[i]) T(get(i));
			((T*)_data + i)->~T();
		}
	
		if (_data != NULL)
			free(_data);
	
		_data = (daeMemoryRef)newData;
		_capacity = newCapacity;
	}

	/**
	 * Removes an item at a specific index in the @c daeArray. 
	 * @param index  Index number of the item to delete.
	 * @return Returns DAE_OK if success, a negative value defined in daeError.h otherwise.
	 * @note The @c daeElement objects sometimes list
	 * objects in two places, the class member and the <i> @c _contents </i> array, when you remove something from the
	 * dom, you must remove it from both places.
	 */
	virtual daeInt removeIndex(size_t index)
	{
		if (index >= _count)
			return(DAE_ERR_INVALID_CALL);

		for (size_t i = index; i < _count-1; i++)
			*((T*)_data+i) = *((T*)_data+i+1);
		((T*)_data+(_count-1))->~T();
		_count--;
		return DAE_OK;
	}

	/**
	 * Resets the number of elements in the array. If the array increases in size, the new 
	 * elements will be initialized to the specified value.
	 * @param nElements The new size of the array.
	 * @param value The value new elements will be initialized to.
	 * @note Shrinking the array does NOT free up memory.
	 */
	void setCount(size_t nElements, const T& value)
	{
		grow(nElements);
		// Destruct the elements that are being chopped off
		for (size_t i = nElements; i < _count; i++)
			((T*)_data+i)->~T();
		// Use value to initialize the new elements
		for (size_t i = _count; i < nElements; i++)
			new ((void*)((T*)_data+i)) T(value);
		_count = nElements;
	}

	/**
	 * Resets the number of elements in the array. If the array increases in size, the new 
	 * elements will be initialized with a default constructor.
	 * @param nElements The new size of the array.
	 * @note Shrinking the array does NOT free up memory.
	 */
	virtual void setCount(size_t nElements) {
		if (prototype)
			setCount(nElements, *prototype);
		else
			setCount(nElements, T());
	}
	
	/**
	 * Sets a specific index in the @c daeArray, growing the array if necessary.
	 * @param index Index of the object to set, asserts if the index is out of bounds.
	 * @param value Value to store at index in the array.
	 */
	void set(size_t index, const T& value) {
		if (index >= _count)
			setCount(index+1);
		((T*)_data)[index] = value;
	}
	
	/**
	 * Gets the object at a specific index in the @c daeArray.
	 * @param index Index of the object to get, asserts if the index is out of bounds.
	 * @return Returns the object at index.
	 */
	T& get(size_t index) {
		assert(index < _count);
		return ((T*)_data)[index]; }
	/**
	 * Gets the object at a specific index in the @c daeArray.
	 * @param index Index of the object to get, asserts if the index is out of bounds.
	 * @return Returns the object at index.
	 */
	const T& get(size_t index) const {
		assert(index < _count);
		return ((T*)_data)[index]; }
	
	/**
	 * Appends a new object to the end of the @c daeArray.
	 * @param value Value of the object to append.
	 * @return Returns the index of the new object.
	 */
	size_t append(const T& value) {
		set(_count, value);
		return _count-1;
	}

	/**
	 * Appends a unique object to the end of the @c daeArray.
	 * Functions the same as @c append(), but does nothing if the value is already in the @c daeArray.
	 * @param value Value of the object to append.
	 * @return Returns the index where this value was appended. If the value already exists in the array, 
	 * returns the index in this array where the value was found.
	 */
	size_t appendUnique(const T& value) {
		size_t ret;
		if (find(value,ret) != DAE_OK)
			return append(value);
		else 
			return ret;
	}

	/**
	 * Adds a new item to the front of the @c daeArray.
	 * @param value Item to be added.
	 */
	void prepend(const T& value) {
		insertAt(0, value);
	}
	
	/**
	 * Removes an item from the @c daeArray.
	 * @param value A reference to the item to delete.
	 * @return Returns DAE_OK if success, a negative value defined in daeError.h otherwise.
	 * @note The @c daeElement objects sometimes list
	 * objects in two places, the class member and the <i> @c _contents </i> array, when you remove something from the
	 * do, you must remove it from both places.
	 */
	daeInt remove(const T& value, size_t *idx = NULL ) 
	{
		size_t index;
		if(find(value,index) == DAE_OK)
		{
			if ( idx != NULL ) {
				*idx = index;
			}
			return(removeIndex( index ));
		}
		else
		{
			return(DAE_ERR_INVALID_CALL);
		}
	}
	/**
	 * Finds an item from the @c daeArray.
	 * @param value A reference to the item to find.
	 * @param index If the function returns DAE_OK, this is set to the index where the value appears in the array.
	 * @return Returns DAE_OK if no error or DAE_ERR_QUERY_NO_MATCH if the value was not found.
	 */
	daeInt find(const T& value, size_t &index) const 
	{
		size_t i;
		for(i=0;i<_count;i++)
		{
			if (((T*)_data)[i] == value)
			{
				index = i;
				return DAE_OK;
			}
		}

		return DAE_ERR_QUERY_NO_MATCH;
	}
	/**
	 * Just like the previous function, but has a more reasonable interface.
	 * @param value The value to find.
	 * @return Returns a pointer to the value if found, null otherwise.
	 */
	T* find(const T& value) const {
		size_t i;
		if (find(value, i) == DAE_OK)
			return get(i);
		return NULL;
	}
	/**
	 * Gets the object at a specific index in the @c daeArray.
	 * @param index Index of the object to get, asserts if the index is out of bounds.
	 * @return Returns the object at @c index.
	 */
	T& operator[](size_t index) {
		assert(index < _count);
		return ((T*)_data)[index]; }
	/**
	 * Gets the object at a specific index in the @c daeArray.
	 * @param index Index of the object to get, asserts if the index is out of bounds.
	 * @return Returns the object at @c index.
	 */
	const T& operator[](size_t index) const {
		assert(index < _count);
		return ((T*)_data)[index]; }
	
	/**
	 * Inserts the specified number of elements at a specific location in the array.
	 * @param index Index into the array where the elements will be inserted
	 * @param n The number of elements to insert
	 * @param val The value to insert
	 */
	void insert(size_t index, size_t n, const T& val = T()) {
		if (index >= _count) {
			// Append to the end of the array
			size_t oldCount = _count;
			setCount(index + n); 
			for (size_t i = oldCount; i < _count; i++)
				get(i) = val;
		}
		else {
			setCount(_count + n);
			for (size_t i = _count-1; i >= index+n; i--)
				get(i) = get(i-n);
			for (size_t i = index; i < index+n; i++)
				get(i) = val;
		}
	}

	/**
	 * Inserts an object at a specific index in the daeArray, growing the array if neccessary
	 * @param index Index into the array for where to place the object
	 * @param value The object to append
	 */
	void insertAt(size_t index, const T& value) {
		insert(index, 1);
		get(index) = value;
	}

	/**
	 * Overloaded assignment operator.
	 * @param other A reference to the array to copy
	 * @return A reference to this object.
	 */
	daeTArray<T> &operator=( const daeTArray<T> &other ) {
		if (this != &other) {
			clear();
			_elementSize = other._elementSize;
			_type = other._type;
			grow(other._count);
			for(size_t i=0;i<other._count;i++)
				append(other[i]);
		}

		return *this;
	}

	/**
	 * Overloaded equality operator
	 * @param other A reference to the other array.
	 * @return true if the arrays are equal, false otherwise.
	 */
	bool operator==(const daeTArray<T>& other) {
		if (getCount() != other.getCount())
			return false;
		for (size_t i = 0; i < getCount(); i++)
			if (get(i) != other.get(i))
				return false;
		return true;
	}

	//some helpers
	/**
	 * Sets the array to the contain the two values specified.
	 * @param one The first value.
	 * @param two The second value.
	 */
	void set2( const T &one, const T &two )
	{
		setCount( 2 );
		set( 0, one );
		set( 1, two );
	}
	/**
	 * Sets the array to the contain the three values specified.
	 * @param one The first value.
	 * @param two The second value.
	 * @param three The third value.
	 */
	void set3( const T &one, const T &two, const T &three )
	{
		setCount( 3 );
		set( 0, one );
		set( 1, two );
		set( 2, three );
	}
	/**
	 * Sets the array to the contain the four values specified.
	 * @param one The first value.
	 * @param two The second value.
	 * @param three The third value.
	 * @param four The fourth value.
	 */
	void set4( const T &one, const T &two, const T &three, const T &four )
	{
		setCount( 4 );
		set( 0, one );
		set( 1, two );
		set( 2, three );
		set( 3, four );
	}

	/**
	 * Sets the values in the array at the specified location to the contain the two 
	 * values specified. This function will grow the array if needed.
	 * @param index The position in the array to start setting.
	 * @param one The first value.
	 * @param two The second value.
	 */
	void set2at( size_t index, const T &one, const T &two )
	{
		set( index, one );
		set( index+1, two );
	}
	/**
	 * Sets the values in the array at the specified location to the contain the three 
	 * values specified. This function will grow the array if needed.
	 * @param index The position in the array to start setting.
	 * @param one The first value.
	 * @param two The second value.
	 * @param three The third value.
	 */
	void set3at( size_t index, const T &one, const T &two, const T &three )
	{
		set( index, one );
		set( index+1, two );
		set( index+2, three );
	}
	/**
	 * Sets the values in the array at the specified location to the contain the four 
	 * values specified. This function will grow the array if needed.
	 * @param index The position in the array to start setting.
	 * @param one The first value.
	 * @param two The second value.
	 * @param three The third value.
	 * @param four The fourth value.
	 */
	void set4at( size_t index, const T &one, const T &two, const T &three, const T &four )
	{
		set( index, one );
		set( index+1, two );
		set( index+2, three );
		set( index+3, four );
	}

	/**
	 * Appends two values to the array.
	 * @param one The first value.
	 * @param two The second value.
	 */
	void append2( const T &one, const T &two )
	{
		append( one );
		append( two );
	}
	/**
	 * Appends three values to the array.
	 * @param one The first value.
	 * @param two The second value.
	 * @param three The third value.
	 */
	void append3( const T &one, const T &two, const T &three )
	{
		append( one );
		append( two );
		append( three );
	}
	/**
	 * Appends four values to the array.
	 * @param one The first value.
	 * @param two The second value.
	 * @param three The third value.
	 * @param four The fourth value.
	 */
	void append4( const T &one, const T &two, const T &three, const T &four )
	{
		append( one );
		append( two );
		append( three );
		append( four );
	}

	/**
	 * Inserts two values into the array at the specified location.
	 * @param index The position in the array to start inserting.
	 * @param one The first value.
	 * @param two The second value.
	 */
	void insert2at( size_t index, const T &one, const T &two )
	{
		insert(index, 2);
		set(index, one);
		set(index+1, two);
	}
	/**
	 * Inserts three values into the array at the specified location.
	 * @param index The position in the array to start inserting.
	 * @param one The first value.
	 * @param two The second value.
	 * @param three The third value.
	 */
	void insert3at( size_t index, const T &one, const T &two, const T &three )
	{
		insert(index, 3);
		set( index, one );
		set( index+1, two );
		set( index+2, three );
	}
	/**
	 * Inserts four values into the array at the specified location.
	 * @param index The position in the array to start inserting.
	 * @param one The first value.
	 * @param two The second value.
	 * @param three The third value.
	 * @param four The fourth value.
	 */
	void insert4at( size_t index, const T &one, const T &two, const T &three, const T &four )
	{
		insert(index, 4);
		set( index, one );
		set( index+1, two );
		set( index+2, three );
		set( index+4, four );
	}

	/**
	 * Gets two values from the array at the specified location.
	 * @param index The position in the array to start getting.
	 * @param one Variable to store the first value.
	 * @param two Variable to store the second value.
	 * @return Returns The number of elements retrieved.
	 */
	daeInt get2at( size_t index, T &one, T &two )
	{
		daeInt retVal = 0;
		if ( index < _count )
		{
			one = get(index);
			retVal++;
		}
		if ( index+1 < _count )
		{
			two = get(index+1);
			retVal++;
		}
		return retVal;
	} 
	/**
	 * Gets three values from the array at the specified location.
	 * @param index The position in the array to start getting.
	 * @param one Variable to store the first value.
	 * @param two Variable to store the second value.
	 * @param three Variable to store the third value.
	 * @return Returns The number of elements retrieved.
	 */
	daeInt get3at( size_t index, T &one, T &two, T &three )
	{
		daeInt retVal = 0;
		if ( index < _count )
		{
			one = get(index);
			retVal++;
		}
		if ( index+1 < _count )
		{
			two = get(index+1);
			retVal++;
		}
		if ( index+2 < _count )
		{
			three = get(index+2);
			retVal++;
		}
		return retVal;
	}
	/**
	 * Gets four values from the array at the specified location.
	 * @param index The position in the array to start getting.
	 * @param one Variable to store the first value.
	 * @param two Variable to store the second value.
	 * @param three Variable to store the third value.
	 * @param four Variable to store the fourth value.
	 * @return Returns The number of elements retrieved.
	 */
	daeInt get4at( size_t index, T &one, T &two, T &three, T &four )
	{
		daeInt retVal = 0;
		if ( index < _count )
		{
			one = get(index);
			retVal++;
		}
		if ( index+1 < _count )
		{
			two = get(index+1);
			retVal++;
		}
		if ( index+2 < _count )
		{
			three = get(index+2);
			retVal++;
		}
		if ( index+3 < _count )
		{
			four = get(index+3);
			retVal++;
		}
		return retVal;
	}

	/**
	 * Appends a number of elements to this array from a C native array.
	 * @param num The number of elements to append.
	 * @param array The C native array that contains the values to append.
	 */
	void appendArray( size_t num, T *array )
	{
		if ( array == NULL )
			return;

		for ( size_t i = 0; i < num; i++ )
			append( array[i] );
	}
	/**
	 * Appends a number of elements to this array from another daeTArray.
	 * @param array The daeTArray that contains the values to append.
	 */
	void appendArray( const daeTArray<T> &array ){
		size_t num = array.getCount();
		for ( size_t i = 0; i < num; i++ )
			append( array[i] );
	}
};


#endif //__DAE_ARRAY_H__
