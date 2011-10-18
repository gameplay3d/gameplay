#ifndef __domBind_joint_axis_h__
#define __domBind_joint_axis_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domCommon_sidref_or_param.h>
#include <dom/domCommon_float_or_param.h>
class DAE;

class domBind_joint_axis : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::BIND_JOINT_AXIS; }
	static daeInt ID() { return 432; }
	virtual daeInt typeID() const { return ID(); }
protected:  // Attribute
	xsToken attrTarget;

protected:  // Elements
	domCommon_sidref_or_paramRef elemAxis;
	domCommon_float_or_paramRef elemValue;

public:	//Accessors and Mutators
	/**
	 * Gets the target attribute.
	 * @return Returns a xsToken of the target attribute.
	 */
	xsToken getTarget() const { return attrTarget; }
	/**
	 * Sets the target attribute.
	 * @param atTarget The new value for the target attribute.
	 */
	void setTarget( xsToken atTarget ) { *(daeStringRef*)&attrTarget = atTarget;}

	/**
	 * Gets the axis element.
	 * @return a daeSmartRef to the axis element.
	 */
	const domCommon_sidref_or_paramRef getAxis() const { return elemAxis; }
	/**
	 * Gets the value element.
	 * @return a daeSmartRef to the value element.
	 */
	const domCommon_float_or_paramRef getValue() const { return elemValue; }
protected:
	/**
	 * Constructor
	 */
	domBind_joint_axis(DAE& dae) : daeElement(dae), attrTarget(), elemAxis(), elemValue() {}
	/**
	 * Destructor
	 */
	virtual ~domBind_joint_axis() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domBind_joint_axis &operator=( const domBind_joint_axis &cpy ) { (void)cpy; return *this; }

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
