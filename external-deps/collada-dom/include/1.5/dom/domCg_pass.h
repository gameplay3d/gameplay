#ifndef __domCg_pass_h__
#define __domCg_pass_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domFx_annotate.h>
#include <dom/domGl_pipeline_settings.h>
#include <dom/domFx_sources.h>
#include <dom/domFx_target.h>
#include <dom/domCg_param.h>
#include <dom/domFx_colortarget.h>
#include <dom/domFx_depthtarget.h>
#include <dom/domFx_stenciltarget.h>
#include <dom/domFx_clearcolor.h>
#include <dom/domFx_cleardepth.h>
#include <dom/domFx_clearstencil.h>
#include <dom/domExtra.h>
class DAE;

class domCg_pass : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::CG_PASS; }
	static daeInt ID() { return 249; }
	virtual daeInt typeID() const { return ID(); }
public:
	class domStates;

	typedef daeSmartRef<domStates> domStatesRef;
	typedef daeTArray<domStatesRef> domStates_Array;

	class domStates : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::STATES; }
		static daeInt ID() { return 250; }
		virtual daeInt typeID() const { return ID(); }

	protected:  // Element
		domGl_pipeline_settings_Array elemGl_pipeline_settings_array;

	public:	//Accessors and Mutators
		/**
		 * Gets the gl_pipeline_settings element array.
		 * @return Returns a reference to the array of gl_pipeline_settings elements.
		 */
		domGl_pipeline_settings_Array &getGl_pipeline_settings_array() { return elemGl_pipeline_settings_array; }
		/**
		 * Gets the gl_pipeline_settings element array.
		 * @return Returns a constant reference to the array of gl_pipeline_settings elements.
		 */
		const domGl_pipeline_settings_Array &getGl_pipeline_settings_array() const { return elemGl_pipeline_settings_array; }
	protected:
		/**
		 * Constructor
		 */
		domStates(DAE& dae) : daeElement(dae), elemGl_pipeline_settings_array() {}
		/**
		 * Destructor
		 */
		virtual ~domStates() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domStates &operator=( const domStates &cpy ) { (void)cpy; return *this; }

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

	class domProgram;

	typedef daeSmartRef<domProgram> domProgramRef;
	typedef daeTArray<domProgramRef> domProgram_Array;

	class domProgram : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::PROGRAM; }
		static daeInt ID() { return 251; }
		virtual daeInt typeID() const { return ID(); }
	public:
		class domShader;

		typedef daeSmartRef<domShader> domShaderRef;
		typedef daeTArray<domShaderRef> domShader_Array;

/**
 * Declare and prepare a shader for execution in the rendering pipeline of
 * a pass.
 */
		class domShader : public daeElement
		{
		public:
			virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::SHADER; }
			static daeInt ID() { return 252; }
			virtual daeInt typeID() const { return ID(); }
		public:
			class domSources;

			typedef daeSmartRef<domSources> domSourcesRef;
			typedef daeTArray<domSourcesRef> domSources_Array;

/**
 * collect the sources together for the shader.  Use concat to make the final
 * string.
 */
			class domSources : public domFx_sources
			{
			public:
				virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::SOURCES; }
				static daeInt ID() { return 253; }
				virtual daeInt typeID() const { return ID(); }
			protected:  // Attribute
/**
 * entry function name for this shader
 */
				xsToken attrEntry;


			public:	//Accessors and Mutators
				/**
				 * Gets the entry attribute.
				 * @return Returns a xsToken of the entry attribute.
				 */
				xsToken getEntry() const { return attrEntry; }
				/**
				 * Sets the entry attribute.
				 * @param atEntry The new value for the entry attribute.
				 */
				void setEntry( xsToken atEntry ) { *(daeStringRef*)&attrEntry = atEntry; _validAttributeArray[0] = true; }

			protected:
				/**
				 * Constructor
				 */
				domSources(DAE& dae) : domFx_sources(dae), attrEntry() {}
				/**
				 * Destructor
				 */
				virtual ~domSources() {}
				/**
				 * Overloaded assignment operator
				 */
				virtual domSources &operator=( const domSources &cpy ) { (void)cpy; return *this; }

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

			class domBind_uniform;

			typedef daeSmartRef<domBind_uniform> domBind_uniformRef;
			typedef daeTArray<domBind_uniformRef> domBind_uniform_Array;

