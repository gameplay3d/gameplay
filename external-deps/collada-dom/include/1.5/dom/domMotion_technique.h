#ifndef __domMotion_technique_h__
#define __domMotion_technique_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domMotion_axis_info.h>
#include <dom/domMotion_effector_info.h>
class DAE;

class domMotion_technique : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::MOTION_TECHNIQUE; }
	static daeInt ID() { return 456; }
	virtual daeInt typeID() const { return ID(); }

protected:  // Elements
	domMotion_axis_info_Array elemAxis_info_array;
	domMotion_effector_infoRef elemEffector_info;

public:	//Accessors and Mutators
	/**
	 * Gets the axis_info element array.
	 * @return Returns a reference to the array of axis_info elements.
	 */
	domMotion_axis_info_Array &getAxis_info_array() { return elemAxis_info_array; }
	/**
	 * Gets the axis_info element array.
	 * @return Returns a constant reference to the array of axis_info elements.
	 */
	const domMotion_axis_info_Array &getAxis_info_array() const { return elemAxis_info_array; }
	/**
	 * Gets the effector_info element.
	 * @return a daeSmartRef to the effector_info element.
	 */
	const domMotion_effector_infoRef getEffector_info() const { return elemEffector_info; }
protected:
	/**
	 * Constructor
	 */
	domMotion_technique(DAE& dae) : daeElement(dae), elemAxis_info_array(), elemEffector_info() {}
	/**
	 * Destructor
	 */
	virtual ~domMotion_technique() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domMotion_technique &operator=( const domMotion_technique &cpy ) { (void)cpy; return *this; }

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
