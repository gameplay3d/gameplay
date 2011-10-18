/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 

#ifndef __domTargetableFloat3_h__
#define __domTargetableFloat3_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

class DAE;

/**
 * The TargetableFloat3 type is used to represent elements which contain a
 * float3 value which can  be targeted for animation.
 */
class domTargetableFloat3_complexType 
{
protected:  // Attribute
/**
 *  The sid attribute is a text string value containing the sub-identifier
 * of this element.  This value must be unique within the scope of the parent
 * element. Optional attribute. 
 */
	xsNCName attrSid;

protected:  // Value
	/**
	 * The domFloat3 value of the text data of this element. 
	 */
	domFloat3 _value;

public:	//Accessors and Mutators
	/**
	 * Gets the sid attribute.
	 * @return Returns a xsNCName of the sid attribute.
	 */
	xsNCName getSid() const { return attrSid; }
	/**
	 * Sets the sid attribute.
	 * @param atSid The new value for the sid attribute.
	 */
	void setSid( xsNCName atSid ) { *(daeStringRef*)&attrSid = atSid;}

	/**
	 * Gets the _value array.
	 * @return Returns a domFloat3 reference of the _value array.
	 */
	domFloat3 &getValue() { return _value; }
	/**
	 * Gets the _value array.
	 * @return Returns a constant domFloat3 reference of the _value array.
	 */
	const domFloat3 &getValue() const { return _value; }
	/**
	 * Sets the _value array.
	 * @param val The new value for the _value array.
	 */
	void setValue( const domFloat3 &val ) { _value = val; }

protected:
	/**
	 * Constructor
	 */
	domTargetableFloat3_complexType(DAE& dae, daeElement* elt) : attrSid(), _value() {}
	/**
	 * Destructor
	 */
	virtual ~domTargetableFloat3_complexType() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domTargetableFloat3_complexType &operator=( const domTargetableFloat3_complexType &cpy ) { (void)cpy; return *this; }
};

/**
 * An element of type domTargetableFloat3_complexType.
 */
class domTargetableFloat3 : public daeElement, public domTargetableFloat3_complexType
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::TARGETABLEFLOAT3; }
	static daeInt ID() { return 5; }
	virtual daeInt typeID() const { return ID(); }

public:	//Accessors and Mutators
	/**
	 * Gets the sid attribute.
	 * @return Returns a xsNCName of the sid attribute.
	 */
	xsNCName getSid() const { return attrSid; }
	/**
	 * Sets the sid attribute.
	 * @param atSid The new value for the sid attribute.
	 */
	void setSid( xsNCName atSid ) { *(daeStringRef*)&attrSid = atSid; _validAttributeArray[0] = true; }

protected:
	/**
	 * Constructor
	 */
	domTargetableFloat3(DAE& dae) : daeElement(dae), domTargetableFloat3_complexType(dae, this) {}
	/**
	 * Destructor
	 */
	virtual ~domTargetableFloat3() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domTargetableFloat3 &operator=( const domTargetableFloat3 &cpy ) { (void)cpy; return *this; }

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
