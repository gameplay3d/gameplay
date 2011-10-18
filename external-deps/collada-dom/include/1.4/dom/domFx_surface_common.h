/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 

#ifndef __domFx_surface_common_h__
#define __domFx_surface_common_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domFx_surface_init_common.h>
#include <dom/domExtra.h>
#include <dom/domFx_surface_format_hint_common.h>
class DAE;

/**
 * The fx_surface_common type is used to declare a resource that can be used
 * both as the source for texture samples and as the target of a rendering
 * pass.
 */
class domFx_surface_common_complexType 
{
public:
	class domFormat;

	typedef daeSmartRef<domFormat> domFormatRef;
	typedef daeTArray<domFormatRef> domFormat_Array;

/**
 * Contains a string representing the profile and platform specific texel
 * format that the author would like this surface to use.  If this element
 * is not specified then the application will use a common format R8G8B8A8
 * with linear color gradient, not  sRGB.
 */
	class domFormat : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FORMAT; }
		static daeInt ID() { return 22; }
		virtual daeInt typeID() const { return ID(); }

	protected:  // Value
		/**
		 * The xsToken value of the text data of this element. 
		 */
		xsToken _value;

	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return Returns a xsToken of the value.
		 */
		xsToken getValue() const { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( xsToken val ) { *(daeStringRef*)&_value = val; }

	protected:
		/**
		 * Constructor
		 */
		domFormat(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domFormat() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domFormat &operator=( const domFormat &cpy ) { (void)cpy; return *this; }

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

	class domSize;

	typedef daeSmartRef<domSize> domSizeRef;
	typedef daeTArray<domSizeRef> domSize_Array;

/**
 * The surface should be sized to these exact dimensions
 */
	class domSize : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::SIZE; }
		static daeInt ID() { return 23; }
		virtual daeInt typeID() const { return ID(); }

	protected:  // Value
		/**
		 * The domInt3 value of the text data of this element. 
		 */
		domInt3 _value;

	public:	//Accessors and Mutators
		/**
		 * Gets the _value array.
		 * @return Returns a domInt3 reference of the _value array.
		 */
		domInt3 &getValue() { return _value; }
		/**
		 * Gets the _value array.
		 * @return Returns a constant domInt3 reference of the _value array.
		 */
		const domInt3 &getValue() const { return _value; }
		/**
		 * Sets the _value array.
		 * @param val The new value for the _value array.
		 */
		void setValue( const domInt3 &val ) { _value = val; }

	protected:
		/**
		 * Constructor
		 */
		domSize(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domSize() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domSize &operator=( const domSize &cpy ) { (void)cpy; return *this; }

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

	class domViewport_ratio;

	typedef daeSmartRef<domViewport_ratio> domViewport_ratioRef;
	typedef daeTArray<domViewport_ratioRef> domViewport_ratio_Array;

/**
 * The surface should be sized to a dimension based on this ratio of the viewport's
 * dimensions in pixels
 */
	class domViewport_ratio : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::VIEWPORT_RATIO; }
		static daeInt ID() { return 24; }
		virtual daeInt typeID() const { return ID(); }

	protected:  // Value
		/**
		 * The domFloat2 value of the text data of this element. 
		 */
		domFloat2 _value;

	public:	//Accessors and Mutators
		/**
		 * Gets the _value array.
		 * @return Returns a domFloat2 reference of the _value array.
		 */
		domFloat2 &getValue() { return _value; }
		/**
		 * Gets the _value array.
		 * @return Returns a constant domFloat2 reference of the _value array.
		 */
		const domFloat2 &getValue() const { return _value; }
		/**
		 * Sets the _value array.
		 * @param val The new value for the _value array.
		 */
		void setValue( const domFloat2 &val ) { _value = val; }

	protected:
		/**
		 * Constructor
		 */
		domViewport_ratio(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domViewport_ratio() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domViewport_ratio &operator=( const domViewport_ratio &cpy ) { (void)cpy; return *this; }

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

	class domMip_levels;

	typedef daeSmartRef<domMip_levels> domMip_levelsRef;
	typedef daeTArray<domMip_levelsRef> domMip_levels_Array;

/**
 * the surface should contain the following number of MIP levels.  If this
 * element is not present it is assumed that all miplevels exist until a dimension
 * becomes 1 texel.  To create a surface that has only one level of mip maps
 * (mip=0) set this to 1.  If the value is 0 the result is the same as if
 * mip_levels was unspecified, all possible mip_levels will exist.
 */
	class domMip_levels : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::MIP_LEVELS; }
		static daeInt ID() { return 25; }
		virtual daeInt typeID() const { return ID(); }

	protected:  // Value
		/**
		 * The xsUnsignedInt value of the text data of this element. 
		 */
		xsUnsignedInt _value;

	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a xsUnsignedInt of the value.
		 */
		xsUnsignedInt getValue() const { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( xsUnsignedInt val ) { _value = val; }

	protected:
		/**
		 * Constructor
		 */
		domMip_levels(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domMip_levels() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domMip_levels &operator=( const domMip_levels &cpy ) { (void)cpy; return *this; }

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

	class domMipmap_generate;

	typedef daeSmartRef<domMipmap_generate> domMipmap_generateRef;
	typedef daeTArray<domMipmap_generateRef> domMipmap_generate_Array;

/**
 * By default it is assumed that mipmaps are supplied by the author so, if
 * not all subsurfaces are initialized, it is invalid and will result in profile
 * and platform specific behavior unless mipmap_generate is responsible for
 * initializing the remainder of the sub-surfaces
 */
	class domMipmap_generate : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::MIPMAP_GENERATE; }
		static daeInt ID() { return 26; }
		virtual daeInt typeID() const { return ID(); }

	protected:  // Value
		/**
		 * The xsBoolean value of the text data of this element. 
		 */
		xsBoolean _value;

	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a xsBoolean of the value.
		 */
		xsBoolean getValue() const { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( xsBoolean val ) { _value = val; }

	protected:
		/**
		 * Constructor
		 */
		domMipmap_generate(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domMipmap_generate() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domMipmap_generate &operator=( const domMipmap_generate &cpy ) { (void)cpy; return *this; }

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
 * Specifying the type of a surface is mandatory though the type may be "UNTYPED".
 * When a surface is typed as UNTYPED, it is said to be temporarily untyped
 * and instead will be typed later by the context it is used in such as which
 * samplers reference it in that are used in a particular technique or pass.
 * If there is a type mismatch between what is set into it later and what
 * the runtime decides the type should be the result in profile and platform
 * specific behavior.
 */
	domFx_surface_type_enum attrType;

protected:  // Elements
/**
 * The common set of initalization options for surfaces.  Choose which is
 * appropriate for your surface based on the type attribute and other characteristics
 * described by the annotation docs on the choiced child elements of this
 * type. @see domFx_surface_init_common
 */
	domFx_surface_init_commonRef elemFx_surface_init_common;
/**
 * Contains a string representing the profile and platform specific texel
 * format that the author would like this surface to use.  If this element
 * is not specified then the application will use a common format R8G8B8A8
 * with linear color gradient, not  sRGB. @see domFormat
 */
	domFormatRef elemFormat;
/**
 * If the exact format cannot be resolved via the "format" element then the
 * format_hint will describe the important features of the format so that
 * the application may select a compatable or close format @see domFormat_hint
 */
	domFx_surface_format_hint_commonRef elemFormat_hint;
/**
 * The surface should be sized to these exact dimensions @see domSize
 */
	domSizeRef elemSize;
/**
 * The surface should be sized to a dimension based on this ratio of the viewport's
 * dimensions in pixels @see domViewport_ratio
 */
	domViewport_ratioRef elemViewport_ratio;
/**
 * the surface should contain the following number of MIP levels.  If this
 * element is not present it is assumed that all miplevels exist until a dimension
 * becomes 1 texel.  To create a surface that has only one level of mip maps
 * (mip=0) set this to 1.  If the value is 0 the result is the same as if
 * mip_levels was unspecified, all possible mip_levels will exist. @see domMip_levels
 */
	domMip_levelsRef elemMip_levels;
/**
 * By default it is assumed that mipmaps are supplied by the author so, if
 * not all subsurfaces are initialized, it is invalid and will result in profile
 * and platform specific behavior unless mipmap_generate is responsible for
 * initializing the remainder of the sub-surfaces @see domMipmap_generate
 */
	domMipmap_generateRef elemMipmap_generate;
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
	 * Gets the type attribute.
	 * @return Returns a domFx_surface_type_enum of the type attribute.
	 */
	domFx_surface_type_enum getType() const { return attrType; }
	/**
	 * Sets the type attribute.
	 * @param atType The new value for the type attribute.
	 */
	void setType( domFx_surface_type_enum atType ) { attrType = atType; }

	/**
	 * Gets the fx_surface_init_common element.
	 * @return a daeSmartRef to the fx_surface_init_common element.
	 */
	const domFx_surface_init_commonRef getFx_surface_init_common() const { return elemFx_surface_init_common; }
	/**
	 * Gets the format element.
	 * @return a daeSmartRef to the format element.
	 */
	const domFormatRef getFormat() const { return elemFormat; }
	/**
	 * Gets the format_hint element.
	 * @return a daeSmartRef to the format_hint element.
	 */
	const domFx_surface_format_hint_commonRef getFormat_hint() const { return elemFormat_hint; }
	/**
	 * Gets the size element.
	 * @return a daeSmartRef to the size element.
	 */
	const domSizeRef getSize() const { return elemSize; }
	/**
	 * Gets the viewport_ratio element.
	 * @return a daeSmartRef to the viewport_ratio element.
	 */
	const domViewport_ratioRef getViewport_ratio() const { return elemViewport_ratio; }
	/**
	 * Gets the mip_levels element.
	 * @return a daeSmartRef to the mip_levels element.
	 */
	const domMip_levelsRef getMip_levels() const { return elemMip_levels; }
	/**
	 * Gets the mipmap_generate element.
	 * @return a daeSmartRef to the mipmap_generate element.
	 */
	const domMipmap_generateRef getMipmap_generate() const { return elemMipmap_generate; }
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
	domFx_surface_common_complexType(DAE& dae, daeElement* elt) : attrType(), elemFx_surface_init_common(), elemFormat(), elemFormat_hint(), elemSize(), elemViewport_ratio(), elemMip_levels(), elemMipmap_generate(), elemExtra_array() {}
	/**
	 * Destructor
	 */
	virtual ~domFx_surface_common_complexType() { daeElement::deleteCMDataArray(_CMData); }
	/**
	 * Overloaded assignment operator
	 */
	virtual domFx_surface_common_complexType &operator=( const domFx_surface_common_complexType &cpy ) { (void)cpy; return *this; }
};

/**
 * An element of type domFx_surface_common_complexType.
 */
class domFx_surface_common : public daeElement, public domFx_surface_common_complexType
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FX_SURFACE_COMMON; }
	static daeInt ID() { return 27; }
	virtual daeInt typeID() const { return ID(); }

public:	//Accessors and Mutators
	/**
	 * Gets the type attribute.
	 * @return Returns a domFx_surface_type_enum of the type attribute.
	 */
	domFx_surface_type_enum getType() const { return attrType; }
	/**
	 * Sets the type attribute.
	 * @param atType The new value for the type attribute.
	 */
	void setType( domFx_surface_type_enum atType ) { attrType = atType; _validAttributeArray[0] = true; }

protected:
	/**
	 * Constructor
	 */
	domFx_surface_common(DAE& dae) : daeElement(dae), domFx_surface_common_complexType(dae, this) {}
	/**
	 * Destructor
	 */
	virtual ~domFx_surface_common() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domFx_surface_common &operator=( const domFx_surface_common &cpy ) { (void)cpy; return *this; }

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
