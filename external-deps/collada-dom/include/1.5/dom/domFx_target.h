#ifndef __domFx_target_h__
#define __domFx_target_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

class DAE;

/**
 * identify the platform_sku and compiler options to build a binary
 */
class domFx_target : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FX_TARGET; }
	static daeInt ID() { return 185; }
	virtual daeInt typeID() const { return ID(); }
public:
	class domBinary;

	typedef daeSmartRef<domBinary> domBinaryRef;
	typedef daeTArray<domBinaryRef> domBinary_Array;

	class domBinary : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::BINARY; }
		static daeInt ID() { return 186; }
		virtual daeInt typeID() const { return ID(); }
	public:
		class domRef;

		typedef daeSmartRef<domRef> domRefRef;
		typedef daeTArray<domRefRef> domRef_Array;

		class domRef : public daeElement
		{
		public:
			virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::REF; }
			static daeInt ID() { return 187; }
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
 * contains a sequence of hexadecimal encoded binary
 */
		class domHex : public daeElement
		{
		public:
			virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::HEX; }
			static daeInt ID() { return 188; }
			virtual daeInt typeID() const { return ID(); }
		protected:  // Attribute
/**
 * binary format which may include target device identificatrion
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
		domRefRef elemRef;
/**
 * contains a sequence of hexadecimal encoded binary @see domHex
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
		domBinary(DAE& dae) : daeElement(dae), elemRef(), elemHex() {}
		/**
		 * Destructor
		 */
		virtual ~domBinary() { daeElement::deleteCMDataArray(_CMData); }
		/**
		 * Overloaded assignment operator
		 */
		virtual domBinary &operator=( const domBinary &cpy ) { (void)cpy; return *this; }

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
 * Name of sub-platform.  May be identical to the platform_series
 */
	xsString attrPlatform;
/**
 * compiler or linker results symbol.  Ex arbvp1, arbfp1, glslv, glslf, hlslv,
 * hlslf, vs_3_0, ps_3_0
 */
	xsString attrTarget;
/**
 * compiler or linker options
 */
	xsString attrOptions;

protected:  // Element
	domBinaryRef elemBinary;

public:	//Accessors and Mutators
	/**
	 * Gets the platform attribute.
	 * @return Returns a xsString of the platform attribute.
	 */
	xsString getPlatform() const { return attrPlatform; }
	/**
	 * Sets the platform attribute.
	 * @param atPlatform The new value for the platform attribute.
	 */
	void setPlatform( xsString atPlatform ) { *(daeStringRef*)&attrPlatform = atPlatform;}

	/**
	 * Gets the target attribute.
	 * @return Returns a xsString of the target attribute.
	 */
	xsString getTarget() const { return attrTarget; }
	/**
	 * Sets the target attribute.
	 * @param atTarget The new value for the target attribute.
	 */
	void setTarget( xsString atTarget ) { *(daeStringRef*)&attrTarget = atTarget;}

	/**
	 * Gets the options attribute.
	 * @return Returns a xsString of the options attribute.
	 */
	xsString getOptions() const { return attrOptions; }
	/**
	 * Sets the options attribute.
	 * @param atOptions The new value for the options attribute.
	 */
	void setOptions( xsString atOptions ) { *(daeStringRef*)&attrOptions = atOptions;}

	/**
	 * Gets the binary element.
	 * @return a daeSmartRef to the binary element.
	 */
	const domBinaryRef getBinary() const { return elemBinary; }
protected:
	/**
	 * Constructor
	 */
	domFx_target(DAE& dae) : daeElement(dae), attrPlatform(), attrTarget(), attrOptions(), elemBinary() {}
	/**
	 * Destructor
	 */
	virtual ~domFx_target() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domFx_target &operator=( const domFx_target &cpy ) { (void)cpy; return *this; }

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
