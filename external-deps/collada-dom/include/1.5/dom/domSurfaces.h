#ifndef __domSurfaces_h__
#define __domSurfaces_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domSurface.h>
#include <dom/domExtra.h>
class DAE;

class domSurfaces : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::SURFACES; }
	static daeInt ID() { return 362; }
	virtual daeInt typeID() const { return ID(); }

protected:  // Elements
	domSurface_Array elemSurface_array;
	domExtra_Array elemExtra_array;

public:	//Accessors and Mutators
	/**
	 * Gets the surface element array.
	 * @return Returns a reference to the array of surface elements.
	 */
	domSurface_Array &getSurface_array() { return elemSurface_array; }
	/**
	 * Gets the surface element array.
	 * @return Returns a constant reference to the array of surface elements.
	 */
	const domSurface_Array &getSurface_array() const { return elemSurface_array; }
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
	domSurfaces(DAE& dae) : daeElement(dae), elemSurface_array(), elemExtra_array() {}
	/**
	 * Destructor
	 */
	virtual ~domSurfaces() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domSurfaces &operator=( const domSurfaces &cpy ) { (void)cpy; return *this; }

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
