#ifndef __domSurface_curves_h__
#define __domSurface_curves_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domCurve.h>
#include <dom/domExtra.h>
class DAE;

/**
 * This element holds all the pcurves that are needed for the geometrical
 * description of the topological entities edges on the surfaces they lie
 * on.
 */
class domSurface_curves : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::SURFACE_CURVES; }
	static daeInt ID() { return 350; }
	virtual daeInt typeID() const { return ID(); }

protected:  // Elements
	domCurve_Array elemCurve_array;
	domExtra_Array elemExtra_array;

public:	//Accessors and Mutators
	/**
	 * Gets the curve element array.
	 * @return Returns a reference to the array of curve elements.
	 */
	domCurve_Array &getCurve_array() { return elemCurve_array; }
	/**
	 * Gets the curve element array.
	 * @return Returns a constant reference to the array of curve elements.
	 */
	const domCurve_Array &getCurve_array() const { return elemCurve_array; }
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
	domSurface_curves(DAE& dae) : daeElement(dae), elemCurve_array(), elemExtra_array() {}
	/**
	 * Destructor
	 */
	virtual ~domSurface_curves() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domSurface_curves &operator=( const domSurface_curves &cpy ) { (void)cpy; return *this; }

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
