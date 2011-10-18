/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 

#ifndef __DAE_ATOMIC_TYPE_H__
#define __DAE_ATOMIC_TYPE_H__

#include <sstream>
#include <dae/daeTypes.h>
#include <dae/daeStringRef.h>
#include <dae/daeArray.h>
#include <dae/daeElement.h>

#ifndef _WIN32
#include <stdint.h>
#endif 

class DAE;
class daeAtomicType;
class daeMetaElement;

typedef daeTArray<daeAtomicType*> daeAtomicTypeArray;
class daeMetaAttribute;
typedef daeSmartRef<daeMetaAttribute> daeMetaAttributeRef;

/**
 * The @c daeAtomicType class implements a standard interface for
 * data elements in the reflective object system.
 *
 * @c daeAtomicType provides a central virtual interface that can be
 * used by the rest of the reflective object system.
 *
 * The atomic type system if very useful for file IO and building
 * automatic tools for data inspection and manipulation,
 * such as hierarchy examiners and object editors.
 *
 * Types provide the following symantic operations:
 * - @c print()
 * - @c memoryToString()
 * - @c stringToMemory()
 *
 * Types are also able to align data pointers appropriately.
 */
class DLLSPEC daeAtomicType
{
public:
	/**
	 * destructor
	 */
	virtual ~daeAtomicType() {}

	/**
	 * constructor
	 */
	daeAtomicType(DAE& dae);
	
public:
	/**
	 * Prints an atomic typed element into a destination string.
	 * @param src Source of the raw data from which to get the typed items.
	 * @param dst Destination to output the string version of the elements to.
	 * @return Returns true if the operation was successful, false if not successful.  
	 */
	virtual daeBool memoryToString(daeChar* src, std::ostringstream& dst) = 0;

	/**
	 * Reads an atomic typed item into the destination runtime memory.
	 * @param src Source string.
	 * @param dst Raw binary location to store the resulting value.
	 * @return Returns true if the operation was successful, false if not successful.
	 */
	virtual daeBool stringToMemory(daeChar* src, daeChar* dst);

	/**
	 * Converts an array of atomic items into a whitespace separated string.
	 * @param array The array of data.
	 * @param buffer The buffer to write into.
	 */
	virtual void arrayToString(daeArray& array, std::ostringstream& buffer);

	/**
	 * Reads a whitespace separated list of atomic items into an array. The array is
	 * cleared before writing into it.
	 * @param src Whitespace separated list of items.
	 * @param array The output array of data.
	 * @return Returns true if the operation was successful, false otherwise.
	 */
	virtual daeBool stringToArray(daeChar* src, daeArray& array);

	/**
	 * Creates a new object of the appropriate type for this daeAtomicType and returns it
	 * as a pointer. The return value must be freed by calling destroy.
	 * @return Returns a pointer to a new value. The memory must be freed by calling destroy.
	 */
	virtual daeMemoryRef create() = 0;

	/**
	 * Deletes an object previously allocated with create.
	 * @param obj The object previously allocated with create.
	 */
	virtual void destroy(daeMemoryRef obj) = 0;

	/**
	 * Creates a daeTArray of the appropriate type (e.g. daeTArray<int>, daeTArray<daeIDRef>)
	 * and returns it as a daeArray*.
	 * @return Returns a daeArray*. This array should be freed by the caller with
	 * operator delete.
	 */
	virtual daeArray* createArray() = 0;

	/**
	 * Performs a virtual comparison operation between two values of the same atomic type.
	 * @param value1 Memory location of the first value.
	 * @param value2 Memory location of the second value.
	 * @return Returns a positive integer if value1 > value2, a negative integer if 
	 * value1 < value2, and 0 if value1 == value2.
	 */
	virtual daeInt compare(daeChar* value1, daeChar* value2);

	/**
	 * Array version of the compare function.
	 * @param value1 First array to compare.
	 * @param value2 Second array to compare.
	 * @return Returns a positive integer if value1 > value2, a negative integer if 
	 * value1 < value2, and 0 if value1 == value2.
	 */
	virtual daeInt compareArray(daeArray& value1, daeArray& value2);

