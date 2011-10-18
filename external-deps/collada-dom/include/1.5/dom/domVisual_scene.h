#ifndef __domVisual_scene_h__
#define __domVisual_scene_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domAsset.h>
#include <dom/domNode.h>
#include <dom/domExtra.h>
class DAE;

/**
 * The visual_scene element declares the base of the visual_scene hierarchy
 * or scene graph. The  scene contains elements that comprise much of the
 * visual and transformational information  content as created by the authoring
 * tools.
 */
class domVisual_scene : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::VISUAL_SCENE; }
	static daeInt ID() { return 388; }
	virtual daeInt typeID() const { return ID(); }
public:
	class domEvaluate_scene;

	typedef daeSmartRef<domEvaluate_scene> domEvaluate_sceneRef;
	typedef daeTArray<domEvaluate_sceneRef> domEvaluate_scene_Array;

/**
 * The evaluate_scene element declares information specifying how to evaluate
 * this visual_scene. There may be any number of evaluate_scene elements.
 * They are evaluated in order and particular one may be disabled via setting
 * enabled=false.
 */
	class domEvaluate_scene : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::EVALUATE_SCENE; }
		static daeInt ID() { return 389; }
		virtual daeInt typeID() const { return ID(); }
	public:
		class domRender;

		typedef daeSmartRef<domRender> domRenderRef;
		typedef daeTArray<domRenderRef> domRender_Array;

/**
 * The render element describes one effect pass to evaluate the scene. There
 * must be at least one render element.
 */
		class domRender : public daeElement
		{
		public:
			virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::RENDER; }
			static daeInt ID() { return 390; }
			virtual daeInt typeID() const { return ID(); }
		public:
			class domLayer;

			typedef daeSmartRef<domLayer> domLayerRef;
			typedef daeTArray<domLayerRef> domLayer_Array;

/**
 * The layer element specifies which layer to render in this compositing step
 * while evaluating the scene. You may specify any number of layers.  No layers
 * mans that the entire scene will be used.
 */
			class domLayer : public daeElement
			{
			public:
				virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::LAYER; }
				static daeInt ID() { return 391; }
				virtual daeInt typeID() const { return ID(); }


			public:	//Accessors and Mutators
				/**
				 * Gets the value of this element.
				 * @return Returns a xsNCName of the value.
				 */
				xsNCName getValue() const { return _value; }
				/**
				 * Sets the _value of this element.
				 * @param val The new value for this element.
				 */
				void setValue( xsNCName val ) { *(daeStringRef*)&_value = val; }

			protected:  // Value
				/**
				 * The xsNCName value of the text data of this element. 
				 */
				xsNCName _value;
			protected:
				/**
				 * Constructor
				 */
				domLayer(DAE& dae) : daeElement(dae), _value() {}
				/**
				 * Destructor
				 */
				virtual ~domLayer() {}
				/**
				 * Overloaded assignment operator
				 */
				virtual domLayer &operator=( const domLayer &cpy ) { (void)cpy; return *this; }

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

			class domInstance_material;

			typedef daeSmartRef<domInstance_material> domInstance_materialRef;
			typedef daeTArray<domInstance_materialRef> domInstance_material_Array;

/**
 * The instance_material element specifies which effect to render in this
 * compositing step  while evaluating the scene.
 */
			class domInstance_material : public daeElement
			{
			public:
				virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::INSTANCE_MATERIAL; }
				static daeInt ID() { return 392; }
				virtual daeInt typeID() const { return ID(); }
			public:
				class domTechnique_override;

				typedef daeSmartRef<domTechnique_override> domTechnique_overrideRef;
				typedef daeTArray<domTechnique_overrideRef> domTechnique_override_Array;

				class domTechnique_override : public daeElement
				{
				public:
					virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::TECHNIQUE_OVERRIDE; }
					static daeInt ID() { return 393; }
					virtual daeInt typeID() const { return ID(); }
				protected:  // Attributes
/**
 * technique
 */
					xsNCName attrRef;
