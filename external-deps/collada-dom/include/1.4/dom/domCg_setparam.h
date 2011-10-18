/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 

#ifndef __domCg_setparam_h__
#define __domCg_setparam_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domCg_param_type.h>
#include <dom/domCg_setuser_type.h>
#include <dom/domCg_setarray_type.h>
#include <dom/domCg_connect_param.h>
class DAE;

/**
 * Assigns a new value to a previously defined parameter.
 */
class domCg_setparam_complexType 
{
protected:  // Attributes
	domCg_identifier attrRef;
	xsNCName attrProgram;

protected:  // Elements
	domCg_param_typeRef elemCg_param_type;
	domCg_setuser_typeRef elemUsertype;
	domCg_setarray_typeRef elemArray;
	domCg_connect_paramRef elemConnect_param;
	/**
	 * Used to preserve order in elements that do not specify strict sequencing of sub-elements.
	 */
	daeElementRefArray _contents;
	/**
	 * Used to preserve order in elements that have a complex content model.
	 */
	daeUIntArray       _contentsOrder;

	/**
	 * Used to store information needed for some content model objects.
	 */
	daeTArray< daeCharArray * > _CMData;


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
	 * Gets the program attribute.
	 * @return Returns a xsNCName of the program attribute.
	 */
	xsNCName getProgram() const { return attrProgram; }
	/**
	 * Sets the program attribute.
	 * @param atProgram The new value for the program attribute.
	 */
	void setProgram( xsNCName atProgram ) { *(daeStringRef*)&attrProgram = atProgram;}

	/**
	 * Gets the cg_param_type element.
	 * @return a daeSmartRef to the cg_param_type element.
	 */
	const domCg_param_typeRef getCg_param_type() const { return elemCg_param_type; }
	/**
	 * Gets the usertype element.
	 * @return a daeSmartRef to the usertype element.
	 */
	const domCg_setuser_typeRef getUsertype() const { return elemUsertype; }
	/**
	 * Gets the array element.
	 * @return a daeSmartRef to the array element.
	 */
	const domCg_setarray_typeRef getArray() const { return elemArray; }
	/**
	 * Gets the connect_param element.
	 * @return a daeSmartRef to the connect_param element.
	 */
	const domCg_connect_paramRef getConnect_param() const { return elemConnect_param; }
	/**
	 * Gets the _contents array.
	 * @return Returns a reference to the _contents element array.
	 */
	daeElementRefArray &getContents() { return _contents; }
	/**
	 * Gets the _contents array.
	 * @return Returns a constant reference to the _contents element array.
	 */
	const daeElementRefArray &getContents() const { return _contents; }

protected:
	/**
	 * Constructor
	 */
	domCg_setparam_complexType(DAE& dae, daeElement* elt) : attrRef(), attrProgram(), elemCg_param_type(), elemUsertype(), elemArray(), elemConnect_param() {}
	/**
	 * Destructor
	 */
	virtual ~domCg_setparam_complexType() { daeElement::deleteCMDataArray(_CMData); }
	/**
	 * Overloaded assignment operator
	 */
	virtual domCg_setparam_complexType &operator=( const domCg_setparam_complexType &cpy ) { (void)cpy; return *this; }
};

/**
 * An element of type domCg_setparam_complexType.
 */
class domCg_setparam : public daeElement, public domCg_setparam_complexType
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::CG_SETPARAM; }
	static daeInt ID() { return 144; }
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

	/**
	 * Gets the program attribute.
	 * @return Returns a xsNCName of the program attribute.
	 */
	xsNCName getProgram() const { return attrProgram; }
	/**
	 * Sets the program attribute.
	 * @param atProgram The new value for the program attribute.
	 */
	void setProgram( xsNCName atProgram ) { *(daeStringRef*)&attrProgram = atProgram; _validAttributeArray[1] = true; }

protected:
	/**
	 * Constructor
	 */
	domCg_setparam(DAE& dae) : daeElement(dae), domCg_setparam_complexType(dae, this) {}
	/**
	 * Destructor
	 */
	virtual ~domCg_setparam() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domCg_setparam &operator=( const domCg_setparam &cpy ) { (void)cpy; return *this; }

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
