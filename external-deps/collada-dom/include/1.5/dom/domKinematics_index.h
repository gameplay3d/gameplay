#ifndef __domKinematics_index_h__
#define __domKinematics_index_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domCommon_int_or_param.h>
class DAE;

class domKinematics_index : public domCommon_int_or_param
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::KINEMATICS_INDEX; }
	static daeInt ID() { return 441; }
	virtual daeInt typeID() const { return ID(); }
protected:  // Attribute
	xsNMTOKEN attrSemantic;


public:	//Accessors and Mutators
	/**
	 * Gets the semantic attribute.
	 * @return Returns a xsNMTOKEN of the semantic attribute.
	 */
	xsNMTOKEN getSemantic() const { return attrSemantic; }
	/**
	 * Sets the semantic attribute.
	 * @param atSemantic The new value for the semantic attribute.
	 */
	void setSemantic( xsNMTOKEN atSemantic ) { *(daeStringRef*)&attrSemantic = atSemantic;}

protected:
	/**
	 * Constructor
	 */
	domKinematics_index(DAE& dae) : domCommon_int_or_param(dae), attrSemantic() {}
	/**
	 * Destructor
	 */
	virtual ~domKinematics_index() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domKinematics_index &operator=( const domKinematics_index &cpy ) { (void)cpy; return *this; }

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
