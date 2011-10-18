#ifndef __domGlsl_shader_h__
#define __domGlsl_shader_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domFx_sources.h>
#include <dom/domExtra.h>
class DAE;

/**
 * Identify code fragments and bind their parameters to effect parameters
 * to identify how their values will be filled in
 */
class domGlsl_shader : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::GLSL_SHADER; }
	static daeInt ID() { return 231; }
	virtual daeInt typeID() const { return ID(); }
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
	domFx_sourcesRef elemSources;
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
	const domFx_sourcesRef getSources() const { return elemSources; }
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
	domGlsl_shader(DAE& dae) : daeElement(dae), attrStage(), elemSources(), elemExtra_array() {}
	/**
	 * Destructor
	 */
	virtual ~domGlsl_shader() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domGlsl_shader &operator=( const domGlsl_shader &cpy ) { (void)cpy; return *this; }

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
