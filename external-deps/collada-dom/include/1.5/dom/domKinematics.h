#ifndef __domKinematics_h__
#define __domKinematics_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domInstance_kinematics_model.h>
#include <dom/domKinematics_technique.h>
#include <dom/domTechnique.h>
#include <dom/domExtra.h>
class DAE;

class domKinematics : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::KINEMATICS; }
	static daeInt ID() { return 484; }
	virtual daeInt typeID() const { return ID(); }

protected:  // Elements
	domInstance_kinematics_model_Array elemInstance_kinematics_model_array;
	domKinematics_techniqueRef elemTechnique_common;
	domTechnique_Array elemTechnique_array;
	domExtra_Array elemExtra_array;

public:	//Accessors and Mutators
	/**
	 * Gets the instance_kinematics_model element array.
	 * @return Returns a reference to the array of instance_kinematics_model elements.
	 */
	domInstance_kinematics_model_Array &getInstance_kinematics_model_array() { return elemInstance_kinematics_model_array; }
	/**
	 * Gets the instance_kinematics_model element array.
	 * @return Returns a constant reference to the array of instance_kinematics_model elements.
	 */
	const domInstance_kinematics_model_Array &getInstance_kinematics_model_array() const { return elemInstance_kinematics_model_array; }
	/**
	 * Gets the technique_common element.
	 * @return a daeSmartRef to the technique_common element.
	 */
	const domKinematics_techniqueRef getTechnique_common() const { return elemTechnique_common; }
	/**
	 * Gets the technique element array.
	 * @return Returns a reference to the array of technique elements.
	 */
	domTechnique_Array &getTechnique_array() { return elemTechnique_array; }
	/**
	 * Gets the technique element array.
	 * @return Returns a constant reference to the array of technique elements.
	 */
	const domTechnique_Array &getTechnique_array() const { return elemTechnique_array; }
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
	domKinematics(DAE& dae) : daeElement(dae), elemInstance_kinematics_model_array(), elemTechnique_common(), elemTechnique_array(), elemExtra_array() {}
	/**
	 * Destructor
	 */
	virtual ~domKinematics() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domKinematics &operator=( const domKinematics &cpy ) { (void)cpy; return *this; }

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
