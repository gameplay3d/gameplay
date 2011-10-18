/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 

#ifndef __domGl_samplerRECT_h__
#define __domGl_samplerRECT_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domFx_samplerRECT_common.h>
class DAE;

/**
 * A two-dimensional texture sampler for the GLSL profile.
 */
class domGl_samplerRECT_complexType  : public domFx_samplerRECT_common_complexType
{

protected:
	/**
	 * Constructor
	 */
	domGl_samplerRECT_complexType(DAE& dae, daeElement* elt) : domFx_samplerRECT_common_complexType(dae, elt) {}
	/**
	 * Destructor
	 */
	virtual ~domGl_samplerRECT_complexType() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domGl_samplerRECT_complexType &operator=( const domGl_samplerRECT_complexType &cpy ) { (void)cpy; return *this; }
};

/**
 * An element of type domGl_samplerRECT_complexType.
 */
class domGl_samplerRECT : public daeElement, public domGl_samplerRECT_complexType
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::GL_SAMPLERRECT; }
	static daeInt ID() { return 101; }
	virtual daeInt typeID() const { return ID(); }
protected:
	/**
	 * Constructor
	 */
	domGl_samplerRECT(DAE& dae) : daeElement(dae), domGl_samplerRECT_complexType(dae, this) {}
	/**
	 * Destructor
	 */
	virtual ~domGl_samplerRECT() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domGl_samplerRECT &operator=( const domGl_samplerRECT &cpy ) { (void)cpy; return *this; }

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
