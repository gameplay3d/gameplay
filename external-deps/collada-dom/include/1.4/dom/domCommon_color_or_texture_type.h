/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 

#ifndef __domCommon_color_or_texture_type_h__
#define __domCommon_color_or_texture_type_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domExtra.h>
class DAE;

class domCommon_color_or_texture_type_complexType 
{
public:
	class domColor;

	typedef daeSmartRef<domColor> domColorRef;
	typedef daeTArray<domColorRef> domColor_Array;

	class domColor : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::COLOR; }
		static daeInt ID() { return 116; }
		virtual daeInt typeID() const { return ID(); }
	protected:  // Attribute
		xsNCName attrSid;

	protected:  // Value
		/**
		 * The domFx_color_common value of the text data of this element. 
		 */
		domFx_color_common _value;

	public:	//Accessors and Mutators
		/**
		 * Gets the sid attribute.
		 * @return Returns a xsNCName of the sid attribute.
		 */
		xsNCName getSid() const { return attrSid; }
		/**
		 * Sets the sid attribute.
		 * @param atSid The new value for the sid attribute.
		 */
		void setSid( xsNCName atSid ) { *(daeStringRef*)&attrSid = atSid; _validAttributeArray[0] = true; }

		/**
		 * Gets the _value array.
		 * @return Returns a domFx_color_common reference of the _value array.
		 */
		domFx_color_common &getValue() { return _value; }
		/**
		 * Gets the _value array.
		 * @return Returns a constant domFx_color_common reference of the _value array.
		 */
		const domFx_color_common &getValue() const { return _value; }
		/**
		 * Sets the _value array.
		 * @param val The new value for the _value array.
		 */
		void setValue( const domFx_color_common &val ) { _value = val; }

	protected:
		/**
		 * Constructor
		 */
		domColor(DAE& dae) : daeElement(dae), attrSid(), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domColor() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domColor &operator=( const domColor &cpy ) { (void)cpy; return *this; }

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

	class domParam;

	typedef daeSmartRef<domParam> domParamRef;
	typedef daeTArray<domParamRef> domParam_Array;

	class domParam : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::PARAM; }
		static daeInt ID() { return 117; }
		virtual daeInt typeID() const { return ID(); }
	protected:  // Attribute
		xsNCName attrRef;


	public:	//Accessors and Mutators
		/**
		 * Gets the ref attribute.
		 * @return Returns a xsNCName of the ref attribute.
		 */
		xsNCName getRef() const { return attrRef; }
		/**
		 * Sets the ref attribute.
		 * @param atRef The new value for the ref attribute.
		 */
		void setRef( xsNCName atRef ) { *(daeStringRef*)&attrRef = atRef; _validAttributeArray[0] = true; }

	protected:
		/**
		 * Constructor
		 */
		domParam(DAE& dae) : daeElement(dae), attrRef() {}
		/**
		 * Destructor
		 */
		virtual ~domParam() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domParam &operator=( const domParam &cpy ) { (void)cpy; return *this; }

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

	class domTexture;

	typedef daeSmartRef<domTexture> domTextureRef;
	typedef daeTArray<domTextureRef> domTexture_Array;

	class domTexture : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::TEXTURE; }
		static daeInt ID() { return 118; }
		virtual daeInt typeID() const { return ID(); }
	protected:  // Attributes
		xsNCName attrTexture;
		xsNCName attrTexcoord;

	protected:  // Element
		domExtraRef elemExtra;

	public:	//Accessors and Mutators
		/**
		 * Gets the texture attribute.
		 * @return Returns a xsNCName of the texture attribute.
		 */
		xsNCName getTexture() const { return attrTexture; }
		/**
		 * Sets the texture attribute.
		 * @param atTexture The new value for the texture attribute.
		 */
		void setTexture( xsNCName atTexture ) { *(daeStringRef*)&attrTexture = atTexture; _validAttributeArray[0] = true; }

		/**
		 * Gets the texcoord attribute.
		 * @return Returns a xsNCName of the texcoord attribute.
		 */
		xsNCName getTexcoord() const { return attrTexcoord; }
		/**
		 * Sets the texcoord attribute.
		 * @param atTexcoord The new value for the texcoord attribute.
		 */
		void setTexcoord( xsNCName atTexcoord ) { *(daeStringRef*)&attrTexcoord = atTexcoord; _validAttributeArray[1] = true; }

		/**
		 * Gets the extra element.
		 * @return a daeSmartRef to the extra element.
		 */
		const domExtraRef getExtra() const { return elemExtra; }
	protected:
		/**
		 * Constructor
		 */
		domTexture(DAE& dae) : daeElement(dae), attrTexture(), attrTexcoord(), elemExtra() {}
		/**
		 * Destructor
		 */
		virtual ~domTexture() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domTexture &operator=( const domTexture &cpy ) { (void)cpy; return *this; }

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
	domColorRef elemColor;
	domParamRef elemParam;
	domTextureRef elemTexture;
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
	 * Gets the color element.
	 * @return a daeSmartRef to the color element.
	 */
	const domColorRef getColor() const { return elemColor; }
	/**
	 * Gets the param element.
	 * @return a daeSmartRef to the param element.
	 */
	const domParamRef getParam() const { return elemParam; }
	/**
	 * Gets the texture element.
	 * @return a daeSmartRef to the texture element.
	 */
	const domTextureRef getTexture() const { return elemTexture; }
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
	domCommon_color_or_texture_type_complexType(DAE& dae, daeElement* elt) : elemColor(), elemParam(), elemTexture() {}
	/**
	 * Destructor
	 */
	virtual ~domCommon_color_or_texture_type_complexType() { daeElement::deleteCMDataArray(_CMData); }
	/**
	 * Overloaded assignment operator
	 */
	virtual domCommon_color_or_texture_type_complexType &operator=( const domCommon_color_or_texture_type_complexType &cpy ) { (void)cpy; return *this; }
};

/**
 * An element of type domCommon_color_or_texture_type_complexType.
 */
class domCommon_color_or_texture_type : public daeElement, public domCommon_color_or_texture_type_complexType
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::COMMON_COLOR_OR_TEXTURE_TYPE; }
	static daeInt ID() { return 119; }
	virtual daeInt typeID() const { return ID(); }
protected:
	/**
	 * Constructor
	 */
	domCommon_color_or_texture_type(DAE& dae) : daeElement(dae), domCommon_color_or_texture_type_complexType(dae, this) {}
	/**
	 * Destructor
	 */
	virtual ~domCommon_color_or_texture_type() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domCommon_color_or_texture_type &operator=( const domCommon_color_or_texture_type &cpy ) { (void)cpy; return *this; }

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
