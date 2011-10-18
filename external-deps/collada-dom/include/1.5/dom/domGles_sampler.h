#ifndef __domGles_sampler_h__
#define __domGles_sampler_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domGles_sampler_states.h>
#include <dom/domInstance_image.h>
class DAE;

/**
 * Sampling state that can be sharable between samplers because there is often
 * heavy re-use
 */
class domGles_sampler : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::GLES_SAMPLER; }
	static daeInt ID() { return 268; }
	virtual daeInt typeID() const { return ID(); }
public:
	class domTexcoord;

	typedef daeSmartRef<domTexcoord> domTexcoordRef;
	typedef daeTArray<domTexcoordRef> domTexcoord_Array;

	class domTexcoord : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::TEXCOORD; }
		static daeInt ID() { return 269; }
		virtual daeInt typeID() const { return ID(); }
	protected:  // Attribute
		xsNCName attrSemantic;


	public:	//Accessors and Mutators
		/**
		 * Gets the semantic attribute.
		 * @return Returns a xsNCName of the semantic attribute.
		 */
		xsNCName getSemantic() const { return attrSemantic; }
		/**
		 * Sets the semantic attribute.
		 * @param atSemantic The new value for the semantic attribute.
		 */
		void setSemantic( xsNCName atSemantic ) { *(daeStringRef*)&attrSemantic = atSemantic; _validAttributeArray[0] = true; }

	protected:
		/**
		 * Constructor
		 */
		domTexcoord(DAE& dae) : daeElement(dae), attrSemantic() {}
		/**
		 * Destructor
		 */
		virtual ~domTexcoord() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domTexcoord &operator=( const domTexcoord &cpy ) { (void)cpy; return *this; }

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



protected:  // Elements
	domInstance_imageRef elemInstance_image;
	domTexcoordRef elemTexcoord;
	domGles_sampler_statesRef elemGles_sampler_states;

public:	//Accessors and Mutators
	/**
	 * Gets the instance_image element.
	 * @return a daeSmartRef to the instance_image element.
	 */
	const domInstance_imageRef getInstance_image() const { return elemInstance_image; }
	/**
	 * Gets the texcoord element.
	 * @return a daeSmartRef to the texcoord element.
	 */
	const domTexcoordRef getTexcoord() const { return elemTexcoord; }
	/**
	 * Gets the gles_sampler_states element.
	 * @return a daeSmartRef to the gles_sampler_states element.
	 */
	const domGles_sampler_statesRef getGles_sampler_states() const { return elemGles_sampler_states; }
protected:
	/**
	 * Constructor
	 */
	domGles_sampler(DAE& dae) : daeElement(dae), elemInstance_image(), elemTexcoord(), elemGles_sampler_states() {}
	/**
	 * Destructor
	 */
	virtual ~domGles_sampler() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domGles_sampler &operator=( const domGles_sampler &cpy ) { (void)cpy; return *this; }

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