/**
 * Explicitly select one pass.  If empty then use all passes of the technique.
 */
					xsNCName attrPass;


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

					/**
					 * Gets the pass attribute.
					 * @return Returns a xsNCName of the pass attribute.
					 */
					xsNCName getPass() const { return attrPass; }
					/**
					 * Sets the pass attribute.
					 * @param atPass The new value for the pass attribute.
					 */
					void setPass( xsNCName atPass ) { *(daeStringRef*)&attrPass = atPass; _validAttributeArray[1] = true; }

				protected:
					/**
					 * Constructor
					 */
					domTechnique_override(DAE& dae) : daeElement(dae), attrRef(), attrPass() {}
					/**
					 * Destructor
					 */
					virtual ~domTechnique_override() {}
					/**
					 * Overloaded assignment operator
					 */
					virtual domTechnique_override &operator=( const domTechnique_override &cpy ) { (void)cpy; return *this; }

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

				class domBind;

				typedef daeSmartRef<domBind> domBindRef;
				typedef daeTArray<domBindRef> domBind_Array;

/**
 * The bind element binds values to effect parameters upon instantiation.
 */
				class domBind : public daeElement
				{
				public:
					virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::BIND; }
					static daeInt ID() { return 394; }
					virtual daeInt typeID() const { return ID(); }
				protected:  // Attributes
/**
 *  The semantic attribute specifies which effect parameter to bind. 
 */
					xsNCName attrSemantic;
/**
 *  The target attribute specifies the location of the value to bind to the
 * specified semantic.  This text string is a path-name following a simple
 * syntax described in the ?Addressing Syntax?  section. 
 */
					xsToken attrTarget;


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

					/**
					 * Gets the target attribute.
					 * @return Returns a xsToken of the target attribute.
					 */
					xsToken getTarget() const { return attrTarget; }
					/**
					 * Sets the target attribute.
					 * @param atTarget The new value for the target attribute.
					 */
					void setTarget( xsToken atTarget ) { *(daeStringRef*)&attrTarget = atTarget; _validAttributeArray[1] = true; }

				protected:
					/**
					 * Constructor
					 */
					domBind(DAE& dae) : daeElement(dae), attrSemantic(), attrTarget() {}
					/**
					 * Destructor
					 */
					virtual ~domBind() {}
					/**
					 * Overloaded assignment operator
					 */
					virtual domBind &operator=( const domBind &cpy ) { (void)cpy; return *this; }

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
 * URL to a material
 */
				xsAnyURI attrUrl;

			protected:  // Elements
				domTechnique_overrideRef elemTechnique_override;
/**
 * The bind element binds values to effect parameters upon instantiation.
 * @see domBind
 */
				domBind_Array elemBind_array;
/**
 * The extra element may appear any number of times. @see domExtra
 */
				domExtra_Array elemExtra_array;

			public:	//Accessors and Mutators
				/**
				 * Gets the url attribute.
				 * @return Returns a xsAnyURI reference of the url attribute.
				 */
				xsAnyURI &getUrl() { return attrUrl; }
				/**
				 * Gets the url attribute.
				 * @return Returns a constant xsAnyURI reference of the url attribute.
				 */
				const xsAnyURI &getUrl() const { return attrUrl; }
				/**
				 * Sets the url attribute.
				 * @param atUrl The new value for the url attribute.
				 */
				void setUrl( const xsAnyURI &atUrl ) { attrUrl = atUrl; _validAttributeArray[0] = true; }
				/**
				 * Sets the url attribute.
				 * @param atUrl The new value for the url attribute.
				 */
				void setUrl( xsString atUrl ) { attrUrl = atUrl; _validAttributeArray[0] = true; }

				/**
				 * Gets the technique_override element.
				 * @return a daeSmartRef to the technique_override element.
				 */
				const domTechnique_overrideRef getTechnique_override() const { return elemTechnique_override; }
				/**
				 * Gets the bind element array.
				 * @return Returns a reference to the array of bind elements.
				 */
				domBind_Array &getBind_array() { return elemBind_array; }
				/**
				 * Gets the bind element array.
				 * @return Returns a constant reference to the array of bind elements.
				 */
				const domBind_Array &getBind_array() const { return elemBind_array; }
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
				domInstance_material(DAE& dae) : daeElement(dae), attrUrl(dae, *this), elemTechnique_override(), elemBind_array(), elemExtra_array() {}
				/**
				 * Destructor
				 */
				virtual ~domInstance_material() {}
				/**
				 * Overloaded assignment operator
				 */
				virtual domInstance_material &operator=( const domInstance_material &cpy ) { (void)cpy; return *this; }

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
			domSid attrSid;
