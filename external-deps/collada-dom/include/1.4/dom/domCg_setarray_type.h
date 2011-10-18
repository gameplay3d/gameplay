/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 

#ifndef __domCg_setarray_type_h__
#define __domCg_setarray_type_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domCg_param_type.h>
#include <dom/domCg_setarray_type.h>
#include <dom/domCg_setuser_type.h>
class DAE;

/**
 * Creates a parameter of a one-dimensional array type.
 */
class domCg_setarray_type_complexType 
{
protected:  // Attribute
/**
 *  The length attribute specifies the length of the array. 
 */
	xsPositiveInteger attrLength;

protected:  // Elements
	domCg_param_type_Array elemCg_param_type_array;
/**
 * Nested array elements allow you to create multidemensional arrays. @see
 * domArray
 */
	domCg_setarray_type_Array elemArray_array;
/**
 * The usertype element allows you to create arrays of usertypes. @see domUsertype
 */
	domCg_setuser_type_Array elemUsertype_array;
	/**
	 * Used to preserve order in elements that do not specify strict sequencing of sub-elements.
	 */
	daeElementRefArray _contents;
	/**
	 * Used to preserve order in elements that have a complex content model.
	 */
	daeUIntArray       _contentsOrder;

	/**
	 * Used to store information needed for some content model objects.
	 */
	daeTArray< daeCharArray * > _CMData;


public:	//Accessors and Mutators
	/**
	 * Gets the length attribute.
	 * @return Returns a xsPositiveInteger of the length attribute.
	 */
	xsPositiveInteger getLength() const { return attrLength; }
	/**
	 * Sets the length attribute.
	 * @param atLength The new value for the length attribute.
	 */
	void setLength( xsPositiveInteger atLength ) { attrLength = atLength; }

	/**
	 * Gets the cg_param_type element array.
	 * @return Returns a reference to the array of cg_param_type elements.
	 */
	domCg_param_type_Array &getCg_param_type_array() { return elemCg_param_type_array; }
	/**
	 * Gets the cg_param_type element array.
	 * @return Returns a constant reference to the array of cg_param_type elements.
	 */
	const domCg_param_type_Array &getCg_param_type_array() const { return elemCg_param_type_array; }
	/**
	 * Gets the array element array.
	 * @return Returns a reference to the array of array elements.
	 */
	domCg_setarray_type_Array &getArray_array() { return elemArray_array; }
	/**
	 * Gets the array element array.
	 * @return Returns a constant reference to the array of array elements.
	 */
	const domCg_setarray_type_Array &getArray_array() const { return elemArray_array; }
	/**
	 * Gets the usertype element array.
	 * @return Returns a reference to the array of usertype elements.
	 */
	domCg_setuser_type_Array &getUsertype_array() { return elemUsertype_array; }
	/**
	 * Gets the usertype element array.
	 * @return Returns a constant reference to the array of usertype elements.
	 */
	const domCg_setuser_type_Array &getUsertype_array() const { return elemUsertype_array; }
	/**
	 * Gets the _contents array.
	 * @return Returns a reference to the _contents element array.
	 */
	daeElementRefArray &getContents() { return _contents; }
	/**
	 * Gets the _contents array.
	 * @return Returns a constant reference to the _contents element array.
	 */
	const daeElementRefArray &getContents() const { return _contents; }

protected:
	/**
	 * Constructor
	 */
	domCg_setarray_type_complexType(DAE& dae, daeElement* elt) : attrLength(), elemCg_param_type_array(), elemArray_array(), elemUsertype_array() {}
	/**
	 * Destructor
	 */
	virtual ~domCg_setarray_type_complexType() { daeElement::deleteCMDataArray(_CMData); }
	/**
	 * Overloaded assignment operator
	 */
	virtual domCg_setarray_type_complexType &operator=( const domCg_setarray_type_complexType &cpy ) { (void)cpy; return *this; }
};

/**
 * An element of type domCg_setarray_type_complexType.
 */
class domCg_setarray_type : public daeElement, public domCg_setarray_type_complexType
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::CG_SETARRAY_TYPE; }
	static daeInt ID() { return 135; }
	virtual daeInt typeID() const { return ID(); }

public:	//Accessors and Mutators
	/**
	 * Gets the length attribute.
	 * @return Returns a xsPositiveInteger of the length attribute.
	 */
	xsPositiveInteger getLength() const { return attrLength; }
	/**
	 * Sets the length attribute.
	 * @param atLength The new value for the length attribute.
	 */
	void setLength( xsPositiveInteger atLength ) { attrLength = atLength; _validAttributeArray[0] = true; }

protected:
	/**
	 * Constructor
	 */
	domCg_setarray_type(DAE& dae) : daeElement(dae), domCg_setarray_type_complexType(dae, this) {}
	/**
	 * Destructor
	 */
	virtual ~domCg_setarray_type() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domCg_setarray_type &operator=( const domCg_setarray_type &cpy ) { (void)cpy; return *this; }

public: // STATIC METHODS
	/**
	 * Creates an instance of this class and returns a daeElementRef referencing it.
	 * @return a daeElementRef referencing an instance of this object.
	 */
	static DLLSPEC daeElementRef create(DAE& dae);
	/**
	 * Creates a daeMetaElement object that describes this element in the meta object reflection framework.
	 * If a daeMetaElement already exists it will return that instead of creating a new one. 
	 * @return A daeMetaElement describing this COLLADA element.
	 */
	static DLLSPEC daeMetaElement* registerElement(DAE& dae);
};


#endif
