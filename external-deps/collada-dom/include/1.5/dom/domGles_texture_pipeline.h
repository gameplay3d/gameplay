#ifndef __domGles_texture_pipeline_h__
#define __domGles_texture_pipeline_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domGles_texcombiner_command.h>
#include <dom/domGles_texenv_command.h>
#include <dom/domExtra.h>
class DAE;

/**
 * Defines a set of texturing commands that will be converted into multitexturing
 * operations using glTexEnv in regular and combiner mode.
 */
class domGles_texture_pipeline : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::GLES_TEXTURE_PIPELINE; }
	static daeInt ID() { return 267; }
	virtual daeInt typeID() const { return ID(); }
protected:  // Attribute
/**
 *  The sid attribute is a text string value containing the sub-identifier
 * of this element.  This value must be unique within the scope of the parent
 * element. Optional attribute. 
 */
	domSid attrSid;

protected:  // Elements
/**
 * Defines a texture_pipeline command. This is a combiner-mode texturing operation.
 * @see domTexcombiner
 */
	domGles_texcombiner_command_Array elemTexcombiner_array;
/**
 * Defines a texture_pipeline command. It is a simple noncombiner mode of
 * texturing operations. @see domTexenv
 */
	domGles_texenv_command_Array elemTexenv_array;
/**
 * The extra element may appear any number of times. OpenGL ES extensions
 * may be used here. @see domExtra
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
	 * Gets the texcombiner element array.
	 * @return Returns a reference to the array of texcombiner elements.
	 */
	domGles_texcombiner_command_Array &getTexcombiner_array() { return elemTexcombiner_array; }
	/**
	 * Gets the texcombiner element array.
	 * @return Returns a constant reference to the array of texcombiner elements.
	 */
	const domGles_texcombiner_command_Array &getTexcombiner_array() const { return elemTexcombiner_array; }
	/**
	 * Gets the texenv element array.
	 * @return Returns a reference to the array of texenv elements.
	 */
	domGles_texenv_command_Array &getTexenv_array() { return elemTexenv_array; }
	/**
	 * Gets the texenv element array.
	 * @return Returns a constant reference to the array of texenv elements.
	 */
	const domGles_texenv_command_Array &getTexenv_array() const { return elemTexenv_array; }
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
	domGles_texture_pipeline(DAE& dae) : daeElement(dae), attrSid(), elemTexcombiner_array(), elemTexenv_array(), elemExtra_array() {}
	/**
	 * Destructor
	 */
	virtual ~domGles_texture_pipeline() { daeElement::deleteCMDataArray(_CMData); }
	/**
	 * Overloaded assignment operator
	 */
	virtual domGles_texture_pipeline &operator=( const domGles_texture_pipeline &cpy ) { (void)cpy; return *this; }

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
