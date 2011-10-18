#ifndef __domProfile_gles_h__
#define __domProfile_gles_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domAsset.h>
#include <dom/domGles_newparam.h>
#include <dom/domFx_annotate.h>
#include <dom/domGles_pipeline_settings.h>
#include <dom/domFx_colortarget.h>
#include <dom/domFx_depthtarget.h>
#include <dom/domFx_stenciltarget.h>
#include <dom/domFx_clearcolor.h>
#include <dom/domFx_cleardepth.h>
#include <dom/domFx_clearstencil.h>
#include <dom/domExtra.h>
class DAE;

/**
 * Opens a block of GLES platform-specific data types and technique declarations.
 */
class domProfile_gles : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::PROFILE_GLES; }
	static daeInt ID() { return 273; }
	virtual daeInt typeID() const { return ID(); }
public:
	class domTechnique;

	typedef daeSmartRef<domTechnique> domTechniqueRef;
	typedef daeTArray<domTechniqueRef> domTechnique_Array;

/**
 * Holds a description of the textures, samplers, shaders, parameters, and
 * passes necessary for rendering this effect using one method.
 */
	class domTechnique : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::TECHNIQUE; }
		static daeInt ID() { return 274; }
		virtual daeInt typeID() const { return ID(); }
	public:
		class domPass;

		typedef daeSmartRef<domPass> domPassRef;
		typedef daeTArray<domPassRef> domPass_Array;

/**
 * A static declaration of all the render states, shaders, and settings for
 * one rendering pipeline.
 */
		class domPass : public daeElement
		{
		public:
			virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::PASS; }
			static daeInt ID() { return 275; }
			virtual daeInt typeID() const { return ID(); }
		public:
			class domStates;

			typedef daeSmartRef<domStates> domStatesRef;
			typedef daeTArray<domStatesRef> domStates_Array;

			class domStates : public daeElement
			{
			public:
				virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::STATES; }
				static daeInt ID() { return 276; }
				virtual daeInt typeID() const { return ID(); }

			protected:  // Element
				domGles_pipeline_settings_Array elemGles_pipeline_settings_array;

			public:	//Accessors and Mutators
				/**
				 * Gets the gles_pipeline_settings element array.
				 * @return Returns a reference to the array of gles_pipeline_settings elements.
				 */
				domGles_pipeline_settings_Array &getGles_pipeline_settings_array() { return elemGles_pipeline_settings_array; }
				/**
				 * Gets the gles_pipeline_settings element array.
				 * @return Returns a constant reference to the array of gles_pipeline_settings elements.
				 */
				const domGles_pipeline_settings_Array &getGles_pipeline_settings_array() const { return elemGles_pipeline_settings_array; }
			protected:
				/**
				 * Constructor
				 */
				domStates(DAE& dae) : daeElement(dae), elemGles_pipeline_settings_array() {}
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

			class domEvaluate;

			typedef daeSmartRef<domEvaluate> domEvaluateRef;
			typedef daeTArray<domEvaluateRef> domEvaluate_Array;

			class domEvaluate : public daeElement
			{
			public:
				virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::EVALUATE; }
				static daeInt ID() { return 277; }
				virtual daeInt typeID() const { return ID(); }
			public:
				class domDraw;

				typedef daeSmartRef<domDraw> domDrawRef;
				typedef daeTArray<domDrawRef> domDraw_Array;

				class domDraw : public daeElement
				{
				public:
					virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::DRAW; }
					static daeInt ID() { return 278; }
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
			void setSid( domSid atSid ) { *(daeStringRef*)&attrSid = atSid; _validAttributeArray[0] = true; }

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
			domPass(DAE& dae) : daeElement(dae), attrSid(), elemAnnotate_array(), elemStates(), elemEvaluate(), elemExtra_array() {}
			/**
			 * Destructor
			 */
			virtual ~domPass() {}
			/**
			 * Overloaded assignment operator
			 */
			virtual domPass &operator=( const domPass &cpy ) { (void)cpy; return *this; }

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
/**
 *  The sid attribute is a text string value containing the sub-identifier
 * of this element.  This value must be unique within the scope of the parent
 * element. 
 */
		domSid attrSid;

	protected:  // Elements
		domAssetRef elemAsset;
		domFx_annotate_Array elemAnnotate_array;
/**
 * A static declaration of all the render states, shaders, and settings for
 * one rendering pipeline. @see domPass
 */
		domPass_Array elemPass_array;
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
		 * Gets the asset element.
		 * @return a daeSmartRef to the asset element.
		 */
		const domAssetRef getAsset() const { return elemAsset; }
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
		 * Gets the pass element array.
		 * @return Returns a reference to the array of pass elements.
		 */
		domPass_Array &getPass_array() { return elemPass_array; }
		/**
		 * Gets the pass element array.
		 * @return Returns a constant reference to the array of pass elements.
		 */
		const domPass_Array &getPass_array() const { return elemPass_array; }
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
		domTechnique(DAE& dae) : daeElement(dae), attrId(), attrSid(), elemAsset(), elemAnnotate_array(), elemPass_array(), elemExtra_array() {}
		/**
		 * Destructor
		 */
		virtual ~domTechnique() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domTechnique &operator=( const domTechnique &cpy ) { (void)cpy; return *this; }

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
 * this element.  This value must be unique within the instance document.
 * Optional attribute. 
 */
	xsID attrId;
/**
 *  The type of platform. This is a vendor-defined character string that indicates
 * the platform or capability target for the technique. Optional 
 */
	xsNCName attrPlatform;

protected:  // Elements
	domAssetRef elemAsset;
	domGles_newparam_Array elemNewparam_array;
/**
 * Holds a description of the textures, samplers, shaders, parameters, and
 * passes necessary for rendering this effect using one method. @see domTechnique
 */
	domTechnique_Array elemTechnique_array;
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
	 * Gets the platform attribute.
	 * @return Returns a xsNCName of the platform attribute.
	 */
	xsNCName getPlatform() const { return attrPlatform; }
	/**
	 * Sets the platform attribute.
	 * @param atPlatform The new value for the platform attribute.
	 */
	void setPlatform( xsNCName atPlatform ) { *(daeStringRef*)&attrPlatform = atPlatform;}

	/**
	 * Gets the asset element.
	 * @return a daeSmartRef to the asset element.
	 */
	const domAssetRef getAsset() const { return elemAsset; }
	/**
	 * Gets the newparam element array.
	 * @return Returns a reference to the array of newparam elements.
	 */
	domGles_newparam_Array &getNewparam_array() { return elemNewparam_array; }
	/**
	 * Gets the newparam element array.
	 * @return Returns a constant reference to the array of newparam elements.
	 */
	const domGles_newparam_Array &getNewparam_array() const { return elemNewparam_array; }
	/**
	 * Gets the technique element array.
	 * @return Returns a reference to the array of technique elements.
	 */
	domTechnique_Array &getTechnique_array() { return elemTechnique_array; }
	/**
	 * Gets the technique element array.
	 * @return Returns a constant reference to the array of technique elements.
	 */
	const domTechnique_Array &getTechnique_array() const { return elemTechnique_array; }
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
	domProfile_gles(DAE& dae) : daeElement(dae), attrId(), attrPlatform(), elemAsset(), elemNewparam_array(), elemTechnique_array(), elemExtra_array() {}
	/**
	 * Destructor
	 */
	virtual ~domProfile_gles() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domProfile_gles &operator=( const domProfile_gles &cpy ) { (void)cpy; return *this; }

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
