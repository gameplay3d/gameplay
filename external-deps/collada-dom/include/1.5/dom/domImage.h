#ifndef __domImage_h__
#define __domImage_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domAsset.h>
#include <dom/domImage_source.h>
#include <dom/domImage_mips.h>
#include <dom/domExtra.h>
class DAE;

class domImage : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::IMAGE; }
	static daeInt ID() { return 60; }
	virtual daeInt typeID() const { return ID(); }
public:
	class domRenderable;

	typedef daeSmartRef<domRenderable> domRenderableRef;
	typedef daeTArray<domRenderableRef> domRenderable_Array;

/**
 * This image is renderable  if this element is used
 */
	class domRenderable : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::RENDERABLE; }
		static daeInt ID() { return 61; }
		virtual daeInt typeID() const { return ID(); }
	protected:  // Attribute
/**
 * As a render target, true = shared across all users, false = unique per
 * instance
 */
		xsBoolean attrShare;


	public:	//Accessors and Mutators
		/**
		 * Gets the share attribute.
		 * @return Returns a xsBoolean of the share attribute.
		 */
		xsBoolean getShare() const { return attrShare; }
		/**
		 * Sets the share attribute.
		 * @param atShare The new value for the share attribute.
		 */
		void setShare( xsBoolean atShare ) { attrShare = atShare; _validAttributeArray[0] = true; }

	protected:
		/**
		 * Constructor
		 */
		domRenderable(DAE& dae) : daeElement(dae), attrShare() {}
		/**
		 * Destructor
		 */
		virtual ~domRenderable() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domRenderable &operator=( const domRenderable &cpy ) { (void)cpy; return *this; }

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

	class domInit_from;

	typedef daeSmartRef<domInit_from> domInit_fromRef;
	typedef daeTArray<domInit_fromRef> domInit_from_Array;

/**
 * initialize the whole image structure and data from formats like DDS
 */
	class domInit_from : public domImage_source
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::INIT_FROM; }
		static daeInt ID() { return 62; }
		virtual daeInt typeID() const { return ID(); }
	protected:  // Attribute
/**
 * If true, unless mips already exist in data, generate the mipmaps based
 * on information loaded into mip0, 
 */
		xsBoolean attrMips_generate;


	public:	//Accessors and Mutators
		/**
		 * Gets the mips_generate attribute.
		 * @return Returns a xsBoolean of the mips_generate attribute.
		 */
		xsBoolean getMips_generate() const { return attrMips_generate; }
		/**
		 * Sets the mips_generate attribute.
		 * @param atMips_generate The new value for the mips_generate attribute.
		 */
		void setMips_generate( xsBoolean atMips_generate ) { attrMips_generate = atMips_generate; _validAttributeArray[0] = true; }

	protected:
		/**
		 * Constructor
		 */
		domInit_from(DAE& dae) : domImage_source(dae), attrMips_generate() {}
		/**
		 * Destructor
		 */
		virtual ~domInit_from() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domInit_from &operator=( const domInit_from &cpy ) { (void)cpy; return *this; }

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

	class domCreate_2d;

	typedef daeSmartRef<domCreate_2d> domCreate_2dRef;
	typedef daeTArray<domCreate_2dRef> domCreate_2d_Array;

/**
 * Initialize a custom 2d image
 */
	class domCreate_2d : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::CREATE_2D; }
		static daeInt ID() { return 63; }
		virtual daeInt typeID() const { return ID(); }
	public:
		class domSize_exact;

		typedef daeSmartRef<domSize_exact> domSize_exactRef;
		typedef daeTArray<domSize_exactRef> domSize_exact_Array;

/**
 * The surface should be sized to these exact dimensions
 */
		class domSize_exact : public daeElement
		{
		public:
			virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::SIZE_EXACT; }
			static daeInt ID() { return 64; }
			virtual daeInt typeID() const { return ID(); }
		protected:  // Attributes
			xsUnsignedInt attrWidth;
			xsUnsignedInt attrHeight;


		public:	//Accessors and Mutators
			/**
			 * Gets the width attribute.
			 * @return Returns a xsUnsignedInt of the width attribute.
			 */
			xsUnsignedInt getWidth() const { return attrWidth; }
			/**
			 * Sets the width attribute.
			 * @param atWidth The new value for the width attribute.
			 */
			void setWidth( xsUnsignedInt atWidth ) { attrWidth = atWidth; _validAttributeArray[0] = true; }

			/**
			 * Gets the height attribute.
			 * @return Returns a xsUnsignedInt of the height attribute.
			 */
			xsUnsignedInt getHeight() const { return attrHeight; }
			/**
			 * Sets the height attribute.
			 * @param atHeight The new value for the height attribute.
			 */
			void setHeight( xsUnsignedInt atHeight ) { attrHeight = atHeight; _validAttributeArray[1] = true; }

		protected:
			/**
			 * Constructor
			 */
			domSize_exact(DAE& dae) : daeElement(dae), attrWidth(), attrHeight() {}
			/**
			 * Destructor
			 */
			virtual ~domSize_exact() {}
			/**
			 * Overloaded assignment operator
			 */
			virtual domSize_exact &operator=( const domSize_exact &cpy ) { (void)cpy; return *this; }

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

		class domSize_ratio;

		typedef daeSmartRef<domSize_ratio> domSize_ratioRef;
		typedef daeTArray<domSize_ratioRef> domSize_ratio_Array;

/**
 * Image size should be relative to the size of the viewport.  Ex 1,1 is the
 * same size as the viewport.  0.5,0.5 is 1/2 the size of the viewport
 */
		class domSize_ratio : public daeElement
		{
		public:
			virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::SIZE_RATIO; }
			static daeInt ID() { return 65; }
			virtual daeInt typeID() const { return ID(); }
		protected:  // Attributes
			xsFloat attrWidth;
			xsFloat attrHeight;


		public:	//Accessors and Mutators
			/**
			 * Gets the width attribute.
			 * @return Returns a xsFloat of the width attribute.
			 */
			xsFloat getWidth() const { return attrWidth; }
			/**
			 * Sets the width attribute.
			 * @param atWidth The new value for the width attribute.
			 */
			void setWidth( xsFloat atWidth ) { attrWidth = atWidth; _validAttributeArray[0] = true; }

			/**
			 * Gets the height attribute.
			 * @return Returns a xsFloat of the height attribute.
			 */
			xsFloat getHeight() const { return attrHeight; }
			/**
			 * Sets the height attribute.
			 * @param atHeight The new value for the height attribute.
			 */
			void setHeight( xsFloat atHeight ) { attrHeight = atHeight; _validAttributeArray[1] = true; }

		protected:
			/**
			 * Constructor
			 */
			domSize_ratio(DAE& dae) : daeElement(dae), attrWidth(), attrHeight() {}
			/**
			 * Destructor
			 */
			virtual ~domSize_ratio() {}
			/**
			 * Overloaded assignment operator
			 */
			virtual domSize_ratio &operator=( const domSize_ratio &cpy ) { (void)cpy; return *this; }

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

		class domUnnormalized;

		typedef daeSmartRef<domUnnormalized> domUnnormalizedRef;
		typedef daeTArray<domUnnormalizedRef> domUnnormalized_Array;

