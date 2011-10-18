/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 

#ifndef __domFx_cleardepth_common_h__
#define __domFx_cleardepth_common_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

class DAE;

class domFx_cleardepth_common_complexType 
{
protected:  // Attribute
	xsNonNegativeInteger attrIndex;

protected:  // Value
	/**
	 * The domFloat value of the text data of this element. 
	 */
	domFloat _value;

public:	//Accessors and Mutators
	/**
	 * Gets the index attribute.
	 * @return Returns a xsNonNegativeInteger of the index attribute.
	 */
	xsNonNegativeInteger getIndex() const { return attrIndex; }
	/**
	 * Sets the index attribute.
	 * @param atIndex The new value for the index attribute.
	 */
	void setIndex( xsNonNegativeInteger atIndex ) { attrIndex = atIndex; }

	/**
	 * Gets the value of this element.
	 * @return a domFloat of the value.
	 */
	domFloat getValue() const { return _value; }
	/**
	 * Sets the _value of this element.
	 * @param val The new value for this element.
	 */
	void setValue( domFloat val ) { _value = val; }

protected:
	/**
	 * Constructor
	 */
	domFx_cleardepth_common_complexType(DAE& dae, daeElement* elt) : attrIndex(), _value() {}
	/**
	 * Destructor
	 */
	virtual ~domFx_cleardepth_common_complexType() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domFx_cleardepth_common_complexType &operator=( const domFx_cleardepth_common_complexType &cpy ) { (void)cpy; return *this; }
};

/**
 * An element of type domFx_cleardepth_common_complexType.
 */
class domFx_cleardepth_common : public daeElement, public domFx_cleardepth_common_complexType
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FX_CLEARDEPTH_COMMON; }
	static daeInt ID() { return 89; }
	virtual daeInt typeID() const { return ID(); }

public:	//Accessors and Mutators
	/**
	 * Gets the index attribute.
	 * @return Returns a xsNonNegativeInteger of the index attribute.
	 */
	xsNonNegativeInteger getIndex() const { return attrIndex; }
	/**
	 * Sets the index attribute.
	 * @param atIndex The new value for the index attribute.
	 */
	void setIndex( xsNonNegativeInteger atIndex ) { attrIndex = atIndex; _validAttributeArray[0] = true; }

protected:
	/**
	 * Constructor
	 */
	domFx_cleardepth_common(DAE& dae) : daeElement(dae), domFx_cleardepth_common_complexType(dae, this) {}
	/**
	 * Destructor
	 */
	virtual ~domFx_cleardepth_common() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domFx_cleardepth_common &operator=( const domFx_cleardepth_common &cpy ) { (void)cpy; return *this; }

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
