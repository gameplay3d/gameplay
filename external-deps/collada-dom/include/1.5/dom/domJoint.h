#ifndef __domJoint_h__
#define __domJoint_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domAxis_constraint.h>
#include <dom/domExtra.h>
class DAE;

class domJoint : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::JOINT; }
	static daeInt ID() { return 462; }
	virtual daeInt typeID() const { return ID(); }
protected:  // Attributes
	xsID attrId;
	xsToken attrName;
	domSid attrSid;

protected:  // Elements
	domAxis_constraint_Array elemPrismatic_array;
	domAxis_constraint_Array elemRevolute_array;
	domExtra_Array elemExtra_array;
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
	 * Gets the id attribute.
	 * @return Returns a xsID of the id attribute.
	 */
	xsID getId() const { return attrId; }
	/**
	 * Sets the id attribute.
	 * @param atId The new value for the id attribute.
	 */
	void setId( xsID atId ) { *(daeStringRef*)&attrId = atId;
		if( _document != NULL ) _document->changeElementID( this, attrId );
	}

	/**
	 * Gets the name attribute.
	 * @return Returns a xsToken of the name attribute.
	 */
	xsToken getName() const { return attrName; }
	/**
	 * Sets the name attribute.
	 * @param atName The new value for the name attribute.
	 */
	void setName( xsToken atName ) { *(daeStringRef*)&attrName = atName;}

	/**
	 * Gets the sid attribute.
	 * @return Returns a domSid of the sid attribute.
	 */
	domSid getSid() const { return attrSid; }
	/**
	 * Sets the sid attribute.
	 * @param atSid The new value for the sid attribute.
	 */
	void setSid( domSid atSid ) { *(daeStringRef*)&attrSid = atSid;}

	/**
	 * Gets the prismatic element array.
	 * @return Returns a reference to the array of prismatic elements.
	 */
	domAxis_constraint_Array &getPrismatic_array() { return elemPrismatic_array; }
	/**
	 * Gets the prismatic element array.
	 * @return Returns a constant reference to the array of prismatic elements.
	 */
	const domAxis_constraint_Array &getPrismatic_array() const { return elemPrismatic_array; }
	/**
	 * Gets the revolute element array.
	 * @return Returns a reference to the array of revolute elements.
	 */
	domAxis_constraint_Array &getRevolute_array() { return elemRevolute_array; }
	/**
	 * Gets the revolute element array.
	 * @return Returns a constant reference to the array of revolute elements.
	 */
	const domAxis_constraint_Array &getRevolute_array() const { return elemRevolute_array; }
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
	domJoint(DAE& dae) : daeElement(dae), attrId(), attrName(), attrSid(), elemPrismatic_array(), elemRevolute_array(), elemExtra_array() {}
	/**
	 * Destructor
	 */
	virtual ~domJoint() { daeElement::deleteCMDataArray(_CMData); }
	/**
	 * Overloaded assignment operator
	 */
	virtual domJoint &operator=( const domJoint &cpy ) { (void)cpy; return *this; }

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
