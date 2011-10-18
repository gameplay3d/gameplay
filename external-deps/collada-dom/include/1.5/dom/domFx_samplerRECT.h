#ifndef __domFx_samplerRECT_h__
#define __domFx_samplerRECT_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domFx_sampler.h>
class DAE;

#include <dom/domFx_sampler.h>
/**
 * A two-dimensional texture sampler.
 */
class domFx_samplerRECT : public domFx_sampler
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FX_SAMPLERRECT; }
	static daeInt ID() { return 127; }
	virtual daeInt typeID() const { return ID(); }

protected:
	/**
	 * Constructor
	 */
	domFx_samplerRECT(DAE& dae) : domFx_sampler(dae) {}
	/**
	 * Destructor
	 */
	virtual ~domFx_samplerRECT() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domFx_samplerRECT &operator=( const domFx_samplerRECT &cpy ) { (void)cpy; return *this; }

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
