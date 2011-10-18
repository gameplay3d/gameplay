#ifndef __domGlsl_program_h__
#define __domGlsl_program_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domGlsl_shader.h>
#include <dom/domGlsl_value.h>
class DAE;

/**
 * a program is one or more shaders linked together
 */
class domGlsl_program : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::GLSL_PROGRAM; }
	static daeInt ID() { return 232; }
	virtual daeInt typeID() const { return ID(); }
public:
	class domBind_attribute;

	typedef daeSmartRef<domBind_attribute> domBind_attributeRef;
	typedef daeTArray<domBind_attributeRef> domBind_attribute_Array;

/**
 * Bind attributes.  If not bound then it is assumed to be the same semantic
 * as the variable name.  If the variable name is prefixed with "gl_" or "sv_"
 * then the prefix may be ignored.
 */
	class domBind_attribute : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::BIND_ATTRIBUTE; }
		static daeInt ID() { return 233; }
		virtual daeInt typeID() const { return ID(); }
	public:
		class domSemantic;

		typedef daeSmartRef<domSemantic> domSemanticRef;
		typedef daeTArray<domSemanticRef> domSemantic_Array;

/**
 * Bind attributes to semantics to enable semantic association to attributes
 * that are not redeclared as parameters.  Without this, the semantic is the
 * same as the attribute's name.
 */
		class domSemantic : public daeElement
		{
		public:
			virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::SEMANTIC; }
			static daeInt ID() { return 234; }
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
			domSemantic(DAE& dae) : daeElement(dae), _value() {}
			/**
			 * Destructor
			 */
			virtual ~domSemantic() {}
			/**
			 * Overloaded assignment operator
			 */
			virtual domSemantic &operator=( const domSemantic &cpy ) { (void)cpy; return *this; }

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
 * shader variable name
 */
		xsToken attrSymbol;

	protected:  // Element
/**
 * Bind attributes to semantics to enable semantic association to attributes
 * that are not redeclared as parameters.  Without this, the semantic is the
 * same as the attribute's name. @see domSemantic
 */
		domSemanticRef elemSemantic;
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
		 * @return Returns a xsToken of the symbol attribute.
		 */
		xsToken getSymbol() const { return attrSymbol; }
		/**
		 * Sets the symbol attribute.
		 * @param atSymbol The new value for the symbol attribute.
		 */
		void setSymbol( xsToken atSymbol ) { *(daeStringRef*)&attrSymbol = atSymbol; _validAttributeArray[0] = true; }

		/**
		 * Gets the semantic element.
		 * @return a daeSmartRef to the semantic element.
		 */
		const domSemanticRef getSemantic() const { return elemSemantic; }
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
		domBind_attribute(DAE& dae) : daeElement(dae), attrSymbol(), elemSemantic() {}
		/**
		 * Destructor
		 */
		virtual ~domBind_attribute() { daeElement::deleteCMDataArray(_CMData); }
		/**
		 * Overloaded assignment operator
		 */
		virtual domBind_attribute &operator=( const domBind_attribute &cpy ) { (void)cpy; return *this; }

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
 * Bind uniforms and samplers
 */
	class domBind_uniform : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::BIND_UNIFORM; }
		static daeInt ID() { return 235; }
		virtual daeInt typeID() const { return ID(); }
	public:
		class domParam;

		typedef daeSmartRef<domParam> domParamRef;
		typedef daeTArray<domParamRef> domParam_Array;

/**
 * Bind uniforms to parameters
 */
		class domParam : public daeElement
		{
		public:
			virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::PARAM; }
			static daeInt ID() { return 236; }
			virtual daeInt typeID() const { return ID(); }
		protected:  // Attribute
/**
 * SID of parameter
 */
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
 * shader variable name
 */
		xsToken attrSymbol;

	protected:  // Elements
/**
 * Bind uniforms to parameters @see domParam
 */
		domParamRef elemParam;
		domGlsl_valueRef elemGlsl_value;
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
		 * @return Returns a xsToken of the symbol attribute.
		 */
		xsToken getSymbol() const { return attrSymbol; }
		/**
		 * Sets the symbol attribute.
		 * @param atSymbol The new value for the symbol attribute.
		 */
		void setSymbol( xsToken atSymbol ) { *(daeStringRef*)&attrSymbol = atSymbol; _validAttributeArray[0] = true; }

		/**
		 * Gets the param element.
		 * @return a daeSmartRef to the param element.
		 */
		const domParamRef getParam() const { return elemParam; }
		/**
		 * Gets the glsl_value element.
		 * @return a daeSmartRef to the glsl_value element.
		 */
		const domGlsl_valueRef getGlsl_value() const { return elemGlsl_value; }
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
		domBind_uniform(DAE& dae) : daeElement(dae), attrSymbol(), elemParam(), elemGlsl_value() {}
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



protected:  // Elements
/**
 * compile shader stages @see domShader
 */
	domGlsl_shader_Array elemShader_array;
/**
 * Bind attributes.  If not bound then it is assumed to be the same semantic
 * as the variable name.  If the variable name is prefixed with "gl_" or "sv_"
 * then the prefix may be ignored. @see domBind_attribute
 */
	domBind_attribute_Array elemBind_attribute_array;
/**
 * Bind uniforms and samplers @see domBind_uniform
 */
	domBind_uniform_Array elemBind_uniform_array;

public:	//Accessors and Mutators
	/**
	 * Gets the shader element array.
	 * @return Returns a reference to the array of shader elements.
	 */
	domGlsl_shader_Array &getShader_array() { return elemShader_array; }
	/**
	 * Gets the shader element array.
	 * @return Returns a constant reference to the array of shader elements.
	 */
	const domGlsl_shader_Array &getShader_array() const { return elemShader_array; }
	/**
	 * Gets the bind_attribute element array.
	 * @return Returns a reference to the array of bind_attribute elements.
	 */
	domBind_attribute_Array &getBind_attribute_array() { return elemBind_attribute_array; }
	/**
	 * Gets the bind_attribute element array.
	 * @return Returns a constant reference to the array of bind_attribute elements.
	 */
	const domBind_attribute_Array &getBind_attribute_array() const { return elemBind_attribute_array; }
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
	domGlsl_program(DAE& dae) : daeElement(dae), elemShader_array(), elemBind_attribute_array(), elemBind_uniform_array() {}
	/**
	 * Destructor
	 */
	virtual ~domGlsl_program() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domGlsl_program &operator=( const domGlsl_program &cpy ) { (void)cpy; return *this; }

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
