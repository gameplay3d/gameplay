#ifndef __domEffect_h__
#define __domEffect_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domFx_profile.h>
#include <dom/domAsset.h>
#include <dom/domFx_annotate.h>
#include <dom/domFx_newparam.h>
#include <dom/domExtra.h>
class DAE;

/**
 * A self contained description of a shader effect.
 */
class domEffect : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::EFFECT; }
	static daeInt ID() { return 279; }
	virtual daeInt typeID() const { return ID(); }
protected:  // Attributes
/**
 *  The id attribute is a text string containing the unique identifier of
 * this element.  This value must be unique within the instance document.
 * Optional attribute. 
 */
	xsID attrId;
/**
 *  The name attribute is the text string name of this element. Optional attribute.
 */
	xsToken attrName;

protected:  // Elements
/**
 * The effect element may contain an asset element. @see domAsset
 */
	domAssetRef elemAsset;
/**
 * The annotate element allows you to specify an annotation on this effect.
 * @see domAnnotate
 */
	domFx_annotate_Array elemAnnotate_array;
/**
 * The newparam element allows you to create new effect parameters which can
 * be shared by multipe profiles. @see domNewparam
 */
	domFx_newparam_Array elemNewparam_array;
	domFx_profile_Array elemFx_profile_array;
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
	 * Gets the newparam element array.
	 * @return Returns a reference to the array of newparam elements.
	 */
	domFx_newparam_Array &getNewparam_array() { return elemNewparam_array; }
	/**
	 * Gets the newparam element array.
	 * @return Returns a constant reference to the array of newparam elements.
	 */
	const domFx_newparam_Array &getNewparam_array() const { return elemNewparam_array; }
	/**
	 * Gets the fx_profile element array.
	 * @return Returns a reference to the array of fx_profile elements.
	 */
	domFx_profile_Array &getFx_profile_array() { return elemFx_profile_array; }
	/**
	 * Gets the fx_profile element array.
	 * @return Returns a constant reference to the array of fx_profile elements.
	 */
	const domFx_profile_Array &getFx_profile_array() const { return elemFx_profile_array; }
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
	domEffect(DAE& dae) : daeElement(dae), attrId(), attrName(), elemAsset(), elemAnnotate_array(), elemNewparam_array(), elemFx_profile_array(), elemExtra_array() {}
	/**
	 * Destructor
	 */
	virtual ~domEffect() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domEffect &operator=( const domEffect &cpy ) { (void)cpy; return *this; }

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
