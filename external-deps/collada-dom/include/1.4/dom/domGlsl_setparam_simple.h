/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 

#ifndef __domGlsl_setparam_simple_h__
#define __domGlsl_setparam_simple_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domGlsl_param_type.h>
#include <dom/domFx_annotate_common.h>
class DAE;

class domGlsl_setparam_simple_complexType 
{
protected:  // Attribute
	domGlsl_identifier attrRef;

protected:  // Elements
	domFx_annotate_common_Array elemAnnotate_array;
	domGlsl_param_typeRef elemGlsl_param_type;

public:	//Accessors and Mutators
	/**
	 * Gets the ref attribute.
	 * @return Returns a domGlsl_identifier of the ref attribute.
	 */
	domGlsl_identifier getRef() const { return attrRef; }
	/**
	 * Sets the ref attribute.
	 * @param atRef The new value for the ref attribute.
	 */
	void setRef( domGlsl_identifier atRef ) { attrRef = atRef; }

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
	 * Gets the glsl_param_type element.
	 * @return a daeSmartRef to the glsl_param_type element.
	 */
	const domGlsl_param_typeRef getGlsl_param_type() const { return elemGlsl_param_type; }
protected:
	/**
	 * Constructor
	 */
	domGlsl_setparam_simple_complexType(DAE& dae, daeElement* elt) : attrRef(), elemAnnotate_array(), elemGlsl_param_type() {}
	/**
	 * Destructor
	 */
	virtual ~domGlsl_setparam_simple_complexType() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domGlsl_setparam_simple_complexType &operator=( const domGlsl_setparam_simple_complexType &cpy ) { (void)cpy; return *this; }
};

/**
 * An element of type domGlsl_setparam_simple_complexType.
 */
class domGlsl_setparam_simple : public daeElement, public domGlsl_setparam_simple_complexType
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::GLSL_SETPARAM_SIMPLE; }
	static daeInt ID() { return 111; }
	virtual daeInt typeID() const { return ID(); }

public:	//Accessors and Mutators
	/**
	 * Gets the ref attribute.
	 * @return Returns a domGlsl_identifier of the ref attribute.
	 */
	domGlsl_identifier getRef() const { return attrRef; }
	/**
	 * Sets the ref attribute.
	 * @param atRef The new value for the ref attribute.
	 */
	void setRef( domGlsl_identifier atRef ) { attrRef = atRef; _validAttributeArray[0] = true; }

protected:
	/**
	 * Constructor
	 */
	domGlsl_setparam_simple(DAE& dae) : daeElement(dae), domGlsl_setparam_simple_complexType(dae, this) {}
	/**
	 * Destructor
	 */
	virtual ~domGlsl_setparam_simple() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domGlsl_setparam_simple &operator=( const domGlsl_setparam_simple &cpy ) { (void)cpy; return *this; }

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