/**
 * Unnormalized addressing of texels.  (0-W, 0-H).  This cannot be used with
 * mips since the addressing is not uniform per level.  This is equivilant
 * to OpenGL textureRECT extension.
 */
		class domUnnormalized : public daeElement
		{
		public:
			virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::UNNORMALIZED; }
			static daeInt ID() { return 66; }
			virtual daeInt typeID() const { return ID(); }

		protected:
			/**
			 * Constructor
			 */
			domUnnormalized(DAE& dae) : daeElement(dae) {}
			/**
			 * Destructor
			 */
			virtual ~domUnnormalized() {}
			/**
			 * Overloaded assignment operator
			 */
			virtual domUnnormalized &operator=( const domUnnormalized &cpy ) { (void)cpy; return *this; }

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

		class domArray;

		typedef daeSmartRef<domArray> domArrayRef;
		typedef daeTArray<domArrayRef> domArray_Array;

		class domArray : public daeElement
		{
		public:
			virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::ARRAY; }
			static daeInt ID() { return 67; }
			virtual daeInt typeID() const { return ID(); }
		protected:  // Attribute
			xsPositiveInteger attrLength;


		public:	//Accessors and Mutators
			/**
			 * Gets the length attribute.
			 * @return Returns a xsPositiveInteger of the length attribute.
			 */
			xsPositiveInteger getLength() const { return attrLength; }
			/**
			 * Sets the length attribute.
			 * @param atLength The new value for the length attribute.
			 */
			void setLength( xsPositiveInteger atLength ) { attrLength = atLength; _validAttributeArray[0] = true; }

		protected:
			/**
			 * Constructor
			 */
			domArray(DAE& dae) : daeElement(dae), attrLength() {}
			/**
			 * Destructor
			 */
			virtual ~domArray() {}
			/**
			 * Overloaded assignment operator
			 */
			virtual domArray &operator=( const domArray &cpy ) { (void)cpy; return *this; }

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

		class domFormat;

		typedef daeSmartRef<domFormat> domFormatRef;
		typedef daeTArray<domFormatRef> domFormat_Array;

/**
 * If not present it is assumed to be R8G8B8A8 linear.
 */
		class domFormat : public daeElement
		{
		public:
			virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FORMAT; }
			static daeInt ID() { return 68; }
			virtual daeInt typeID() const { return ID(); }
		public:
			class domHint;

			typedef daeSmartRef<domHint> domHintRef;
			typedef daeTArray<domHintRef> domHint_Array;

/**
 * if this element or a higher precidence element is not present then use
 * a common format R8G8B8A8 with linear color gradient, not  sRGB.
 */
			class domHint : public daeElement
			{
			public:
				virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::HINT; }
				static daeInt ID() { return 69; }
				virtual daeInt typeID() const { return ID(); }
			protected:  // Attributes
/**
 * The per-texel layout of the format.  The length of the string indicate
 * how many channels there are and the letter respresents the name of the
 * channel.  There are typically 0 to 4 channels.
 */
				domImage_format_hint_channels attrChannels;
/**
 * Each channel represents a range of values. Some example ranges are signed
 * or unsigned integers, or between between a clamped range such as 0.0f to
 * 1.0f, or high dynamic range via floating point
 */
				domImage_format_hint_range attrRange;
/**
 * Each channel of the texel has a precision.  Typically these are all linked
 * together.  An exact format lay lower the precision of an individual channel
 * but applying a higher precision by linking the channels together may still
 * convey the same information.
 */
				domImage_format_hint_precision attrPrecision;
/**
 * linear(default), sRGB 
 */
				xsToken attrSpace;


			public:	//Accessors and Mutators
				/**
				 * Gets the channels attribute.
				 * @return Returns a domImage_format_hint_channels of the channels attribute.
				 */
				domImage_format_hint_channels getChannels() const { return attrChannels; }
				/**
				 * Sets the channels attribute.
				 * @param atChannels The new value for the channels attribute.
				 */
				void setChannels( domImage_format_hint_channels atChannels ) { attrChannels = atChannels; _validAttributeArray[0] = true; }

				/**
				 * Gets the range attribute.
				 * @return Returns a domImage_format_hint_range of the range attribute.
				 */
				domImage_format_hint_range getRange() const { return attrRange; }
				/**
				 * Sets the range attribute.
				 * @param atRange The new value for the range attribute.
				 */
				void setRange( domImage_format_hint_range atRange ) { attrRange = atRange; _validAttributeArray[1] = true; }

				/**
				 * Gets the precision attribute.
				 * @return Returns a domImage_format_hint_precision of the precision attribute.
				 */
				domImage_format_hint_precision getPrecision() const { return attrPrecision; }
				/**
				 * Sets the precision attribute.
				 * @param atPrecision The new value for the precision attribute.
				 */
				void setPrecision( domImage_format_hint_precision atPrecision ) { attrPrecision = atPrecision; _validAttributeArray[2] = true; }

				/**
				 * Gets the space attribute.
				 * @return Returns a xsToken of the space attribute.
				 */
				xsToken getSpace() const { return attrSpace; }
				/**
				 * Sets the space attribute.
				 * @param atSpace The new value for the space attribute.
				 */
				void setSpace( xsToken atSpace ) { *(daeStringRef*)&attrSpace = atSpace; _validAttributeArray[3] = true; }

			protected:
				/**
				 * Constructor
				 */
				domHint(DAE& dae) : daeElement(dae), attrChannels(), attrRange(), attrPrecision(), attrSpace() {}
				/**
				 * Destructor
				 */
				virtual ~domHint() {}
				/**
				 * Overloaded assignment operator
				 */
				virtual domHint &operator=( const domHint &cpy ) { (void)cpy; return *this; }

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

			class domExact;

			typedef daeSmartRef<domExact> domExactRef;
			typedef daeTArray<domExactRef> domExact_Array;