/**
 * Binds values to uniform inputs of a shader.
 */
			class domBind_uniform : public daeElement
			{
			public:
				virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::BIND_UNIFORM; }
				static daeInt ID() { return 254; }
				virtual daeInt typeID() const { return ID(); }
			public:
				class domParam;

				typedef daeSmartRef<domParam> domParamRef;
				typedef daeTArray<domParamRef> domParam_Array;

/**
 * References a predefined parameter in shader binding declarations.
 */
				class domParam : public daeElement
				{
				public:
					virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::PARAM; }
					static daeInt ID() { return 255; }
					virtual daeInt typeID() const { return ID(); }
				protected:  // Attribute
					xsNCName attrRef;


				public:	//Accessors and Mutators
					/**
					 * Gets the ref attribute.
					 * @return Returns a xsNCName of the ref attribute.
					 */
					xsNCName getRef() const { return attrRef; }
					/**
					 * Sets the ref attribute.
					 * @param atRef The new value for the ref attribute.
					 */
					void setRef( xsNCName atRef ) { *(daeStringRef*)&attrRef = atRef; _validAttributeArray[0] = true; }

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


			protected:  // Attribute
/**
 *  The identifier for a uniform input parameter to the shader (a formal function
 * parameter or in-scope  global) that will be bound to an external resource.
 */
				xsNCName attrSymbol;

			protected:  // Elements
/**
 * References a predefined parameter in shader binding declarations. @see
 * domParam
 */
				domParamRef elemParam;
				domCg_paramRef elemCg_param;
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
				 * Gets the symbol attribute.
				 * @return Returns a xsNCName of the symbol attribute.
				 */
				xsNCName getSymbol() const { return attrSymbol; }
				/**
				 * Sets the symbol attribute.
				 * @param atSymbol The new value for the symbol attribute.
				 */
				void setSymbol( xsNCName atSymbol ) { *(daeStringRef*)&attrSymbol = atSymbol; _validAttributeArray[0] = true; }

				/**
				 * Gets the param element.
				 * @return a daeSmartRef to the param element.
				 */
				const domParamRef getParam() const { return elemParam; }
				/**
				 * Gets the cg_param element.
				 * @return a daeSmartRef to the cg_param element.
				 */
				const domCg_paramRef getCg_param() const { return elemCg_param; }
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
				domBind_uniform(DAE& dae) : daeElement(dae), attrSymbol(), elemParam(), elemCg_param() {}
				/**
				 * Destructor
				 */
				virtual ~domBind_uniform() { daeElement::deleteCMDataArray(_CMData); }
				/**
				 * Overloaded assignment operator
				 */
				virtual domBind_uniform &operator=( const domBind_uniform &cpy ) { (void)cpy; return *this; }

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


		protected:  // Attribute
/**
 *  In which pipeline stage this programmable shader is designed to execute,
 * for example, VERTEX, FRAGMENT, etc. 
 */
			domFx_pipeline_stage attrStage;

		protected:  // Elements
/**
 * collect the sources together for the shader.  Use concat to make the final
 * string. @see domSources
 */
			domSourcesRef elemSources;
			domFx_target_Array elemCompiler_array;
/**
 * Binds values to uniform inputs of a shader. @see domBind_uniform
 */
			domBind_uniform_Array elemBind_uniform_array;

		public:	//Accessors and Mutators
			/**
			 * Gets the stage attribute.
			 * @return Returns a domFx_pipeline_stage of the stage attribute.
			 */
			domFx_pipeline_stage getStage() const { return attrStage; }
			/**
			 * Sets the stage attribute.
			 * @param atStage The new value for the stage attribute.
			 */
			void setStage( domFx_pipeline_stage atStage ) { attrStage = atStage; _validAttributeArray[0] = true; }

			/**
			 * Gets the sources element.
			 * @return a daeSmartRef to the sources element.
			 */
			const domSourcesRef getSources() const { return elemSources; }
			/**
			 * Gets the compiler element array.
			 * @return Returns a reference to the array of compiler elements.
			 */
			domFx_target_Array &getCompiler_array() { return elemCompiler_array; }
			/**
			 * Gets the compiler element array.
			 * @return Returns a constant reference to the array of compiler elements.
			 */
			const domFx_target_Array &getCompiler_array() const { return elemCompiler_array; }
			/**
			 * Gets the bind_uniform element array.
			 * @return Returns a reference to the array of bind_uniform elements.
			 */
			domBind_uniform_Array &getBind_uniform_array() { return elemBind_uniform_array; }
			/**
			 * Gets the bind_uniform element array.
			 * @return Returns a constant reference to the array of bind_uniform elements.
			 */
			const domBind_uniform_Array &getBind_uniform_array() const { return elemBind_uniform_array; }
		protected:
			/**
			 * Constructor
			 */
			domShader(DAE& dae) : daeElement(dae), attrStage(), elemSources(), elemCompiler_array(), elemBind_uniform_array() {}
			/**
			 * Destructor
			 */
			virtual ~domShader() {}
			/**
			 * Overloaded assignment operator
			 */
			virtual domShader &operator=( const domShader &cpy ) { (void)cpy; return *this; }

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



	protected:  // Element