	/**
	 * Performs a virtual copy operation.
	 * @param src Memory location of the value to copy from.
	 * @param dst Memory location of the value to copy to.
	 */
	virtual void copy(daeChar* src, daeChar* dst) = 0;

	/**
	 * Array version of the copy function.
	 * @param src Array to copy from.
	 * @param dst Array to copy to.
	 */
	virtual void copyArray(daeArray& src, daeArray& dst);

	/**
	 * Gets the array of strings as name bindings for this type.
	 * @return Returns the array of strings.
	 */
	daeStringRefArray&	getNameBindings() { return _nameBindings; }

	/**
	 * Gets the enum associated with this atomic type. This is not scalable and only
	 * works for base types, otherwise 'extension' is used.
	 * @return Returns the enum associated with this atomic type.
	 */
	daeEnum				getTypeEnum() { return _typeEnum; }

	/**
	 * Gets the size in bytes for this atomic type.
	 * @return Returns the size of the atomic type in bytes.
	 */
	daeInt				getSize() { return _size; }

	/**
	 * Gets the scanf format used for this type. 
	 * @return Returns the scanf format.
	 * @note
	 * Warning - this field is only for convenience and may not always work.
	 * It is used only when the read functions are left to the base
	 * implementation.
	 */
	daeStringRef		getScanFormat() { return _scanFormat; }

	/**
	 * Gets the printf format used for this type.
	 * @return Returns the printf format.
	 * @note
	 * Warning - this field is only for convenience and may not always work.
	 * It is used only when the print functions are left to the base
	 * implementation.
	 */
	daeStringRef		getPrintFormat() { return _printFormat; }

	/**
	 * Gets the alignment in bytes necessary for this type on this
	 * platform.
	 * @return Returns the alignment in bytes.
	 */
	daeInt				getAlignment() { return _alignment; }

	/**
	 * Gets the string associated with this type.
	 * @return Returns the string associated with this type.
	 */
	daeStringRef		getTypeString() { return _typeString; }

	/**
	 * Performs an alignment based on the alignment for this type.
	 * @param ptr Pointer to be aligned.
	 * @return Returns the aligned pointer computed via
	 * <tt> (ptr+alignment-1)&(~(alignment-1).	</tt>
	 * 
	 */
	daeChar*			align(daeChar* ptr) {
		return (daeChar*)(((intptr_t)(ptr+_alignment-1))&(~(_alignment - 1))); }

	/**
	 * Notifies an object when the containing document changes.
	 * @param value Memory location of the atomic type value.
	 * @param doc The new document.
	 */
	virtual void setDocument(daeChar* value, daeDocument* doc) { }

	/**
	 * Same as the previous method, but works on an array of objects.
	 * @param values Array of the atomic type values.
	 * @param doc The new document.
	 */
	virtual void setDocument(daeArray& array, daeDocument* doc) { }
	
protected:
	DAE* _dae;
	daeInt _size;
	daeInt _alignment;
	daeEnum _typeEnum;
	daeStringRef _typeString;
	daeStringRef _printFormat;
	daeStringRef _scanFormat;
	daeInt _maxStringLength;
	
public:
	/**
	 * An array of strings as name bindings for this type.
	 */
	daeStringRefArray _nameBindings;

public: // Static Interface
	/** An enum for identifying the different atomic types */
	enum daeAtomicTypes {
		/** bool atomic type */
		BoolType,
		/** enum atomic type */
		EnumType,
		/** character atomic type */
		CharType,
		/** short integer atomic type */
		ShortType,
		/** integer atomic type */
		IntType,
		/** unsigned integer atomic type */
		UIntType,
		/** long integer atomic type */
		LongType,
		/** unsigned long integer atomic type */
		ULongType,
		/** floating point atomic type */
		FloatType,
		/** double precision floating point atomic type */
		DoubleType,
		/** string reference atomic type */
		StringRefType,
		/** element reference atomic type */
		ElementRefType,
		/** memory reference atomic type */
		MemoryRefType,
		/** void reference atomic type */
		RawRefType,
		/** resolver  atomic type */
		ResolverType,
		/** ID resolver atomic type */
		IDResolverType,
		/** string token atomic type */
		TokenType,
		/** extension atomic type */
		ExtensionType
	};
};


