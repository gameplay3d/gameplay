/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 

#ifndef __domGles_texenv_command_type_h__
#define __domGles_texenv_command_type_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domGles_texture_constant_type.h>
class DAE;

class domGles_texenv_command_type_complexType 
{
protected:  // Attributes
	domGles_texenv_mode_enums attrOperator;
	xsNCName attrUnit;

protected:  // Element
	domGles_texture_constant_typeRef elemConstant;

public:	//Accessors and Mutators
	/**
	 * Gets the operator attribute.
	 * @return Returns a domGles_texenv_mode_enums of the operator attribute.
	 */
	domGles_texenv_mode_enums getOperator() const { return attrOperator; }
	/**
	 * Sets the operator attribute.
	 * @param atOperator The new value for the operator attribute.
	 */
	void setOperator( domGles_texenv_mode_enums atOperator ) { attrOperator = atOperator; }

	/**
	 * Gets the unit attribute.
	 * @return Returns a xsNCName of the unit attribute.
	 */
	xsNCName getUnit() const { return attrUnit; }
	/**
	 * Sets the unit attribute.
	 * @param atUnit The new value for the unit attribute.
	 */
	void setUnit( xsNCName atUnit ) { *(daeStringRef*)&attrUnit = atUnit;}

	/**
	 * Gets the constant element.
	 * @return a daeSmartRef to the constant element.
	 */
	const domGles_texture_constant_typeRef getConstant() const { return elemConstant; }
protected:
	/**
	 * Constructor
	 */
	domGles_texenv_command_type_complexType(DAE& dae, daeElement* elt) : attrOperator(), attrUnit(), elemConstant() {}
	/**
	 * Destructor
	 */
	virtual ~domGles_texenv_command_type_complexType() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domGles_texenv_command_type_complexType &operator=( const domGles_texenv_command_type_complexType &cpy ) { (void)cpy; return *this; }
};

/**
 * An element of type domGles_texenv_command_type_complexType.
 */
class domGles_texenv_command_type : public daeElement, public domGles_texenv_command_type_complexType
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::GLES_TEXENV_COMMAND_TYPE; }
	static daeInt ID() { return 146; }
	virtual daeInt typeID() const { return ID(); }

public:	//Accessors and Mutators
	/**
	 * Gets the operator attribute.
	 * @return Returns a domGles_texenv_mode_enums of the operator attribute.
	 */
	domGles_texenv_mode_enums getOperator() const { return attrOperator; }
	/**
	 * Sets the operator attribute.
	 * @param atOperator The new value for the operator attribute.
	 */
	void setOperator( domGles_texenv_mode_enums atOperator ) { attrOperator = atOperator; _validAttributeArray[0] = true; }

	/**
	 * Gets the unit attribute.
	 * @return Returns a xsNCName of the unit attribute.
	 */
	xsNCName getUnit() const { return attrUnit; }
	/**
	 * Sets the unit attribute.
	 * @param atUnit The new value for the unit attribute.
	 */
	void setUnit( xsNCName atUnit ) { *(daeStringRef*)&attrUnit = atUnit; _validAttributeArray[1] = true; }

protected:
	/**
	 * Constructor
	 */
	domGles_texenv_command_type(DAE& dae) : daeElement(dae), domGles_texenv_command_type_complexType(dae, this) {}
	/**
	 * Destructor
	 */
	virtual ~domGles_texenv_command_type() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domGles_texenv_command_type &operator=( const domGles_texenv_command_type &cpy ) { (void)cpy; return *this; }

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