/**
 * Contains a string representing the profile and platform specific texel
 * format that the author would like this surface to use.  If this element
 * is not specified then the application will fall back to the hint
 */
			class domExact : public daeElement
			{
			public:
				virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::EXACT; }
				static daeInt ID() { return 70; }
				virtual daeInt typeID() const { return ID(); }


			public:	//Accessors and Mutators
				/**
				 * Gets the value of this element.
				 * @return Returns a xsToken of the value.
				 */
				xsToken getValue() const { return _value; }
				/**
				 * Sets the _value of this element.
				 * @param val The new value for this element.
				 */
				void setValue( xsToken val ) { *(daeStringRef*)&_value = val; }

			protected:  // Value
				/**
				 * The xsToken value of the text data of this element. 
				 */
				xsToken _value;
			protected:
				/**
				 * Constructor
				 */
				domExact(DAE& dae) : daeElement(dae), _value() {}
				/**
				 * Destructor
				 */
				virtual ~domExact() {}
				/**
				 * Overloaded assignment operator
				 */
				virtual domExact &operator=( const domExact &cpy ) { (void)cpy; return *this; }

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
 * if this element or a higher precidence element is not present then use
 * a common format R8G8B8A8 with linear color gradient, not  sRGB. @see domHint
 */
			domHintRef elemHint;
/**
 * Contains a string representing the profile and platform specific texel
 * format that the author would like this surface to use.  If this element
 * is not specified then the application will fall back to the hint @see domExact
 */
			domExactRef elemExact;

		public:	//Accessors and Mutators
			/**
			 * Gets the hint element.
			 * @return a daeSmartRef to the hint element.
			 */
			const domHintRef getHint() const { return elemHint; }
			/**
			 * Gets the exact element.
			 * @return a daeSmartRef to the exact element.
			 */
			const domExactRef getExact() const { return elemExact; }
		protected:
			/**
			 * Constructor
			 */
			domFormat(DAE& dae) : daeElement(dae), elemHint(), elemExact() {}
			/**
			 * Destructor
			 */
			virtual ~domFormat() {}
			/**
			 * Overloaded assignment operator
			 */
			virtual domFormat &operator=( const domFormat &cpy ) { (void)cpy; return *this; }

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

		class domInit_from;

		typedef daeSmartRef<domInit_from> domInit_fromRef;
		typedef daeTArray<domInit_fromRef> domInit_from_Array;

/**
 * init the image data from one or more partial pieces of data.  The load
 * attribute indicates what portion of the image will be loaded based on the
 * ''address" specified.  mip_generate may be used to complete the image.
 */
		class domInit_from : public domImage_source
		{
		public:
			virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::INIT_FROM; }
			static daeInt ID() { return 71; }
			virtual daeInt typeID() const { return ID(); }
		protected:  // Attributes
			xsUnsignedInt attrMip_index;
			xsUnsignedInt attrArray_index;


		public:	//Accessors and Mutators
			/**
			 * Gets the mip_index attribute.
			 * @return Returns a xsUnsignedInt of the mip_index attribute.
			 */
			xsUnsignedInt getMip_index() const { return attrMip_index; }
			/**
			 * Sets the mip_index attribute.
			 * @param atMip_index The new value for the mip_index attribute.
			 */
			void setMip_index( xsUnsignedInt atMip_index ) { attrMip_index = atMip_index; _validAttributeArray[0] = true; }

			/**
			 * Gets the array_index attribute.
			 * @return Returns a xsUnsignedInt of the array_index attribute.
			 */
			xsUnsignedInt getArray_index() const { return attrArray_index; }
			/**
			 * Sets the array_index attribute.
			 * @param atArray_index The new value for the array_index attribute.
			 */
			void setArray_index( xsUnsignedInt atArray_index ) { attrArray_index = atArray_index; _validAttributeArray[1] = true; }

		protected:
			/**
			 * Constructor
			 */
			domInit_from(DAE& dae) : domImage_source(dae), attrMip_index(), attrArray_index() {}
			/**
			 * Destructor
			 */
			virtual ~domInit_from() {}
			/**
			 * Overloaded assignment operator
			 */
			virtual domInit_from &operator=( const domInit_from &cpy ) { (void)cpy; return *this; }

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
 * The surface should be sized to these exact dimensions @see domSize_exact
 */
		domSize_exactRef elemSize_exact;
/**
 * Image size should be relative to the size of the viewport.  Ex 1,1 is the
 * same size as the viewport.  0.5,0.5 is 1/2 the size of the viewport @see
 * domSize_ratio
 */
		domSize_ratioRef elemSize_ratio;
		domImage_mipsRef elemMips;
/**
 * Unnormalized addressing of texels.  (0-W, 0-H).  This cannot be used with
 * mips since the addressing is not uniform per level.  This is equivilant
 * to OpenGL textureRECT extension. @see domUnnormalized
 */
		domUnnormalizedRef elemUnnormalized;
		domArrayRef elemArray;
/**
 * If not present it is assumed to be R8G8B8A8 linear. @see domFormat
 */
		domFormatRef elemFormat;
/**
 * init the image data from one or more partial pieces of data.  The load
 * attribute indicates what portion of the image will be loaded based on the
 * ''address" specified.  mip_generate may be used to complete the image.
 * @see domInit_from
 */
		domInit_from_Array elemInit_from_array;
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
		 * Gets the size_exact element.
		 * @return a daeSmartRef to the size_exact element.
		 */
		const domSize_exactRef getSize_exact() const { return elemSize_exact; }
		/**
		 * Gets the size_ratio element.
		 * @return a daeSmartRef to the size_ratio element.
		 */
		const domSize_ratioRef getSize_ratio() const { return elemSize_ratio; }
		/**
		 * Gets the mips element.
		 * @return a daeSmartRef to the mips element.
		 */
		const domImage_mipsRef getMips() const { return elemMips; }
		/**
		 * Gets the unnormalized element.
		 * @return a daeSmartRef to the unnormalized element.
		 */
		const domUnnormalizedRef getUnnormalized() const { return elemUnnormalized; }
		/**
		 * Gets the array element.
		 * @return a daeSmartRef to the array element.
		 */
		const domArrayRef getArray() const { return elemArray; }
		/**
		 * Gets the format element.
		 * @return a daeSmartRef to the format element.
		 */
		const domFormatRef getFormat() const { return elemFormat; }
		/**
		 * Gets the init_from element array.
		 * @return Returns a reference to the array of init_from elements.
		 */
		domInit_from_Array &getInit_from_array() { return elemInit_from_array; }
		/**
		 * Gets the init_from element array.
		 * @return Returns a constant reference to the array of init_from elements.
		 */
		const domInit_from_Array &getInit_from_array() const { return elemInit_from_array; }
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
		domCreate_2d(DAE& dae) : daeElement(dae), elemSize_exact(), elemSize_ratio(), elemMips(), elemUnnormalized(), elemArray(), elemFormat(), elemInit_from_array() {}
		/**
		 * Destructor
		 */
		virtual ~domCreate_2d() { daeElement::deleteCMDataArray(_CMData); }
		/**
		 * Overloaded assignment operator
		 */
		virtual domCreate_2d &operator=( const domCreate_2d &cpy ) { (void)cpy; return *this; }

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

	class domCreate_3d;

	typedef daeSmartRef<domCreate_3d> domCreate_3dRef;
	typedef daeTArray<domCreate_3dRef> domCreate_3d_Array;

