#ifndef __domKinematics_model_technique_h__
#define __domKinematics_model_technique_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domKinematics_newparam.h>
#include <dom/domInstance_joint.h>
#include <dom/domJoint.h>
#include <dom/domLink.h>
#include <dom/domFormula.h>
#include <dom/domInstance_formula.h>
class DAE;

class domKinematics_model_technique : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::KINEMATICS_MODEL_TECHNIQUE; }
	static daeInt ID() { return 479; }
	virtual daeInt typeID() const { return ID(); }

protected:  // Elements
	domKinematics_newparam_Array elemNewparam_array;
	domInstance_joint_Array elemInstance_joint_array;
	domJoint_Array elemJoint_array;
	domLink_Array elemLink_array;
	domFormula_Array elemFormula_array;
	domInstance_formula_Array elemInstance_formula_array;
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
	 * Gets the instance_joint element array.
	 * @return Returns a reference to the array of instance_joint elements.
	 */
	domInstance_joint_Array &getInstance_joint_array() { return elemInstance_joint_array; }
	/**
	 * Gets the instance_joint element array.
	 * @return Returns a constant reference to the array of instance_joint elements.
	 */
	const domInstance_joint_Array &getInstance_joint_array() const { return elemInstance_joint_array; }
	/**
	 * Gets the joint element array.
	 * @return Returns a reference to the array of joint elements.
	 */
	domJoint_Array &getJoint_array() { return elemJoint_array; }
	/**
	 * Gets the joint element array.
	 * @return Returns a constant reference to the array of joint elements.
	 */
	const domJoint_Array &getJoint_array() const { return elemJoint_array; }
	/**
	 * Gets the link element array.
	 * @return Returns a reference to the array of link elements.
	 */
	domLink_Array &getLink_array() { return elemLink_array; }
	/**
	 * Gets the link element array.
	 * @return Returns a constant reference to the array of link elements.
	 */
	const domLink_Array &getLink_array() const { return elemLink_array; }
	/**
	 * Gets the formula element array.
	 * @return Returns a reference to the array of formula elements.
	 */
	domFormula_Array &getFormula_array() { return elemFormula_array; }
	/**
	 * Gets the formula element array.
	 * @return Returns a constant reference to the array of formula elements.
	 */
	const domFormula_Array &getFormula_array() const { return elemFormula_array; }
	/**
	 * Gets the instance_formula element array.
	 * @return Returns a reference to the array of instance_formula elements.
	 */
	domInstance_formula_Array &getInstance_formula_array() { return elemInstance_formula_array; }
	/**
	 * Gets the instance_formula element array.
	 * @return Returns a constant reference to the array of instance_formula elements.
	 */
	const domInstance_formula_Array &getInstance_formula_array() const { return elemInstance_formula_array; }
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
	domKinematics_model_technique(DAE& dae) : daeElement(dae), elemNewparam_array(), elemInstance_joint_array(), elemJoint_array(), elemLink_array(), elemFormula_array(), elemInstance_formula_array() {}
	/**
	 * Destructor
	 */
	virtual ~domKinematics_model_technique() { daeElement::deleteCMDataArray(_CMData); }
	/**
	 * Overloaded assignment operator
	 */
	virtual domKinematics_model_technique &operator=( const domKinematics_model_technique &cpy ) { (void)cpy; return *this; }

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
