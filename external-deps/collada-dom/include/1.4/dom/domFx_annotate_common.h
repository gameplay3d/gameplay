/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 

#ifndef __domFx_annotate_common_h__
#define __domFx_annotate_common_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domFx_annotate_type_common.h>
class DAE;

class domFx_annotate_common_complexType 
{
protected:  // Attribute
	xsNCName attrName;

protected:  // Element
	domFx_annotate_type_commonRef elemFx_annotate_type_common;

public:	//Accessors and Mutators
	/**
	 * Gets the name attribute.
	 * @return Returns a xsNCName of the name attribute.
	 */
	xsNCName getName() const { return attrName; }
	/**
	 * Sets the name attribute.
	 * @param atName The new value for the name attribute.
	 */
	void setName( xsNCName atName ) { *(daeStringRef*)&attrName = atName;}

	/**
	 * Gets the fx_annotate_type_common element.
	 * @return a daeSmartRef to the fx_annotate_type_common element.
	 */
	const domFx_annotate_type_commonRef getFx_annotate_type_common() const { return elemFx_annotate_type_common; }
protected:
	/**
	 * Constructor
	 */
	domFx_annotate_common_complexType(DAE& dae, daeElement* elt) : attrName(), elemFx_annotate_type_common() {}
	/**
	 * Destructor
	 */
	virtual ~domFx_annotate_common_complexType() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domFx_annotate_common_complexType &operator=( const domFx_annotate_common_complexType &cpy ) { (void)cpy; return *this; }
};

/**
 * An element of type domFx_annotate_common_complexType.
 */
class domFx_annotate_common : public daeElement, public domFx_annotate_common_complexType
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FX_ANNOTATE_COMMON; }
	static daeInt ID() { return 91; }
	virtual daeInt typeID() const { return ID(); }

public:	//Accessors and Mutators
	/**
	 * Gets the name attribute.
	 * @return Returns a xsNCName of the name attribute.
	 */
	xsNCName getName() const { return attrName; }
	/**
	 * Sets the name attribute.
	 * @param atName The new value for the name attribute.
	 */
	void setName( xsNCName atName ) { *(daeStringRef*)&attrName = atName; _validAttributeArray[0] = true; }

protected:
	/**
	 * Constructor
	 */
	domFx_annotate_common(DAE& dae) : daeElement(dae), domFx_annotate_common_complexType(dae, this) {}
	/**
	 * Destructor
	 */
	virtual ~domFx_annotate_common() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domFx_annotate_common &operator=( const domFx_annotate_common &cpy ) { (void)cpy; return *this; }

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