/**
 * Initialize a custom 3d image
 */
	class domCreate_3d : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::CREATE_3D; }
		static daeInt ID() { return 72; }
		virtual daeInt typeID() const { return ID(); }
	public:
		class domSize;

		typedef daeSmartRef<domSize> domSizeRef;
		typedef daeTArray<domSizeRef> domSize_Array;

/**
 * The surface should be sized to these exact dimensions
 */
		class domSize : public daeElement
		{
		public:
			virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::SIZE; }
			static daeInt ID() { return 73; }
			virtual daeInt typeID() const { return ID(); }
		protected:  // Attributes
			xsUnsignedInt attrWidth;
			xsUnsignedInt attrHeight;
			xsUnsignedInt attrDepth;


		public:	//Accessors and Mutators
			/**
			 * Gets the width attribute.
			 * @return Returns a xsUnsignedInt of the width attribute.
			 */
			xsUnsignedInt getWidth() const { return attrWidth; }
			/**
			 * Sets the width attribute.
			 * @param atWidth The new value for the width attribute.
			 */
			void setWidth( xsUnsignedInt atWidth ) { attrWidth = atWidth; _validAttributeArray[0] = true; }

			/**
			 * Gets the height attribute.
			 * @return Returns a xsUnsignedInt of the height attribute.
			 */
			xsUnsignedInt getHeight() const { return attrHeight; }
			/**
			 * Sets the height attribute.
			 * @param atHeight The new value for the height attribute.
			 */
			void setHeight( xsUnsignedInt atHeight ) { attrHeight = atHeight; _validAttributeArray[1] = true; }

			/**
			 * Gets the depth attribute.
			 * @return Returns a xsUnsignedInt of the depth attribute.
			 */
			xsUnsignedInt getDepth() const { return attrDepth; }
			/**
			 * Sets the depth attribute.
			 * @param atDepth The new value for the depth attribute.
			 */
			void setDepth( xsUnsignedInt atDepth ) { attrDepth = atDepth; _validAttributeArray[2] = true; }

		protected:
			/**
			 * Constructor
			 */
			domSize(DAE& dae) : daeElement(dae), attrWidth(), attrHeight(), attrDepth() {}
			/**
			 * Destructor
			 */
			virtual ~domSize() {}
			/**
			 * Overloaded assignment operator
			 */
			virtual domSize &operator=( const domSize &cpy ) { (void)cpy; return *this; }

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

		class domArray;

		typedef daeSmartRef<domArray> domArrayRef;
		typedef daeTArray<domArrayRef> domArray_Array;

		class domArray : public daeElement
		{
		public:
			virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::ARRAY; }
			static daeInt ID() { return 74; }
			virtual daeInt typeID() const { return ID(); }
		protected:  // Attribute
/**
 * Zero is not an array
 */
			xsUnsignedInt attrLength;


		public:	//Accessors and Mutators
			/**
			 * Gets the length attribute.
			 * @return Returns a xsUnsignedInt of the length attribute.
			 */
			xsUnsignedInt getLength() const { return attrLength; }
			/**
			 * Sets the length attribute.
			 * @param atLength The new value for the length attribute.
			 */
			void setLength( xsUnsignedInt atLength ) { attrLength = atLength; _validAttributeArray[0] = true; }

		protected:
			/**
			 * Constructor
			 */
			domArray(DAE& dae) : daeElement(dae), attrLength() {}
			/**
			 * Destructor
			 */
			virtual ~domArray() {}
			/**
			 * Overloaded assignment operator
			 */
			virtual domArray &operator=( const domArray &cpy ) { (void)cpy; return *this; }

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

		class domFormat;

		typedef daeSmartRef<domFormat> domFormatRef;
		typedef daeTArray<domFormatRef> domFormat_Array;

/**
 * If not present it is assumed to be R8G8B8A8 linear.
 */
		class domFormat : public daeElement
		{
		public:
			virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FORMAT; }
			static daeInt ID() { return 75; }
			virtual daeInt typeID() const { return ID(); }
		public:
			class domHint;

			typedef daeSmartRef<domHint> domHintRef;
			typedef daeTArray<domHintRef> domHint_Array;

/**
 * if this element or a higher precidence element is not present then use
 * a common format R8G8B8A8 with linear color gradient, not  sRGB.
 */
			class domHint : public daeElement
			{
			public:
				virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::HINT; }
				static daeInt ID() { return 76; }
				virtual daeInt typeID() const { return ID(); }
			protected:  // Attributes
/**
 * The per-texel layout of the format.  The length of the string indicate
 * how many channels there are and the letter respresents the name of the
 * channel.  There are typically 0 to 4 channels.
 */
				domImage_format_hint_channels attrChannels;
/**
 * Each channel represents a range of values. Some example ranges are signed
 * or unsigned integers, or between between a clamped range such as 0.0f to
 * 1.0f, or high dynamic range via floating point
 */
				domImage_format_hint_range attrRange;
/**
 * Each channel of the texel has a precision.  Typically these are all linked
 * together.  An exact format lay lower the precision of an individual channel
 * but applying a higher precision by linking the channels together may still
 * convey the same information.
 */
				domImage_format_hint_precision attrPrecision;
