/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 

#ifndef __domFx_code_profile_h__
#define __domFx_code_profile_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

class DAE;

/**
 * The fx_code_profile type allows you to specify an inline block of source
 * code.
 */
class domFx_code_profile_complexType 
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
	 * The xsString value of the text data of this element. 
	 */
	xsString _value;

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
	 * Gets the value of this element.
	 * @return Returns a xsString of the value.
	 */
	xsString getValue() const { return _value; }
	/**
	 * Sets the _value of this element.
	 * @param val The new value for this element.
	 */
	void setValue( xsString val ) { *(daeStringRef*)&_value = val; }

protected:
	/**
	 * Constructor
	 */
	domFx_code_profile_complexType(DAE& dae, daeElement* elt) : attrSid(), _value() {}
	/**
	 * Destructor
	 */
	virtual ~domFx_code_profile_complexType() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domFx_code_profile_complexType &operator=( const domFx_code_profile_complexType &cpy ) { (void)cpy; return *this; }
};

/**
 * An element of type domFx_code_profile_complexType.
 */
class domFx_code_profile : public daeElement, public domFx_code_profile_complexType
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FX_CODE_PROFILE; }
	static daeInt ID() { return 96; }
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
	domFx_code_profile(DAE& dae) : daeElement(dae), domFx_code_profile_complexType(dae, this) {}
	/**
	 * Destructor
	 */
	virtual ~domFx_code_profile() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domFx_code_profile &operator=( const domFx_code_profile &cpy ) { (void)cpy; return *this; }

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
