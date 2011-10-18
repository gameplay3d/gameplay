#ifndef __domKinematics_axis_info_h__
#define __domKinematics_axis_info_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domKinematics_newparam.h>
#include <dom/domCommon_bool_or_param.h>
#include <dom/domKinematics_index.h>
#include <dom/domKinematics_limits.h>
#include <dom/domFormula.h>
#include <dom/domInstance_formula.h>
class DAE;

class domKinematics_axis_info : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::KINEMATICS_AXIS_INFO; }
	static daeInt ID() { return 482; }
	virtual daeInt typeID() const { return ID(); }
protected:  // Attributes
	domSid attrSid;
	xsToken attrName;
	xsToken attrAxis;

protected:  // Elements
	domKinematics_newparam_Array elemNewparam_array;
	domCommon_bool_or_paramRef elemActive;
	domCommon_bool_or_paramRef elemLocked;
	domKinematics_index_Array elemIndex_array;
	domKinematics_limitsRef elemLimits;
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
	 * Gets the active element.
	 * @return a daeSmartRef to the active element.
	 */
	const domCommon_bool_or_paramRef getActive() const { return elemActive; }
	/**
	 * Gets the locked element.
	 * @return a daeSmartRef to the locked element.
	 */
	const domCommon_bool_or_paramRef getLocked() const { return elemLocked; }
	/**
	 * Gets the index element array.
	 * @return Returns a reference to the array of index elements.
	 */
	domKinematics_index_Array &getIndex_array() { return elemIndex_array; }
	/**
	 * Gets the index element array.
	 * @return Returns a constant reference to the array of index elements.
	 */
	const domKinematics_index_Array &getIndex_array() const { return elemIndex_array; }
	/**
	 * Gets the limits element.
	 * @return a daeSmartRef to the limits element.
	 */
	const domKinematics_limitsRef getLimits() const { return elemLimits; }
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
	domKinematics_axis_info(DAE& dae) : daeElement(dae), attrSid(), attrName(), attrAxis(), elemNewparam_array(), elemActive(), elemLocked(), elemIndex_array(), elemLimits(), elemFormula_array(), elemInstance_formula_array() {}
	/**
	 * Destructor
	 */
	virtual ~domKinematics_axis_info() { daeElement::deleteCMDataArray(_CMData); }
	/**
	 * Overloaded assignment operator
	 */
	virtual domKinematics_axis_info &operator=( const domKinematics_axis_info &cpy ) { (void)cpy; return *this; }

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
