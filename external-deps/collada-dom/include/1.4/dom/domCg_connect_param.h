/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 

#ifndef __domCg_connect_param_h__
#define __domCg_connect_param_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

class DAE;

/**
 * Creates a symbolic connection between two previously defined parameters.
 */
class domCg_connect_param_complexType 
{
protected:  // Attribute
	domCg_identifier attrRef;


public:	//Accessors and Mutators
	/**
	 * Gets the ref attribute.
	 * @return Returns a domCg_identifier of the ref attribute.
	 */
	domCg_identifier getRef() const { return attrRef; }
	/**
	 * Sets the ref attribute.
	 * @param atRef The new value for the ref attribute.
	 */
	void setRef( domCg_identifier atRef ) { attrRef = atRef; }

protected:
	/**
	 * Constructor
	 */
	domCg_connect_param_complexType(DAE& dae, daeElement* elt) : attrRef() {}
	/**
	 * Destructor
	 */
	virtual ~domCg_connect_param_complexType() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domCg_connect_param_complexType &operator=( const domCg_connect_param_complexType &cpy ) { (void)cpy; return *this; }
};

/**
 * An element of type domCg_connect_param_complexType.
 */
class domCg_connect_param : public daeElement, public domCg_connect_param_complexType
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::CG_CONNECT_PARAM; }
	static daeInt ID() { return 133; }
	virtual daeInt typeID() const { return ID(); }

public:	//Accessors and Mutators
	/**
	 * Gets the ref attribute.
	 * @return Returns a domCg_identifier of the ref attribute.
	 */
	domCg_identifier getRef() const { return attrRef; }
	/**
	 * Sets the ref attribute.
	 * @param atRef The new value for the ref attribute.
	 */
	void setRef( domCg_identifier atRef ) { attrRef = atRef; _validAttributeArray[0] = true; }

protected:
	/**
	 * Constructor
	 */
	domCg_connect_param(DAE& dae) : daeElement(dae), domCg_connect_param_complexType(dae, this) {}
	/**
	 * Destructor
	 */
	virtual ~domCg_connect_param() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domCg_connect_param &operator=( const domCg_connect_param &cpy ) { (void)cpy; return *this; }

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
