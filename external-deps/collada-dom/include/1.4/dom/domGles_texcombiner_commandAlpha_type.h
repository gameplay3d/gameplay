/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 

#ifndef __domGles_texcombiner_commandAlpha_type_h__
#define __domGles_texcombiner_commandAlpha_type_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domGles_texcombiner_argumentAlpha_type.h>
class DAE;

class domGles_texcombiner_commandAlpha_type_complexType 
{
protected:  // Attributes
	domGles_texcombiner_operatorAlpha_enums attrOperator;
	xsFloat attrScale;

protected:  // Element
	domGles_texcombiner_argumentAlpha_type_Array elemArgument_array;

public:	//Accessors and Mutators
	/**
	 * Gets the operator attribute.
	 * @return Returns a domGles_texcombiner_operatorAlpha_enums of the operator attribute.
	 */
	domGles_texcombiner_operatorAlpha_enums getOperator() const { return attrOperator; }
	/**
	 * Sets the operator attribute.
	 * @param atOperator The new value for the operator attribute.
	 */
	void setOperator( domGles_texcombiner_operatorAlpha_enums atOperator ) { attrOperator = atOperator; }

	/**
	 * Gets the scale attribute.
	 * @return Returns a xsFloat of the scale attribute.
	 */
	xsFloat getScale() const { return attrScale; }
	/**
	 * Sets the scale attribute.
	 * @param atScale The new value for the scale attribute.
	 */
	void setScale( xsFloat atScale ) { attrScale = atScale; }

	/**
	 * Gets the argument element array.
	 * @return Returns a reference to the array of argument elements.
	 */
	domGles_texcombiner_argumentAlpha_type_Array &getArgument_array() { return elemArgument_array; }
	/**
	 * Gets the argument element array.
	 * @return Returns a constant reference to the array of argument elements.
	 */
	const domGles_texcombiner_argumentAlpha_type_Array &getArgument_array() const { return elemArgument_array; }
protected:
	/**
	 * Constructor
	 */
	domGles_texcombiner_commandAlpha_type_complexType(DAE& dae, daeElement* elt) : attrOperator(), attrScale(), elemArgument_array() {}
	/**
	 * Destructor
	 */
	virtual ~domGles_texcombiner_commandAlpha_type_complexType() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domGles_texcombiner_commandAlpha_type_complexType &operator=( const domGles_texcombiner_commandAlpha_type_complexType &cpy ) { (void)cpy; return *this; }
};

/**
 * An element of type domGles_texcombiner_commandAlpha_type_complexType.
 */
class domGles_texcombiner_commandAlpha_type : public daeElement, public domGles_texcombiner_commandAlpha_type_complexType
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::GLES_TEXCOMBINER_COMMANDALPHA_TYPE; }
	static daeInt ID() { return 150; }
	virtual daeInt typeID() const { return ID(); }

public:	//Accessors and Mutators
	/**
	 * Gets the operator attribute.
	 * @return Returns a domGles_texcombiner_operatorAlpha_enums of the operator attribute.
	 */
	domGles_texcombiner_operatorAlpha_enums getOperator() const { return attrOperator; }
	/**
	 * Sets the operator attribute.
	 * @param atOperator The new value for the operator attribute.
	 */
	void setOperator( domGles_texcombiner_operatorAlpha_enums atOperator ) { attrOperator = atOperator; _validAttributeArray[0] = true; }

	/**
	 * Gets the scale attribute.
	 * @return Returns a xsFloat of the scale attribute.
	 */
	xsFloat getScale() const { return attrScale; }
	/**
	 * Sets the scale attribute.
	 * @param atScale The new value for the scale attribute.
	 */
	void setScale( xsFloat atScale ) { attrScale = atScale; _validAttributeArray[1] = true; }

protected:
	/**
	 * Constructor
	 */
	domGles_texcombiner_commandAlpha_type(DAE& dae) : daeElement(dae), domGles_texcombiner_commandAlpha_type_complexType(dae, this) {}
	/**
	 * Destructor
	 */
	virtual ~domGles_texcombiner_commandAlpha_type() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domGles_texcombiner_commandAlpha_type &operator=( const domGles_texcombiner_commandAlpha_type &cpy ) { (void)cpy; return *this; }

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