// This is a container class for storing a modifiable list of daeAtomicType objects.
class DLLSPEC daeAtomicTypeList {
public:
	daeAtomicTypeList(DAE& dae);
	~daeAtomicTypeList();
	
	/**
	 * Appends a new type to the list.
	 * @param t Type to append.
	 * @return Returns the index of the type in the list.
	 */
	daeInt append(daeAtomicType* t);

	/**
	 * Gets a type from the list of types, based on its index.
	 * @param index Index of the type to retrieve.
	 * @return Returns the @c daeAtomicType indicated by index.
	 */
	const daeAtomicType* getByIndex(daeInt index);

	/**
	 * Gets the number of atomic types in the list.
	 * @return Returns the number of atomic types in the list.
	 */
	daeInt getCount();

	/**
	 * Finds a type by its string name.
	 * @param type String name of the type.
	 * @return Returns the type with the corresponding name.
	 */
	daeAtomicType* get(daeStringRef type);

	/**
	 * Finds a type by its enum.
	 * @param type Enum representing the desired type.
	 * @return Returns the type with the corresponding enum.
	 */
	daeAtomicType* get(daeEnum type);

private:
	daeAtomicTypeArray types;
};


/**
 * The @c daeBoolType class is derived from @c daeAtomicType, and implements
 * the reflective system for objects of type @c daeBool.
 */
class DLLSPEC daeBoolType : public daeAtomicType
{
public:
	/**
	 * Constructor
	 */
	daeBoolType(DAE& dae);
public:
	virtual daeBool memoryToString(daeChar* src, std::ostringstream& dst);
	
	virtual daeBool stringToMemory(daeChar* src, daeChar* dst);

	virtual daeMemoryRef create();

	virtual void destroy(daeMemoryRef obj);

	virtual void copy(daeChar* src, daeChar* dst);

	virtual daeArray* createArray();
};

/**
 * The @c daeIntType class is derived from @c daeAtomicType, and implements
 * the reflective system for objects of type @c daeInt.
 */
class DLLSPEC daeIntType : public daeAtomicType
{
public:
	/**
	 * Constructor
	 */
	daeIntType(DAE& dae);
public:
	virtual daeBool memoryToString(daeChar* src, std::ostringstream& dst);

	virtual daeMemoryRef create();

	virtual void destroy(daeMemoryRef obj);

	virtual void copy(daeChar* src, daeChar* dst);

	virtual daeArray* createArray();
};

/**
 * The @c daeLongType class is derived from @c daeAtomicType, and implements
 * the reflective system for objects of type @c daeLong.
 */
class DLLSPEC daeLongType : public daeAtomicType
{
public:
	/**
	 * Constructor
	 */
	daeLongType(DAE& dae);
public:
	virtual daeBool memoryToString(daeChar* src, std::ostringstream& dst);

	virtual daeMemoryRef create();

	virtual void destroy(daeMemoryRef obj);

	virtual void copy(daeChar* src, daeChar* dst);

	virtual daeArray* createArray();
};

/**
 * The @c daeUIntType class is derived from @c daeAtomicType, and implements
 * the reflective system for objects of type @c daeUInt.
 */
class DLLSPEC daeUIntType : public daeAtomicType
{
public:
	/**
	 * Constructor
	 */
	daeUIntType(DAE& dae);
public:
	virtual daeBool memoryToString(daeChar* src, std::ostringstream& dst);

	virtual daeMemoryRef create();

	virtual void destroy(daeMemoryRef obj);

