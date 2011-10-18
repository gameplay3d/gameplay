/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 

#ifndef __domCg_setparam_simple_h__
#define __domCg_setparam_simple_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domCg_param_type.h>
#include <dom/domFx_annotate_common.h>
class DAE;

class domCg_setparam_simple_complexType 
{
protected:  // Attribute
	domCg_identifier attrRef;

protected:  // Elements
	domFx_annotate_common_Array elemAnnotate_array;
	domCg_param_typeRef elemCg_param_type;

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

	/**
	 * Gets the annotate element array.
	 * @return Returns a reference to the array of annotate elements.
	 */
	domFx_annotate_common_Array &getAnnotate_array() { return elemAnnotate_array; }
	/**
	 * Gets the annotate element array.
	 * @return Returns a constant reference to the array of annotate elements.
	 */
	const domFx_annotate_common_Array &getAnnotate_array() const { return elemAnnotate_array; }
	/**
	 * Gets the cg_param_type element.
	 * @return a daeSmartRef to the cg_param_type element.
	 */
	const domCg_param_typeRef getCg_param_type() const { return elemCg_param_type; }
protected:
	/**
	 * Constructor
	 */
	domCg_setparam_simple_complexType(DAE& dae, daeElement* elt) : attrRef(), elemAnnotate_array(), elemCg_param_type() {}
	/**
	 * Destructor
	 */
	virtual ~domCg_setparam_simple_complexType() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domCg_setparam_simple_complexType &operator=( const domCg_setparam_simple_complexType &cpy ) { (void)cpy; return *this; }
};

/**
 * An element of type domCg_setparam_simple_complexType.
 */
class domCg_setparam_simple : public daeElement, public domCg_setparam_simple_complexType
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::CG_SETPARAM_SIMPLE; }
	static daeInt ID() { return 143; }
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
	domCg_setparam_simple(DAE& dae) : daeElement(dae), domCg_setparam_simple_complexType(dae, this) {}
	/**
	 * Destructor
	 */
	virtual ~domCg_setparam_simple() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domCg_setparam_simple &operator=( const domCg_setparam_simple &cpy ) { (void)cpy; return *this; }

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
