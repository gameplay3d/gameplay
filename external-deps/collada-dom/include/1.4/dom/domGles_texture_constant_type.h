/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 

#ifndef __domGles_texture_constant_type_h__
#define __domGles_texture_constant_type_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

class DAE;

class domGles_texture_constant_type_complexType 
{
protected:  // Attributes
	domFloat4 attrValue;
	xsNCName attrParam;


public:	//Accessors and Mutators
	/**
	 * Gets the value array attribute.
	 * @return Returns a domFloat4 reference of the value array attribute.
	 */
	domFloat4 &getValue() { return attrValue; }
	/**
	 * Gets the value array attribute.
	 * @return Returns a constant domFloat4 reference of the value array attribute.
	 */
	const domFloat4 &getValue() const { return attrValue; }
	/**
	 * Sets the value array attribute.
	 * @param atValue The new value for the value array attribute.
	 */
	void setValue( const domFloat4 &atValue ) { attrValue = atValue; }

	/**
	 * Gets the param attribute.
	 * @return Returns a xsNCName of the param attribute.
	 */
	xsNCName getParam() const { return attrParam; }
	/**
	 * Sets the param attribute.
	 * @param atParam The new value for the param attribute.
	 */
	void setParam( xsNCName atParam ) { *(daeStringRef*)&attrParam = atParam;}

protected:
	/**
	 * Constructor
	 */
	domGles_texture_constant_type_complexType(DAE& dae, daeElement* elt) : attrValue(), attrParam() {}
	/**
	 * Destructor
	 */
	virtual ~domGles_texture_constant_type_complexType() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domGles_texture_constant_type_complexType &operator=( const domGles_texture_constant_type_complexType &cpy ) { (void)cpy; return *this; }
};

/**
 * An element of type domGles_texture_constant_type_complexType.
 */
class domGles_texture_constant_type : public daeElement, public domGles_texture_constant_type_complexType
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::GLES_TEXTURE_CONSTANT_TYPE; }
	static daeInt ID() { return 145; }
	virtual daeInt typeID() const { return ID(); }

public:	//Accessors and Mutators
	/**
	 * Gets the value array attribute.
	 * @return Returns a domFloat4 reference of the value array attribute.
	 */
	domFloat4 &getValue() { return attrValue; }
	/**
	 * Gets the value array attribute.
	 * @return Returns a constant domFloat4 reference of the value array attribute.
	 */
	const domFloat4 &getValue() const { return attrValue; }
	/**
	 * Sets the value array attribute.
	 * @param atValue The new value for the value array attribute.
	 */
	void setValue( const domFloat4 &atValue ) { attrValue = atValue; _validAttributeArray[0] = true; }

	/**
	 * Gets the param attribute.
	 * @return Returns a xsNCName of the param attribute.
	 */
	xsNCName getParam() const { return attrParam; }
	/**
	 * Sets the param attribute.
	 * @param atParam The new value for the param attribute.
	 */
	void setParam( xsNCName atParam ) { *(daeStringRef*)&attrParam = atParam; _validAttributeArray[1] = true; }

protected:
	/**
	 * Constructor
	 */
	domGles_texture_constant_type(DAE& dae) : daeElement(dae), domGles_texture_constant_type_complexType(dae, this) {}
	/**
	 * Destructor
	 */
	virtual ~domGles_texture_constant_type() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domGles_texture_constant_type &operator=( const domGles_texture_constant_type &cpy ) { (void)cpy; return *this; }

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
