/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 

#ifndef __DAE_META_ATTRIBUTE_H__
#define __DAE_META_ATTRIBUTE_H__

#include <string>
#include <sstream>
#include <dae/daeTypes.h>
#include <dae/daeStringRef.h>
#include <dae/daeAtomicType.h>
#include <dae/daeElement.h>
#include <dae/daeArray.h>

class daeElement;
class daeMetaElement;
class daeMetaAttribute;
class daeMetaElementAttribute;

/**
 * The @c daeMetaAttribute class describes one attribute in a C++ COLLADA dom element.
 *
 * In the case of the C++ object model a conceptual attribute can be
 * either a dom attribute, a dom element, or a dom value.
 * Essentially, the meta attribute describes fields on the C++ class.
 * However these attributes are stored separately in the containing meta
 * @c daeMetaElement.
 * @c daeMetaAttributes always exist inside of @c daeMetaElements.
 * Each @c daeMetaAttribute has certain semantic operations it is capable of
 * including @c set(), @c get(), and @c print().
 * @c daeMetaAttributes use the @c daeAtomicType system as their underlying semantic
 * implementation, but contain additional information about the packaging
 * of the atomic types into the C++ dom classes such as offset, and
 * array information.
 */
class DLLSPEC daeMetaAttribute : public daeRefCountedObj
{
protected:
	daeStringRef    _name;
	daeInt          _offset;
	daeAtomicType*  _type;
	daeMetaElement*	_container;
	std::string     _defaultString;
	daeMemoryRef    _defaultValue;
	daeBool         _isRequired;

public:
	/**
	 * Constructor
	 */
	daeMetaAttribute();

	/**
	 * Destructor
	 */
	virtual ~daeMetaAttribute();
public:
	/** 
	 * Determines if the schema indicates that this is a required attribute.
	 * @return Returns true if this is a required attribute, false if not.
	 */
	daeBool getIsRequired() {return _isRequired; }
	/**
	 * Sets the value that indicates that this attribute is required by the schema.  If set, the attribute
	 * will always be exported by the API regardless of its value.
	 * @param isRequired Indicates if the schema says this attribute is required, true if it is, false if not.
	 */
	void setIsRequired(daeBool isRequired) {_isRequired = isRequired;}
	/**
	 * Sets the byte offset (from @c this) where this attribute's storage is
	 * found in its container element class.
	 * @param offset Integer byte offset from @c this pointer.
	 */
	void setOffset(daeInt offset) { _offset = offset; }

	/**
	 * Gets the byte offset (from @ this) where this attribute's storage is
	 * found in its container element class.
	 * @return Returns the integer byte offset from @c this pointer for this attribute.
	 */
	daeInt getOffset() { return _offset; }
	 
	/**
	 * Sets the name of the attribute.
	 * @param name @c daeString that is directly stored as a pointer
	 * without being copied.
	 */
	void setName(daeString name) { _name = name; }
	
	/**
	 * Gets the name of this attribute.
	 * @return Returnsthe name of this attribute.
	 */
	daeStringRef getName() { return _name; }

	/**
	 * Sets the type of the attribute.
	 * @param type @c daeAtomicType to use for interacting with this
	 * attribute in a containing @c daeElement.
	 */
	void setType(daeAtomicType* type) { _type = type; }
	
	/**
	 * Gets the @c daeAtomicType used by this attribute.
	 * @return Returns the @c daeAtomicType that this attribute uses for its
	 * implementation.
	 */
	daeAtomicType* getType() { return _type; }

	/**
	 * Sets the default for this attribute via a string.
	 * @param defaultVal @c daeString representing the default value.
	 */
	virtual void setDefaultString(daeString defaultVal);

	/**
	 * Sets the default for this attribute via a memory pointer.
	 * @param defaultVal @c daeMemoryRef representing the default value.
	 */
	virtual void setDefaultValue(daeMemoryRef defaultVal);

	/**
	 * Gets the default for this attribute as a string.
	 * @return Returns a @c daeString representing the default value.
	 */
	daeString getDefaultString();

	/**
	 * Gets the default for this attribute as a memory value.
	 * @return Returns a @c daeMemoryRef representing the default value.
	 */
	daeMemoryRef getDefaultValue();

	/**
	 * Sets the containing @c daeMetaElement for this attribute.
	 * @param container Element on which this @c daeMetaAttribute belongs.
	 */
	void setContainer(daeMetaElement* container) { _container = container; }

	/**
	 * Gets the containing @c daeMetaElement for this attribute.
	 * @return Returns the @c daeMetaElement to which this @c daeAttribute belongs.
	 */
	daeMetaElement* getContainer() { return _container; }

	/**
	 * Notifies an attribute when the containing document changes.
	 */
	virtual void setDocument(daeElement* e, daeDocument* doc);
	  
	/**
	 * Converts an element's attribute value to a string.
	 */
	virtual void memoryToString(daeElement* e, std::ostringstream& buffer);

	/**
	 * Converts a string to a memory value in the specified element.
	 */
	virtual void stringToMemory(daeElement* e, daeString s);

