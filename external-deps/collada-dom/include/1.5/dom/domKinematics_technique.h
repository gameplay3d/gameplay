#ifndef __domKinematics_technique_h__
#define __domKinematics_technique_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domKinematics_axis_info.h>
#include <dom/domKinematics_frame.h>
class DAE;

class domKinematics_technique : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::KINEMATICS_TECHNIQUE; }
	static daeInt ID() { return 483; }
	virtual daeInt typeID() const { return ID(); }

protected:  // Elements
	domKinematics_axis_info_Array elemAxis_info_array;
	domKinematics_frameRef elemFrame_origin;
	domKinematics_frameRef elemFrame_tip;
	domKinematics_frameRef elemFrame_tcp;
	domKinematics_frameRef elemFrame_object;

public:	//Accessors and Mutators
	/**
	 * Gets the axis_info element array.
	 * @return Returns a reference to the array of axis_info elements.
	 */
	domKinematics_axis_info_Array &getAxis_info_array() { return elemAxis_info_array; }
	/**
	 * Gets the axis_info element array.
	 * @return Returns a constant reference to the array of axis_info elements.
	 */
	const domKinematics_axis_info_Array &getAxis_info_array() const { return elemAxis_info_array; }
	/**
	 * Gets the frame_origin element.
	 * @return a daeSmartRef to the frame_origin element.
	 */
	const domKinematics_frameRef getFrame_origin() const { return elemFrame_origin; }
	/**
	 * Gets the frame_tip element.
	 * @return a daeSmartRef to the frame_tip element.
	 */
	const domKinematics_frameRef getFrame_tip() const { return elemFrame_tip; }
	/**
	 * Gets the frame_tcp element.
	 * @return a daeSmartRef to the frame_tcp element.
	 */
	const domKinematics_frameRef getFrame_tcp() const { return elemFrame_tcp; }
	/**
	 * Gets the frame_object element.
	 * @return a daeSmartRef to the frame_object element.
	 */
	const domKinematics_frameRef getFrame_object() const { return elemFrame_object; }
protected:
	/**
	 * Constructor
	 */
	domKinematics_technique(DAE& dae) : daeElement(dae), elemAxis_info_array(), elemFrame_origin(), elemFrame_tip(), elemFrame_tcp(), elemFrame_object() {}
	/**
	 * Destructor
	 */
	virtual ~domKinematics_technique() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domKinematics_technique &operator=( const domKinematics_technique &cpy ) { (void)cpy; return *this; }

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
