#ifndef __domImage_source_h__
#define __domImage_source_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

class DAE;

class domImage_source : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::IMAGE_SOURCE; }
	static daeInt ID() { return 19; }
	virtual daeInt typeID() const { return ID(); }
public:
	class domRef;

	typedef daeSmartRef<domRef> domRefRef;
	typedef daeTArray<domRefRef> domRef_Array;

/**
 * Initialize purely from a file.  Assume the characteristics of the file.
 * If it is a complex format like DDS this may include cube maps, volumes,
 * mips, etc.
 */
	class domRef : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::REF; }
		static daeInt ID() { return 20; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return Returns a xsAnyURI of the value.
		 */
		xsAnyURI &getValue() { return _value; }
		/**
		 * Gets the value of this element.
		 * @return Returns a constant xsAnyURI of the value.
		 */
		const xsAnyURI &getValue() const { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const xsAnyURI &val ) { _value = val; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( xsString val ) { _value = val; }

	protected:  // Value
		/**
		 * The xsAnyURI value of the text data of this element. 
		 */
		xsAnyURI _value;
	protected:
		/**
		 * Constructor
		 */
		domRef(DAE& dae) : daeElement(dae), _value(dae, *this) {}
		/**
		 * Destructor
		 */
		virtual ~domRef() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domRef &operator=( const domRef &cpy ) { (void)cpy; return *this; }

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

	class domHex;

	typedef daeSmartRef<domHex> domHexRef;
	typedef daeTArray<domHexRef> domHex_Array;

/**
 * The data element contains a sequence of hexadecimal encoded binary octets
 * representing the embedded image data.  The format attribute is use the
 * determine which codec decodes the image's descriptions and data.
 */
	class domHex : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::HEX; }
		static daeInt ID() { return 21; }
		virtual daeInt typeID() const { return ID(); }
	protected:  // Attribute
/**
 * The format attribute is a text string value that indicates the image format.
 * Examples include "DDS", "EXR", "BMP", "JPG", and "PNG".  Note that all
 * of these formats have internal descriptors of size, bpp, etc.
 */
		xsToken attrFormat;


	public:	//Accessors and Mutators
		/**
		 * Gets the format attribute.
		 * @return Returns a xsToken of the format attribute.
		 */
		xsToken getFormat() const { return attrFormat; }
		/**
		 * Sets the format attribute.
		 * @param atFormat The new value for the format attribute.
		 */
		void setFormat( xsToken atFormat ) { *(daeStringRef*)&attrFormat = atFormat; _validAttributeArray[0] = true; }

		/**
		 * Gets the value of this element.
		 * @return a domList_of_hex_binary of the value.
		 */
		domList_of_hex_binary& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const domList_of_hex_binary& val ) { _value = val; }

	protected:  // Value
		/**
		 * The domList_of_hex_binary value of the text data of this element. 
		 */
		domList_of_hex_binary _value;
	protected:
		/**
		 * Constructor
		 */
		domHex(DAE& dae) : daeElement(dae), attrFormat(), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domHex() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domHex &operator=( const domHex &cpy ) { (void)cpy; return *this; }

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
/**
 * Initialize purely from a file.  Assume the characteristics of the file.
 * If it is a complex format like DDS this may include cube maps, volumes,
 * mips, etc. @see domRef
 */
	domRefRef elemRef;
/**
 * The data element contains a sequence of hexadecimal encoded binary octets
 * representing the embedded image data.  The format attribute is use the
 * determine which codec decodes the image's descriptions and data. @see domHex
 */
	domHexRef elemHex;
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
	 * Gets the ref element.
	 * @return a daeSmartRef to the ref element.
	 */
	const domRefRef getRef() const { return elemRef; }
	/**
	 * Gets the hex element.
	 * @return a daeSmartRef to the hex element.
	 */
	const domHexRef getHex() const { return elemHex; }
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
	domImage_source(DAE& dae) : daeElement(dae), elemRef(), elemHex() {}
	/**
	 * Destructor
	 */
	virtual ~domImage_source() { daeElement::deleteCMDataArray(_CMData); }
	/**
	 * Overloaded assignment operator
	 */
	virtual domImage_source &operator=( const domImage_source &cpy ) { (void)cpy; return *this; }

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