	virtual void copy(daeChar* src, daeChar* dst);

	virtual daeArray* createArray();
};

/**
 * The @c daeUIntType class is derived from @c daeAtomicType, and implements
 * the reflective system for objects of type @c daeUInt.
 */
class DLLSPEC daeULongType : public daeAtomicType
{
public:
	/**
	 * Constructor
	 */
	daeULongType(DAE& dae);
public:
	virtual daeBool memoryToString(daeChar* src, std::ostringstream& dst);

	virtual daeMemoryRef create();

	virtual void destroy(daeMemoryRef obj);

	virtual void copy(daeChar* src, daeChar* dst);

	virtual daeArray* createArray();
};

/**
 * The @c daeShortType is  derived from @c daeAtomicType, and implements
 * the reflective system for objects of type @c daeShort.
 */
class DLLSPEC daeShortType : public daeAtomicType
{
public:
	/**
	 * Constructor
	 */
	daeShortType(DAE& dae);
public:
	virtual daeBool memoryToString(daeChar* src, std::ostringstream& dst);

	virtual daeMemoryRef create();

	virtual void destroy(daeMemoryRef obj);

	virtual void copy(daeChar* src, daeChar* dst);

	virtual daeArray* createArray();
};

/**
 * The @c daeFloatType is derived from @c daeAtomicType, and implements
 * the reflective system for objects of type @c daeFloat.
 */
class DLLSPEC daeFloatType : public daeAtomicType
{
public:
	/**
	 * Constructor
	 */
	daeFloatType(DAE& dae);
public:
	virtual daeBool memoryToString(daeChar* src, std::ostringstream& dst);

	virtual daeBool stringToMemory(daeChar* src, daeChar* dst);

	virtual daeMemoryRef create();

	virtual void destroy(daeMemoryRef obj);

	virtual void copy(daeChar* src, daeChar* dst);

	virtual daeArray* createArray();
};

/**
 * The @c daeDoubleType is derived from @c daeAtomicType, and implements
 * the reflective system for objects of type @c daeDouble.
 */
class DLLSPEC daeDoubleType : public daeAtomicType
{
public:
	/**
	 * Constructor
	 */
	daeDoubleType(DAE& dae);
public:
	virtual daeBool memoryToString(daeChar* src, std::ostringstream& dst);

	virtual daeBool stringToMemory(daeChar* src, daeChar* dst);

	virtual daeMemoryRef create();

	virtual void destroy(daeMemoryRef obj);

	virtual void copy(daeChar* src, daeChar* dst);

	virtual daeArray* createArray();
};

/**
 * The @c daeStringRefType class is derived from @c daeAtomicType, and implements
 * the reflective system for objects of type @c daeStringRef.
 */
class DLLSPEC daeStringRefType : public daeAtomicType
{
public:
	/**
	 * Constructor
	 */
	daeStringRefType(DAE& dae);
public:
	virtual daeBool memoryToString(daeChar* src, std::ostringstream& dst);

	virtual daeBool stringToMemory(daeChar* src, daeChar* dst);

	virtual daeInt compare(daeChar* value1, daeChar* value2);

	virtual daeMemoryRef create();

	virtual void destroy(daeMemoryRef obj);

	virtual void copy(daeChar* src, daeChar* dst);

	virtual daeArray* createArray();
};

/**
 * The @c daeTokenType class is derived from @c daeStringRefType, and implements
 * the reflective system for objects of type daeStringRef, with specialized
 * treatment from the parser.
 */
class DLLSPEC daeTokenType : public daeStringRefType
{
public:
	/**
	 * Constructor
	 */
	daeTokenType(DAE& dae);
	
public:
	virtual daeBool stringToMemory(daeChar* src, daeChar* dst);
	
	virtual daeMemoryRef create();

	virtual void destroy(daeMemoryRef obj);

	virtual void copy(daeChar* src, daeChar* dst);

	virtual daeArray* createArray();
};

