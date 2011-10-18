#ifndef __domInstance_kinematics_scene_h__
#define __domInstance_kinematics_scene_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domAsset.h>
#include <dom/domKinematics_newparam.h>
#include <dom/domKinematics_setparam.h>
#include <dom/domBind_kinematics_model.h>
#include <dom/domBind_joint_axis.h>
#include <dom/domExtra.h>
class DAE;

class domInstance_kinematics_scene : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::INSTANCE_KINEMATICS_SCENE; }
	static daeInt ID() { return 440; }
	virtual daeInt typeID() const { return ID(); }
protected:  // Attributes
	xsAnyURI attrUrl;
	domSid attrSid;
	xsToken attrName;

protected:  // Elements
	domAssetRef elemAsset;
	domKinematics_newparam_Array elemNewparam_array;
	domKinematics_setparam_Array elemSetparam_array;
	domBind_kinematics_model_Array elemBind_kinematics_model_array;
	domBind_joint_axis_Array elemBind_joint_axis_array;
	domExtra_Array elemExtra_array;

public:	//Accessors and Mutators
	/**
	 * Gets the url attribute.
	 * @return Returns a xsAnyURI reference of the url attribute.
	 */
	xsAnyURI &getUrl() { return attrUrl; }
	/**
	 * Gets the url attribute.
	 * @return Returns a constant xsAnyURI reference of the url attribute.
	 */
	const xsAnyURI &getUrl() const { return attrUrl; }
	/**
	 * Sets the url attribute.
	 * @param atUrl The new value for the url attribute.
	 */
	void setUrl( const xsAnyURI &atUrl ) { attrUrl = atUrl; }
	/**
	 * Sets the url attribute.
	 * @param atUrl The new value for the url attribute.
	 */
	void setUrl( xsString atUrl ) { attrUrl = atUrl; }

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
	 * Gets the asset element.
	 * @return a daeSmartRef to the asset element.
	 */
	const domAssetRef getAsset() const { return elemAsset; }
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
	 * Gets the bind_kinematics_model element array.
	 * @return Returns a reference to the array of bind_kinematics_model elements.
	 */
	domBind_kinematics_model_Array &getBind_kinematics_model_array() { return elemBind_kinematics_model_array; }
	/**
	 * Gets the bind_kinematics_model element array.
	 * @return Returns a constant reference to the array of bind_kinematics_model elements.
	 */
	const domBind_kinematics_model_Array &getBind_kinematics_model_array() const { return elemBind_kinematics_model_array; }
	/**
	 * Gets the bind_joint_axis element array.
	 * @return Returns a reference to the array of bind_joint_axis elements.
	 */
	domBind_joint_axis_Array &getBind_joint_axis_array() { return elemBind_joint_axis_array; }
	/**
	 * Gets the bind_joint_axis element array.
	 * @return Returns a constant reference to the array of bind_joint_axis elements.
	 */
	const domBind_joint_axis_Array &getBind_joint_axis_array() const { return elemBind_joint_axis_array; }
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
	domInstance_kinematics_scene(DAE& dae) : daeElement(dae), attrUrl(dae, *this), attrSid(), attrName(), elemAsset(), elemNewparam_array(), elemSetparam_array(), elemBind_kinematics_model_array(), elemBind_joint_axis_array(), elemExtra_array() {}
	/**
	 * Destructor
	 */
	virtual ~domInstance_kinematics_scene() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domInstance_kinematics_scene &operator=( const domInstance_kinematics_scene &cpy ) { (void)cpy; return *this; }

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
