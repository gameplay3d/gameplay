#ifndef __domFx_rendertarget_h__
#define __domFx_rendertarget_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domInstance_image.h>
class DAE;

class domFx_rendertarget : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FX_RENDERTARGET; }
	static daeInt ID() { return 129; }
	virtual daeInt typeID() const { return ID(); }
public:
	class domParam;

	typedef daeSmartRef<domParam> domParamRef;
	typedef daeTArray<domParamRef> domParam_Array;

	class domParam : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::PARAM; }
		static daeInt ID() { return 130; }
		virtual daeInt typeID() const { return ID(); }
	protected:  // Attribute
		xsToken attrRef;


	public:	//Accessors and Mutators
		/**
		 * Gets the ref attribute.
		 * @return Returns a xsToken of the ref attribute.
		 */
		xsToken getRef() const { return attrRef; }
		/**
		 * Sets the ref attribute.
		 * @param atRef The new value for the ref attribute.
		 */
		void setRef( xsToken atRef ) { *(daeStringRef*)&attrRef = atRef; _validAttributeArray[0] = true; }

	protected:
		/**
		 * Constructor
		 */
		domParam(DAE& dae) : daeElement(dae), attrRef() {}
		/**
		 * Destructor
		 */
		virtual ~domParam() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domParam &operator=( const domParam &cpy ) { (void)cpy; return *this; }

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


protected:  // Attributes
/**
 * array index
 */
	xsNonNegativeInteger attrIndex;
/**
 * mip level
 */
	xsNonNegativeInteger attrMip;
/**
 * cube face
 */
	domImage_face attrFace;
/**
 * volume depth
 */
	xsNonNegativeInteger attrSlice;

protected:  // Elements
	domParamRef elemParam;
	domInstance_imageRef elemInstance_image;
	/**
	 * Used to preserve order in elements that do not specify strict sequencing of sub-elements.
	 */
	daeElementRefArray _contents;
	/**
	 * Used to preserve order in elements that have a complex content model.
	 */
	daeUIntArray       _contentsOrder;

	/**
	 * Used to store information needed for some content model objects.
	 */
	daeTArray< daeCharArray * > _CMData;


public:	//Accessors and Mutators
	/**
	 * Gets the index attribute.
	 * @return Returns a xsNonNegativeInteger of the index attribute.
	 */
	xsNonNegativeInteger getIndex() const { return attrIndex; }
	/**
	 * Sets the index attribute.
	 * @param atIndex The new value for the index attribute.
	 */
	void setIndex( xsNonNegativeInteger atIndex ) { attrIndex = atIndex; }

	/**
	 * Gets the mip attribute.
	 * @return Returns a xsNonNegativeInteger of the mip attribute.
	 */
	xsNonNegativeInteger getMip() const { return attrMip; }
	/**
	 * Sets the mip attribute.
	 * @param atMip The new value for the mip attribute.
	 */
	void setMip( xsNonNegativeInteger atMip ) { attrMip = atMip; }

	/**
	 * Gets the face attribute.
	 * @return Returns a domImage_face of the face attribute.
	 */
	domImage_face getFace() const { return attrFace; }
	/**
	 * Sets the face attribute.
	 * @param atFace The new value for the face attribute.
	 */
	void setFace( domImage_face atFace ) { attrFace = atFace; }

	/**
	 * Gets the slice attribute.
	 * @return Returns a xsNonNegativeInteger of the slice attribute.
	 */
	xsNonNegativeInteger getSlice() const { return attrSlice; }
	/**
	 * Sets the slice attribute.
	 * @param atSlice The new value for the slice attribute.
	 */
	void setSlice( xsNonNegativeInteger atSlice ) { attrSlice = atSlice; }

	/**
	 * Gets the param element.
	 * @return a daeSmartRef to the param element.
	 */
	const domParamRef getParam() const { return elemParam; }
	/**
	 * Gets the instance_image element.
	 * @return a daeSmartRef to the instance_image element.
	 */
	const domInstance_imageRef getInstance_image() const { return elemInstance_image; }
	/**
	 * Gets the _contents array.
	 * @return Returns a reference to the _contents element array.
	 */
	daeElementRefArray &getContents() { return _contents; }
	/**
	 * Gets the _contents array.
	 * @return Returns a constant reference to the _contents element array.
	 */
	const daeElementRefArray &getContents() const { return _contents; }

protected:
	/**
	 * Constructor
	 */
	domFx_rendertarget(DAE& dae) : daeElement(dae), attrIndex(), attrMip(), attrFace(), attrSlice(), elemParam(), elemInstance_image() {}
	/**
	 * Destructor
	 */
	virtual ~domFx_rendertarget() { daeElement::deleteCMDataArray(_CMData); }
	/**
	 * Overloaded assignment operator
	 */
	virtual domFx_rendertarget &operator=( const domFx_rendertarget &cpy ) { (void)cpy; return *this; }

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
