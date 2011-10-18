#ifndef __domGles2_pass_h__
#define __domGles2_pass_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domFx_annotate.h>
#include <dom/domGles2_pipeline_settings.h>
#include <dom/domGles2_program.h>
#include <dom/domFx_colortarget.h>
#include <dom/domFx_depthtarget.h>
#include <dom/domFx_stenciltarget.h>
#include <dom/domFx_clearcolor.h>
#include <dom/domFx_clearstencil.h>
#include <dom/domFx_cleardepth.h>
#include <dom/domExtra.h>
class DAE;

/**
 * Set up pipeline state and shaders for rendering
 */
class domGles2_pass : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::GLES2_PASS; }
	static daeInt ID() { return 220; }
	virtual daeInt typeID() const { return ID(); }
public:
	class domStates;

	typedef daeSmartRef<domStates> domStatesRef;
	typedef daeTArray<domStatesRef> domStates_Array;

/**
 * API state
 */
	class domStates : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::STATES; }
		static daeInt ID() { return 221; }
		virtual daeInt typeID() const { return ID(); }

	protected:  // Element
		domGles2_pipeline_settings_Array elemGles2_pipeline_settings_array;

	public:	//Accessors and Mutators
		/**
		 * Gets the gles2_pipeline_settings element array.
		 * @return Returns a reference to the array of gles2_pipeline_settings elements.
		 */
		domGles2_pipeline_settings_Array &getGles2_pipeline_settings_array() { return elemGles2_pipeline_settings_array; }
		/**
		 * Gets the gles2_pipeline_settings element array.
		 * @return Returns a constant reference to the array of gles2_pipeline_settings elements.
		 */
		const domGles2_pipeline_settings_Array &getGles2_pipeline_settings_array() const { return elemGles2_pipeline_settings_array; }
	protected:
		/**
		 * Constructor
		 */
		domStates(DAE& dae) : daeElement(dae), elemGles2_pipeline_settings_array() {}
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

/**
 * runtime execution helpers
 */
	class domEvaluate : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::EVALUATE; }
		static daeInt ID() { return 222; }
		virtual daeInt typeID() const { return ID(); }
	public:
		class domDraw;

		typedef daeSmartRef<domDraw> domDrawRef;
		typedef daeTArray<domDrawRef> domDraw_Array;

/**
 * Identify what type of goemetry should be drawn durring this pass.  See
 * comments in fx_draw_common for a list of common strings.
 */
		class domDraw : public daeElement
		{
		public:
			virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::DRAW; }
			static daeInt ID() { return 223; }
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
/**
 * Identify that this pass is intended to render it's colors into a surface
 * parameter @see domColor_target
 */
		domFx_colortarget_Array elemColor_target_array;
/**
 * Identify that this pass is intended to render it's depthinto a surface
 * parameter @see domDepth_target
 */
		domFx_depthtarget_Array elemDepth_target_array;
/**
 * Identify that this pass is intended to render it's stencil bits into a
 * surface parameter @see domStencil_target
 */
		domFx_stenciltarget_Array elemStencil_target_array;
/**
 * The indexed color render target is supposed to be cleared to this color
 * @see domColor_clear
 */
		domFx_clearcolor_Array elemColor_clear_array;
/**
 * The indexed stencil bits render target is supposed to be cleared to this
 * color @see domStencil_clear
 */
		domFx_clearstencil_Array elemStencil_clear_array;
/**
 * The indexed depth render target is supposed to be cleared to this color
 * @see domDepth_clear
 */
		domFx_cleardepth_Array elemDepth_clear_array;
/**
 * Identify what type of goemetry should be drawn durring this pass.  See
 * comments in fx_draw_common for a list of common strings. @see domDraw
 */
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
		 * Gets the draw element.
		 * @return a daeSmartRef to the draw element.
		 */
		const domDrawRef getDraw() const { return elemDraw; }
	protected:
		/**
		 * Constructor
		 */
		domEvaluate(DAE& dae) : daeElement(dae), elemColor_target_array(), elemDepth_target_array(), elemStencil_target_array(), elemColor_clear_array(), elemStencil_clear_array(), elemDepth_clear_array(), elemDraw() {}
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
/**
 * Add metadata to this pass that may be used by games or tools to make decisions
 * @see domAnnotate
 */
	domFx_annotate_Array elemAnnotate_array;
/**
 * API state @see domStates
 */
	domStatesRef elemStates;
/**
 * A vertex_shader and fragment_shader are linked together to produce a OpenGL
 * ES 2 program. @see domProgram
 */
	domGles2_programRef elemProgram;
/**
 * runtime execution helpers @see domEvaluate
 */
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
	const domGles2_programRef getProgram() const { return elemProgram; }
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
	domGles2_pass(DAE& dae) : daeElement(dae), attrSid(), elemAnnotate_array(), elemStates(), elemProgram(), elemEvaluate(), elemExtra_array() {}
	/**
	 * Destructor
	 */
	virtual ~domGles2_pass() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domGles2_pass &operator=( const domGles2_pass &cpy ) { (void)cpy; return *this; }

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
