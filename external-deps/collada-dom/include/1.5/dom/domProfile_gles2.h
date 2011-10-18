#ifndef __domProfile_gles2_h__
#define __domProfile_gles2_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domAsset.h>
#include <dom/domFx_include.h>
#include <dom/domFx_code.h>
#include <dom/domGles2_newparam.h>
#include <dom/domFx_annotate.h>
#include <dom/domGles2_pass.h>
#include <dom/domExtra.h>
class DAE;

/**
 * FX for OpenGL ES 2.0
 */
class domProfile_gles2 : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::PROFILE_GLES2; }
	static daeInt ID() { return 224; }
	virtual daeInt typeID() const { return ID(); }
public:
	class domNewparam;

	typedef daeSmartRef<domNewparam> domNewparamRef;
	typedef daeTArray<domNewparamRef> domNewparam_Array;

	class domNewparam : public domGles2_newparam
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::NEWPARAM; }
		static daeInt ID() { return 225; }
		virtual daeInt typeID() const { return ID(); }

	protected:
		/**
		 * Constructor
		 */
		domNewparam(DAE& dae) : domGles2_newparam(dae) {}
		/**
		 * Destructor
		 */
		virtual ~domNewparam() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domNewparam &operator=( const domNewparam &cpy ) { (void)cpy; return *this; }

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
		static daeInt ID() { return 226; }
		virtual daeInt typeID() const { return ID(); }
	protected:  // Attributes
		xsID attrId;
/**
 *  The sid attribute is a text string value containing the sub-identifier
 * of this element.  This value must be unique within the scope of the parent
 * element. Optional attribute. 
 */
		domSid attrSid;

	protected:  // Elements
		domAssetRef elemAsset;
/**
 * Add metadata to this pass that may be used by games or tools to make decisions
 * @see domAnnotate
 */
		domFx_annotate_Array elemAnnotate_array;
/**
 * A static declaration of all the render states, shaders, and settings for
 * one rendering pipeline. @see domPass
 */
		domGles2_pass_Array elemPass_array;
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
		domGles2_pass_Array &getPass_array() { return elemPass_array; }
		/**
		 * Gets the pass element array.
		 * @return Returns a constant reference to the array of pass elements.
		 */
		const domGles2_pass_Array &getPass_array() const { return elemPass_array; }
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
 * Unique identifier for referencing
 */
	xsID attrId;
/**
 * Shading language that will be used.  This is not an enum to support future
 * languages.  Current known languages at this time are GLSL-ES, CG
 */
	xsNCName attrLanguage;
/**
 * Support for multiple OpenGL ES 2.0 platforms
 */
	domList_of_names attrPlatforms;

protected:  // Elements
	domAssetRef elemAsset;
/**
 * Include shader code from a remote URI.  If binary, assume raw. @see domInclude
 */
	domFx_include_Array elemInclude_array;
/**
 * The fx_code_profile type allows you to specify an inline block of source
 * code.  It is HIGHLY recommended to use a CDATA block inside to avoid issues
 * with @see domCode
 */
	domFx_code_Array elemCode_array;
	domNewparam_Array elemNewparam_array;
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
	 * Gets the language attribute.
	 * @return Returns a xsNCName of the language attribute.
	 */
	xsNCName getLanguage() const { return attrLanguage; }
	/**
	 * Sets the language attribute.
	 * @param atLanguage The new value for the language attribute.
	 */
	void setLanguage( xsNCName atLanguage ) { *(daeStringRef*)&attrLanguage = atLanguage;}

	/**
	 * Gets the platforms attribute.
	 * @return Returns a domList_of_names of the platforms attribute.
	 */
	domList_of_names getPlatforms() const { return attrPlatforms; }
	/**
	 * Sets the platforms attribute.
	 * @param atPlatforms The new value for the platforms attribute.
	 */
	void setPlatforms( domList_of_names atPlatforms ) { attrPlatforms = atPlatforms; }

	/**
	 * Gets the asset element.
	 * @return a daeSmartRef to the asset element.
	 */
	const domAssetRef getAsset() const { return elemAsset; }
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
	 * Gets the newparam element array.
	 * @return Returns a reference to the array of newparam elements.
	 */
	domNewparam_Array &getNewparam_array() { return elemNewparam_array; }
	/**
	 * Gets the newparam element array.
	 * @return Returns a constant reference to the array of newparam elements.
	 */
	const domNewparam_Array &getNewparam_array() const { return elemNewparam_array; }
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
	domProfile_gles2(DAE& dae) : daeElement(dae), attrId(), attrLanguage(), attrPlatforms(), elemAsset(), elemInclude_array(), elemCode_array(), elemNewparam_array(), elemTechnique_array(), elemExtra_array() {}
	/**
	 * Destructor
	 */
	virtual ~domProfile_gles2() { daeElement::deleteCMDataArray(_CMData); }
	/**
	 * Overloaded assignment operator
	 */
	virtual domProfile_gles2 &operator=( const domProfile_gles2 &cpy ) { (void)cpy; return *this; }

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