/**
 * linear(default), sRGB 
 */
				xsToken attrSpace;


			public:	//Accessors and Mutators
				/**
				 * Gets the channels attribute.
				 * @return Returns a domImage_format_hint_channels of the channels attribute.
				 */
				domImage_format_hint_channels getChannels() const { return attrChannels; }
				/**
				 * Sets the channels attribute.
				 * @param atChannels The new value for the channels attribute.
				 */
				void setChannels( domImage_format_hint_channels atChannels ) { attrChannels = atChannels; _validAttributeArray[0] = true; }

				/**
				 * Gets the range attribute.
				 * @return Returns a domImage_format_hint_range of the range attribute.
				 */
				domImage_format_hint_range getRange() const { return attrRange; }
				/**
				 * Sets the range attribute.
				 * @param atRange The new value for the range attribute.
				 */
				void setRange( domImage_format_hint_range atRange ) { attrRange = atRange; _validAttributeArray[1] = true; }

				/**
				 * Gets the precision attribute.
				 * @return Returns a domImage_format_hint_precision of the precision attribute.
				 */
				domImage_format_hint_precision getPrecision() const { return attrPrecision; }
				/**
				 * Sets the precision attribute.
				 * @param atPrecision The new value for the precision attribute.
				 */
				void setPrecision( domImage_format_hint_precision atPrecision ) { attrPrecision = atPrecision; _validAttributeArray[2] = true; }

				/**
				 * Gets the space attribute.
				 * @return Returns a xsToken of the space attribute.
				 */
				xsToken getSpace() const { return attrSpace; }
				/**
				 * Sets the space attribute.
				 * @param atSpace The new value for the space attribute.
				 */
				void setSpace( xsToken atSpace ) { *(daeStringRef*)&attrSpace = atSpace; _validAttributeArray[3] = true; }

			protected:
				/**
				 * Constructor
				 */
				domHint(DAE& dae) : daeElement(dae), attrChannels(), attrRange(), attrPrecision(), attrSpace() {}
				/**
				 * Destructor
				 */
				virtual ~domHint() {}
				/**
				 * Overloaded assignment operator
				 */
				virtual domHint &operator=( const domHint &cpy ) { (void)cpy; return *this; }

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

			class domExact;

			typedef daeSmartRef<domExact> domExactRef;
			typedef daeTArray<domExactRef> domExact_Array;

/**
 * Contains a string representing the profile and platform specific texel
 * format that the author would like this surface to use.  If this element
 * is not specified then the application will fall back to the hint
 */
			class domExact : public daeElement
			{
			public:
				virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::EXACT; }
				static daeInt ID() { return 77; }
				virtual daeInt typeID() const { return ID(); }


			public:	//Accessors and Mutators
				/**
				 * Gets the value of this element.
				 * @return Returns a xsToken of the value.
				 */
				xsToken getValue() const { return _value; }
				/**
				 * Sets the _value of this element.
				 * @param val The new value for this element.
				 */
				void setValue( xsToken val ) { *(daeStringRef*)&_value = val; }

			protected:  // Value
				/**
				 * The xsToken value of the text data of this element. 
				 */
				xsToken _value;
			protected:
				/**
				 * Constructor
				 */
				domExact(DAE& dae) : daeElement(dae), _value() {}
				/**
				 * Destructor
				 */
				virtual ~domExact() {}
				/**
				 * Overloaded assignment operator
				 */
				virtual domExact &operator=( const domExact &cpy ) { (void)cpy; return *this; }

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
 * if this element or a higher precidence element is not present then use
 * a common format R8G8B8A8 with linear color gradient, not  sRGB. @see domHint
 */
			domHintRef elemHint;
/**
 * Contains a string representing the profile and platform specific texel
 * format that the author would like this surface to use.  If this element
 * is not specified then the application will fall back to the hint @see domExact
 */
			domExactRef elemExact;

		public:	//Accessors and Mutators
			/**
			 * Gets the hint element.
			 * @return a daeSmartRef to the hint element.
			 */
			const domHintRef getHint() const { return elemHint; }
			/**
			 * Gets the exact element.
			 * @return a daeSmartRef to the exact element.
			 */
			const domExactRef getExact() const { return elemExact; }
		protected:
			/**
			 * Constructor
			 */
			domFormat(DAE& dae) : daeElement(dae), elemHint(), elemExact() {}
			/**
			 * Destructor
			 */
			virtual ~domFormat() {}
			/**
			 * Overloaded assignment operator
			 */
			virtual domFormat &operator=( const domFormat &cpy ) { (void)cpy; return *this; }

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

		class domInit_from;

		typedef daeSmartRef<domInit_from> domInit_fromRef;
		typedef daeTArray<domInit_fromRef> domInit_from_Array;

/**
 * init the image data from one or more partial pieces of data.  The load
 * attribute indicates what portion of the image will be loaded based on the
 * ''address" specified.  mip_generate may be used to complete the image.
 */
		class domInit_from : public domImage_source
		{
		public:
			virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::INIT_FROM; }
			static daeInt ID() { return 78; }
			virtual daeInt typeID() const { return ID(); }
		protected:  // Attributes
			xsUnsignedInt attrDepth;
			xsUnsignedInt attrMip_index;
			xsUnsignedInt attrArray_index;


		public:	//Accessors and Mutators
			/**
			 * Gets the depth attribute.
			 * @return Returns a xsUnsignedInt of the depth attribute.
			 */
			xsUnsignedInt getDepth() const { return attrDepth; }
			/**
			 * Sets the depth attribute.
			 * @param atDepth The new value for the depth attribute.
			 */
			void setDepth( xsUnsignedInt atDepth ) { attrDepth = atDepth; _validAttributeArray[0] = true; }

			/**
			 * Gets the mip_index attribute.
			 * @return Returns a xsUnsignedInt of the mip_index attribute.
			 */
			xsUnsignedInt getMip_index() const { return attrMip_index; }
			/**
			 * Sets the mip_index attribute.
			 * @param atMip_index The new value for the mip_index attribute.
			 */
			void setMip_index( xsUnsignedInt atMip_index ) { attrMip_index = atMip_index; _validAttributeArray[1] = true; }

			/**
			 * Gets the array_index attribute.
			 * @return Returns a xsUnsignedInt of the array_index attribute.
			 */
			xsUnsignedInt getArray_index() const { return attrArray_index; }
			/**
			 * Sets the array_index attribute.
			 * @param atArray_index The new value for the array_index attribute.
			 */
			void setArray_index( xsUnsignedInt atArray_index ) { attrArray_index = atArray_index; _validAttributeArray[2] = true; }

		protected:
			/**
			 * Constructor
			 */
			domInit_from(DAE& dae) : domImage_source(dae), attrDepth(), attrMip_index(), attrArray_index() {}
			/**
			 * Destructor
			 */
			virtual ~domInit_from() {}
			/**
			 * Overloaded assignment operator
			 */
			virtual domInit_from &operator=( const domInit_from &cpy ) { (void)cpy; return *this; }

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
 * The surface should be sized to these exact dimensions @see domSize
 */
		domSizeRef elemSize;
		domImage_mipsRef elemMips;
		domArrayRef elemArray;
