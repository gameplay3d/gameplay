/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 

#ifndef __domEffect_h__
#define __domEffect_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domAsset.h>
#include <dom/domImage.h>
#include <dom/domFx_profile_abstract.h>
#include <dom/domExtra.h>
#include <dom/domFx_annotate_common.h>
#include <dom/domFx_newparam_common.h>
class DAE;

/**
 * A self contained description of a shader effect.
 */
class domEffect : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::EFFECT; }
	static daeInt ID() { return 728; }
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
	xsNCName attrName;

protected:  // Elements
/**
 *  The effect element may contain an asset element.  @see domAsset
 */
	domAssetRef elemAsset;
/**
 * The annotate element allows you to specify an annotation on this effect.
 * @see domAnnotate
 */
	domFx_annotate_common_Array elemAnnotate_array;
/**
 *  The image element allows you to create image resources which can be shared
 * by multipe profiles.  @see domImage
 */
	domImage_Array elemImage_array;
/**
 * The newparam element allows you to create new effect parameters which can
 * be shared by multipe profiles. @see domNewparam
 */
	domFx_newparam_common_Array elemNewparam_array;
/**
 *  This is the substituion group hook which allows you to swap in other COLLADA
 * FX profiles.  @see domFx_profile_abstract
 */
	domFx_profile_abstract_Array elemFx_profile_abstract_array;
/**
 *  The extra element may appear any number of times.  @see domExtra
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
	 * Gets the name attribute.
	 * @return Returns a xsNCName of the name attribute.
	 */
	xsNCName getName() const { return attrName; }
	/**
	 * Sets the name attribute.
	 * @param atName The new value for the name attribute.
	 */
	void setName( xsNCName atName ) { *(daeStringRef*)&attrName = atName; _validAttributeArray[1] = true; }

	/**
	 * Gets the asset element.
	 * @return a daeSmartRef to the asset element.
	 */
	const domAssetRef getAsset() const { return elemAsset; }
	/**
	 * Gets the annotate element array.
	 * @return Returns a reference to the array of annotate elements.
	 */
	domFx_annotate_common_Array &getAnnotate_array() { return elemAnnotate_array; }
	/**
	 * Gets the annotate element array.
	 * @return Returns a constant reference to the array of annotate elements.
	 */
	const domFx_annotate_common_Array &getAnnotate_array() const { return elemAnnotate_array; }
	/**
	 * Gets the image element array.
	 * @return Returns a reference to the array of image elements.
	 */
	domImage_Array &getImage_array() { return elemImage_array; }
	/**
	 * Gets the image element array.
	 * @return Returns a constant reference to the array of image elements.
	 */
	const domImage_Array &getImage_array() const { return elemImage_array; }
	/**
	 * Gets the newparam element array.
	 * @return Returns a reference to the array of newparam elements.
	 */
	domFx_newparam_common_Array &getNewparam_array() { return elemNewparam_array; }
	/**
	 * Gets the newparam element array.
	 * @return Returns a constant reference to the array of newparam elements.
	 */
	const domFx_newparam_common_Array &getNewparam_array() const { return elemNewparam_array; }
	/**
	 * Gets the fx_profile_abstract element array.
	 * @return Returns a reference to the array of fx_profile_abstract elements.
	 */
	domFx_profile_abstract_Array &getFx_profile_abstract_array() { return elemFx_profile_abstract_array; }
	/**
	 * Gets the fx_profile_abstract element array.
	 * @return Returns a constant reference to the array of fx_profile_abstract elements.
	 */
	const domFx_profile_abstract_Array &getFx_profile_abstract_array() const { return elemFx_profile_abstract_array; }
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
	domEffect(DAE& dae) : daeElement(dae), attrId(), attrName(), elemAsset(), elemAnnotate_array(), elemImage_array(), elemNewparam_array(), elemFx_profile_abstract_array(), elemExtra_array() {}
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
