/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 

#ifndef __domCg_setuser_type_h__
#define __domCg_setuser_type_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domCg_param_type.h>
#include <dom/domCg_setarray_type.h>
#include <dom/domCg_setuser_type.h>
#include <dom/domCg_connect_param.h>
#include <dom/domCg_setparam.h>
class DAE;

/**
 * Creates an instance of a structured class.
 */
class domCg_setuser_type_complexType 
{
protected:  // Attributes
	domCg_identifier attrName;
/**
 *  Reference a code or include element which defines the usertype 
 */
	xsNCName attrSource;

protected:  // Elements
	domCg_param_type_Array elemCg_param_type_array;
	domCg_setarray_type_Array elemArray_array;
	domCg_setuser_type_Array elemUsertype_array;
	domCg_connect_param_Array elemConnect_param_array;
/**
 * Use a series of these to set the members by name.  The ref attribute will
 * be relative to the usertype you are in right now. @see domSetparam
 */
	domCg_setparam_Array elemSetparam_array;
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
	 * Gets the name attribute.
	 * @return Returns a domCg_identifier of the name attribute.
	 */
	domCg_identifier getName() const { return attrName; }
	/**
	 * Sets the name attribute.
	 * @param atName The new value for the name attribute.
	 */
	void setName( domCg_identifier atName ) { attrName = atName; }

	/**
	 * Gets the source attribute.
	 * @return Returns a xsNCName of the source attribute.
	 */
	xsNCName getSource() const { return attrSource; }
	/**
	 * Sets the source attribute.
	 * @param atSource The new value for the source attribute.
	 */
	void setSource( xsNCName atSource ) { *(daeStringRef*)&attrSource = atSource;}

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
	 * Gets the connect_param element array.
	 * @return Returns a reference to the array of connect_param elements.
	 */
	domCg_connect_param_Array &getConnect_param_array() { return elemConnect_param_array; }
	/**
	 * Gets the connect_param element array.
	 * @return Returns a constant reference to the array of connect_param elements.
	 */
	const domCg_connect_param_Array &getConnect_param_array() const { return elemConnect_param_array; }
	/**
	 * Gets the setparam element array.
	 * @return Returns a reference to the array of setparam elements.
	 */
	domCg_setparam_Array &getSetparam_array() { return elemSetparam_array; }
	/**
	 * Gets the setparam element array.
	 * @return Returns a constant reference to the array of setparam elements.
	 */
	const domCg_setparam_Array &getSetparam_array() const { return elemSetparam_array; }
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
	domCg_setuser_type_complexType(DAE& dae, daeElement* elt) : attrName(), attrSource(), elemCg_param_type_array(), elemArray_array(), elemUsertype_array(), elemConnect_param_array(), elemSetparam_array() {}
	/**
	 * Destructor
	 */
	virtual ~domCg_setuser_type_complexType() { daeElement::deleteCMDataArray(_CMData); }
	/**
	 * Overloaded assignment operator
	 */
	virtual domCg_setuser_type_complexType &operator=( const domCg_setuser_type_complexType &cpy ) { (void)cpy; return *this; }
};

/**
 * An element of type domCg_setuser_type_complexType.
 */
class domCg_setuser_type : public daeElement, public domCg_setuser_type_complexType
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::CG_SETUSER_TYPE; }
	static daeInt ID() { return 136; }
	virtual daeInt typeID() const { return ID(); }

public:	//Accessors and Mutators
	/**
	 * Gets the name attribute.
	 * @return Returns a domCg_identifier of the name attribute.
	 */
	domCg_identifier getName() const { return attrName; }
	/**
	 * Sets the name attribute.
	 * @param atName The new value for the name attribute.
	 */
	void setName( domCg_identifier atName ) { attrName = atName; _validAttributeArray[0] = true; }

	/**
	 * Gets the source attribute.
	 * @return Returns a xsNCName of the source attribute.
	 */
	xsNCName getSource() const { return attrSource; }
	/**
	 * Sets the source attribute.
	 * @param atSource The new value for the source attribute.
	 */
	void setSource( xsNCName atSource ) { *(daeStringRef*)&attrSource = atSource; _validAttributeArray[1] = true; }

protected:
	/**
	 * Constructor
	 */
	domCg_setuser_type(DAE& dae) : daeElement(dae), domCg_setuser_type_complexType(dae, this) {}
	/**
	 * Destructor
	 */
	virtual ~domCg_setuser_type() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domCg_setuser_type &operator=( const domCg_setuser_type &cpy ) { (void)cpy; return *this; }

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