/**
 * If not present it is assumed to be R8G8B8A8 linear. @see domFormat
 */
		domFormatRef elemFormat;
/**
 * init the image data from one or more partial pieces of data.  The load
 * attribute indicates what portion of the image will be loaded based on the
 * ''address" specified.  mip_generate may be used to complete the image.
 * @see domInit_from
 */
		domInit_from_Array elemInit_from_array;

	public:	//Accessors and Mutators
		/**
		 * Gets the size element.
		 * @return a daeSmartRef to the size element.
		 */
		const domSizeRef getSize() const { return elemSize; }
		/**
		 * Gets the mips element.
		 * @return a daeSmartRef to the mips element.
		 */
		const domImage_mipsRef getMips() const { return elemMips; }
		/**
		 * Gets the array element.
		 * @return a daeSmartRef to the array element.
		 */
		const domArrayRef getArray() const { return elemArray; }
		/**
		 * Gets the format element.
		 * @return a daeSmartRef to the format element.
		 */
		const domFormatRef getFormat() const { return elemFormat; }
		/**
		 * Gets the init_from element array.
		 * @return Returns a reference to the array of init_from elements.
		 */
		domInit_from_Array &getInit_from_array() { return elemInit_from_array; }
		/**
		 * Gets the init_from element array.
		 * @return Returns a constant reference to the array of init_from elements.
		 */
		const domInit_from_Array &getInit_from_array() const { return elemInit_from_array; }
	protected:
		/**
		 * Constructor
		 */
		domCreate_3d(DAE& dae) : daeElement(dae), elemSize(), elemMips(), elemArray(), elemFormat(), elemInit_from_array() {}
		/**
		 * Destructor
		 */
		virtual ~domCreate_3d() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domCreate_3d &operator=( const domCreate_3d &cpy ) { (void)cpy; return *this; }

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

	class domCreate_cube;

	typedef daeSmartRef<domCreate_cube> domCreate_cubeRef;
	typedef daeTArray<domCreate_cubeRef> domCreate_cube_Array;

/**
 * Initialize a custom cube image
 */
	class domCreate_cube : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::CREATE_CUBE; }
		static daeInt ID() { return 79; }
		virtual daeInt typeID() const { return ID(); }
	public:
		class domSize;

		typedef daeSmartRef<domSize> domSizeRef;
		typedef daeTArray<domSizeRef> domSize_Array;

/**
 * The surface should be sized to these exact dimensions
 */
		class domSize : public daeElement
		{
		public:
			virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::SIZE; }
			static daeInt ID() { return 80; }
			virtual daeInt typeID() const { return ID(); }
		protected:  // Attribute
			xsUnsignedInt attrWidth;


		public:	//Accessors and Mutators
			/**
			 * Gets the width attribute.
			 * @return Returns a xsUnsignedInt of the width attribute.
			 */
			xsUnsignedInt getWidth() const { return attrWidth; }
			/**
			 * Sets the width attribute.
			 * @param atWidth The new value for the width attribute.
			 */
			void setWidth( xsUnsignedInt atWidth ) { attrWidth = atWidth; _validAttributeArray[0] = true; }

		protected:
			/**
			 * Constructor
			 */
			domSize(DAE& dae) : daeElement(dae), attrWidth() {}
			/**
			 * Destructor
			 */
			virtual ~domSize() {}
			/**
			 * Overloaded assignment operator
			 */
			virtual domSize &operator=( const domSize &cpy ) { (void)cpy; return *this; }

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

		class domArray;

		typedef daeSmartRef<domArray> domArrayRef;
		typedef daeTArray<domArrayRef> domArray_Array;

		class domArray : public daeElement
		{
		public:
			virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::ARRAY; }
			static daeInt ID() { return 81; }
			virtual daeInt typeID() const { return ID(); }
		protected:  // Attribute
/**
 * Zero is not an array
 */
			xsUnsignedInt attrLength;


		public:	//Accessors and Mutators
			/**
			 * Gets the length attribute.
			 * @return Returns a xsUnsignedInt of the length attribute.
			 */
			xsUnsignedInt getLength() const { return attrLength; }
			/**
			 * Sets the length attribute.
			 * @param atLength The new value for the length attribute.
			 */
			void setLength( xsUnsignedInt atLength ) { attrLength = atLength; _validAttributeArray[0] = true; }

		protected:
			/**
			 * Constructor
			 */
			domArray(DAE& dae) : daeElement(dae), attrLength() {}
			/**
			 * Destructor
			 */
			virtual ~domArray() {}
			/**
			 * Overloaded assignment operator
			 */
			virtual domArray &operator=( const domArray &cpy ) { (void)cpy; return *this; }

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

		class domFormat;

		typedef daeSmartRef<domFormat> domFormatRef;
		typedef daeTArray<domFormatRef> domFormat_Array;

/**
 * If not present it is assumed to be R8G8B8A8 linear.
 */
		class domFormat : public daeElement
		{
		public:
			virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FORMAT; }
			static daeInt ID() { return 82; }
			virtual daeInt typeID() const { return ID(); }
		public:
			class domHint;

			typedef daeSmartRef<domHint> domHintRef;
			typedef daeTArray<domHintRef> domHint_Array;

/**
 * if this element or a higher precidence element is not present then use
 * a common format R8G8B8A8 with linear color gradient, not  sRGB.
 */
			class domHint : public daeElement
			{
			public:
				virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::HINT; }
				static daeInt ID() { return 83; }
				virtual daeInt typeID() const { return ID(); }
			protected:  // Attributes
/**
 * The per-texel layout of the format.  The length of the string indicate
 * how many channels there are and the letter respresents the name of the
 * channel.  There are typically 0 to 4 channels.
 */
				domImage_format_hint_channels attrChannels;
/**
 * Each channel represents a range of values. Some example ranges are signed
 * or unsigned integers, or between between a clamped range such as 0.0f to
 * 1.0f, or high dynamic range via floating point
 */
				domImage_format_hint_range attrRange;