/**
 * Declare and prepare a shader for execution in the rendering pipeline of
 * a pass. @see domShader
 */
		domShader_Array elemShader_array;

	public:	//Accessors and Mutators
		/**
		 * Gets the shader element array.
		 * @return Returns a reference to the array of shader elements.
		 */
		domShader_Array &getShader_array() { return elemShader_array; }
		/**
		 * Gets the shader element array.
		 * @return Returns a constant reference to the array of shader elements.
		 */
		const domShader_Array &getShader_array() const { return elemShader_array; }
	protected:
		/**
		 * Constructor
		 */
		domProgram(DAE& dae) : daeElement(dae), elemShader_array() {}
		/**
		 * Destructor
		 */
		virtual ~domProgram() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domProgram &operator=( const domProgram &cpy ) { (void)cpy; return *this; }

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

	class domEvaluate;

	typedef daeSmartRef<domEvaluate> domEvaluateRef;
	typedef daeTArray<domEvaluateRef> domEvaluate_Array;

	class domEvaluate : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::EVALUATE; }
		static daeInt ID() { return 256; }
		virtual daeInt typeID() const { return ID(); }
	public:
		class domDraw;

		typedef daeSmartRef<domDraw> domDrawRef;
		typedef daeTArray<domDrawRef> domDraw_Array;

		class domDraw : public daeElement
		{
		public:
			virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::DRAW; }
			static daeInt ID() { return 257; }
			virtual daeInt typeID() const { return ID(); }


		public:	//Accessors and Mutators
			/**
			 * Gets the value of this element.
			 * @return a domFx_draw of the value.
			 */
			domFx_draw& getValue() { return _value; }
			/**
			 * Sets the _value of this element.
			 * @param val The new value for this element.
			 */
			void setValue( const domFx_draw& val ) { _value = val; }

		protected:  // Value
			/**
			 * The domFx_draw value of the text data of this element. 
			 */
			domFx_draw _value;
		protected:
			/**
			 * Constructor
			 */
			domDraw(DAE& dae) : daeElement(dae), _value() {}
			/**
			 * Destructor
			 */
			virtual ~domDraw() {}
			/**
			 * Overloaded assignment operator
			 */
			virtual domDraw &operator=( const domDraw &cpy ) { (void)cpy; return *this; }

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
		domFx_colortarget_Array elemColor_target_array;
		domFx_depthtarget_Array elemDepth_target_array;
		domFx_stenciltarget_Array elemStencil_target_array;
		domFx_clearcolor_Array elemColor_clear_array;
		domFx_cleardepth_Array elemDepth_clear_array;
		domFx_clearstencil_Array elemStencil_clear_array;
		domDrawRef elemDraw;

	public:	//Accessors and Mutators
		/**
		 * Gets the color_target element array.
		 * @return Returns a reference to the array of color_target elements.
		 */
		domFx_colortarget_Array &getColor_target_array() { return elemColor_target_array; }
		/**
		 * Gets the color_target element array.
		 * @return Returns a constant reference to the array of color_target elements.
		 */
		const domFx_colortarget_Array &getColor_target_array() const { return elemColor_target_array; }
		/**
		 * Gets the depth_target element array.
		 * @return Returns a reference to the array of depth_target elements.
		 */
		domFx_depthtarget_Array &getDepth_target_array() { return elemDepth_target_array; }
		/**
		 * Gets the depth_target element array.
		 * @return Returns a constant reference to the array of depth_target elements.
		 */
		const domFx_depthtarget_Array &getDepth_target_array() const { return elemDepth_target_array; }
		/**
		 * Gets the stencil_target element array.
		 * @return Returns a reference to the array of stencil_target elements.
		 */
		domFx_stenciltarget_Array &getStencil_target_array() { return elemStencil_target_array; }
		/**
		 * Gets the stencil_target element array.
		 * @return Returns a constant reference to the array of stencil_target elements.
		 */
		const domFx_stenciltarget_Array &getStencil_target_array() const { return elemStencil_target_array; }
		/**
		 * Gets the color_clear element array.
		 * @return Returns a reference to the array of color_clear elements.
		 */
		domFx_clearcolor_Array &getColor_clear_array() { return elemColor_clear_array; }
		/**
		 * Gets the color_clear element array.
		 * @return Returns a constant reference to the array of color_clear elements.
		 */
		const domFx_clearcolor_Array &getColor_clear_array() const { return elemColor_clear_array; }
		/**
		 * Gets the depth_clear element array.
		 * @return Returns a reference to the array of depth_clear elements.
		 */
		domFx_cleardepth_Array &getDepth_clear_array() { return elemDepth_clear_array; }
		/**
		 * Gets the depth_clear element array.
		 * @return Returns a constant reference to the array of depth_clear elements.
		 */
		const domFx_cleardepth_Array &getDepth_clear_array() const { return elemDepth_clear_array; }
		/**
		 * Gets the stencil_clear element array.
		 * @return Returns a reference to the array of stencil_clear elements.
		 */
		domFx_clearstencil_Array &getStencil_clear_array() { return elemStencil_clear_array; }
		/**
		 * Gets the stencil_clear element array.
		 * @return Returns a constant reference to the array of stencil_clear elements.
		 */
		const domFx_clearstencil_Array &getStencil_clear_array() const { return elemStencil_clear_array; }
		/**
		 * Gets the draw element.
		 * @return a daeSmartRef to the draw element.
		 */
		const domDrawRef getDraw() const { return elemDraw; }
	protected:
		/**
		 * Constructor
		 */
		domEvaluate(DAE& dae) : daeElement(dae), elemColor_target_array(), elemDepth_target_array(), elemStencil_target_array(), elemColor_clear_array(), elemDepth_clear_array(), elemStencil_clear_array(), elemDraw() {}
		/**
		 * Destructor
		 */
		virtual ~domEvaluate() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domEvaluate &operator=( const domEvaluate &cpy ) { (void)cpy; return *this; }

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


