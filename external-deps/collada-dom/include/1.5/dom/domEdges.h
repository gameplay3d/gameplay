#ifndef __domEdges_h__
#define __domEdges_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domInput_local_offset.h>
#include <dom/domP.h>
#include <dom/domExtra.h>
class DAE;

/**
 * This element defines all the edges used in the brep     structure.
 */
class domEdges : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::EDGES; }
	static daeInt ID() { return 300; }
	virtual daeInt typeID() const { return ID(); }
protected:  // Attributes
	xsID attrId;
	xsToken attrName;
	xsInt attrCount;

protected:  // Elements
/**
 * Five input elements are needed to define an edge, one for referencing the
 * curve, two for referencing the two vertices, one for the orientation of
 * the first vertex (the second is the opposite of the first) and one for
 * the parametric values of the curve. @see domInput
 */
	domInput_local_offset_Array elemInput_array;
/**
 * The indices for refrencing curves and        vertices. @see domP
 */
	domPRef elemP;
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
	 * Gets the count attribute.
	 * @return Returns a xsInt of the count attribute.
	 */
	xsInt getCount() const { return attrCount; }
	/**
	 * Sets the count attribute.
	 * @param atCount The new value for the count attribute.
	 */
	void setCount( xsInt atCount ) { attrCount = atCount; }

	/**
	 * Gets the input element array.
	 * @return Returns a reference to the array of input elements.
	 */
	domInput_local_offset_Array &getInput_array() { return elemInput_array; }
	/**
	 * Gets the input element array.
	 * @return Returns a constant reference to the array of input elements.
	 */
	const domInput_local_offset_Array &getInput_array() const { return elemInput_array; }
	/**
	 * Gets the p element.
	 * @return a daeSmartRef to the p element.
	 */
	const domPRef getP() const { return elemP; }
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
	domEdges(DAE& dae) : daeElement(dae), attrId(), attrName(), attrCount(), elemInput_array(), elemP(), elemExtra_array() {}
	/**
	 * Destructor
	 */
	virtual ~domEdges() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domEdges &operator=( const domEdges &cpy ) { (void)cpy; return *this; }

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