/**
 * Each channel of the texel has a precision.  Typically these are all linked
 * together.  An exact format lay lower the precision of an individual channel
 * but applying a higher precision by linking the channels together may still
 * convey the same information.
 */
				domImage_format_hint_precision attrPrecision;
/**
 * linear(default), sRGB 
 */
				xsToken attrSpace;


			public:	//Accessors and Mutators
				/**
				 * Gets the channels attribute.
				 * @return Returns a domImage_format_hint_channels of the channels attribute.
				 */
				domImage_format_hint_channels getChannels() const { return attrChannels; }
				/**
				 * Sets the channels attribute.
				 * @param atChannels The new value for the channels attribute.
				 */
				void setChannels( domImage_format_hint_channels atChannels ) { attrChannels = atChannels; _validAttributeArray[0] = true; }

				/**
				 * Gets the range attribute.
				 * @return Returns a domImage_format_hint_range of the range attribute.
				 */
				domImage_format_hint_range getRange() const { return attrRange; }
				/**
				 * Sets the range attribute.
				 * @param atRange The new value for the range attribute.
				 */
				void setRange( domImage_format_hint_range atRange ) { attrRange = atRange; _validAttributeArray[1] = true; }

				/**
				 * Gets the precision attribute.
				 * @return Returns a domImage_format_hint_precision of the precision attribute.
				 */
				domImage_format_hint_precision getPrecision() const { return attrPrecision; }
				/**
				 * Sets the precision attribute.
				 * @param atPrecision The new value for the precision attribute.
				 */
				void setPrecision( domImage_format_hint_precision atPrecision ) { attrPrecision = atPrecision; _validAttributeArray[2] = true; }

				/**
				 * Gets the space attribute.
				 * @return Returns a xsToken of the space attribute.
				 */
				xsToken getSpace() const { return attrSpace; }
				/**
				 * Sets the space attribute.
				 * @param atSpace The new value for the space attribute.
				 */
				void setSpace( xsToken atSpace ) { *(daeStringRef*)&attrSpace = atSpace; _validAttributeArray[3] = true; }

			protected:
				/**
				 * Constructor
				 */
				domHint(DAE& dae) : daeElement(dae), attrChannels(), attrRange(), attrPrecision(), attrSpace() {}
				/**
				 * Destructor
				 */
				virtual ~domHint() {}
				/**
				 * Overloaded assignment operator
				 */
				virtual domHint &operator=( const domHint &cpy ) { (void)cpy; return *this; }

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

			class domExact;

			typedef daeSmartRef<domExact> domExactRef;
			typedef daeTArray<domExactRef> domExact_Array;

/**
 * Contains a string representing the profile and platform specific texel
 * format that the author would like this surface to use.  If this element
 * is not specified then the application will fall back to the hint
 */
			class domExact : public daeElement
			{
			public:
				virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::EXACT; }
				static daeInt ID() { return 84; }
				virtual daeInt typeID() const { return ID(); }


			public:	//Accessors and Mutators
				/**
				 * Gets the value of this element.
				 * @return Returns a xsToken of the value.
				 */
				xsToken getValue() const { return _value; }
				/**
				 * Sets the _value of this element.
				 * @param val The new value for this element.
				 */
				void setValue( xsToken val ) { *(daeStringRef*)&_value = val; }

			protected:  // Value
				/**
				 * The xsToken value of the text data of this element. 
				 */
				xsToken _value;
			protected:
				/**
				 * Constructor
				 */
				domExact(DAE& dae) : daeElement(dae), _value() {}
				/**
				 * Destructor
				 */
				virtual ~domExact() {}
				/**
				 * Overloaded assignment operator
				 */
				virtual domExact &operator=( const domExact &cpy ) { (void)cpy; return *this; }

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
 * if this element or a higher precidence element is not present then use
 * a common format R8G8B8A8 with linear color gradient, not  sRGB. @see domHint
 */
			domHintRef elemHint;
/**
 * Contains a string representing the profile and platform specific texel
 * format that the author would like this surface to use.  If this element
 * is not specified then the application will fall back to the hint @see domExact
 */
			domExactRef elemExact;

		public:	//Accessors and Mutators
			/**
			 * Gets the hint element.
			 * @return a daeSmartRef to the hint element.
			 */
			const domHintRef getHint() const { return elemHint; }
			/**
			 * Gets the exact element.
			 * @return a daeSmartRef to the exact element.
			 */
			const domExactRef getExact() const { return elemExact; }
		protected:
			/**
			 * Constructor
			 */
			domFormat(DAE& dae) : daeElement(dae), elemHint(), elemExact() {}
			/**
			 * Destructor
			 */
			virtual ~domFormat() {}
			/**
			 * Overloaded assignment operator
			 */
			virtual domFormat &operator=( const domFormat &cpy ) { (void)cpy; return *this; }

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

		class domInit_from;

		typedef daeSmartRef<domInit_from> domInit_fromRef;
		typedef daeTArray<domInit_fromRef> domInit_from_Array;

/**
 * init the image data from one or more partial pieces of data.  The load
 * attribute indicates what portion of the image will be loaded based on the
 * ''address" specified.  mip_generate may be used to complete the image.
 */
		class domInit_from : public domImage_source
		{
		public:
			virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::INIT_FROM; }
			static daeInt ID() { return 85; }
			virtual daeInt typeID() const { return ID(); }
		protected:  // Attributes
			domImage_face attrFace;
			xsUnsignedInt attrMip_index;
			xsUnsignedInt attrArray_index;


		public:	//Accessors and Mutators
			/**
			 * Gets the face attribute.
			 * @return Returns a domImage_face of the face attribute.
			 */
			domImage_face getFace() const { return attrFace; }
			/**
			 * Sets the face attribute.
			 * @param atFace The new value for the face attribute.
			 */
			void setFace( domImage_face atFace ) { attrFace = atFace; _validAttributeArray[0] = true; }

			/**
			 * Gets the mip_index attribute.
			 * @return Returns a xsUnsignedInt of the mip_index attribute.
			 */
			xsUnsignedInt getMip_index() const { return attrMip_index; }
			/**
			 * Sets the mip_index attribute.
			 * @param atMip_index The new value for the mip_index attribute.
			 */
			void setMip_index( xsUnsignedInt atMip_index ) { attrMip_index = atMip_index; _validAttributeArray[1] = true; }

			/**
			 * Gets the array_index attribute.
			 * @return Returns a xsUnsignedInt of the array_index attribute.
			 */
			xsUnsignedInt getArray_index() const { return attrArray_index; }
			/**
			 * Sets the array_index attribute.
			 * @param atArray_index The new value for the array_index attribute.
			 */
			void setArray_index( xsUnsignedInt atArray_index ) { attrArray_index = atArray_index; _validAttributeArray[2] = true; }

		protected:
			/**
			 * Constructor
			 */
			domInit_from(DAE& dae) : domImage_source(dae), attrFace(), attrMip_index(), attrArray_index() {}
			/**
			 * Destructor
			 */
			virtual ~domInit_from() {}
			/**
			 * Overloaded assignment operator
			 */
			virtual domInit_from &operator=( const domInit_from &cpy ) { (void)cpy; return *this; }

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
 * The surface should be sized to these exact dimensions @see domSize
 */
		domSizeRef elemSize;
		domImage_mipsRef elemMips;
		domArrayRef elemArray;
