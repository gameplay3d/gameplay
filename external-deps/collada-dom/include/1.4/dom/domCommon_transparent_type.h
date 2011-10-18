/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 

#ifndef __domCommon_transparent_type_h__
#define __domCommon_transparent_type_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domCommon_color_or_texture_type.h>
class DAE;

class domCommon_transparent_type_complexType  : public domCommon_color_or_texture_type_complexType
{
protected:  // Attribute
	domFx_opaque_enum attrOpaque;


public:	//Accessors and Mutators
	/**
	 * Gets the opaque attribute.
	 * @return Returns a domFx_opaque_enum of the opaque attribute.
	 */
	domFx_opaque_enum getOpaque() const { return attrOpaque; }
	/**
	 * Sets the opaque attribute.
	 * @param atOpaque The new value for the opaque attribute.
	 */
	void setOpaque( domFx_opaque_enum atOpaque ) { attrOpaque = atOpaque; }

protected:
	/**
	 * Constructor
	 */
	domCommon_transparent_type_complexType(DAE& dae, daeElement* elt) : domCommon_color_or_texture_type_complexType(dae, elt), attrOpaque() {}
	/**
	 * Destructor
	 */
	virtual ~domCommon_transparent_type_complexType() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domCommon_transparent_type_complexType &operator=( const domCommon_transparent_type_complexType &cpy ) { (void)cpy; return *this; }
};

/**
 * An element of type domCommon_transparent_type_complexType.
 */
class domCommon_transparent_type : public daeElement, public domCommon_transparent_type_complexType
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::COMMON_TRANSPARENT_TYPE; }
	static daeInt ID() { return 120; }
	virtual daeInt typeID() const { return ID(); }

public:	//Accessors and Mutators
	/**
	 * Gets the opaque attribute.
	 * @return Returns a domFx_opaque_enum of the opaque attribute.
	 */
	domFx_opaque_enum getOpaque() const { return attrOpaque; }
	/**
	 * Sets the opaque attribute.
	 * @param atOpaque The new value for the opaque attribute.
	 */
	void setOpaque( domFx_opaque_enum atOpaque ) { attrOpaque = atOpaque; _validAttributeArray[0] = true; }

protected:
	/**
	 * Constructor
	 */
	domCommon_transparent_type(DAE& dae) : daeElement(dae), domCommon_transparent_type_complexType(dae, this) {}
	/**
	 * Destructor
	 */
	virtual ~domCommon_transparent_type() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domCommon_transparent_type &operator=( const domCommon_transparent_type &cpy ) { (void)cpy; return *this; }

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
