/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 

#ifndef __domPhysics_model_h__
#define __domPhysics_model_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domAsset.h>
#include <dom/domRigid_body.h>
#include <dom/domRigid_constraint.h>
#include <dom/domInstance_physics_model.h>
#include <dom/domExtra.h>
class DAE;

/**
 * This element allows for building complex combinations of rigid-bodies and
 * constraints that  may be instantiated multiple times.
 */
class domPhysics_model : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::PHYSICS_MODEL; }
	static daeInt ID() { return 813; }
	virtual daeInt typeID() const { return ID(); }
protected:  // Attributes
/**
 *  The id attribute is a text string containing the unique identifier of
 * this element.  This value must be unique within the instance document.
 * Optional attribute. 
 */
	xsID attrId;
/**
 *  The name attribute is the text string name of this element. Optional attribute.
 */
	xsNCName attrName;

protected:  // Elements
/**
 *  The physics_model element may contain an asset element.  @see domAsset
 */
	domAssetRef elemAsset;
/**
 *  The physics_model may define any number of rigid_body elements.  @see
 * domRigid_body
 */
	domRigid_body_Array elemRigid_body_array;
/**
 *  The physics_model may define any number of rigid_constraint elements.
 * @see domRigid_constraint
 */
	domRigid_constraint_Array elemRigid_constraint_array;
/**
 *  The physics_model may instance any number of other physics_model elements.
 * @see domInstance_physics_model
 */
	domInstance_physics_model_Array elemInstance_physics_model_array;
/**
 *  The extra element may appear any number of times.  @see domExtra
 */
	domExtra_Array elemExtra_array;

public:	//Accessors and Mutators
	/**
	 * Gets the id attribute.
	 * @return Returns a xsID of the id attribute.
	 */
	xsID getId() const { return attrId; }
	/**
	 * Sets the id attribute.
	 * @param atId The new value for the id attribute.
	 */
	void setId( xsID atId ) { *(daeStringRef*)&attrId = atId; _validAttributeArray[0] = true; 
		if( _document != NULL ) _document->changeElementID( this, attrId );
	}

	/**
	 * Gets the name attribute.
	 * @return Returns a xsNCName of the name attribute.
	 */
	xsNCName getName() const { return attrName; }
	/**
	 * Sets the name attribute.
	 * @param atName The new value for the name attribute.
	 */
	void setName( xsNCName atName ) { *(daeStringRef*)&attrName = atName; _validAttributeArray[1] = true; }

	/**
	 * Gets the asset element.
	 * @return a daeSmartRef to the asset element.
	 */
	const domAssetRef getAsset() const { return elemAsset; }
	/**
	 * Gets the rigid_body element array.
	 * @return Returns a reference to the array of rigid_body elements.
	 */
	domRigid_body_Array &getRigid_body_array() { return elemRigid_body_array; }
	/**
	 * Gets the rigid_body element array.
	 * @return Returns a constant reference to the array of rigid_body elements.
	 */
	const domRigid_body_Array &getRigid_body_array() const { return elemRigid_body_array; }
	/**
	 * Gets the rigid_constraint element array.
	 * @return Returns a reference to the array of rigid_constraint elements.
	 */
	domRigid_constraint_Array &getRigid_constraint_array() { return elemRigid_constraint_array; }
	/**
	 * Gets the rigid_constraint element array.
	 * @return Returns a constant reference to the array of rigid_constraint elements.
	 */
	const domRigid_constraint_Array &getRigid_constraint_array() const { return elemRigid_constraint_array; }
	/**
	 * Gets the instance_physics_model element array.
	 * @return Returns a reference to the array of instance_physics_model elements.
	 */
	domInstance_physics_model_Array &getInstance_physics_model_array() { return elemInstance_physics_model_array; }
	/**
	 * Gets the instance_physics_model element array.
	 * @return Returns a constant reference to the array of instance_physics_model elements.
	 */
	const domInstance_physics_model_Array &getInstance_physics_model_array() const { return elemInstance_physics_model_array; }
	/**
	 * Gets the extra element array.
	 * @return Returns a reference to the array of extra elements.
	 */
	domExtra_Array &getExtra_array() { return elemExtra_array; }
	/**
	 * Gets the extra element array.
	 * @return Returns a constant reference to the array of extra elements.
	 */
	const domExtra_Array &getExtra_array() const { return elemExtra_array; }
protected:
	/**
	 * Constructor
	 */
	domPhysics_model(DAE& dae) : daeElement(dae), attrId(), attrName(), elemAsset(), elemRigid_body_array(), elemRigid_constraint_array(), elemInstance_physics_model_array(), elemExtra_array() {}
	/**
	 * Destructor
	 */
	virtual ~domPhysics_model() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domPhysics_model &operator=( const domPhysics_model &cpy ) { (void)cpy; return *this; }

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
