#ifndef __domProfile_cg_h__
#define __domProfile_cg_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domAsset.h>
#include <dom/domFx_code.h>
#include <dom/domFx_include.h>
#include <dom/domCg_newparam.h>
#include <dom/domFx_annotate.h>
#include <dom/domCg_pass.h>
#include <dom/domExtra.h>
class DAE;

/**
 * Opens a block of CG platform-specific data types and technique declarations.
 */
class domProfile_cg : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::PROFILE_CG; }
	static daeInt ID() { return 258; }
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
		static daeInt ID() { return 259; }
		virtual daeInt typeID() const { return ID(); }
	protected:  // Attributes
/**
 *  The id attribute is a text string containing the unique identifier of
 * this element.  This value must be unique within the instance document.
 * Optional attribute. 
 */
		xsID attrId;
/**
 *  The sid attribute is a text string value containing the sub-identifier
 * of this element.  This value must be unique within the scope of the parent
 * element. Optional attribute. 
 */
		domSid attrSid;

	protected:  // Elements
/**
 * The technique element may contain an asset element. @see domAsset
 */
		domAssetRef elemAsset;
		domFx_annotate_Array elemAnnotate_array;
/**
 * A static declaration of all the render states, shaders, and settings for
 * one rendering pipeline. @see domPass
 */
		domCg_pass_Array elemPass_array;
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
		domCg_pass_Array &getPass_array() { return elemPass_array; }
		/**
		 * Gets the pass element array.
		 * @return Returns a constant reference to the array of pass elements.
		 */
		const domCg_pass_Array &getPass_array() const { return elemPass_array; }
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
	domFx_code_Array elemCode_array;
	domFx_include_Array elemInclude_array;
	domCg_newparam_Array elemNewparam_array;
/**
 * Holds a description of the textures, samplers, shaders, parameters, and
 * passes necessary for rendering this effect using one method. @see domTechnique
 */
	domTechnique_Array elemTechnique_array;
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
	 * Gets the code element array.
	 * @return Returns a reference to the array of code elements.
	 */
	domFx_code_Array &getCode_array() { return elemCode_array; }
	/**
	 * Gets the code element array.
	 * @return Returns a constant reference to the array of code elements.
	 */
	const domFx_code_Array &getCode_array() const { return elemCode_array; }
	/**
	 * Gets the include element array.
	 * @return Returns a reference to the array of include elements.
	 */
	domFx_include_Array &getInclude_array() { return elemInclude_array; }
	/**
	 * Gets the include element array.
	 * @return Returns a constant reference to the array of include elements.
	 */
	const domFx_include_Array &getInclude_array() const { return elemInclude_array; }
	/**
	 * Gets the newparam element array.
	 * @return Returns a reference to the array of newparam elements.
	 */
	domCg_newparam_Array &getNewparam_array() { return elemNewparam_array; }
	/**
	 * Gets the newparam element array.
	 * @return Returns a constant reference to the array of newparam elements.
	 */
	const domCg_newparam_Array &getNewparam_array() const { return elemNewparam_array; }
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
	domProfile_cg(DAE& dae) : daeElement(dae), attrId(), attrPlatform(), elemAsset(), elemCode_array(), elemInclude_array(), elemNewparam_array(), elemTechnique_array(), elemExtra_array() {}
	/**
	 * Destructor
	 */
	virtual ~domProfile_cg() { daeElement::deleteCMDataArray(_CMData); }
	/**
	 * Overloaded assignment operator
	 */
	virtual domProfile_cg &operator=( const domProfile_cg &cpy ) { (void)cpy; return *this; }

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