/**
 * The @c daeElementRefType class is derived from @c  daeAtomicType, and implements
 * the reflective system for objects of type @c daeElementRef.
 */
class DLLSPEC daeElementRefType : public daeAtomicType
{
public:
	/**
	 * The @c daeMetaElement for the type this @c daeElementRefType represents.
	 */
	daeMetaElement* _elementType;

public:
	/**
	 * Constructor
	 */
	daeElementRefType(DAE& dae);
public:
	virtual daeBool memoryToString(daeChar* src, std::ostringstream& dst);

	virtual daeMemoryRef create();

	virtual void destroy(daeMemoryRef obj);

	virtual void copy(daeChar* src, daeChar* dst);

	virtual daeArray* createArray();
};

/**
 * The @c daeEnumType class is  derived from @c  daeAtomicType, and implements
 * the reflective system for objects of type daeEnum.
 */
class DLLSPEC daeEnumType: public daeAtomicType
{
public:
	/**
	 * The array which contains the values used in this enum.
	 */
	daeEnumArray*		_values;
	/**
	 * The array which contains the strings to associate with the values used in this enum.
	 */
	daeStringRefArray*	_strings;
	
public:
	/**
	 * Constructor
	 */
	daeEnumType(DAE& dae);

	/**
	 * Destructor
	 */
	~daeEnumType();
	
public:
	virtual daeBool memoryToString(daeChar* src, std::ostringstream& dst);

	virtual daeBool stringToMemory(daeChar* src, daeChar* dst);

	virtual daeMemoryRef create();

	virtual void destroy(daeMemoryRef obj);

	virtual void copy(daeChar* src, daeChar* dst);

	virtual daeArray* createArray();
};

/**
 * The @c daeRawRefType class is  derived from @c daeAtomicType, and implements
 * the reflective system for objects of type @c daeRawRef.
 */
class DLLSPEC daeRawRefType: public daeAtomicType
{
public:
	/** 
	*  Constructor.
	*/	
	daeRawRefType(DAE& dae);
	
public:
	virtual daeBool memoryToString(daeChar* src, std::ostringstream& dst);

	virtual daeMemoryRef create();

	virtual void destroy(daeMemoryRef obj);

	virtual void copy(daeChar* src, daeChar* dst);

	virtual daeArray* createArray();
};

/**
 * The @c daeResolverType class is derived from @c daeAtomicType, and  implements
 * the reflective system for objects of type @c daeResolver.
 */
class DLLSPEC daeResolverType : public daeAtomicType
{
public:
	/** 
	*  Constructor.
	*/	
	daeResolverType(DAE& dae);
public:
	virtual daeBool memoryToString(daeChar* src, std::ostringstream& dst);

	virtual daeBool stringToMemory(daeChar* src, daeChar* dst);
	
	virtual daeInt compare(daeChar* value1, daeChar* value2);

	virtual daeMemoryRef create();

	virtual void destroy(daeMemoryRef obj);

	virtual void copy(daeChar* src, daeChar* dst);

	virtual daeArray* createArray();

	virtual void setDocument(daeChar* value, daeDocument* doc);

	virtual void setDocument(daeArray& array, daeDocument* doc);
};

/**
 * The @c daeIDResolverType class is  derived from @c daeAtomicType, and implements
 * the reflective system for objects of type @c daeIDResolver.
 */
class DLLSPEC daeIDResolverType : public daeAtomicType
{
public:
	/** 
	*  Constructor.
	*/	
	daeIDResolverType(DAE& dae);
public:
	virtual daeBool memoryToString(daeChar* src, std::ostringstream& dst);

	virtual daeBool stringToMemory(daeChar* src, daeChar* dst);
	
	virtual daeInt compare(daeChar* value1, daeChar* value2);

	virtual daeMemoryRef create();

	virtual void destroy(daeMemoryRef obj);

	virtual void copy(daeChar* src, daeChar* dst);

	virtual daeArray* createArray();
};



#endif // __DAE_ATOMIC_TYPE_H__



