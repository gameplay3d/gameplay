/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 

#ifndef __domGles_texcombiner_argumentRGB_type_h__
#define __domGles_texcombiner_argumentRGB_type_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

class DAE;

class domGles_texcombiner_argumentRGB_type_complexType 
{
protected:  // Attributes
	domGles_texcombiner_source_enums attrSource;
	domGles_texcombiner_operandRGB_enums attrOperand;
	xsNCName attrUnit;


public:	//Accessors and Mutators
	/**
	 * Gets the source attribute.
	 * @return Returns a domGles_texcombiner_source_enums of the source attribute.
	 */
	domGles_texcombiner_source_enums getSource() const { return attrSource; }
	/**
	 * Sets the source attribute.
	 * @param atSource The new value for the source attribute.
	 */
	void setSource( domGles_texcombiner_source_enums atSource ) { attrSource = atSource; }

	/**
	 * Gets the operand attribute.
	 * @return Returns a domGles_texcombiner_operandRGB_enums of the operand attribute.
	 */
	domGles_texcombiner_operandRGB_enums getOperand() const { return attrOperand; }
	/**
	 * Sets the operand attribute.
	 * @param atOperand The new value for the operand attribute.
	 */
	void setOperand( domGles_texcombiner_operandRGB_enums atOperand ) { attrOperand = atOperand; }

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

protected:
	/**
	 * Constructor
	 */
	domGles_texcombiner_argumentRGB_type_complexType(DAE& dae, daeElement* elt) : attrSource(), attrOperand(), attrUnit() {}
	/**
	 * Destructor
	 */
	virtual ~domGles_texcombiner_argumentRGB_type_complexType() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domGles_texcombiner_argumentRGB_type_complexType &operator=( const domGles_texcombiner_argumentRGB_type_complexType &cpy ) { (void)cpy; return *this; }
};

/**
 * An element of type domGles_texcombiner_argumentRGB_type_complexType.
 */
class domGles_texcombiner_argumentRGB_type : public daeElement, public domGles_texcombiner_argumentRGB_type_complexType
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::GLES_TEXCOMBINER_ARGUMENTRGB_TYPE; }
	static daeInt ID() { return 147; }
	virtual daeInt typeID() const { return ID(); }

public:	//Accessors and Mutators
	/**
	 * Gets the source attribute.
	 * @return Returns a domGles_texcombiner_source_enums of the source attribute.
	 */
	domGles_texcombiner_source_enums getSource() const { return attrSource; }
	/**
	 * Sets the source attribute.
	 * @param atSource The new value for the source attribute.
	 */
	void setSource( domGles_texcombiner_source_enums atSource ) { attrSource = atSource; _validAttributeArray[0] = true; }

	/**
	 * Gets the operand attribute.
	 * @return Returns a domGles_texcombiner_operandRGB_enums of the operand attribute.
	 */
	domGles_texcombiner_operandRGB_enums getOperand() const { return attrOperand; }
	/**
	 * Sets the operand attribute.
	 * @param atOperand The new value for the operand attribute.
	 */
	void setOperand( domGles_texcombiner_operandRGB_enums atOperand ) { attrOperand = atOperand; _validAttributeArray[1] = true; }

	/**
	 * Gets the unit attribute.
	 * @return Returns a xsNCName of the unit attribute.
	 */
	xsNCName getUnit() const { return attrUnit; }
	/**
	 * Sets the unit attribute.
	 * @param atUnit The new value for the unit attribute.
	 */
	void setUnit( xsNCName atUnit ) { *(daeStringRef*)&attrUnit = atUnit; _validAttributeArray[2] = true; }

protected:
	/**
	 * Constructor
	 */
	domGles_texcombiner_argumentRGB_type(DAE& dae) : daeElement(dae), domGles_texcombiner_argumentRGB_type_complexType(dae, this) {}
	/**
	 * Destructor
	 */
	virtual ~domGles_texcombiner_argumentRGB_type() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domGles_texcombiner_argumentRGB_type &operator=( const domGles_texcombiner_argumentRGB_type &cpy ) { (void)cpy; return *this; }

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
