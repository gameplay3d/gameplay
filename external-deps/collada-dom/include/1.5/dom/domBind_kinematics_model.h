#ifndef __domBind_kinematics_model_h__
#define __domBind_kinematics_model_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domCommon_sidref_or_param.h>
class DAE;

class domBind_kinematics_model : public domCommon_sidref_or_param
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::BIND_KINEMATICS_MODEL; }
	static daeInt ID() { return 433; }
	virtual daeInt typeID() const { return ID(); }
protected:  // Attribute
	xsToken attrNode;


public:	//Accessors and Mutators
	/**
	 * Gets the node attribute.
	 * @return Returns a xsToken of the node attribute.
	 */
	xsToken getNode() const { return attrNode; }
	/**
	 * Sets the node attribute.
	 * @param atNode The new value for the node attribute.
	 */
	void setNode( xsToken atNode ) { *(daeStringRef*)&attrNode = atNode;}

protected:
	/**
	 * Constructor
	 */
	domBind_kinematics_model(DAE& dae) : domCommon_sidref_or_param(dae), attrNode() {}
	/**
	 * Destructor
	 */
	virtual ~domBind_kinematics_model() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domBind_kinematics_model &operator=( const domBind_kinematics_model &cpy ) { (void)cpy; return *this; }

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
