#ifndef __domMotion_axis_info_h__
#define __domMotion_axis_info_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domKinematics_bind.h>
#include <dom/domKinematics_newparam.h>
#include <dom/domKinematics_setparam.h>
#include <dom/domCommon_float_or_param.h>
class DAE;

class domMotion_axis_info : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::MOTION_AXIS_INFO; }
	static daeInt ID() { return 448; }
	virtual daeInt typeID() const { return ID(); }
protected:  // Attributes
	domSid attrSid;
	xsToken attrAxis;
	xsToken attrName;

protected:  // Elements
	domKinematics_bind_Array elemBind_array;
	domKinematics_newparam_Array elemNewparam_array;
	domKinematics_setparam_Array elemSetparam_array;
	domCommon_float_or_paramRef elemSpeed;
	domCommon_float_or_paramRef elemAcceleration;
	domCommon_float_or_paramRef elemDeceleration;
	domCommon_float_or_paramRef elemJerk;

public:	//Accessors and Mutators
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
	 * Gets the axis attribute.
	 * @return Returns a xsToken of the axis attribute.
	 */
	xsToken getAxis() const { return attrAxis; }
	/**
	 * Sets the axis attribute.
	 * @param atAxis The new value for the axis attribute.
	 */
	void setAxis( xsToken atAxis ) { *(daeStringRef*)&attrAxis = atAxis;}

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
	 * Gets the bind element array.
	 * @return Returns a reference to the array of bind elements.
	 */
	domKinematics_bind_Array &getBind_array() { return elemBind_array; }
	/**
	 * Gets the bind element array.
	 * @return Returns a constant reference to the array of bind elements.
	 */
	const domKinematics_bind_Array &getBind_array() const { return elemBind_array; }
	/**
	 * Gets the newparam element array.
	 * @return Returns a reference to the array of newparam elements.
	 */
	domKinematics_newparam_Array &getNewparam_array() { return elemNewparam_array; }
	/**
	 * Gets the newparam element array.
	 * @return Returns a constant reference to the array of newparam elements.
	 */
	const domKinematics_newparam_Array &getNewparam_array() const { return elemNewparam_array; }
	/**
	 * Gets the setparam element array.
	 * @return Returns a reference to the array of setparam elements.
	 */
	domKinematics_setparam_Array &getSetparam_array() { return elemSetparam_array; }
	/**
	 * Gets the setparam element array.
	 * @return Returns a constant reference to the array of setparam elements.
	 */
	const domKinematics_setparam_Array &getSetparam_array() const { return elemSetparam_array; }
	/**
	 * Gets the speed element.
	 * @return a daeSmartRef to the speed element.
	 */
	const domCommon_float_or_paramRef getSpeed() const { return elemSpeed; }
	/**
	 * Gets the acceleration element.
	 * @return a daeSmartRef to the acceleration element.
	 */
	const domCommon_float_or_paramRef getAcceleration() const { return elemAcceleration; }
	/**
	 * Gets the deceleration element.
	 * @return a daeSmartRef to the deceleration element.
	 */
	const domCommon_float_or_paramRef getDeceleration() const { return elemDeceleration; }
	/**
	 * Gets the jerk element.
	 * @return a daeSmartRef to the jerk element.
	 */
	const domCommon_float_or_paramRef getJerk() const { return elemJerk; }
protected:
	/**
	 * Constructor
	 */
	domMotion_axis_info(DAE& dae) : daeElement(dae), attrSid(), attrAxis(), attrName(), elemBind_array(), elemNewparam_array(), elemSetparam_array(), elemSpeed(), elemAcceleration(), elemDeceleration(), elemJerk() {}
	/**
	 * Destructor
	 */
	virtual ~domMotion_axis_info() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domMotion_axis_info &operator=( const domMotion_axis_info &cpy ) { (void)cpy; return *this; }

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
