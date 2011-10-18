#ifndef __domGles2_shader_h__
#define __domGles2_shader_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domFx_sources.h>
#include <dom/domFx_target.h>
#include <dom/domExtra.h>
class DAE;

/**
 * Identify code fragments and bind their parameters to effect parameters
 * to identify how their values will be filled in
 */
class domGles2_shader : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::GLES2_SHADER; }
	static daeInt ID() { return 213; }
	virtual daeInt typeID() const { return ID(); }
public:
	class domSources;

	typedef daeSmartRef<domSources> domSourcesRef;
	typedef daeTArray<domSourcesRef> domSources_Array;

/**
 * sid of the code or include element which provides the source for this shader.
 * Code may come from local or library_code.
 */
	class domSources : public domFx_sources
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::SOURCES; }
		static daeInt ID() { return 214; }
		virtual daeInt typeID() const { return ID(); }
	protected:  // Attribute
/**
 * Not used by GLSL.  Some languages require functional entry points.  GLSL
 * always uses "main".  CG requires an entry point
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


protected:  // Attribute
/**
 * GPU Pipeline stage for this programmable shader. Current standards are
 * VERTEX (vertex shader), FRAGMENT (pixel or fragment shader).  These are
 * recognized by both GLSL and CG
 */
	domFx_pipeline_stage attrStage;

protected:  // Elements
/**
 * sid of the code or include element which provides the source for this shader.
 * Code may come from local or library_code. @see domSources
 */
	domSourcesRef elemSources;
/**
 * Not used by GLSL.  This identifies a very specific binary target format
 * for such things as a specific generation of GPU.  Ex. NV4x @see domCompiler
 */
	domFx_target_Array elemCompiler_array;
	domExtra_Array elemExtra_array;

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
	void setStage( domFx_pipeline_stage atStage ) { attrStage = atStage; }

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
	domGles2_shader(DAE& dae) : daeElement(dae), attrStage(), elemSources(), elemCompiler_array(), elemExtra_array() {}
	/**
	 * Destructor
	 */
	virtual ~domGles2_shader() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domGles2_shader &operator=( const domGles2_shader &cpy ) { (void)cpy; return *this; }

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