/**
 *  The name attribute is the text string name of this element. Optional attribute.
 */
			xsToken attrName;
/**
 * The camera_node attribute refers to a node that contains a camera describing
 * the viewpoint to render this compositing step from.
 */
			xsAnyURI attrCamera_node;

		protected:  // Elements
/**
 * The layer element specifies which layer to render in this compositing step
 * while evaluating the scene. You may specify any number of layers.  No layers
 * mans that the entire scene will be used. @see domLayer
 */
			domLayer_Array elemLayer_array;
/**
 * The instance_material element specifies which effect to render in this
 * compositing step  while evaluating the scene. @see domInstance_material
 */
			domInstance_materialRef elemInstance_material;
/**
 * The extra element may appear any number of times. @see domExtra
 */
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
			void setSid( domSid atSid ) { *(daeStringRef*)&attrSid = atSid; _validAttributeArray[0] = true; }

			/**
			 * Gets the name attribute.
			 * @return Returns a xsToken of the name attribute.
			 */
			xsToken getName() const { return attrName; }
			/**
			 * Sets the name attribute.
			 * @param atName The new value for the name attribute.
			 */
			void setName( xsToken atName ) { *(daeStringRef*)&attrName = atName; _validAttributeArray[1] = true; }

			/**
			 * Gets the camera_node attribute.
			 * @return Returns a xsAnyURI reference of the camera_node attribute.
			 */
			xsAnyURI &getCamera_node() { return attrCamera_node; }
			/**
			 * Gets the camera_node attribute.
			 * @return Returns a constant xsAnyURI reference of the camera_node attribute.
			 */
			const xsAnyURI &getCamera_node() const { return attrCamera_node; }
			/**
			 * Sets the camera_node attribute.
			 * @param atCamera_node The new value for the camera_node attribute.
			 */
			void setCamera_node( const xsAnyURI &atCamera_node ) { attrCamera_node = atCamera_node; _validAttributeArray[2] = true; }
			/**
			 * Sets the camera_node attribute.
			 * @param atCamera_node The new value for the camera_node attribute.
			 */
			void setCamera_node( xsString atCamera_node ) { attrCamera_node = atCamera_node; _validAttributeArray[2] = true; }

			/**
			 * Gets the layer element array.
			 * @return Returns a reference to the array of layer elements.
			 */
			domLayer_Array &getLayer_array() { return elemLayer_array; }
			/**
			 * Gets the layer element array.
			 * @return Returns a constant reference to the array of layer elements.
			 */
			const domLayer_Array &getLayer_array() const { return elemLayer_array; }
			/**
			 * Gets the instance_material element.
			 * @return a daeSmartRef to the instance_material element.
			 */
			const domInstance_materialRef getInstance_material() const { return elemInstance_material; }
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
			domRender(DAE& dae) : daeElement(dae), attrSid(), attrName(), attrCamera_node(dae, *this), elemLayer_array(), elemInstance_material(), elemExtra_array() {}
			/**
			 * Destructor
			 */
			virtual ~domRender() {}
			/**
			 * Overloaded assignment operator
			 */
			virtual domRender &operator=( const domRender &cpy ) { (void)cpy; return *this; }

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
		xsID attrId;
		domSid attrSid;
		xsToken attrName;
/**
 * allow this to be disabled and hense skipped durring evaluation.  This is
 * useful for debug.
 */
		xsBoolean attrEnable;

	protected:  // Elements
		domAssetRef elemAsset;
/**
 * The render element describes one effect pass to evaluate the scene. There
 * must be at least one render element. @see domRender
 */
		domRender_Array elemRender_array;