/**
 * If not present it is assumed to be R8G8B8A8 linear. @see domFormat
 */
		domFormatRef elemFormat;
/**
 * init the image data from one or more partial pieces of data.  The load
 * attribute indicates what portion of the image will be loaded based on the
 * ''address" specified.  mip_generate may be used to complete the image.
 * @see domInit_from
 */
		domInit_from_Array elemInit_from_array;

	public:	//Accessors and Mutators
		/**
		 * Gets the size element.
		 * @return a daeSmartRef to the size element.
		 */
		const domSizeRef getSize() const { return elemSize; }
		/**
		 * Gets the mips element.
		 * @return a daeSmartRef to the mips element.
		 */
		const domImage_mipsRef getMips() const { return elemMips; }
		/**
		 * Gets the array element.
		 * @return a daeSmartRef to the array element.
		 */
		const domArrayRef getArray() const { return elemArray; }
		/**
		 * Gets the format element.
		 * @return a daeSmartRef to the format element.
		 */
		const domFormatRef getFormat() const { return elemFormat; }
		/**
		 * Gets the init_from element array.
		 * @return Returns a reference to the array of init_from elements.
		 */
		domInit_from_Array &getInit_from_array() { return elemInit_from_array; }
		/**
		 * Gets the init_from element array.
		 * @return Returns a constant reference to the array of init_from elements.
		 */
		const domInit_from_Array &getInit_from_array() const { return elemInit_from_array; }
	protected:
		/**
		 * Constructor
		 */
		domCreate_cube(DAE& dae) : daeElement(dae), elemSize(), elemMips(), elemArray(), elemFormat(), elemInit_from_array() {}
		/**
		 * Destructor
		 */
		virtual ~domCreate_cube() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domCreate_cube &operator=( const domCreate_cube &cpy ) { (void)cpy; return *this; }

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
 *  The id attribute is a text string containing the unique identifier of
 * this element. This value  must be unique within the instance document.
 * Optional attribute. 
 */
	xsID attrId;
/**
 * sub identifier
 */
	domSid attrSid;
/**
 *  The name attribute is the text string name of this element. Optional attribute.
 */
	xsToken attrName;

protected:  // Elements
/**
 * The image element may contain an asset element. @see domAsset
 */
	domAssetRef elemAsset;
/**
 * This image is renderable  if this element is used @see domRenderable
 */
	domRenderableRef elemRenderable;
/**
 * initialize the whole image structure and data from formats like DDS @see
 * domInit_from
 */
	domInit_fromRef elemInit_from;
/**
 * Initialize a custom 2d image @see domCreate_2d
 */
	domCreate_2dRef elemCreate_2d;
/**
 * Initialize a custom 3d image @see domCreate_3d
 */
	domCreate_3dRef elemCreate_3d;
/**
 * Initialize a custom cube image @see domCreate_cube
 */
	domCreate_cubeRef elemCreate_cube;
/**
 * The extra element may appear any number of times. @see domExtra
 */
	domExtra_Array elemExtra_array;
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
	 * Gets the id attribute.
	 * @return Returns a xsID of the id attribute.
	 */
	xsID getId() const { return attrId; }
	/**
	 * Sets the id attribute.
	 * @param atId The new value for the id attribute.
	 */
	void setId( xsID atId ) { *(daeStringRef*)&attrId = atId;
		if( _document != NULL ) _document->changeElementID( this, attrId );
	}

	/**
	 * Gets the sid attribute.
	 * @return Returns a domSid of the sid attribute.
	 */
	domSid getSid() const { return attrSid; }
	/**
	 * Sets the sid attribute.
	 * @param atSid The new value for the sid attribute.
	 */
	void setSid( domSid atSid ) { *(daeStringRef*)&attrSid = atSid;}

	/**
	 * Gets the name attribute.
	 * @return Returns a xsToken of the name attribute.
	 */
	xsToken getName() const { return attrName; }
	/**
	 * Sets the name attribute.
	 * @param atName The new value for the name attribute.
	 */
	void setName( xsToken atName ) { *(daeStringRef*)&attrName = atName;}

	/**
	 * Gets the asset element.
	 * @return a daeSmartRef to the asset element.
	 */
	const domAssetRef getAsset() const { return elemAsset; }
	/**
	 * Gets the renderable element.
	 * @return a daeSmartRef to the renderable element.
	 */
	const domRenderableRef getRenderable() const { return elemRenderable; }
	/**
	 * Gets the init_from element.
	 * @return a daeSmartRef to the init_from element.
	 */
	const domInit_fromRef getInit_from() const { return elemInit_from; }
	/**
	 * Gets the create_2d element.
	 * @return a daeSmartRef to the create_2d element.
	 */
	const domCreate_2dRef getCreate_2d() const { return elemCreate_2d; }
	/**
	 * Gets the create_3d element.
	 * @return a daeSmartRef to the create_3d element.
	 */
	const domCreate_3dRef getCreate_3d() const { return elemCreate_3d; }
	/**
	 * Gets the create_cube element.
	 * @return a daeSmartRef to the create_cube element.
	 */
	const domCreate_cubeRef getCreate_cube() const { return elemCreate_cube; }
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
	domImage(DAE& dae) : daeElement(dae), attrId(), attrSid(), attrName(), elemAsset(), elemRenderable(), elemInit_from(), elemCreate_2d(), elemCreate_3d(), elemCreate_cube(), elemExtra_array() {}
	/**
	 * Destructor
	 */
	virtual ~domImage() { daeElement::deleteCMDataArray(_CMData); }
	/**
	 * Overloaded assignment operator
	 */
	virtual domImage &operator=( const domImage &cpy ) { (void)cpy; return *this; }

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
