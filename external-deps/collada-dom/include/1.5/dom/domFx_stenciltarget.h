#ifndef __domFx_stenciltarget_h__
#define __domFx_stenciltarget_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domFx_rendertarget.h>
class DAE;

class domFx_stenciltarget : public domFx_rendertarget
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FX_STENCILTARGET; }
	static daeInt ID() { return 133; }
	virtual daeInt typeID() const { return ID(); }

protected:
	/**
	 * Constructor
	 */
	domFx_stenciltarget(DAE& dae) : domFx_rendertarget(dae) {}
	/**
	 * Destructor
	 */
	virtual ~domFx_stenciltarget() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domFx_stenciltarget &operator=( const domFx_stenciltarget &cpy ) { (void)cpy; return *this; }

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
