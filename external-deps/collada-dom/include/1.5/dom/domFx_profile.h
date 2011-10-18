#ifndef __domFx_profile_h__
#define __domFx_profile_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domProfile_common.h>
#include <dom/domProfile_bridge.h>
#include <dom/domProfile_gles2.h>
#include <dom/domProfile_glsl.h>
#include <dom/domProfile_cg.h>
#include <dom/domProfile_gles.h>
class DAE;

/**
 * A group that  specifies the allowable COLLADA FX profiles.
 */
class domFx_profile : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FX_PROFILE; }
	static daeInt ID() { return 965; }
	virtual daeInt typeID() const { return ID(); }

protected:  // Elements
	domProfile_commonRef elemProfile_COMMON;
	domProfile_bridgeRef elemProfile_BRIDGE;
	domProfile_gles2Ref elemProfile_GLES2;
	domProfile_glslRef elemProfile_GLSL;
	domProfile_cgRef elemProfile_CG;
	domProfile_glesRef elemProfile_GLES;
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
	 * Gets the profile_COMMON element.
	 * @return a daeSmartRef to the profile_COMMON element.
	 */
	const domProfile_commonRef getProfile_COMMON() const { return elemProfile_COMMON; }
	/**
	 * Gets the profile_BRIDGE element.
	 * @return a daeSmartRef to the profile_BRIDGE element.
	 */
	const domProfile_bridgeRef getProfile_BRIDGE() const { return elemProfile_BRIDGE; }
	/**
	 * Gets the profile_GLES2 element.
	 * @return a daeSmartRef to the profile_GLES2 element.
	 */
	const domProfile_gles2Ref getProfile_GLES2() const { return elemProfile_GLES2; }
	/**
	 * Gets the profile_GLSL element.
	 * @return a daeSmartRef to the profile_GLSL element.
	 */
	const domProfile_glslRef getProfile_GLSL() const { return elemProfile_GLSL; }
	/**
	 * Gets the profile_CG element.
	 * @return a daeSmartRef to the profile_CG element.
	 */
	const domProfile_cgRef getProfile_CG() const { return elemProfile_CG; }
	/**
	 * Gets the profile_GLES element.
	 * @return a daeSmartRef to the profile_GLES element.
	 */
	const domProfile_glesRef getProfile_GLES() const { return elemProfile_GLES; }
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
	domFx_profile(DAE& dae) : daeElement(dae), elemProfile_COMMON(), elemProfile_BRIDGE(), elemProfile_GLES2(), elemProfile_GLSL(), elemProfile_CG(), elemProfile_GLES() {}
	/**
	 * Destructor
	 */
	virtual ~domFx_profile() { daeElement::deleteCMDataArray(_CMData); }
	/**
	 * Overloaded assignment operator
	 */
	virtual domFx_profile &operator=( const domFx_profile &cpy ) { (void)cpy; return *this; }

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