protected:  // Attribute
/**
 *  The sid attribute is a text string value containing the sub-identifier
 * of this element.  This value must be unique within the scope of the parent
 * element. Optional attribute. 
 */
	domSid attrSid;

protected:  // Elements
	domFx_annotate_Array elemAnnotate_array;
	domStatesRef elemStates;
	domProgramRef elemProgram;
	domEvaluateRef elemEvaluate;
	domExtra_Array elemExtra_array;

public:	//Accessors and Mutators
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
	 * Gets the annotate element array.
	 * @return Returns a reference to the array of annotate elements.
	 */
	domFx_annotate_Array &getAnnotate_array() { return elemAnnotate_array; }
	/**
	 * Gets the annotate element array.
	 * @return Returns a constant reference to the array of annotate elements.
	 */
	const domFx_annotate_Array &getAnnotate_array() const { return elemAnnotate_array; }
	/**
	 * Gets the states element.
	 * @return a daeSmartRef to the states element.
	 */
	const domStatesRef getStates() const { return elemStates; }
	/**
	 * Gets the program element.
	 * @return a daeSmartRef to the program element.
	 */
	const domProgramRef getProgram() const { return elemProgram; }
	/**
	 * Gets the evaluate element.
	 * @return a daeSmartRef to the evaluate element.
	 */
	const domEvaluateRef getEvaluate() const { return elemEvaluate; }
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
	domCg_pass(DAE& dae) : daeElement(dae), attrSid(), elemAnnotate_array(), elemStates(), elemProgram(), elemEvaluate(), elemExtra_array() {}
	/**
	 * Destructor
	 */
	virtual ~domCg_pass() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domCg_pass &operator=( const domCg_pass &cpy ) { (void)cpy; return *this; }

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
