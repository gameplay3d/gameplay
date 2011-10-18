#ifndef __domGles2_pipeline_settings_h__
#define __domGles2_pipeline_settings_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

class DAE;

/**
 * All of the pass state settings for OpenGL ES 2 API
 */
class domGles2_pipeline_settings : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::GLES2_PIPELINE_SETTINGS; }
	static daeInt ID() { return 668; }
	virtual daeInt typeID() const { return ID(); }
public:
	class domBlend_color;

	typedef daeSmartRef<domBlend_color> domBlend_colorRef;
	typedef daeTArray<domBlend_colorRef> domBlend_color_Array;

	class domBlend_color : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::BLEND_COLOR; }
		static daeInt ID() { return 669; }
		virtual daeInt typeID() const { return ID(); }
	protected:  // Attributes
		domFloat4 attrValue;
		xsToken attrParam;


	public:	//Accessors and Mutators
		/**
		 * Gets the value attribute.
		 * @return Returns a domFloat4 of the value attribute.
		 */
		domFloat4 getValue() const { return attrValue; }
		/**
		 * Sets the value attribute.
		 * @param atValue The new value for the value attribute.
		 */
		void setValue( domFloat4 atValue ) { attrValue = atValue; _validAttributeArray[0] = true; }

		/**
		 * Gets the param attribute.
		 * @return Returns a xsToken of the param attribute.
		 */
		xsToken getParam() const { return attrParam; }
		/**
		 * Sets the param attribute.
		 * @param atParam The new value for the param attribute.
		 */
		void setParam( xsToken atParam ) { *(daeStringRef*)&attrParam = atParam; _validAttributeArray[1] = true; }

	protected:
		/**
		 * Constructor
		 */
		domBlend_color(DAE& dae) : daeElement(dae), attrValue(), attrParam() {}
		/**
		 * Destructor
		 */
		virtual ~domBlend_color() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domBlend_color &operator=( const domBlend_color &cpy ) { (void)cpy; return *this; }

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

	class domBlend_equation;

	typedef daeSmartRef<domBlend_equation> domBlend_equationRef;
	typedef daeTArray<domBlend_equationRef> domBlend_equation_Array;

	class domBlend_equation : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::BLEND_EQUATION; }
		static daeInt ID() { return 670; }
		virtual daeInt typeID() const { return ID(); }
	protected:  // Attributes
		domGl_blend_equation attrValue;
		xsToken attrParam;


	public:	//Accessors and Mutators
		/**
		 * Gets the value attribute.
		 * @return Returns a domGl_blend_equation of the value attribute.
		 */
		domGl_blend_equation getValue() const { return attrValue; }
		/**
		 * Sets the value attribute.
		 * @param atValue The new value for the value attribute.
		 */
		void setValue( domGl_blend_equation atValue ) { attrValue = atValue; _validAttributeArray[0] = true; }

		/**
		 * Gets the param attribute.
		 * @return Returns a xsToken of the param attribute.
		 */
		xsToken getParam() const { return attrParam; }
		/**
		 * Sets the param attribute.
		 * @param atParam The new value for the param attribute.
		 */
		void setParam( xsToken atParam ) { *(daeStringRef*)&attrParam = atParam; _validAttributeArray[1] = true; }

	protected:
		/**
		 * Constructor
		 */
		domBlend_equation(DAE& dae) : daeElement(dae), attrValue(), attrParam() {}
		/**
		 * Destructor
		 */
		virtual ~domBlend_equation() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domBlend_equation &operator=( const domBlend_equation &cpy ) { (void)cpy; return *this; }

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

	class domBlend_equation_separate;

	typedef daeSmartRef<domBlend_equation_separate> domBlend_equation_separateRef;
	typedef daeTArray<domBlend_equation_separateRef> domBlend_equation_separate_Array;

	class domBlend_equation_separate : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::BLEND_EQUATION_SEPARATE; }
		static daeInt ID() { return 671; }
		virtual daeInt typeID() const { return ID(); }
	public:
		class domRgb;

		typedef daeSmartRef<domRgb> domRgbRef;
		typedef daeTArray<domRgbRef> domRgb_Array;

		class domRgb : public daeElement
		{
		public:
			virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::RGB; }
			static daeInt ID() { return 672; }
			virtual daeInt typeID() const { return ID(); }
		protected:  // Attributes
			domGl_blend_equation attrValue;
			xsToken attrParam;


		public:	//Accessors and Mutators
			/**
			 * Gets the value attribute.
			 * @return Returns a domGl_blend_equation of the value attribute.
			 */
			domGl_blend_equation getValue() const { return attrValue; }
			/**
			 * Sets the value attribute.
			 * @param atValue The new value for the value attribute.
			 */
			void setValue( domGl_blend_equation atValue ) { attrValue = atValue; _validAttributeArray[0] = true; }

			/**
			 * Gets the param attribute.
			 * @return Returns a xsToken of the param attribute.
			 */
			xsToken getParam() const { return attrParam; }
			/**
			 * Sets the param attribute.
			 * @param atParam The new value for the param attribute.
			 */
			void setParam( xsToken atParam ) { *(daeStringRef*)&attrParam = atParam; _validAttributeArray[1] = true; }

		protected:
			/**
			 * Constructor
			 */
			domRgb(DAE& dae) : daeElement(dae), attrValue(), attrParam() {}
			/**
			 * Destructor
			 */
			virtual ~domRgb() {}
			/**
			 * Overloaded assignment operator
			 */
			virtual domRgb &operator=( const domRgb &cpy ) { (void)cpy; return *this; }

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

		class domAlpha;

		typedef daeSmartRef<domAlpha> domAlphaRef;
		typedef daeTArray<domAlphaRef> domAlpha_Array;

		class domAlpha : public daeElement
		{
		public:
			virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::ALPHA; }
			static daeInt ID() { return 673; }
			virtual daeInt typeID() const { return ID(); }
		protected:  // Attributes
			domGl_blend_equation attrValue;
			xsToken attrParam;


		public:	//Accessors and Mutators
			/**
			 * Gets the value attribute.
			 * @return Returns a domGl_blend_equation of the value attribute.
			 */
			domGl_blend_equation getValue() const { return attrValue; }
			/**
			 * Sets the value attribute.
			 * @param atValue The new value for the value attribute.
			 */
			void setValue( domGl_blend_equation atValue ) { attrValue = atValue; _validAttributeArray[0] = true; }

			/**
			 * Gets the param attribute.
			 * @return Returns a xsToken of the param attribute.
			 */
			xsToken getParam() const { return attrParam; }
			/**
			 * Sets the param attribute.
			 * @param atParam The new value for the param attribute.
			 */
			void setParam( xsToken atParam ) { *(daeStringRef*)&attrParam = atParam; _validAttributeArray[1] = true; }

		protected:
			/**
			 * Constructor
			 */
			domAlpha(DAE& dae) : daeElement(dae), attrValue(), attrParam() {}
			/**
			 * Destructor
			 */
			virtual ~domAlpha() {}
			/**
			 * Overloaded assignment operator
			 */
			virtual domAlpha &operator=( const domAlpha &cpy ) { (void)cpy; return *this; }

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
		domRgbRef elemRgb;
		domAlphaRef elemAlpha;

	public:	//Accessors and Mutators
		/**
		 * Gets the rgb element.
		 * @return a daeSmartRef to the rgb element.
		 */
		const domRgbRef getRgb() const { return elemRgb; }
		/**
		 * Gets the alpha element.
		 * @return a daeSmartRef to the alpha element.
		 */
		const domAlphaRef getAlpha() const { return elemAlpha; }
	protected:
		/**
		 * Constructor
		 */
		domBlend_equation_separate(DAE& dae) : daeElement(dae), elemRgb(), elemAlpha() {}
		/**
		 * Destructor
		 */
		virtual ~domBlend_equation_separate() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domBlend_equation_separate &operator=( const domBlend_equation_separate &cpy ) { (void)cpy; return *this; }

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

	class domBlend_func;

	typedef daeSmartRef<domBlend_func> domBlend_funcRef;
	typedef daeTArray<domBlend_funcRef> domBlend_func_Array;

	class domBlend_func : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::BLEND_FUNC; }
		static daeInt ID() { return 674; }
		virtual daeInt typeID() const { return ID(); }
	public:
		class domSrc;

		typedef daeSmartRef<domSrc> domSrcRef;
		typedef daeTArray<domSrcRef> domSrc_Array;

		class domSrc : public daeElement
		{
		public:
			virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::SRC; }
			static daeInt ID() { return 675; }
			virtual daeInt typeID() const { return ID(); }
		protected:  // Attributes
			domGl_blend attrValue;
			xsToken attrParam;


		public:	//Accessors and Mutators
			/**
			 * Gets the value attribute.
			 * @return Returns a domGl_blend of the value attribute.
			 */
			domGl_blend getValue() const { return attrValue; }
			/**
			 * Sets the value attribute.
			 * @param atValue The new value for the value attribute.
			 */
			void setValue( domGl_blend atValue ) { attrValue = atValue; _validAttributeArray[0] = true; }

			/**
			 * Gets the param attribute.
			 * @return Returns a xsToken of the param attribute.
			 */
			xsToken getParam() const { return attrParam; }
			/**
			 * Sets the param attribute.
			 * @param atParam The new value for the param attribute.
			 */
			void setParam( xsToken atParam ) { *(daeStringRef*)&attrParam = atParam; _validAttributeArray[1] = true; }

		protected:
			/**
			 * Constructor
			 */
			domSrc(DAE& dae) : daeElement(dae), attrValue(), attrParam() {}
			/**
			 * Destructor
			 */
			virtual ~domSrc() {}
			/**
			 * Overloaded assignment operator
			 */
			virtual domSrc &operator=( const domSrc &cpy ) { (void)cpy; return *this; }

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

		class domDest;

		typedef daeSmartRef<domDest> domDestRef;
		typedef daeTArray<domDestRef> domDest_Array;

		class domDest : public daeElement
		{
		public:
			virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::DEST; }
			static daeInt ID() { return 676; }
			virtual daeInt typeID() const { return ID(); }
		protected:  // Attributes
			domGl_blend attrValue;
			xsToken attrParam;


		public:	//Accessors and Mutators
			/**
			 * Gets the value attribute.
			 * @return Returns a domGl_blend of the value attribute.
			 */
			domGl_blend getValue() const { return attrValue; }
			/**
			 * Sets the value attribute.
			 * @param atValue The new value for the value attribute.
			 */
			void setValue( domGl_blend atValue ) { attrValue = atValue; _validAttributeArray[0] = true; }

			/**
			 * Gets the param attribute.
			 * @return Returns a xsToken of the param attribute.
			 */
			xsToken getParam() const { return attrParam; }
			/**
			 * Sets the param attribute.
			 * @param atParam The new value for the param attribute.
			 */
			void setParam( xsToken atParam ) { *(daeStringRef*)&attrParam = atParam; _validAttributeArray[1] = true; }

		protected:
			/**
			 * Constructor
			 */
			domDest(DAE& dae) : daeElement(dae), attrValue(), attrParam() {}
			/**
			 * Destructor
			 */
			virtual ~domDest() {}
			/**
			 * Overloaded assignment operator
			 */
			virtual domDest &operator=( const domDest &cpy ) { (void)cpy; return *this; }

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
		domSrcRef elemSrc;
		domDestRef elemDest;

	public:	//Accessors and Mutators
		/**
		 * Gets the src element.
		 * @return a daeSmartRef to the src element.
		 */
		const domSrcRef getSrc() const { return elemSrc; }
		/**
		 * Gets the dest element.
		 * @return a daeSmartRef to the dest element.
		 */
		const domDestRef getDest() const { return elemDest; }
	protected:
		/**
		 * Constructor
		 */
		domBlend_func(DAE& dae) : daeElement(dae), elemSrc(), elemDest() {}
		/**
		 * Destructor
		 */
		virtual ~domBlend_func() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domBlend_func &operator=( const domBlend_func &cpy ) { (void)cpy; return *this; }

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

	class domBlend_func_separate;

	typedef daeSmartRef<domBlend_func_separate> domBlend_func_separateRef;
	typedef daeTArray<domBlend_func_separateRef> domBlend_func_separate_Array;

	class domBlend_func_separate : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::BLEND_FUNC_SEPARATE; }
		static daeInt ID() { return 677; }
		virtual daeInt typeID() const { return ID(); }
	public:
		class domSrc_rgb;

		typedef daeSmartRef<domSrc_rgb> domSrc_rgbRef;
		typedef daeTArray<domSrc_rgbRef> domSrc_rgb_Array;

		class domSrc_rgb : public daeElement
		{
		public:
			virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::SRC_RGB; }
			static daeInt ID() { return 678; }
			virtual daeInt typeID() const { return ID(); }
		protected:  // Attributes
			domGl_blend attrValue;
			xsToken attrParam;


		public:	//Accessors and Mutators
			/**
			 * Gets the value attribute.
			 * @return Returns a domGl_blend of the value attribute.
			 */
			domGl_blend getValue() const { return attrValue; }
			/**
			 * Sets the value attribute.
			 * @param atValue The new value for the value attribute.
			 */
			void setValue( domGl_blend atValue ) { attrValue = atValue; _validAttributeArray[0] = true; }

			/**
			 * Gets the param attribute.
			 * @return Returns a xsToken of the param attribute.
			 */
			xsToken getParam() const { return attrParam; }
			/**
			 * Sets the param attribute.
			 * @param atParam The new value for the param attribute.
			 */
			void setParam( xsToken atParam ) { *(daeStringRef*)&attrParam = atParam; _validAttributeArray[1] = true; }

		protected:
			/**
			 * Constructor
			 */
			domSrc_rgb(DAE& dae) : daeElement(dae), attrValue(), attrParam() {}
			/**
			 * Destructor
			 */
			virtual ~domSrc_rgb() {}
			/**
			 * Overloaded assignment operator
			 */
			virtual domSrc_rgb &operator=( const domSrc_rgb &cpy ) { (void)cpy; return *this; }

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

		class domDest_rgb;

		typedef daeSmartRef<domDest_rgb> domDest_rgbRef;
		typedef daeTArray<domDest_rgbRef> domDest_rgb_Array;

		class domDest_rgb : public daeElement
		{
		public:
			virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::DEST_RGB; }
			static daeInt ID() { return 679; }
			virtual daeInt typeID() const { return ID(); }
		protected:  // Attributes
			domGl_blend attrValue;
			xsToken attrParam;


		public:	//Accessors and Mutators
			/**
			 * Gets the value attribute.
			 * @return Returns a domGl_blend of the value attribute.
			 */
			domGl_blend getValue() const { return attrValue; }
			/**
			 * Sets the value attribute.
			 * @param atValue The new value for the value attribute.
			 */
			void setValue( domGl_blend atValue ) { attrValue = atValue; _validAttributeArray[0] = true; }

			/**
			 * Gets the param attribute.
			 * @return Returns a xsToken of the param attribute.
			 */
			xsToken getParam() const { return attrParam; }
			/**
			 * Sets the param attribute.
			 * @param atParam The new value for the param attribute.
			 */
			void setParam( xsToken atParam ) { *(daeStringRef*)&attrParam = atParam; _validAttributeArray[1] = true; }

		protected:
			/**
			 * Constructor
			 */
			domDest_rgb(DAE& dae) : daeElement(dae), attrValue(), attrParam() {}
			/**
			 * Destructor
			 */
			virtual ~domDest_rgb() {}
			/**
			 * Overloaded assignment operator
			 */
			virtual domDest_rgb &operator=( const domDest_rgb &cpy ) { (void)cpy; return *this; }

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

		class domSrc_alpha;

		typedef daeSmartRef<domSrc_alpha> domSrc_alphaRef;
		typedef daeTArray<domSrc_alphaRef> domSrc_alpha_Array;

		class domSrc_alpha : public daeElement
		{
		public:
			virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::SRC_ALPHA; }
			static daeInt ID() { return 680; }
			virtual daeInt typeID() const { return ID(); }
		protected:  // Attributes
			domGl_blend attrValue;
			xsToken attrParam;


		public:	//Accessors and Mutators
			/**
			 * Gets the value attribute.
			 * @return Returns a domGl_blend of the value attribute.
			 */
			domGl_blend getValue() const { return attrValue; }
			/**
			 * Sets the value attribute.
			 * @param atValue The new value for the value attribute.
			 */
			void setValue( domGl_blend atValue ) { attrValue = atValue; _validAttributeArray[0] = true; }

			/**
			 * Gets the param attribute.
			 * @return Returns a xsToken of the param attribute.
			 */
			xsToken getParam() const { return attrParam; }
			/**
			 * Sets the param attribute.
			 * @param atParam The new value for the param attribute.
			 */
			void setParam( xsToken atParam ) { *(daeStringRef*)&attrParam = atParam; _validAttributeArray[1] = true; }

		protected:
			/**
			 * Constructor
			 */
			domSrc_alpha(DAE& dae) : daeElement(dae), attrValue(), attrParam() {}
			/**
			 * Destructor
			 */
			virtual ~domSrc_alpha() {}
			/**
			 * Overloaded assignment operator
			 */
			virtual domSrc_alpha &operator=( const domSrc_alpha &cpy ) { (void)cpy; return *this; }

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

		class domDest_alpha;

		typedef daeSmartRef<domDest_alpha> domDest_alphaRef;
		typedef daeTArray<domDest_alphaRef> domDest_alpha_Array;

		class domDest_alpha : public daeElement
		{
		public:
			virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::DEST_ALPHA; }
			static daeInt ID() { return 681; }
			virtual daeInt typeID() const { return ID(); }
		protected:  // Attributes
			domGl_blend attrValue;
			xsToken attrParam;


		public:	//Accessors and Mutators
			/**
			 * Gets the value attribute.
			 * @return Returns a domGl_blend of the value attribute.
			 */
			domGl_blend getValue() const { return attrValue; }
			/**
			 * Sets the value attribute.
			 * @param atValue The new value for the value attribute.
			 */
			void setValue( domGl_blend atValue ) { attrValue = atValue; _validAttributeArray[0] = true; }

			/**
			 * Gets the param attribute.
			 * @return Returns a xsToken of the param attribute.
			 */
			xsToken getParam() const { return attrParam; }
			/**
			 * Sets the param attribute.
			 * @param atParam The new value for the param attribute.
			 */
			void setParam( xsToken atParam ) { *(daeStringRef*)&attrParam = atParam; _validAttributeArray[1] = true; }

		protected:
			/**
			 * Constructor
			 */
			domDest_alpha(DAE& dae) : daeElement(dae), attrValue(), attrParam() {}
			/**
			 * Destructor
			 */
			virtual ~domDest_alpha() {}
			/**
			 * Overloaded assignment operator
			 */
			virtual domDest_alpha &operator=( const domDest_alpha &cpy ) { (void)cpy; return *this; }

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
		domSrc_rgbRef elemSrc_rgb;
		domDest_rgbRef elemDest_rgb;
		domSrc_alphaRef elemSrc_alpha;
		domDest_alphaRef elemDest_alpha;

	public:	//Accessors and Mutators
		/**
		 * Gets the src_rgb element.
		 * @return a daeSmartRef to the src_rgb element.
		 */
		const domSrc_rgbRef getSrc_rgb() const { return elemSrc_rgb; }
		/**
		 * Gets the dest_rgb element.
		 * @return a daeSmartRef to the dest_rgb element.
		 */
		const domDest_rgbRef getDest_rgb() const { return elemDest_rgb; }
		/**
		 * Gets the src_alpha element.
		 * @return a daeSmartRef to the src_alpha element.
		 */
		const domSrc_alphaRef getSrc_alpha() const { return elemSrc_alpha; }
		/**
		 * Gets the dest_alpha element.
		 * @return a daeSmartRef to the dest_alpha element.
		 */
		const domDest_alphaRef getDest_alpha() const { return elemDest_alpha; }
	protected:
		/**
		 * Constructor
		 */
		domBlend_func_separate(DAE& dae) : daeElement(dae), elemSrc_rgb(), elemDest_rgb(), elemSrc_alpha(), elemDest_alpha() {}
		/**
		 * Destructor
		 */
		virtual ~domBlend_func_separate() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domBlend_func_separate &operator=( const domBlend_func_separate &cpy ) { (void)cpy; return *this; }

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

	class domColor_mask;

	typedef daeSmartRef<domColor_mask> domColor_maskRef;
	typedef daeTArray<domColor_maskRef> domColor_mask_Array;

	class domColor_mask : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::COLOR_MASK; }
		static daeInt ID() { return 682; }
		virtual daeInt typeID() const { return ID(); }
	protected:  // Attributes
		domBool4 attrValue;
		xsToken attrParam;


	public:	//Accessors and Mutators
		/**
		 * Gets the value attribute.
		 * @return Returns a domBool4 of the value attribute.
		 */
		domBool4 getValue() const { return attrValue; }
		/**
		 * Sets the value attribute.
		 * @param atValue The new value for the value attribute.
		 */
		void setValue( domBool4 atValue ) { attrValue = atValue; _validAttributeArray[0] = true; }

		/**
		 * Gets the param attribute.
		 * @return Returns a xsToken of the param attribute.
		 */
		xsToken getParam() const { return attrParam; }
		/**
		 * Sets the param attribute.
		 * @param atParam The new value for the param attribute.
		 */
		void setParam( xsToken atParam ) { *(daeStringRef*)&attrParam = atParam; _validAttributeArray[1] = true; }

	protected:
		/**
		 * Constructor
		 */
		domColor_mask(DAE& dae) : daeElement(dae), attrValue(), attrParam() {}
		/**
		 * Destructor
		 */
		virtual ~domColor_mask() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domColor_mask &operator=( const domColor_mask &cpy ) { (void)cpy; return *this; }

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

	class domCull_face;

	typedef daeSmartRef<domCull_face> domCull_faceRef;
	typedef daeTArray<domCull_faceRef> domCull_face_Array;

	class domCull_face : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::CULL_FACE; }
		static daeInt ID() { return 683; }
		virtual daeInt typeID() const { return ID(); }
	protected:  // Attributes
		domGl_face attrValue;
		xsToken attrParam;


	public:	//Accessors and Mutators
		/**
		 * Gets the value attribute.
		 * @return Returns a domGl_face of the value attribute.
		 */
		domGl_face getValue() const { return attrValue; }
		/**
		 * Sets the value attribute.
		 * @param atValue The new value for the value attribute.
		 */
		void setValue( domGl_face atValue ) { attrValue = atValue; _validAttributeArray[0] = true; }

		/**
		 * Gets the param attribute.
		 * @return Returns a xsToken of the param attribute.
		 */
		xsToken getParam() const { return attrParam; }
		/**
		 * Sets the param attribute.
		 * @param atParam The new value for the param attribute.
		 */
		void setParam( xsToken atParam ) { *(daeStringRef*)&attrParam = atParam; _validAttributeArray[1] = true; }

	protected:
		/**
		 * Constructor
		 */
		domCull_face(DAE& dae) : daeElement(dae), attrValue(), attrParam() {}
		/**
		 * Destructor
		 */
		virtual ~domCull_face() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domCull_face &operator=( const domCull_face &cpy ) { (void)cpy; return *this; }

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

	class domDepth_func;

	typedef daeSmartRef<domDepth_func> domDepth_funcRef;
	typedef daeTArray<domDepth_funcRef> domDepth_func_Array;

	class domDepth_func : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::DEPTH_FUNC; }
		static daeInt ID() { return 684; }
		virtual daeInt typeID() const { return ID(); }
	protected:  // Attributes
		domGl_func attrValue;
		xsToken attrParam;


	public:	//Accessors and Mutators
		/**
		 * Gets the value attribute.
		 * @return Returns a domGl_func of the value attribute.
		 */
		domGl_func getValue() const { return attrValue; }
		/**
		 * Sets the value attribute.
		 * @param atValue The new value for the value attribute.
		 */
		void setValue( domGl_func atValue ) { attrValue = atValue; _validAttributeArray[0] = true; }

		/**
		 * Gets the param attribute.
		 * @return Returns a xsToken of the param attribute.
		 */
		xsToken getParam() const { return attrParam; }
		/**
		 * Sets the param attribute.
		 * @param atParam The new value for the param attribute.
		 */
		void setParam( xsToken atParam ) { *(daeStringRef*)&attrParam = atParam; _validAttributeArray[1] = true; }

	protected:
		/**
		 * Constructor
		 */
		domDepth_func(DAE& dae) : daeElement(dae), attrValue(), attrParam() {}
		/**
		 * Destructor
		 */
		virtual ~domDepth_func() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domDepth_func &operator=( const domDepth_func &cpy ) { (void)cpy; return *this; }

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

	class domDepth_mask;

	typedef daeSmartRef<domDepth_mask> domDepth_maskRef;
	typedef daeTArray<domDepth_maskRef> domDepth_mask_Array;

	class domDepth_mask : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::DEPTH_MASK; }
		static daeInt ID() { return 685; }
		virtual daeInt typeID() const { return ID(); }
	protected:  // Attributes
		xsBoolean attrValue;
		xsToken attrParam;


	public:	//Accessors and Mutators
		/**
		 * Gets the value attribute.
		 * @return Returns a xsBoolean of the value attribute.
		 */
		xsBoolean getValue() const { return attrValue; }
		/**
		 * Sets the value attribute.
		 * @param atValue The new value for the value attribute.
		 */
		void setValue( xsBoolean atValue ) { attrValue = atValue; _validAttributeArray[0] = true; }

		/**
		 * Gets the param attribute.
		 * @return Returns a xsToken of the param attribute.
		 */
		xsToken getParam() const { return attrParam; }
		/**
		 * Sets the param attribute.
		 * @param atParam The new value for the param attribute.
		 */
		void setParam( xsToken atParam ) { *(daeStringRef*)&attrParam = atParam; _validAttributeArray[1] = true; }

	protected:
		/**
		 * Constructor
		 */
		domDepth_mask(DAE& dae) : daeElement(dae), attrValue(), attrParam() {}
		/**
		 * Destructor
		 */
		virtual ~domDepth_mask() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domDepth_mask &operator=( const domDepth_mask &cpy ) { (void)cpy; return *this; }

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

	class domDepth_range;

	typedef daeSmartRef<domDepth_range> domDepth_rangeRef;
	typedef daeTArray<domDepth_rangeRef> domDepth_range_Array;

	class domDepth_range : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::DEPTH_RANGE; }
		static daeInt ID() { return 686; }
		virtual daeInt typeID() const { return ID(); }
	protected:  // Attributes
		domFloat2 attrValue;
		xsToken attrParam;


	public:	//Accessors and Mutators
		/**
		 * Gets the value attribute.
		 * @return Returns a domFloat2 of the value attribute.
		 */
		domFloat2 getValue() const { return attrValue; }
		/**
		 * Sets the value attribute.
		 * @param atValue The new value for the value attribute.
		 */
		void setValue( domFloat2 atValue ) { attrValue = atValue; _validAttributeArray[0] = true; }

		/**
		 * Gets the param attribute.
		 * @return Returns a xsToken of the param attribute.
		 */
		xsToken getParam() const { return attrParam; }
		/**
		 * Sets the param attribute.
		 * @param atParam The new value for the param attribute.
		 */
		void setParam( xsToken atParam ) { *(daeStringRef*)&attrParam = atParam; _validAttributeArray[1] = true; }

	protected:
		/**
		 * Constructor
		 */
		domDepth_range(DAE& dae) : daeElement(dae), attrValue(), attrParam() {}
		/**
		 * Destructor
		 */
		virtual ~domDepth_range() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domDepth_range &operator=( const domDepth_range &cpy ) { (void)cpy; return *this; }

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

	class domFront_face;

	typedef daeSmartRef<domFront_face> domFront_faceRef;
	typedef daeTArray<domFront_faceRef> domFront_face_Array;

	class domFront_face : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FRONT_FACE; }
		static daeInt ID() { return 687; }
		virtual daeInt typeID() const { return ID(); }
	protected:  // Attributes
		domGl_front_face attrValue;
		xsToken attrParam;


	public:	//Accessors and Mutators
		/**
		 * Gets the value attribute.
		 * @return Returns a domGl_front_face of the value attribute.
		 */
		domGl_front_face getValue() const { return attrValue; }
		/**
		 * Sets the value attribute.
		 * @param atValue The new value for the value attribute.
		 */
		void setValue( domGl_front_face atValue ) { attrValue = atValue; _validAttributeArray[0] = true; }

		/**
		 * Gets the param attribute.
		 * @return Returns a xsToken of the param attribute.
		 */
		xsToken getParam() const { return attrParam; }
		/**
		 * Sets the param attribute.
		 * @param atParam The new value for the param attribute.
		 */
		void setParam( xsToken atParam ) { *(daeStringRef*)&attrParam = atParam; _validAttributeArray[1] = true; }

	protected:
		/**
		 * Constructor
		 */
		domFront_face(DAE& dae) : daeElement(dae), attrValue(), attrParam() {}
		/**
		 * Destructor
		 */
		virtual ~domFront_face() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domFront_face &operator=( const domFront_face &cpy ) { (void)cpy; return *this; }

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

	class domLine_width;

	typedef daeSmartRef<domLine_width> domLine_widthRef;
	typedef daeTArray<domLine_widthRef> domLine_width_Array;

	class domLine_width : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::LINE_WIDTH; }
		static daeInt ID() { return 688; }
		virtual daeInt typeID() const { return ID(); }
	protected:  // Attributes
		domFloat attrValue;
		xsToken attrParam;


	public:	//Accessors and Mutators
		/**
		 * Gets the value attribute.
		 * @return Returns a domFloat of the value attribute.
		 */
		domFloat getValue() const { return attrValue; }
		/**
		 * Sets the value attribute.
		 * @param atValue The new value for the value attribute.
		 */
		void setValue( domFloat atValue ) { attrValue = atValue; _validAttributeArray[0] = true; }

		/**
		 * Gets the param attribute.
		 * @return Returns a xsToken of the param attribute.
		 */
		xsToken getParam() const { return attrParam; }
		/**
		 * Sets the param attribute.
		 * @param atParam The new value for the param attribute.
		 */
		void setParam( xsToken atParam ) { *(daeStringRef*)&attrParam = atParam; _validAttributeArray[1] = true; }

	protected:
		/**
		 * Constructor
		 */
		domLine_width(DAE& dae) : daeElement(dae), attrValue(), attrParam() {}
		/**
		 * Destructor
		 */
		virtual ~domLine_width() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domLine_width &operator=( const domLine_width &cpy ) { (void)cpy; return *this; }

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

	class domPolygon_offset;

	typedef daeSmartRef<domPolygon_offset> domPolygon_offsetRef;
	typedef daeTArray<domPolygon_offsetRef> domPolygon_offset_Array;

	class domPolygon_offset : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::POLYGON_OFFSET; }
		static daeInt ID() { return 689; }
		virtual daeInt typeID() const { return ID(); }
	protected:  // Attributes
		domFloat2 attrValue;
		xsToken attrParam;


	public:	//Accessors and Mutators
		/**
		 * Gets the value attribute.
		 * @return Returns a domFloat2 of the value attribute.
		 */
		domFloat2 getValue() const { return attrValue; }
		/**
		 * Sets the value attribute.
		 * @param atValue The new value for the value attribute.
		 */
		void setValue( domFloat2 atValue ) { attrValue = atValue; _validAttributeArray[0] = true; }

		/**
		 * Gets the param attribute.
		 * @return Returns a xsToken of the param attribute.
		 */
		xsToken getParam() const { return attrParam; }
		/**
		 * Sets the param attribute.
		 * @param atParam The new value for the param attribute.
		 */
		void setParam( xsToken atParam ) { *(daeStringRef*)&attrParam = atParam; _validAttributeArray[1] = true; }

	protected:
		/**
		 * Constructor
		 */
		domPolygon_offset(DAE& dae) : daeElement(dae), attrValue(), attrParam() {}
		/**
		 * Destructor
		 */
		virtual ~domPolygon_offset() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domPolygon_offset &operator=( const domPolygon_offset &cpy ) { (void)cpy; return *this; }

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

	class domPoint_size;

	typedef daeSmartRef<domPoint_size> domPoint_sizeRef;
	typedef daeTArray<domPoint_sizeRef> domPoint_size_Array;

	class domPoint_size : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::POINT_SIZE; }
		static daeInt ID() { return 690; }
		virtual daeInt typeID() const { return ID(); }
	protected:  // Attributes
		domFloat attrValue;
		xsToken attrParam;


	public:	//Accessors and Mutators
		/**
		 * Gets the value attribute.
		 * @return Returns a domFloat of the value attribute.
		 */
		domFloat getValue() const { return attrValue; }
		/**
		 * Sets the value attribute.
		 * @param atValue The new value for the value attribute.
		 */
		void setValue( domFloat atValue ) { attrValue = atValue; _validAttributeArray[0] = true; }

		/**
		 * Gets the param attribute.
		 * @return Returns a xsToken of the param attribute.
		 */
		xsToken getParam() const { return attrParam; }
		/**
		 * Sets the param attribute.
		 * @param atParam The new value for the param attribute.
		 */
		void setParam( xsToken atParam ) { *(daeStringRef*)&attrParam = atParam; _validAttributeArray[1] = true; }

	protected:
		/**
		 * Constructor
		 */
		domPoint_size(DAE& dae) : daeElement(dae), attrValue(), attrParam() {}
		/**
		 * Destructor
		 */
		virtual ~domPoint_size() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domPoint_size &operator=( const domPoint_size &cpy ) { (void)cpy; return *this; }

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

	class domSample_coverage;

	typedef daeSmartRef<domSample_coverage> domSample_coverageRef;
	typedef daeTArray<domSample_coverageRef> domSample_coverage_Array;

	class domSample_coverage : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::SAMPLE_COVERAGE; }
		static daeInt ID() { return 691; }
		virtual daeInt typeID() const { return ID(); }
	public:
		class domValue;

		typedef daeSmartRef<domValue> domValueRef;
		typedef daeTArray<domValueRef> domValue_Array;

		class domValue : public daeElement
		{
		public:
			virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::VALUE; }
			static daeInt ID() { return 692; }
			virtual daeInt typeID() const { return ID(); }
		protected:  // Attributes
			xsFloat attrValue;
			xsToken attrParam;


		public:	//Accessors and Mutators
			/**
			 * Gets the value attribute.
			 * @return Returns a xsFloat of the value attribute.
			 */
			xsFloat getValue() const { return attrValue; }
			/**
			 * Sets the value attribute.
			 * @param atValue The new value for the value attribute.
			 */
			void setValue( xsFloat atValue ) { attrValue = atValue; _validAttributeArray[0] = true; }

			/**
			 * Gets the param attribute.
			 * @return Returns a xsToken of the param attribute.
			 */
			xsToken getParam() const { return attrParam; }
			/**
			 * Sets the param attribute.
			 * @param atParam The new value for the param attribute.
			 */
			void setParam( xsToken atParam ) { *(daeStringRef*)&attrParam = atParam; _validAttributeArray[1] = true; }

		protected:
			/**
			 * Constructor
			 */
			domValue(DAE& dae) : daeElement(dae), attrValue(), attrParam() {}
			/**
			 * Destructor
			 */
			virtual ~domValue() {}
			/**
			 * Overloaded assignment operator
			 */
			virtual domValue &operator=( const domValue &cpy ) { (void)cpy; return *this; }

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

		class domInvert;

		typedef daeSmartRef<domInvert> domInvertRef;
		typedef daeTArray<domInvertRef> domInvert_Array;

		class domInvert : public daeElement
		{
		public:
			virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::INVERT; }
			static daeInt ID() { return 693; }
			virtual daeInt typeID() const { return ID(); }
		protected:  // Attributes
			xsBoolean attrValue;
			xsToken attrParam;


		public:	//Accessors and Mutators
			/**
			 * Gets the value attribute.
			 * @return Returns a xsBoolean of the value attribute.
			 */
			xsBoolean getValue() const { return attrValue; }
			/**
			 * Sets the value attribute.
			 * @param atValue The new value for the value attribute.
			 */
			void setValue( xsBoolean atValue ) { attrValue = atValue; _validAttributeArray[0] = true; }

			/**
			 * Gets the param attribute.
			 * @return Returns a xsToken of the param attribute.
			 */
			xsToken getParam() const { return attrParam; }
			/**
			 * Sets the param attribute.
			 * @param atParam The new value for the param attribute.
			 */
			void setParam( xsToken atParam ) { *(daeStringRef*)&attrParam = atParam; _validAttributeArray[1] = true; }

		protected:
			/**
			 * Constructor
			 */
			domInvert(DAE& dae) : daeElement(dae), attrValue(), attrParam() {}
			/**
			 * Destructor
			 */
			virtual ~domInvert() {}
			/**
			 * Overloaded assignment operator
			 */
			virtual domInvert &operator=( const domInvert &cpy ) { (void)cpy; return *this; }

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
		domValueRef elemValue;
		domInvertRef elemInvert;

	public:	//Accessors and Mutators
		/**
		 * Gets the value element.
		 * @return a daeSmartRef to the value element.
		 */
		const domValueRef getValue() const { return elemValue; }
		/**
		 * Gets the invert element.
		 * @return a daeSmartRef to the invert element.
		 */
		const domInvertRef getInvert() const { return elemInvert; }
	protected:
		/**
		 * Constructor
		 */
		domSample_coverage(DAE& dae) : daeElement(dae), elemValue(), elemInvert() {}
		/**
		 * Destructor
		 */
		virtual ~domSample_coverage() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domSample_coverage &operator=( const domSample_coverage &cpy ) { (void)cpy; return *this; }

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

	class domScissor;

	typedef daeSmartRef<domScissor> domScissorRef;
	typedef daeTArray<domScissorRef> domScissor_Array;

	class domScissor : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::SCISSOR; }
		static daeInt ID() { return 694; }
		virtual daeInt typeID() const { return ID(); }
	protected:  // Attributes
		domInt4 attrValue;
		xsToken attrParam;


	public:	//Accessors and Mutators
		/**
		 * Gets the value attribute.
		 * @return Returns a domInt4 of the value attribute.
		 */
		domInt4 getValue() const { return attrValue; }
		/**
		 * Sets the value attribute.
		 * @param atValue The new value for the value attribute.
		 */
		void setValue( domInt4 atValue ) { attrValue = atValue; _validAttributeArray[0] = true; }

		/**
		 * Gets the param attribute.
		 * @return Returns a xsToken of the param attribute.
		 */
		xsToken getParam() const { return attrParam; }
		/**
		 * Sets the param attribute.
		 * @param atParam The new value for the param attribute.
		 */
		void setParam( xsToken atParam ) { *(daeStringRef*)&attrParam = atParam; _validAttributeArray[1] = true; }

	protected:
		/**
		 * Constructor
		 */
		domScissor(DAE& dae) : daeElement(dae), attrValue(), attrParam() {}
		/**
		 * Destructor
		 */
		virtual ~domScissor() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domScissor &operator=( const domScissor &cpy ) { (void)cpy; return *this; }

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

	class domStencil_func;

	typedef daeSmartRef<domStencil_func> domStencil_funcRef;
	typedef daeTArray<domStencil_funcRef> domStencil_func_Array;

	class domStencil_func : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::STENCIL_FUNC; }
		static daeInt ID() { return 695; }
		virtual daeInt typeID() const { return ID(); }
	public:
		class domFunc;

		typedef daeSmartRef<domFunc> domFuncRef;
		typedef daeTArray<domFuncRef> domFunc_Array;

		class domFunc : public daeElement
		{
		public:
			virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FUNC; }
			static daeInt ID() { return 696; }
			virtual daeInt typeID() const { return ID(); }
		protected:  // Attributes
			domGl_func attrValue;
			xsToken attrParam;


		public:	//Accessors and Mutators
			/**
			 * Gets the value attribute.
			 * @return Returns a domGl_func of the value attribute.
			 */
			domGl_func getValue() const { return attrValue; }
			/**
			 * Sets the value attribute.
			 * @param atValue The new value for the value attribute.
			 */
			void setValue( domGl_func atValue ) { attrValue = atValue; _validAttributeArray[0] = true; }

			/**
			 * Gets the param attribute.
			 * @return Returns a xsToken of the param attribute.
			 */
			xsToken getParam() const { return attrParam; }
			/**
			 * Sets the param attribute.
			 * @param atParam The new value for the param attribute.
			 */
			void setParam( xsToken atParam ) { *(daeStringRef*)&attrParam = atParam; _validAttributeArray[1] = true; }

		protected:
			/**
			 * Constructor
			 */
			domFunc(DAE& dae) : daeElement(dae), attrValue(), attrParam() {}
			/**
			 * Destructor
			 */
			virtual ~domFunc() {}
			/**
			 * Overloaded assignment operator
			 */
			virtual domFunc &operator=( const domFunc &cpy ) { (void)cpy; return *this; }

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

		class domRef;

		typedef daeSmartRef<domRef> domRefRef;
		typedef daeTArray<domRefRef> domRef_Array;

		class domRef : public daeElement
		{
		public:
			virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::REF; }
			static daeInt ID() { return 697; }
			virtual daeInt typeID() const { return ID(); }
		protected:  // Attributes
			xsUnsignedByte attrValue;
			xsToken attrParam;


		public:	//Accessors and Mutators
			/**
			 * Gets the value attribute.
			 * @return Returns a xsUnsignedByte of the value attribute.
			 */
			xsUnsignedByte getValue() const { return attrValue; }
			/**
			 * Sets the value attribute.
			 * @param atValue The new value for the value attribute.
			 */
			void setValue( xsUnsignedByte atValue ) { attrValue = atValue; _validAttributeArray[0] = true; }

			/**
			 * Gets the param attribute.
			 * @return Returns a xsToken of the param attribute.
			 */
			xsToken getParam() const { return attrParam; }
			/**
			 * Sets the param attribute.
			 * @param atParam The new value for the param attribute.
			 */
			void setParam( xsToken atParam ) { *(daeStringRef*)&attrParam = atParam; _validAttributeArray[1] = true; }

		protected:
			/**
			 * Constructor
			 */
			domRef(DAE& dae) : daeElement(dae), attrValue(), attrParam() {}
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

		class domMask;

		typedef daeSmartRef<domMask> domMaskRef;
		typedef daeTArray<domMaskRef> domMask_Array;

		class domMask : public daeElement
		{
		public:
			virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::MASK; }
			static daeInt ID() { return 698; }
			virtual daeInt typeID() const { return ID(); }
		protected:  // Attributes
			xsUnsignedByte attrValue;
			xsToken attrParam;


		public:	//Accessors and Mutators
			/**
			 * Gets the value attribute.
			 * @return Returns a xsUnsignedByte of the value attribute.
			 */
			xsUnsignedByte getValue() const { return attrValue; }
			/**
			 * Sets the value attribute.
			 * @param atValue The new value for the value attribute.
			 */
			void setValue( xsUnsignedByte atValue ) { attrValue = atValue; _validAttributeArray[0] = true; }

			/**
			 * Gets the param attribute.
			 * @return Returns a xsToken of the param attribute.
			 */
			xsToken getParam() const { return attrParam; }
			/**
			 * Sets the param attribute.
			 * @param atParam The new value for the param attribute.
			 */
			void setParam( xsToken atParam ) { *(daeStringRef*)&attrParam = atParam; _validAttributeArray[1] = true; }

		protected:
			/**
			 * Constructor
			 */
			domMask(DAE& dae) : daeElement(dae), attrValue(), attrParam() {}
			/**
			 * Destructor
			 */
			virtual ~domMask() {}
			/**
			 * Overloaded assignment operator
			 */
			virtual domMask &operator=( const domMask &cpy ) { (void)cpy; return *this; }

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
		domFuncRef elemFunc;
		domRefRef elemRef;
		domMaskRef elemMask;

	public:	//Accessors and Mutators
		/**
		 * Gets the func element.
		 * @return a daeSmartRef to the func element.
		 */
		const domFuncRef getFunc() const { return elemFunc; }
		/**
		 * Gets the ref element.
		 * @return a daeSmartRef to the ref element.
		 */
		const domRefRef getRef() const { return elemRef; }
		/**
		 * Gets the mask element.
		 * @return a daeSmartRef to the mask element.
		 */
		const domMaskRef getMask() const { return elemMask; }
	protected:
		/**
		 * Constructor
		 */
		domStencil_func(DAE& dae) : daeElement(dae), elemFunc(), elemRef(), elemMask() {}
		/**
		 * Destructor
		 */
		virtual ~domStencil_func() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domStencil_func &operator=( const domStencil_func &cpy ) { (void)cpy; return *this; }

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

	class domStencil_func_separate;

	typedef daeSmartRef<domStencil_func_separate> domStencil_func_separateRef;
	typedef daeTArray<domStencil_func_separateRef> domStencil_func_separate_Array;

	class domStencil_func_separate : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::STENCIL_FUNC_SEPARATE; }
		static daeInt ID() { return 699; }
		virtual daeInt typeID() const { return ID(); }
	public:
		class domFront;

		typedef daeSmartRef<domFront> domFrontRef;
		typedef daeTArray<domFrontRef> domFront_Array;

		class domFront : public daeElement
		{
		public:
			virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FRONT; }
			static daeInt ID() { return 700; }
			virtual daeInt typeID() const { return ID(); }
		protected:  // Attributes
			domGl_func attrValue;
			xsToken attrParam;


		public:	//Accessors and Mutators
			/**
			 * Gets the value attribute.
			 * @return Returns a domGl_func of the value attribute.
			 */
			domGl_func getValue() const { return attrValue; }
			/**
			 * Sets the value attribute.
			 * @param atValue The new value for the value attribute.
			 */
			void setValue( domGl_func atValue ) { attrValue = atValue; _validAttributeArray[0] = true; }

			/**
			 * Gets the param attribute.
			 * @return Returns a xsToken of the param attribute.
			 */
			xsToken getParam() const { return attrParam; }
			/**
			 * Sets the param attribute.
			 * @param atParam The new value for the param attribute.
			 */
			void setParam( xsToken atParam ) { *(daeStringRef*)&attrParam = atParam; _validAttributeArray[1] = true; }

		protected:
			/**
			 * Constructor
			 */
			domFront(DAE& dae) : daeElement(dae), attrValue(), attrParam() {}
			/**
			 * Destructor
			 */
			virtual ~domFront() {}
			/**
			 * Overloaded assignment operator
			 */
			virtual domFront &operator=( const domFront &cpy ) { (void)cpy; return *this; }

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

		class domBack;

		typedef daeSmartRef<domBack> domBackRef;
		typedef daeTArray<domBackRef> domBack_Array;

		class domBack : public daeElement
		{
		public:
			virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::BACK; }
			static daeInt ID() { return 701; }
			virtual daeInt typeID() const { return ID(); }
		protected:  // Attributes
			domGl_func attrValue;
			xsToken attrParam;


		public:	//Accessors and Mutators
			/**
			 * Gets the value attribute.
			 * @return Returns a domGl_func of the value attribute.
			 */
			domGl_func getValue() const { return attrValue; }
			/**
			 * Sets the value attribute.
			 * @param atValue The new value for the value attribute.
			 */
			void setValue( domGl_func atValue ) { attrValue = atValue; _validAttributeArray[0] = true; }

			/**
			 * Gets the param attribute.
			 * @return Returns a xsToken of the param attribute.
			 */
			xsToken getParam() const { return attrParam; }
			/**
			 * Sets the param attribute.
			 * @param atParam The new value for the param attribute.
			 */
			void setParam( xsToken atParam ) { *(daeStringRef*)&attrParam = atParam; _validAttributeArray[1] = true; }

		protected:
			/**
			 * Constructor
			 */
			domBack(DAE& dae) : daeElement(dae), attrValue(), attrParam() {}
			/**
			 * Destructor
			 */
			virtual ~domBack() {}
			/**
			 * Overloaded assignment operator
			 */
			virtual domBack &operator=( const domBack &cpy ) { (void)cpy; return *this; }

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

		class domRef;

		typedef daeSmartRef<domRef> domRefRef;
		typedef daeTArray<domRefRef> domRef_Array;

		class domRef : public daeElement
		{
		public:
			virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::REF; }
			static daeInt ID() { return 702; }
			virtual daeInt typeID() const { return ID(); }
		protected:  // Attributes
			xsUnsignedByte attrValue;
			xsToken attrParam;


		public:	//Accessors and Mutators
			/**
			 * Gets the value attribute.
			 * @return Returns a xsUnsignedByte of the value attribute.
			 */
			xsUnsignedByte getValue() const { return attrValue; }
			/**
			 * Sets the value attribute.
			 * @param atValue The new value for the value attribute.
			 */
			void setValue( xsUnsignedByte atValue ) { attrValue = atValue; _validAttributeArray[0] = true; }

			/**
			 * Gets the param attribute.
			 * @return Returns a xsToken of the param attribute.
			 */
			xsToken getParam() const { return attrParam; }
			/**
			 * Sets the param attribute.
			 * @param atParam The new value for the param attribute.
			 */
			void setParam( xsToken atParam ) { *(daeStringRef*)&attrParam = atParam; _validAttributeArray[1] = true; }

		protected:
			/**
			 * Constructor
			 */
			domRef(DAE& dae) : daeElement(dae), attrValue(), attrParam() {}
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

		class domMask;

		typedef daeSmartRef<domMask> domMaskRef;
		typedef daeTArray<domMaskRef> domMask_Array;

		class domMask : public daeElement
		{
		public:
			virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::MASK; }
			static daeInt ID() { return 703; }
			virtual daeInt typeID() const { return ID(); }
		protected:  // Attributes
			xsUnsignedByte attrValue;
			xsToken attrParam;


		public:	//Accessors and Mutators
			/**
			 * Gets the value attribute.
			 * @return Returns a xsUnsignedByte of the value attribute.
			 */
			xsUnsignedByte getValue() const { return attrValue; }
			/**
			 * Sets the value attribute.
			 * @param atValue The new value for the value attribute.
			 */
			void setValue( xsUnsignedByte atValue ) { attrValue = atValue; _validAttributeArray[0] = true; }

			/**
			 * Gets the param attribute.
			 * @return Returns a xsToken of the param attribute.
			 */
			xsToken getParam() const { return attrParam; }
			/**
			 * Sets the param attribute.
			 * @param atParam The new value for the param attribute.
			 */
			void setParam( xsToken atParam ) { *(daeStringRef*)&attrParam = atParam; _validAttributeArray[1] = true; }

		protected:
			/**
			 * Constructor
			 */
			domMask(DAE& dae) : daeElement(dae), attrValue(), attrParam() {}
			/**
			 * Destructor
			 */
			virtual ~domMask() {}
			/**
			 * Overloaded assignment operator
			 */
			virtual domMask &operator=( const domMask &cpy ) { (void)cpy; return *this; }

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
		domFrontRef elemFront;
		domBackRef elemBack;
		domRefRef elemRef;
		domMaskRef elemMask;

	public:	//Accessors and Mutators
		/**
		 * Gets the front element.
		 * @return a daeSmartRef to the front element.
		 */
		const domFrontRef getFront() const { return elemFront; }
		/**
		 * Gets the back element.
		 * @return a daeSmartRef to the back element.
		 */
		const domBackRef getBack() const { return elemBack; }
		/**
		 * Gets the ref element.
		 * @return a daeSmartRef to the ref element.
		 */
		const domRefRef getRef() const { return elemRef; }
		/**
		 * Gets the mask element.
		 * @return a daeSmartRef to the mask element.
		 */
		const domMaskRef getMask() const { return elemMask; }
	protected:
		/**
		 * Constructor
		 */
		domStencil_func_separate(DAE& dae) : daeElement(dae), elemFront(), elemBack(), elemRef(), elemMask() {}
		/**
		 * Destructor
		 */
		virtual ~domStencil_func_separate() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domStencil_func_separate &operator=( const domStencil_func_separate &cpy ) { (void)cpy; return *this; }

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

	class domStencil_mask;

	typedef daeSmartRef<domStencil_mask> domStencil_maskRef;
	typedef daeTArray<domStencil_maskRef> domStencil_mask_Array;

	class domStencil_mask : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::STENCIL_MASK; }
		static daeInt ID() { return 704; }
		virtual daeInt typeID() const { return ID(); }
	protected:  // Attributes
		domInt attrValue;
		xsToken attrParam;


	public:	//Accessors and Mutators
		/**
		 * Gets the value attribute.
		 * @return Returns a domInt of the value attribute.
		 */
		domInt getValue() const { return attrValue; }
		/**
		 * Sets the value attribute.
		 * @param atValue The new value for the value attribute.
		 */
		void setValue( domInt atValue ) { attrValue = atValue; _validAttributeArray[0] = true; }

		/**
		 * Gets the param attribute.
		 * @return Returns a xsToken of the param attribute.
		 */
		xsToken getParam() const { return attrParam; }
		/**
		 * Sets the param attribute.
		 * @param atParam The new value for the param attribute.
		 */
		void setParam( xsToken atParam ) { *(daeStringRef*)&attrParam = atParam; _validAttributeArray[1] = true; }

	protected:
		/**
		 * Constructor
		 */
		domStencil_mask(DAE& dae) : daeElement(dae), attrValue(), attrParam() {}
		/**
		 * Destructor
		 */
		virtual ~domStencil_mask() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domStencil_mask &operator=( const domStencil_mask &cpy ) { (void)cpy; return *this; }

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

	class domStencil_mask_separate;

	typedef daeSmartRef<domStencil_mask_separate> domStencil_mask_separateRef;
	typedef daeTArray<domStencil_mask_separateRef> domStencil_mask_separate_Array;

	class domStencil_mask_separate : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::STENCIL_MASK_SEPARATE; }
		static daeInt ID() { return 705; }
		virtual daeInt typeID() const { return ID(); }
	public:
		class domFace;

		typedef daeSmartRef<domFace> domFaceRef;
		typedef daeTArray<domFaceRef> domFace_Array;

		class domFace : public daeElement
		{
		public:
			virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FACE; }
			static daeInt ID() { return 706; }
			virtual daeInt typeID() const { return ID(); }
		protected:  // Attributes
			domGl_face attrValue;
			xsToken attrParam;


		public:	//Accessors and Mutators
			/**
			 * Gets the value attribute.
			 * @return Returns a domGl_face of the value attribute.
			 */
			domGl_face getValue() const { return attrValue; }
			/**
			 * Sets the value attribute.
			 * @param atValue The new value for the value attribute.
			 */
			void setValue( domGl_face atValue ) { attrValue = atValue; _validAttributeArray[0] = true; }

			/**
			 * Gets the param attribute.
			 * @return Returns a xsToken of the param attribute.
			 */
			xsToken getParam() const { return attrParam; }
			/**
			 * Sets the param attribute.
			 * @param atParam The new value for the param attribute.
			 */
			void setParam( xsToken atParam ) { *(daeStringRef*)&attrParam = atParam; _validAttributeArray[1] = true; }

		protected:
			/**
			 * Constructor
			 */
			domFace(DAE& dae) : daeElement(dae), attrValue(), attrParam() {}
			/**
			 * Destructor
			 */
			virtual ~domFace() {}
			/**
			 * Overloaded assignment operator
			 */
			virtual domFace &operator=( const domFace &cpy ) { (void)cpy; return *this; }

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

		class domMask;

		typedef daeSmartRef<domMask> domMaskRef;
		typedef daeTArray<domMaskRef> domMask_Array;

		class domMask : public daeElement
		{
		public:
			virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::MASK; }
			static daeInt ID() { return 707; }
			virtual daeInt typeID() const { return ID(); }
		protected:  // Attributes
			xsUnsignedByte attrValue;
			xsToken attrParam;


		public:	//Accessors and Mutators
			/**
			 * Gets the value attribute.
			 * @return Returns a xsUnsignedByte of the value attribute.
			 */
			xsUnsignedByte getValue() const { return attrValue; }
			/**
			 * Sets the value attribute.
			 * @param atValue The new value for the value attribute.
			 */
			void setValue( xsUnsignedByte atValue ) { attrValue = atValue; _validAttributeArray[0] = true; }

			/**
			 * Gets the param attribute.
			 * @return Returns a xsToken of the param attribute.
			 */
			xsToken getParam() const { return attrParam; }
			/**
			 * Sets the param attribute.
			 * @param atParam The new value for the param attribute.
			 */
			void setParam( xsToken atParam ) { *(daeStringRef*)&attrParam = atParam; _validAttributeArray[1] = true; }

		protected:
			/**
			 * Constructor
			 */
			domMask(DAE& dae) : daeElement(dae), attrValue(), attrParam() {}
			/**
			 * Destructor
			 */
			virtual ~domMask() {}
			/**
			 * Overloaded assignment operator
			 */
			virtual domMask &operator=( const domMask &cpy ) { (void)cpy; return *this; }

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
		domFaceRef elemFace;
		domMaskRef elemMask;

	public:	//Accessors and Mutators
		/**
		 * Gets the face element.
		 * @return a daeSmartRef to the face element.
		 */
		const domFaceRef getFace() const { return elemFace; }
		/**
		 * Gets the mask element.
		 * @return a daeSmartRef to the mask element.
		 */
		const domMaskRef getMask() const { return elemMask; }
	protected:
		/**
		 * Constructor
		 */
		domStencil_mask_separate(DAE& dae) : daeElement(dae), elemFace(), elemMask() {}
		/**
		 * Destructor
		 */
		virtual ~domStencil_mask_separate() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domStencil_mask_separate &operator=( const domStencil_mask_separate &cpy ) { (void)cpy; return *this; }

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

	class domStencil_op;

	typedef daeSmartRef<domStencil_op> domStencil_opRef;
	typedef daeTArray<domStencil_opRef> domStencil_op_Array;

	class domStencil_op : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::STENCIL_OP; }
		static daeInt ID() { return 708; }
		virtual daeInt typeID() const { return ID(); }
	public:
		class domFail;

		typedef daeSmartRef<domFail> domFailRef;
		typedef daeTArray<domFailRef> domFail_Array;

		class domFail : public daeElement
		{
		public:
			virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FAIL; }
			static daeInt ID() { return 709; }
			virtual daeInt typeID() const { return ID(); }
		protected:  // Attributes
			domGl_stencil_op attrValue;
			xsToken attrParam;


		public:	//Accessors and Mutators
			/**
			 * Gets the value attribute.
			 * @return Returns a domGl_stencil_op of the value attribute.
			 */
			domGl_stencil_op getValue() const { return attrValue; }
			/**
			 * Sets the value attribute.
			 * @param atValue The new value for the value attribute.
			 */
			void setValue( domGl_stencil_op atValue ) { attrValue = atValue; _validAttributeArray[0] = true; }

			/**
			 * Gets the param attribute.
			 * @return Returns a xsToken of the param attribute.
			 */
			xsToken getParam() const { return attrParam; }
			/**
			 * Sets the param attribute.
			 * @param atParam The new value for the param attribute.
			 */
			void setParam( xsToken atParam ) { *(daeStringRef*)&attrParam = atParam; _validAttributeArray[1] = true; }

		protected:
			/**
			 * Constructor
			 */
			domFail(DAE& dae) : daeElement(dae), attrValue(), attrParam() {}
			/**
			 * Destructor
			 */
			virtual ~domFail() {}
			/**
			 * Overloaded assignment operator
			 */
			virtual domFail &operator=( const domFail &cpy ) { (void)cpy; return *this; }

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

		class domZfail;

		typedef daeSmartRef<domZfail> domZfailRef;
		typedef daeTArray<domZfailRef> domZfail_Array;

		class domZfail : public daeElement
		{
		public:
			virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::ZFAIL; }
			static daeInt ID() { return 710; }
			virtual daeInt typeID() const { return ID(); }
		protected:  // Attributes
			domGl_stencil_op attrValue;
			xsToken attrParam;


		public:	//Accessors and Mutators
			/**
			 * Gets the value attribute.
			 * @return Returns a domGl_stencil_op of the value attribute.
			 */
			domGl_stencil_op getValue() const { return attrValue; }
			/**
			 * Sets the value attribute.
			 * @param atValue The new value for the value attribute.
			 */
			void setValue( domGl_stencil_op atValue ) { attrValue = atValue; _validAttributeArray[0] = true; }

			/**
			 * Gets the param attribute.
			 * @return Returns a xsToken of the param attribute.
			 */
			xsToken getParam() const { return attrParam; }
			/**
			 * Sets the param attribute.
			 * @param atParam The new value for the param attribute.
			 */
			void setParam( xsToken atParam ) { *(daeStringRef*)&attrParam = atParam; _validAttributeArray[1] = true; }

		protected:
			/**
			 * Constructor
			 */
			domZfail(DAE& dae) : daeElement(dae), attrValue(), attrParam() {}
			/**
			 * Destructor
			 */
			virtual ~domZfail() {}
			/**
			 * Overloaded assignment operator
			 */
			virtual domZfail &operator=( const domZfail &cpy ) { (void)cpy; return *this; }

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

		class domZpass;

		typedef daeSmartRef<domZpass> domZpassRef;
		typedef daeTArray<domZpassRef> domZpass_Array;

		class domZpass : public daeElement
		{
		public:
			virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::ZPASS; }
			static daeInt ID() { return 711; }
			virtual daeInt typeID() const { return ID(); }
		protected:  // Attributes
			domGl_stencil_op attrValue;
			xsToken attrParam;


		public:	//Accessors and Mutators
			/**
			 * Gets the value attribute.
			 * @return Returns a domGl_stencil_op of the value attribute.
			 */
			domGl_stencil_op getValue() const { return attrValue; }
			/**
			 * Sets the value attribute.
			 * @param atValue The new value for the value attribute.
			 */
			void setValue( domGl_stencil_op atValue ) { attrValue = atValue; _validAttributeArray[0] = true; }

			/**
			 * Gets the param attribute.
			 * @return Returns a xsToken of the param attribute.
			 */
			xsToken getParam() const { return attrParam; }
			/**
			 * Sets the param attribute.
			 * @param atParam The new value for the param attribute.
			 */
			void setParam( xsToken atParam ) { *(daeStringRef*)&attrParam = atParam; _validAttributeArray[1] = true; }

		protected:
			/**
			 * Constructor
			 */
			domZpass(DAE& dae) : daeElement(dae), attrValue(), attrParam() {}
			/**
			 * Destructor
			 */
			virtual ~domZpass() {}
			/**
			 * Overloaded assignment operator
			 */
			virtual domZpass &operator=( const domZpass &cpy ) { (void)cpy; return *this; }

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
		domFailRef elemFail;
		domZfailRef elemZfail;
		domZpassRef elemZpass;

	public:	//Accessors and Mutators
		/**
		 * Gets the fail element.
		 * @return a daeSmartRef to the fail element.
		 */
		const domFailRef getFail() const { return elemFail; }
		/**
		 * Gets the zfail element.
		 * @return a daeSmartRef to the zfail element.
		 */
		const domZfailRef getZfail() const { return elemZfail; }
		/**
		 * Gets the zpass element.
		 * @return a daeSmartRef to the zpass element.
		 */
		const domZpassRef getZpass() const { return elemZpass; }
	protected:
		/**
		 * Constructor
		 */
		domStencil_op(DAE& dae) : daeElement(dae), elemFail(), elemZfail(), elemZpass() {}
		/**
		 * Destructor
		 */
		virtual ~domStencil_op() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domStencil_op &operator=( const domStencil_op &cpy ) { (void)cpy; return *this; }

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

	class domStencil_op_separate;

	typedef daeSmartRef<domStencil_op_separate> domStencil_op_separateRef;
	typedef daeTArray<domStencil_op_separateRef> domStencil_op_separate_Array;

	class domStencil_op_separate : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::STENCIL_OP_SEPARATE; }
		static daeInt ID() { return 712; }
		virtual daeInt typeID() const { return ID(); }
	public:
		class domFace;

		typedef daeSmartRef<domFace> domFaceRef;
		typedef daeTArray<domFaceRef> domFace_Array;

		class domFace : public daeElement
		{
		public:
			virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FACE; }
			static daeInt ID() { return 713; }
			virtual daeInt typeID() const { return ID(); }
		protected:  // Attributes
			domGl_face attrValue;
			xsToken attrParam;


		public:	//Accessors and Mutators
			/**
			 * Gets the value attribute.
			 * @return Returns a domGl_face of the value attribute.
			 */
			domGl_face getValue() const { return attrValue; }
			/**
			 * Sets the value attribute.
			 * @param atValue The new value for the value attribute.
			 */
			void setValue( domGl_face atValue ) { attrValue = atValue; _validAttributeArray[0] = true; }

			/**
			 * Gets the param attribute.
			 * @return Returns a xsToken of the param attribute.
			 */
			xsToken getParam() const { return attrParam; }
			/**
			 * Sets the param attribute.
			 * @param atParam The new value for the param attribute.
			 */
			void setParam( xsToken atParam ) { *(daeStringRef*)&attrParam = atParam; _validAttributeArray[1] = true; }

		protected:
			/**
			 * Constructor
			 */
			domFace(DAE& dae) : daeElement(dae), attrValue(), attrParam() {}
			/**
			 * Destructor
			 */
			virtual ~domFace() {}
			/**
			 * Overloaded assignment operator
			 */
			virtual domFace &operator=( const domFace &cpy ) { (void)cpy; return *this; }

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

		class domFail;

		typedef daeSmartRef<domFail> domFailRef;
		typedef daeTArray<domFailRef> domFail_Array;

		class domFail : public daeElement
		{
		public:
			virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FAIL; }
			static daeInt ID() { return 714; }
			virtual daeInt typeID() const { return ID(); }
		protected:  // Attributes
			domGl_stencil_op attrValue;
			xsToken attrParam;


		public:	//Accessors and Mutators
			/**
			 * Gets the value attribute.
			 * @return Returns a domGl_stencil_op of the value attribute.
			 */
			domGl_stencil_op getValue() const { return attrValue; }
			/**
			 * Sets the value attribute.
			 * @param atValue The new value for the value attribute.
			 */
			void setValue( domGl_stencil_op atValue ) { attrValue = atValue; _validAttributeArray[0] = true; }

			/**
			 * Gets the param attribute.
			 * @return Returns a xsToken of the param attribute.
			 */
			xsToken getParam() const { return attrParam; }
			/**
			 * Sets the param attribute.
			 * @param atParam The new value for the param attribute.
			 */
			void setParam( xsToken atParam ) { *(daeStringRef*)&attrParam = atParam; _validAttributeArray[1] = true; }

		protected:
			/**
			 * Constructor
			 */
			domFail(DAE& dae) : daeElement(dae), attrValue(), attrParam() {}
			/**
			 * Destructor
			 */
			virtual ~domFail() {}
			/**
			 * Overloaded assignment operator
			 */
			virtual domFail &operator=( const domFail &cpy ) { (void)cpy; return *this; }

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

		class domZfail;

		typedef daeSmartRef<domZfail> domZfailRef;
		typedef daeTArray<domZfailRef> domZfail_Array;

		class domZfail : public daeElement
		{
		public:
			virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::ZFAIL; }
			static daeInt ID() { return 715; }
			virtual daeInt typeID() const { return ID(); }
		protected:  // Attributes
			domGl_stencil_op attrValue;
			xsToken attrParam;


		public:	//Accessors and Mutators
			/**
			 * Gets the value attribute.
			 * @return Returns a domGl_stencil_op of the value attribute.
			 */
			domGl_stencil_op getValue() const { return attrValue; }
			/**
			 * Sets the value attribute.
			 * @param atValue The new value for the value attribute.
			 */
			void setValue( domGl_stencil_op atValue ) { attrValue = atValue; _validAttributeArray[0] = true; }

			/**
			 * Gets the param attribute.
			 * @return Returns a xsToken of the param attribute.
			 */
			xsToken getParam() const { return attrParam; }
			/**
			 * Sets the param attribute.
			 * @param atParam The new value for the param attribute.
			 */
			void setParam( xsToken atParam ) { *(daeStringRef*)&attrParam = atParam; _validAttributeArray[1] = true; }

		protected:
			/**
			 * Constructor
			 */
			domZfail(DAE& dae) : daeElement(dae), attrValue(), attrParam() {}
			/**
			 * Destructor
			 */
			virtual ~domZfail() {}
			/**
			 * Overloaded assignment operator
			 */
			virtual domZfail &operator=( const domZfail &cpy ) { (void)cpy; return *this; }

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

		class domZpass;

		typedef daeSmartRef<domZpass> domZpassRef;
		typedef daeTArray<domZpassRef> domZpass_Array;

		class domZpass : public daeElement
		{
		public:
			virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::ZPASS; }
			static daeInt ID() { return 716; }
			virtual daeInt typeID() const { return ID(); }
		protected:  // Attributes
			domGl_stencil_op attrValue;
			xsToken attrParam;


		public:	//Accessors and Mutators
			/**
			 * Gets the value attribute.
			 * @return Returns a domGl_stencil_op of the value attribute.
			 */
			domGl_stencil_op getValue() const { return attrValue; }
			/**
			 * Sets the value attribute.
			 * @param atValue The new value for the value attribute.
			 */
			void setValue( domGl_stencil_op atValue ) { attrValue = atValue; _validAttributeArray[0] = true; }

			/**
			 * Gets the param attribute.
			 * @return Returns a xsToken of the param attribute.
			 */
			xsToken getParam() const { return attrParam; }
			/**
			 * Sets the param attribute.
			 * @param atParam The new value for the param attribute.
			 */
			void setParam( xsToken atParam ) { *(daeStringRef*)&attrParam = atParam; _validAttributeArray[1] = true; }

		protected:
			/**
			 * Constructor
			 */
			domZpass(DAE& dae) : daeElement(dae), attrValue(), attrParam() {}
			/**
			 * Destructor
			 */
			virtual ~domZpass() {}
			/**
			 * Overloaded assignment operator
			 */
			virtual domZpass &operator=( const domZpass &cpy ) { (void)cpy; return *this; }

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
		domFaceRef elemFace;
		domFailRef elemFail;
		domZfailRef elemZfail;
		domZpassRef elemZpass;

	public:	//Accessors and Mutators
		/**
		 * Gets the face element.
		 * @return a daeSmartRef to the face element.
		 */
		const domFaceRef getFace() const { return elemFace; }
		/**
		 * Gets the fail element.
		 * @return a daeSmartRef to the fail element.
		 */
		const domFailRef getFail() const { return elemFail; }
		/**
		 * Gets the zfail element.
		 * @return a daeSmartRef to the zfail element.
		 */
		const domZfailRef getZfail() const { return elemZfail; }
		/**
		 * Gets the zpass element.
		 * @return a daeSmartRef to the zpass element.
		 */
		const domZpassRef getZpass() const { return elemZpass; }
	protected:
		/**
		 * Constructor
		 */
		domStencil_op_separate(DAE& dae) : daeElement(dae), elemFace(), elemFail(), elemZfail(), elemZpass() {}
		/**
		 * Destructor
		 */
		virtual ~domStencil_op_separate() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domStencil_op_separate &operator=( const domStencil_op_separate &cpy ) { (void)cpy; return *this; }

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

	class domBlend_enable;

	typedef daeSmartRef<domBlend_enable> domBlend_enableRef;
	typedef daeTArray<domBlend_enableRef> domBlend_enable_Array;

	class domBlend_enable : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::BLEND_ENABLE; }
		static daeInt ID() { return 717; }
		virtual daeInt typeID() const { return ID(); }
	protected:  // Attributes
		xsBoolean attrValue;
		xsToken attrParam;


	public:	//Accessors and Mutators
		/**
		 * Gets the value attribute.
		 * @return Returns a xsBoolean of the value attribute.
		 */
		xsBoolean getValue() const { return attrValue; }
		/**
		 * Sets the value attribute.
		 * @param atValue The new value for the value attribute.
		 */
		void setValue( xsBoolean atValue ) { attrValue = atValue; _validAttributeArray[0] = true; }

		/**
		 * Gets the param attribute.
		 * @return Returns a xsToken of the param attribute.
		 */
		xsToken getParam() const { return attrParam; }
		/**
		 * Sets the param attribute.
		 * @param atParam The new value for the param attribute.
		 */
		void setParam( xsToken atParam ) { *(daeStringRef*)&attrParam = atParam; _validAttributeArray[1] = true; }

	protected:
		/**
		 * Constructor
		 */
		domBlend_enable(DAE& dae) : daeElement(dae), attrValue(), attrParam() {}
		/**
		 * Destructor
		 */
		virtual ~domBlend_enable() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domBlend_enable &operator=( const domBlend_enable &cpy ) { (void)cpy; return *this; }

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

	class domCull_face_enable;

	typedef daeSmartRef<domCull_face_enable> domCull_face_enableRef;
	typedef daeTArray<domCull_face_enableRef> domCull_face_enable_Array;

	class domCull_face_enable : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::CULL_FACE_ENABLE; }
		static daeInt ID() { return 718; }
		virtual daeInt typeID() const { return ID(); }
	protected:  // Attributes
		xsBoolean attrValue;
		xsToken attrParam;


	public:	//Accessors and Mutators
		/**
		 * Gets the value attribute.
		 * @return Returns a xsBoolean of the value attribute.
		 */
		xsBoolean getValue() const { return attrValue; }
		/**
		 * Sets the value attribute.
		 * @param atValue The new value for the value attribute.
		 */
		void setValue( xsBoolean atValue ) { attrValue = atValue; _validAttributeArray[0] = true; }

		/**
		 * Gets the param attribute.
		 * @return Returns a xsToken of the param attribute.
		 */
		xsToken getParam() const { return attrParam; }
		/**
		 * Sets the param attribute.
		 * @param atParam The new value for the param attribute.
		 */
		void setParam( xsToken atParam ) { *(daeStringRef*)&attrParam = atParam; _validAttributeArray[1] = true; }

	protected:
		/**
		 * Constructor
		 */
		domCull_face_enable(DAE& dae) : daeElement(dae), attrValue(), attrParam() {}
		/**
		 * Destructor
		 */
		virtual ~domCull_face_enable() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domCull_face_enable &operator=( const domCull_face_enable &cpy ) { (void)cpy; return *this; }

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

	class domDepth_test_enable;

	typedef daeSmartRef<domDepth_test_enable> domDepth_test_enableRef;
	typedef daeTArray<domDepth_test_enableRef> domDepth_test_enable_Array;

	class domDepth_test_enable : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::DEPTH_TEST_ENABLE; }
		static daeInt ID() { return 719; }
		virtual daeInt typeID() const { return ID(); }
	protected:  // Attributes
		xsBoolean attrValue;
		xsToken attrParam;


	public:	//Accessors and Mutators
		/**
		 * Gets the value attribute.
		 * @return Returns a xsBoolean of the value attribute.
		 */
		xsBoolean getValue() const { return attrValue; }
		/**
		 * Sets the value attribute.
		 * @param atValue The new value for the value attribute.
		 */
		void setValue( xsBoolean atValue ) { attrValue = atValue; _validAttributeArray[0] = true; }

		/**
		 * Gets the param attribute.
		 * @return Returns a xsToken of the param attribute.
		 */
		xsToken getParam() const { return attrParam; }
		/**
		 * Sets the param attribute.
		 * @param atParam The new value for the param attribute.
		 */
		void setParam( xsToken atParam ) { *(daeStringRef*)&attrParam = atParam; _validAttributeArray[1] = true; }

	protected:
		/**
		 * Constructor
		 */
		domDepth_test_enable(DAE& dae) : daeElement(dae), attrValue(), attrParam() {}
		/**
		 * Destructor
		 */
		virtual ~domDepth_test_enable() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domDepth_test_enable &operator=( const domDepth_test_enable &cpy ) { (void)cpy; return *this; }

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

	class domDither_enable;

	typedef daeSmartRef<domDither_enable> domDither_enableRef;
	typedef daeTArray<domDither_enableRef> domDither_enable_Array;

	class domDither_enable : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::DITHER_ENABLE; }
		static daeInt ID() { return 720; }
		virtual daeInt typeID() const { return ID(); }
	protected:  // Attributes
		xsBoolean attrValue;
		xsToken attrParam;


	public:	//Accessors and Mutators
		/**
		 * Gets the value attribute.
		 * @return Returns a xsBoolean of the value attribute.
		 */
		xsBoolean getValue() const { return attrValue; }
		/**
		 * Sets the value attribute.
		 * @param atValue The new value for the value attribute.
		 */
		void setValue( xsBoolean atValue ) { attrValue = atValue; _validAttributeArray[0] = true; }

		/**
		 * Gets the param attribute.
		 * @return Returns a xsToken of the param attribute.
		 */
		xsToken getParam() const { return attrParam; }
		/**
		 * Sets the param attribute.
		 * @param atParam The new value for the param attribute.
		 */
		void setParam( xsToken atParam ) { *(daeStringRef*)&attrParam = atParam; _validAttributeArray[1] = true; }

	protected:
		/**
		 * Constructor
		 */
		domDither_enable(DAE& dae) : daeElement(dae), attrValue(), attrParam() {}
		/**
		 * Destructor
		 */
		virtual ~domDither_enable() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domDither_enable &operator=( const domDither_enable &cpy ) { (void)cpy; return *this; }

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

	class domPolygon_offset_fill_enable;

	typedef daeSmartRef<domPolygon_offset_fill_enable> domPolygon_offset_fill_enableRef;
	typedef daeTArray<domPolygon_offset_fill_enableRef> domPolygon_offset_fill_enable_Array;

	class domPolygon_offset_fill_enable : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::POLYGON_OFFSET_FILL_ENABLE; }
		static daeInt ID() { return 721; }
		virtual daeInt typeID() const { return ID(); }
	protected:  // Attributes
		xsBoolean attrValue;
		xsToken attrParam;


	public:	//Accessors and Mutators
		/**
		 * Gets the value attribute.
		 * @return Returns a xsBoolean of the value attribute.
		 */
		xsBoolean getValue() const { return attrValue; }
		/**
		 * Sets the value attribute.
		 * @param atValue The new value for the value attribute.
		 */
		void setValue( xsBoolean atValue ) { attrValue = atValue; _validAttributeArray[0] = true; }

		/**
		 * Gets the param attribute.
		 * @return Returns a xsToken of the param attribute.
		 */
		xsToken getParam() const { return attrParam; }
		/**
		 * Sets the param attribute.
		 * @param atParam The new value for the param attribute.
		 */
		void setParam( xsToken atParam ) { *(daeStringRef*)&attrParam = atParam; _validAttributeArray[1] = true; }

	protected:
		/**
		 * Constructor
		 */
		domPolygon_offset_fill_enable(DAE& dae) : daeElement(dae), attrValue(), attrParam() {}
		/**
		 * Destructor
		 */
		virtual ~domPolygon_offset_fill_enable() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domPolygon_offset_fill_enable &operator=( const domPolygon_offset_fill_enable &cpy ) { (void)cpy; return *this; }

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

	class domPoint_size_enable;

	typedef daeSmartRef<domPoint_size_enable> domPoint_size_enableRef;
	typedef daeTArray<domPoint_size_enableRef> domPoint_size_enable_Array;

	class domPoint_size_enable : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::POINT_SIZE_ENABLE; }
		static daeInt ID() { return 722; }
		virtual daeInt typeID() const { return ID(); }
	protected:  // Attributes
		xsBoolean attrValue;
		xsToken attrParam;


	public:	//Accessors and Mutators
		/**
		 * Gets the value attribute.
		 * @return Returns a xsBoolean of the value attribute.
		 */
		xsBoolean getValue() const { return attrValue; }
		/**
		 * Sets the value attribute.
		 * @param atValue The new value for the value attribute.
		 */
		void setValue( xsBoolean atValue ) { attrValue = atValue; _validAttributeArray[0] = true; }

		/**
		 * Gets the param attribute.
		 * @return Returns a xsToken of the param attribute.
		 */
		xsToken getParam() const { return attrParam; }
		/**
		 * Sets the param attribute.
		 * @param atParam The new value for the param attribute.
		 */
		void setParam( xsToken atParam ) { *(daeStringRef*)&attrParam = atParam; _validAttributeArray[1] = true; }

	protected:
		/**
		 * Constructor
		 */
		domPoint_size_enable(DAE& dae) : daeElement(dae), attrValue(), attrParam() {}
		/**
		 * Destructor
		 */
		virtual ~domPoint_size_enable() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domPoint_size_enable &operator=( const domPoint_size_enable &cpy ) { (void)cpy; return *this; }

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

	class domSample_alpha_to_coverage_enable;

	typedef daeSmartRef<domSample_alpha_to_coverage_enable> domSample_alpha_to_coverage_enableRef;
	typedef daeTArray<domSample_alpha_to_coverage_enableRef> domSample_alpha_to_coverage_enable_Array;

	class domSample_alpha_to_coverage_enable : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::SAMPLE_ALPHA_TO_COVERAGE_ENABLE; }
		static daeInt ID() { return 723; }
		virtual daeInt typeID() const { return ID(); }
	protected:  // Attributes
		xsBoolean attrValue;
		xsToken attrParam;


	public:	//Accessors and Mutators
		/**
		 * Gets the value attribute.
		 * @return Returns a xsBoolean of the value attribute.
		 */
		xsBoolean getValue() const { return attrValue; }
		/**
		 * Sets the value attribute.
		 * @param atValue The new value for the value attribute.
		 */
		void setValue( xsBoolean atValue ) { attrValue = atValue; _validAttributeArray[0] = true; }

		/**
		 * Gets the param attribute.
		 * @return Returns a xsToken of the param attribute.
		 */
		xsToken getParam() const { return attrParam; }
		/**
		 * Sets the param attribute.
		 * @param atParam The new value for the param attribute.
		 */
		void setParam( xsToken atParam ) { *(daeStringRef*)&attrParam = atParam; _validAttributeArray[1] = true; }

	protected:
		/**
		 * Constructor
		 */
		domSample_alpha_to_coverage_enable(DAE& dae) : daeElement(dae), attrValue(), attrParam() {}
		/**
		 * Destructor
		 */
		virtual ~domSample_alpha_to_coverage_enable() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domSample_alpha_to_coverage_enable &operator=( const domSample_alpha_to_coverage_enable &cpy ) { (void)cpy; return *this; }

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

	class domSample_coverage_enable;

	typedef daeSmartRef<domSample_coverage_enable> domSample_coverage_enableRef;
	typedef daeTArray<domSample_coverage_enableRef> domSample_coverage_enable_Array;

	class domSample_coverage_enable : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::SAMPLE_COVERAGE_ENABLE; }
		static daeInt ID() { return 724; }
		virtual daeInt typeID() const { return ID(); }
	protected:  // Attributes
		xsBoolean attrValue;
		xsToken attrParam;


	public:	//Accessors and Mutators
		/**
		 * Gets the value attribute.
		 * @return Returns a xsBoolean of the value attribute.
		 */
		xsBoolean getValue() const { return attrValue; }
		/**
		 * Sets the value attribute.
		 * @param atValue The new value for the value attribute.
		 */
		void setValue( xsBoolean atValue ) { attrValue = atValue; _validAttributeArray[0] = true; }

		/**
		 * Gets the param attribute.
		 * @return Returns a xsToken of the param attribute.
		 */
		xsToken getParam() const { return attrParam; }
		/**
		 * Sets the param attribute.
		 * @param atParam The new value for the param attribute.
		 */
		void setParam( xsToken atParam ) { *(daeStringRef*)&attrParam = atParam; _validAttributeArray[1] = true; }

	protected:
		/**
		 * Constructor
		 */
		domSample_coverage_enable(DAE& dae) : daeElement(dae), attrValue(), attrParam() {}
		/**
		 * Destructor
		 */
		virtual ~domSample_coverage_enable() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domSample_coverage_enable &operator=( const domSample_coverage_enable &cpy ) { (void)cpy; return *this; }

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

	class domScissor_test_enable;

	typedef daeSmartRef<domScissor_test_enable> domScissor_test_enableRef;
	typedef daeTArray<domScissor_test_enableRef> domScissor_test_enable_Array;

	class domScissor_test_enable : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::SCISSOR_TEST_ENABLE; }
		static daeInt ID() { return 725; }
		virtual daeInt typeID() const { return ID(); }
	protected:  // Attributes
		xsBoolean attrValue;
		xsToken attrParam;


	public:	//Accessors and Mutators
		/**
		 * Gets the value attribute.
		 * @return Returns a xsBoolean of the value attribute.
		 */
		xsBoolean getValue() const { return attrValue; }
		/**
		 * Sets the value attribute.
		 * @param atValue The new value for the value attribute.
		 */
		void setValue( xsBoolean atValue ) { attrValue = atValue; _validAttributeArray[0] = true; }

		/**
		 * Gets the param attribute.
		 * @return Returns a xsToken of the param attribute.
		 */
		xsToken getParam() const { return attrParam; }
		/**
		 * Sets the param attribute.
		 * @param atParam The new value for the param attribute.
		 */
		void setParam( xsToken atParam ) { *(daeStringRef*)&attrParam = atParam; _validAttributeArray[1] = true; }

	protected:
		/**
		 * Constructor
		 */
		domScissor_test_enable(DAE& dae) : daeElement(dae), attrValue(), attrParam() {}
		/**
		 * Destructor
		 */
		virtual ~domScissor_test_enable() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domScissor_test_enable &operator=( const domScissor_test_enable &cpy ) { (void)cpy; return *this; }

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

	class domStencil_test_enable;

	typedef daeSmartRef<domStencil_test_enable> domStencil_test_enableRef;
	typedef daeTArray<domStencil_test_enableRef> domStencil_test_enable_Array;

	class domStencil_test_enable : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::STENCIL_TEST_ENABLE; }
		static daeInt ID() { return 726; }
		virtual daeInt typeID() const { return ID(); }
	protected:  // Attributes
		xsBoolean attrValue;
		xsToken attrParam;


	public:	//Accessors and Mutators
		/**
		 * Gets the value attribute.
		 * @return Returns a xsBoolean of the value attribute.
		 */
		xsBoolean getValue() const { return attrValue; }
		/**
		 * Sets the value attribute.
		 * @param atValue The new value for the value attribute.
		 */
		void setValue( xsBoolean atValue ) { attrValue = atValue; _validAttributeArray[0] = true; }

		/**
		 * Gets the param attribute.
		 * @return Returns a xsToken of the param attribute.
		 */
		xsToken getParam() const { return attrParam; }
		/**
		 * Sets the param attribute.
		 * @param atParam The new value for the param attribute.
		 */
		void setParam( xsToken atParam ) { *(daeStringRef*)&attrParam = atParam; _validAttributeArray[1] = true; }

	protected:
		/**
		 * Constructor
		 */
		domStencil_test_enable(DAE& dae) : daeElement(dae), attrValue(), attrParam() {}
		/**
		 * Destructor
		 */
		virtual ~domStencil_test_enable() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domStencil_test_enable &operator=( const domStencil_test_enable &cpy ) { (void)cpy; return *this; }

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
	domBlend_colorRef elemBlend_color;
	domBlend_equationRef elemBlend_equation;
	domBlend_equation_separateRef elemBlend_equation_separate;
	domBlend_funcRef elemBlend_func;
	domBlend_func_separateRef elemBlend_func_separate;
	domColor_maskRef elemColor_mask;
	domCull_faceRef elemCull_face;
	domDepth_funcRef elemDepth_func;
	domDepth_maskRef elemDepth_mask;
	domDepth_rangeRef elemDepth_range;
	domFront_faceRef elemFront_face;
	domLine_widthRef elemLine_width;
	domPolygon_offsetRef elemPolygon_offset;
	domPoint_sizeRef elemPoint_size;
	domSample_coverageRef elemSample_coverage;
	domScissorRef elemScissor;
	domStencil_funcRef elemStencil_func;
	domStencil_func_separateRef elemStencil_func_separate;
	domStencil_maskRef elemStencil_mask;
	domStencil_mask_separateRef elemStencil_mask_separate;
	domStencil_opRef elemStencil_op;
	domStencil_op_separateRef elemStencil_op_separate;
	domBlend_enableRef elemBlend_enable;
	domCull_face_enableRef elemCull_face_enable;
	domDepth_test_enableRef elemDepth_test_enable;
	domDither_enableRef elemDither_enable;
	domPolygon_offset_fill_enableRef elemPolygon_offset_fill_enable;
	domPoint_size_enableRef elemPoint_size_enable;
	domSample_alpha_to_coverage_enableRef elemSample_alpha_to_coverage_enable;
	domSample_coverage_enableRef elemSample_coverage_enable;
	domScissor_test_enableRef elemScissor_test_enable;
	domStencil_test_enableRef elemStencil_test_enable;
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
	 * Gets the blend_color element.
	 * @return a daeSmartRef to the blend_color element.
	 */
	const domBlend_colorRef getBlend_color() const { return elemBlend_color; }
	/**
	 * Gets the blend_equation element.
	 * @return a daeSmartRef to the blend_equation element.
	 */
	const domBlend_equationRef getBlend_equation() const { return elemBlend_equation; }
	/**
	 * Gets the blend_equation_separate element.
	 * @return a daeSmartRef to the blend_equation_separate element.
	 */
	const domBlend_equation_separateRef getBlend_equation_separate() const { return elemBlend_equation_separate; }
	/**
	 * Gets the blend_func element.
	 * @return a daeSmartRef to the blend_func element.
	 */
	const domBlend_funcRef getBlend_func() const { return elemBlend_func; }
	/**
	 * Gets the blend_func_separate element.
	 * @return a daeSmartRef to the blend_func_separate element.
	 */
	const domBlend_func_separateRef getBlend_func_separate() const { return elemBlend_func_separate; }
	/**
	 * Gets the color_mask element.
	 * @return a daeSmartRef to the color_mask element.
	 */
	const domColor_maskRef getColor_mask() const { return elemColor_mask; }
	/**
	 * Gets the cull_face element.
	 * @return a daeSmartRef to the cull_face element.
	 */
	const domCull_faceRef getCull_face() const { return elemCull_face; }
	/**
	 * Gets the depth_func element.
	 * @return a daeSmartRef to the depth_func element.
	 */
	const domDepth_funcRef getDepth_func() const { return elemDepth_func; }
	/**
	 * Gets the depth_mask element.
	 * @return a daeSmartRef to the depth_mask element.
	 */
	const domDepth_maskRef getDepth_mask() const { return elemDepth_mask; }
	/**
	 * Gets the depth_range element.
	 * @return a daeSmartRef to the depth_range element.
	 */
	const domDepth_rangeRef getDepth_range() const { return elemDepth_range; }
	/**
	 * Gets the front_face element.
	 * @return a daeSmartRef to the front_face element.
	 */
	const domFront_faceRef getFront_face() const { return elemFront_face; }
	/**
	 * Gets the line_width element.
	 * @return a daeSmartRef to the line_width element.
	 */
	const domLine_widthRef getLine_width() const { return elemLine_width; }
	/**
	 * Gets the polygon_offset element.
	 * @return a daeSmartRef to the polygon_offset element.
	 */
	const domPolygon_offsetRef getPolygon_offset() const { return elemPolygon_offset; }
	/**
	 * Gets the point_size element.
	 * @return a daeSmartRef to the point_size element.
	 */
	const domPoint_sizeRef getPoint_size() const { return elemPoint_size; }
	/**
	 * Gets the sample_coverage element.
	 * @return a daeSmartRef to the sample_coverage element.
	 */
	const domSample_coverageRef getSample_coverage() const { return elemSample_coverage; }
	/**
	 * Gets the scissor element.
	 * @return a daeSmartRef to the scissor element.
	 */
	const domScissorRef getScissor() const { return elemScissor; }
	/**
	 * Gets the stencil_func element.
	 * @return a daeSmartRef to the stencil_func element.
	 */
	const domStencil_funcRef getStencil_func() const { return elemStencil_func; }
	/**
	 * Gets the stencil_func_separate element.
	 * @return a daeSmartRef to the stencil_func_separate element.
	 */
	const domStencil_func_separateRef getStencil_func_separate() const { return elemStencil_func_separate; }
	/**
	 * Gets the stencil_mask element.
	 * @return a daeSmartRef to the stencil_mask element.
	 */
	const domStencil_maskRef getStencil_mask() const { return elemStencil_mask; }
	/**
	 * Gets the stencil_mask_separate element.
	 * @return a daeSmartRef to the stencil_mask_separate element.
	 */
	const domStencil_mask_separateRef getStencil_mask_separate() const { return elemStencil_mask_separate; }
	/**
	 * Gets the stencil_op element.
	 * @return a daeSmartRef to the stencil_op element.
	 */
	const domStencil_opRef getStencil_op() const { return elemStencil_op; }
	/**
	 * Gets the stencil_op_separate element.
	 * @return a daeSmartRef to the stencil_op_separate element.
	 */
	const domStencil_op_separateRef getStencil_op_separate() const { return elemStencil_op_separate; }
	/**
	 * Gets the blend_enable element.
	 * @return a daeSmartRef to the blend_enable element.
	 */
	const domBlend_enableRef getBlend_enable() const { return elemBlend_enable; }
	/**
	 * Gets the cull_face_enable element.
	 * @return a daeSmartRef to the cull_face_enable element.
	 */
	const domCull_face_enableRef getCull_face_enable() const { return elemCull_face_enable; }
	/**
	 * Gets the depth_test_enable element.
	 * @return a daeSmartRef to the depth_test_enable element.
	 */
	const domDepth_test_enableRef getDepth_test_enable() const { return elemDepth_test_enable; }
	/**
	 * Gets the dither_enable element.
	 * @return a daeSmartRef to the dither_enable element.
	 */
	const domDither_enableRef getDither_enable() const { return elemDither_enable; }
	/**
	 * Gets the polygon_offset_fill_enable element.
	 * @return a daeSmartRef to the polygon_offset_fill_enable element.
	 */
	const domPolygon_offset_fill_enableRef getPolygon_offset_fill_enable() const { return elemPolygon_offset_fill_enable; }
	/**
	 * Gets the point_size_enable element.
	 * @return a daeSmartRef to the point_size_enable element.
	 */
	const domPoint_size_enableRef getPoint_size_enable() const { return elemPoint_size_enable; }
	/**
	 * Gets the sample_alpha_to_coverage_enable element.
	 * @return a daeSmartRef to the sample_alpha_to_coverage_enable element.
	 */
	const domSample_alpha_to_coverage_enableRef getSample_alpha_to_coverage_enable() const { return elemSample_alpha_to_coverage_enable; }
	/**
	 * Gets the sample_coverage_enable element.
	 * @return a daeSmartRef to the sample_coverage_enable element.
	 */
	const domSample_coverage_enableRef getSample_coverage_enable() const { return elemSample_coverage_enable; }
	/**
	 * Gets the scissor_test_enable element.
	 * @return a daeSmartRef to the scissor_test_enable element.
	 */
	const domScissor_test_enableRef getScissor_test_enable() const { return elemScissor_test_enable; }
	/**
	 * Gets the stencil_test_enable element.
	 * @return a daeSmartRef to the stencil_test_enable element.
	 */
	const domStencil_test_enableRef getStencil_test_enable() const { return elemStencil_test_enable; }
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
	domGles2_pipeline_settings(DAE& dae) : daeElement(dae), elemBlend_color(), elemBlend_equation(), elemBlend_equation_separate(), elemBlend_func(), elemBlend_func_separate(), elemColor_mask(), elemCull_face(), elemDepth_func(), elemDepth_mask(), elemDepth_range(), elemFront_face(), elemLine_width(), elemPolygon_offset(), elemPoint_size(), elemSample_coverage(), elemScissor(), elemStencil_func(), elemStencil_func_separate(), elemStencil_mask(), elemStencil_mask_separate(), elemStencil_op(), elemStencil_op_separate(), elemBlend_enable(), elemCull_face_enable(), elemDepth_test_enable(), elemDither_enable(), elemPolygon_offset_fill_enable(), elemPoint_size_enable(), elemSample_alpha_to_coverage_enable(), elemSample_coverage_enable(), elemScissor_test_enable(), elemStencil_test_enable() {}
	/**
	 * Destructor
	 */
	virtual ~domGles2_pipeline_settings() { daeElement::deleteCMDataArray(_CMData); }
	/**
	 * Overloaded assignment operator
	 */
	virtual domGles2_pipeline_settings &operator=( const domGles2_pipeline_settings &cpy ) { (void)cpy; return *this; }

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