	/**
	 * Gets the attribute's memory pointer from containing element <tt><i>e</i></tt>.
	 * @param e Containing element from which to get the value.
	 * @return Returns the memory pointer corresponding to this attribute  out of parent element e.
	 */
	virtual daeMemoryRef get(daeElement* e);

	/**
	 * Gets if this attribute is an array attribute.
	 * @return Returns true if this attribute is an array type.
	 */
	virtual daeBool isArrayAttribute()		{ return false; }
	  
public:
	/**
	 * Gets the number of bytes for this attribute.
	 * @return Returns the number of bytes in the C++ COLLADA dom element for this
	 * attribute.
	 */
	virtual daeInt getSize();

	/**
	 * Gets the alignment in bytes on the class of this meta attribute type.
	 * @return Returns the alignment in bytes.
	 */
	virtual daeInt getAlignment();

	/**
	 * Copies the value of this attribute from fromElement into toElement.
	 * @param toElement Pointer to a @c daeElement to copy this attribute to.
	 * @param fromElement Pointer to a @c daeElement to copy this attribute from.
	 */
	virtual void copy(daeElement* toElement, daeElement* fromElement);

	/**
	 * Copies the default value of this attribute to the element
	 * @param element Pointer to a @c daeElement to copy the default value to.
	 */
	virtual void copyDefault(daeElement* element);

	/**
	 * Compares the value of this attribute in the given elements.
	 * @param elt1 The first element whose attribute value should be compared.
	 * @param elt2 The second element whose attribute value should be compared.
	 * @return Returns a positive integer if value1 > value2, a negative integer if 
	 * value1 < value2, and 0 if value1 == value2.
	 */
	virtual daeInt compare(daeElement* elt1, daeElement* elt2);

	/**
	 * Compares the value of this attribute from the given element to the default value
	 * of this attribute (if one exists).
	 * @param e The element whose value should be compared to the default value.
	 * @return Returns a positive integer if value > default, a negative integer if 
	 * value < default, and 0 if value == default.
	 */
	virtual daeInt compareToDefault(daeElement* e);
	
public:
	// These methods are deprecated.
	virtual daeChar* getWritableMemory(daeElement* e);  // Use get instead.
	virtual void set(daeElement* element, daeString s); // Use stringToMemory instead.
};


/**
 * The @c daeMetaArrayAttribute class is simple a wrapper that implements
 * an array of atomic types rather than a singleton.
 * The corresponding storage is an array
 * and the corresponding operations are implemented on the array
 * data structure rather than on inlined storage in elements.
 */
class DLLSPEC daeMetaArrayAttribute : public daeMetaAttribute
{
public:
	virtual ~daeMetaArrayAttribute();

	/**
	 * Defines the override version of this method from @c daeMetaAttribute.
	 * @param toElement Pointer to a @c daeElement to copy this attribute to.
	 * @param fromElement Pointer to a @c daeElement to copy this attribute from.
	 */
	virtual void copy(daeElement* toElement, daeElement* fromElement);

	/**
	 * Copies the default value of this attribute to the element
	 * @param element Pointer to a @c daeElement to copy the default value to.
	 */
	virtual void copyDefault(daeElement* element);

	/**
	 * Compares the value of this attribute in the given elements.
	 * @param elt1 The first element whose attribute value should be compared.
	 * @param elt2 The second element whose attribute value should be compared.
	 * @return Returns a positive integer if value1 > value2, a negative integer if 
	 * value1 < value2, and 0 if value1 == value2.
	 */
	virtual daeInt compare(daeElement* elt1, daeElement* elt2);

	/**
	 * Compares the value of this attribute from the given element to the default value
	 * of this attribute (if one exists).
	 * @param e The element whose value should be compared to the default value.
	 * @return Returns a positive integer if value > default, a negative integer if 
	 * value < default, and 0 if value == default.
	 */
	virtual daeInt compareToDefault(daeElement* e);

	/**
	 * Converts an element's attribute value to a string.
	 */
	virtual void memoryToString(daeElement* e, std::ostringstream& buffer);

	/**
	 * Converts a string to a memory value in the specified element.
	 */
	virtual void stringToMemory(daeElement* e, daeString s);

	/**
	 * Sets the default for this attribute via a string.
	 * @param defaultVal @c daeString representing the default value.
	 */
	virtual void setDefaultString(daeString defaultVal);

	/**
	 * Sets the default for this attribute via a memory pointer.
	 * @param defaultVal @c daeMemoryRef representing the default value.
	 */
	virtual void setDefaultValue(daeMemoryRef defaultVal);

	/**
	 * Gets if this attribute is an array attribute.
	 * @return Returns true if this attribute is an array type.
	 */
	virtual daeBool isArrayAttribute() { return true; }

	/**
	 * Notifies an attribute when the containing document changes.
	 */
	virtual void setDocument(daeElement* e, daeDocument* doc);
};


typedef daeSmartRef<daeMetaAttribute> daeMetaAttributeRef;

typedef daeTArray<daeMetaAttributeRef> daeMetaAttributeRefArray;
typedef daeTArray<daeMetaAttribute*> daeMetaAttributePtrArray;

#endif //__DAE_META_ATTRIBUTE_H__






