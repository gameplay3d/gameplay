#ifndef __domMotion_h__
#define __domMotion_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domInstance_articulated_system.h>
#include <dom/domMotion_technique.h>
#include <dom/domTechnique.h>
#include <dom/domExtra.h>
class DAE;

class domMotion : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::MOTION; }
	static daeInt ID() { return 457; }
	virtual daeInt typeID() const { return ID(); }

protected:  // Elements
	domInstance_articulated_systemRef elemInstance_articulated_system;
	domMotion_techniqueRef elemTechnique_common;
	domTechnique_Array elemTechnique_array;
	domExtra_Array elemExtra_array;

public:	//Accessors and Mutators
	/**
	 * Gets the instance_articulated_system element.
	 * @return a daeSmartRef to the instance_articulated_system element.
	 */
	const domInstance_articulated_systemRef getInstance_articulated_system() const { return elemInstance_articulated_system; }
	/**
	 * Gets the technique_common element.
	 * @return a daeSmartRef to the technique_common element.
	 */
	const domMotion_techniqueRef getTechnique_common() const { return elemTechnique_common; }
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
	domMotion(DAE& dae) : daeElement(dae), elemInstance_articulated_system(), elemTechnique_common(), elemTechnique_array(), elemExtra_array() {}
	/**
	 * Destructor
	 */
	virtual ~domMotion() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domMotion &operator=( const domMotion &cpy ) { (void)cpy; return *this; }

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