/**
 * The extra element may appear any number of times. @see domExtra
 */
		domExtra_Array elemExtra_array;

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
		void setId( xsID atId ) { *(daeStringRef*)&attrId = atId; _validAttributeArray[0] = true; 
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
		void setSid( domSid atSid ) { *(daeStringRef*)&attrSid = atSid; _validAttributeArray[1] = true; }

		/**
		 * Gets the name attribute.
		 * @return Returns a xsToken of the name attribute.
		 */
		xsToken getName() const { return attrName; }
		/**
		 * Sets the name attribute.
		 * @param atName The new value for the name attribute.
		 */
		void setName( xsToken atName ) { *(daeStringRef*)&attrName = atName; _validAttributeArray[2] = true; }

		/**
		 * Gets the enable attribute.
		 * @return Returns a xsBoolean of the enable attribute.
		 */
		xsBoolean getEnable() const { return attrEnable; }
		/**
		 * Sets the enable attribute.
		 * @param atEnable The new value for the enable attribute.
		 */
		void setEnable( xsBoolean atEnable ) { attrEnable = atEnable; _validAttributeArray[3] = true; }

		/**
		 * Gets the asset element.
		 * @return a daeSmartRef to the asset element.
		 */
		const domAssetRef getAsset() const { return elemAsset; }
		/**
		 * Gets the render element array.
		 * @return Returns a reference to the array of render elements.
		 */
		domRender_Array &getRender_array() { return elemRender_array; }
		/**
		 * Gets the render element array.
		 * @return Returns a constant reference to the array of render elements.
		 */
		const domRender_Array &getRender_array() const { return elemRender_array; }
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
		domEvaluate_scene(DAE& dae) : daeElement(dae), attrId(), attrSid(), attrName(), attrEnable(), elemAsset(), elemRender_array(), elemExtra_array() {}
		/**
		 * Destructor
		 */
		virtual ~domEvaluate_scene() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domEvaluate_scene &operator=( const domEvaluate_scene &cpy ) { (void)cpy; return *this; }

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
 * this element. This  value must be unique within the instance document.
 * Optional attribute. 
 */
	xsID attrId;
/**
 *  The name attribute is the text string name of this element. Optional attribute.
 */
	xsToken attrName;

protected:  // Elements
/**
 * The visual_scene element may contain an asset element. @see domAsset
 */
	domAssetRef elemAsset;
/**
 * The visual_scene element must have at least one node element. @see domNode
 */
	domNode_Array elemNode_array;
/**
 * The evaluate_scene element declares information specifying how to evaluate
 * this visual_scene. There may be any number of evaluate_scene elements.
 * They are evaluated in order and particular one may be disabled via setting
 * enabled=false. @see domEvaluate_scene
 */
	domEvaluate_scene_Array elemEvaluate_scene_array;
/**
 * The extra element may appear any number of times. @see domExtra
 */
	domExtra_Array elemExtra_array;

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
	 * Gets the node element array.
	 * @return Returns a reference to the array of node elements.
	 */
	domNode_Array &getNode_array() { return elemNode_array; }
	/**
	 * Gets the node element array.
	 * @return Returns a constant reference to the array of node elements.
	 */
	const domNode_Array &getNode_array() const { return elemNode_array; }
	/**
	 * Gets the evaluate_scene element array.
	 * @return Returns a reference to the array of evaluate_scene elements.
	 */
	domEvaluate_scene_Array &getEvaluate_scene_array() { return elemEvaluate_scene_array; }
	/**
	 * Gets the evaluate_scene element array.
	 * @return Returns a constant reference to the array of evaluate_scene elements.
	 */
	const domEvaluate_scene_Array &getEvaluate_scene_array() const { return elemEvaluate_scene_array; }
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
	domVisual_scene(DAE& dae) : daeElement(dae), attrId(), attrName(), elemAsset(), elemNode_array(), elemEvaluate_scene_array(), elemExtra_array() {}
	/**
	 * Destructor
	 */
	virtual ~domVisual_scene() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domVisual_scene &operator=( const domVisual_scene &cpy ) { (void)cpy; return *this; }

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
