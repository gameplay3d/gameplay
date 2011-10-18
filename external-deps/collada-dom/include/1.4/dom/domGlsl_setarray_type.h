/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 

#ifndef __domGlsl_setarray_type_h__
#define __domGlsl_setarray_type_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domGlsl_param_type.h>
#include <dom/domGlsl_setarray_type.h>
class DAE;

/**
 * The glsl_newarray_type is used to creates a parameter of a one-dimensional
 * array type.
 */
class domGlsl_setarray_type_complexType 
{
protected:  // Attribute
/**
 *  The length attribute specifies the length of the array. 
 */
	xsPositiveInteger attrLength;

protected:  // Elements
	domGlsl_param_type_Array elemGlsl_param_type_array;
/**
 * You may recursively nest glsl_newarray elements to create multidimensional
 * arrays. @see domArray
 */
	domGlsl_setarray_type_Array elemArray_array;
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
	 * Gets the glsl_param_type element array.
	 * @return Returns a reference to the array of glsl_param_type elements.
	 */
	domGlsl_param_type_Array &getGlsl_param_type_array() { return elemGlsl_param_type_array; }
	/**
	 * Gets the glsl_param_type element array.
	 * @return Returns a constant reference to the array of glsl_param_type elements.
	 */
	const domGlsl_param_type_Array &getGlsl_param_type_array() const { return elemGlsl_param_type_array; }
	/**
	 * Gets the array element array.
	 * @return Returns a reference to the array of array elements.
	 */
	domGlsl_setarray_type_Array &getArray_array() { return elemArray_array; }
	/**
	 * Gets the array element array.
	 * @return Returns a constant reference to the array of array elements.
	 */
	const domGlsl_setarray_type_Array &getArray_array() const { return elemArray_array; }
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
	domGlsl_setarray_type_complexType(DAE& dae, daeElement* elt) : attrLength(), elemGlsl_param_type_array(), elemArray_array() {}
	/**
	 * Destructor
	 */
	virtual ~domGlsl_setarray_type_complexType() { daeElement::deleteCMDataArray(_CMData); }
	/**
	 * Overloaded assignment operator
	 */
	virtual domGlsl_setarray_type_complexType &operator=( const domGlsl_setarray_type_complexType &cpy ) { (void)cpy; return *this; }
};

/**
 * An element of type domGlsl_setarray_type_complexType.
 */
class domGlsl_setarray_type : public daeElement, public domGlsl_setarray_type_complexType
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::GLSL_SETARRAY_TYPE; }
	static daeInt ID() { return 104; }
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
	domGlsl_setarray_type(DAE& dae) : daeElement(dae), domGlsl_setarray_type_complexType(dae, this) {}
	/**
	 * Destructor
	 */
	virtual ~domGlsl_setarray_type() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domGlsl_setarray_type &operator=( const domGlsl_setarray_type &cpy ) { (void)cpy; return *this; }

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
