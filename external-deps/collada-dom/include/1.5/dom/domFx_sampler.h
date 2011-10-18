#ifndef __domFx_sampler_h__
#define __domFx_sampler_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domFx_sampler_states.h>
#include <dom/domInstance_image.h>
class DAE;

/**
 * Sampling state that can be sharable between samplers because there is often
 * heavy re-use
 */
class domFx_sampler : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FX_SAMPLER; }
	static daeInt ID() { return 122; }
	virtual daeInt typeID() const { return ID(); }

protected:  // Elements
	domInstance_imageRef elemInstance_image;
	domFx_sampler_statesRef elemFx_sampler_states;

public:	//Accessors and Mutators
	/**
	 * Gets the instance_image element.
	 * @return a daeSmartRef to the instance_image element.
	 */
	const domInstance_imageRef getInstance_image() const { return elemInstance_image; }
	/**
	 * Gets the fx_sampler_states element.
	 * @return a daeSmartRef to the fx_sampler_states element.
	 */
	const domFx_sampler_statesRef getFx_sampler_states() const { return elemFx_sampler_states; }
protected:
	/**
	 * Constructor
	 */
	domFx_sampler(DAE& dae) : daeElement(dae), elemInstance_image(), elemFx_sampler_states() {}
	/**
	 * Destructor
	 */
	virtual ~domFx_sampler() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domFx_sampler &operator=( const domFx_sampler &cpy ) { (void)cpy; return *this; }

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
