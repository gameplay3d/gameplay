/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 

#ifndef __domCommon_newparam_type_h__
#define __domCommon_newparam_type_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domFx_surface_common.h>
#include <dom/domFx_sampler2D_common.h>
class DAE;

class domCommon_newparam_type_complexType 
{
public:
	class domSemantic;

	typedef daeSmartRef<domSemantic> domSemanticRef;
	typedef daeTArray<domSemanticRef> domSemantic_Array;

	class domSemantic : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::SEMANTIC; }
		static daeInt ID() { return 121; }
		virtual daeInt typeID() const { return ID(); }

	protected:  // Value
		/**
		 * The xsNCName value of the text data of this element. 
		 */
		xsNCName _value;

	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return Returns a xsNCName of the value.
		 */
		xsNCName getValue() const { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( xsNCName val ) { *(daeStringRef*)&_value = val; }

	protected:
		/**
		 * Constructor
		 */
		domSemantic(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domSemantic() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domSemantic &operator=( const domSemantic &cpy ) { (void)cpy; return *this; }

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

	class domFloat;

	typedef daeSmartRef<domFloat> domFloatRef;
	typedef daeTArray<domFloatRef> domFloat_Array;

	class domFloat : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FLOAT; }
		static daeInt ID() { return 122; }
		virtual daeInt typeID() const { return ID(); }

	protected:  // Value
		/**
		 * The ::domFloat value of the text data of this element. 
		 */
		::domFloat _value;

	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domFloat of the value.
		 */
		::domFloat getValue() const { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( ::domFloat val ) { _value = val; }

	protected:
		/**
		 * Constructor
		 */
		domFloat(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domFloat() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domFloat &operator=( const domFloat &cpy ) { (void)cpy; return *this; }

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

	class domFloat2;

	typedef daeSmartRef<domFloat2> domFloat2Ref;
	typedef daeTArray<domFloat2Ref> domFloat2_Array;

	class domFloat2 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FLOAT2; }
		static daeInt ID() { return 123; }
		virtual daeInt typeID() const { return ID(); }

	protected:  // Value
		/**
		 * The ::domFloat2 value of the text data of this element. 
		 */
		::domFloat2 _value;

	public:	//Accessors and Mutators
		/**
		 * Gets the _value array.
		 * @return Returns a ::domFloat2 reference of the _value array.
		 */
		::domFloat2 &getValue() { return _value; }
		/**
		 * Gets the _value array.
		 * @return Returns a constant ::domFloat2 reference of the _value array.
		 */
		const ::domFloat2 &getValue() const { return _value; }
		/**
		 * Sets the _value array.
		 * @param val The new value for the _value array.
		 */
		void setValue( const ::domFloat2 &val ) { _value = val; }

	protected:
		/**
		 * Constructor
		 */
		domFloat2(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domFloat2() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domFloat2 &operator=( const domFloat2 &cpy ) { (void)cpy; return *this; }

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

	class domFloat3;

	typedef daeSmartRef<domFloat3> domFloat3Ref;
	typedef daeTArray<domFloat3Ref> domFloat3_Array;

	class domFloat3 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FLOAT3; }
		static daeInt ID() { return 124; }
		virtual daeInt typeID() const { return ID(); }

	protected:  // Value
		/**
		 * The ::domFloat3 value of the text data of this element. 
		 */
		::domFloat3 _value;

	public:	//Accessors and Mutators
		/**
		 * Gets the _value array.
		 * @return Returns a ::domFloat3 reference of the _value array.
		 */
		::domFloat3 &getValue() { return _value; }
		/**
		 * Gets the _value array.
		 * @return Returns a constant ::domFloat3 reference of the _value array.
		 */
		const ::domFloat3 &getValue() const { return _value; }
		/**
		 * Sets the _value array.
		 * @param val The new value for the _value array.
		 */
		void setValue( const ::domFloat3 &val ) { _value = val; }

	protected:
		/**
		 * Constructor
		 */
		domFloat3(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domFloat3() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domFloat3 &operator=( const domFloat3 &cpy ) { (void)cpy; return *this; }

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

	class domFloat4;

	typedef daeSmartRef<domFloat4> domFloat4Ref;
	typedef daeTArray<domFloat4Ref> domFloat4_Array;

	class domFloat4 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FLOAT4; }
		static daeInt ID() { return 125; }
		virtual daeInt typeID() const { return ID(); }

	protected:  // Value
		/**
		 * The ::domFloat4 value of the text data of this element. 
		 */
		::domFloat4 _value;

	public:	//Accessors and Mutators
		/**
		 * Gets the _value array.
		 * @return Returns a ::domFloat4 reference of the _value array.
		 */
		::domFloat4 &getValue() { return _value; }
		/**
		 * Gets the _value array.
		 * @return Returns a constant ::domFloat4 reference of the _value array.
		 */
		const ::domFloat4 &getValue() const { return _value; }
		/**
		 * Sets the _value array.
		 * @param val The new value for the _value array.
		 */
		void setValue( const ::domFloat4 &val ) { _value = val; }

	protected:
		/**
		 * Constructor
		 */
		domFloat4(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domFloat4() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domFloat4 &operator=( const domFloat4 &cpy ) { (void)cpy; return *this; }

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
 *  The sid attribute is a text string value containing the sub-identifier
 * of this element.  This value must be unique within the scope of the parent
 * element. Optional attribute. 
 */
	xsNCName attrSid;

protected:  // Elements
	domSemanticRef elemSemantic;
	domFloatRef elemFloat;
	domFloat2Ref elemFloat2;
	domFloat3Ref elemFloat3;
	domFloat4Ref elemFloat4;
	domFx_surface_commonRef elemSurface;
	domFx_sampler2D_commonRef elemSampler2D;
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
	 * @return Returns a xsNCName of the sid attribute.
	 */
	xsNCName getSid() const { return attrSid; }
	/**
	 * Sets the sid attribute.
	 * @param atSid The new value for the sid attribute.
	 */
	void setSid( xsNCName atSid ) { *(daeStringRef*)&attrSid = atSid;}

	/**
	 * Gets the semantic element.
	 * @return a daeSmartRef to the semantic element.
	 */
	const domSemanticRef getSemantic() const { return elemSemantic; }
	/**
	 * Gets the float element.
	 * @return a daeSmartRef to the float element.
	 */
	const domFloatRef getFloat() const { return elemFloat; }
	/**
	 * Gets the float2 element.
	 * @return a daeSmartRef to the float2 element.
	 */
	const domFloat2Ref getFloat2() const { return elemFloat2; }
	/**
	 * Gets the float3 element.
	 * @return a daeSmartRef to the float3 element.
	 */
	const domFloat3Ref getFloat3() const { return elemFloat3; }
	/**
	 * Gets the float4 element.
	 * @return a daeSmartRef to the float4 element.
	 */
	const domFloat4Ref getFloat4() const { return elemFloat4; }
	/**
	 * Gets the surface element.
	 * @return a daeSmartRef to the surface element.
	 */
	const domFx_surface_commonRef getSurface() const { return elemSurface; }
	/**
	 * Gets the sampler2D element.
	 * @return a daeSmartRef to the sampler2D element.
	 */
	const domFx_sampler2D_commonRef getSampler2D() const { return elemSampler2D; }
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
	domCommon_newparam_type_complexType(DAE& dae, daeElement* elt) : attrSid(), elemSemantic(), elemFloat(), elemFloat2(), elemFloat3(), elemFloat4(), elemSurface(), elemSampler2D() {}
	/**
	 * Destructor
	 */
	virtual ~domCommon_newparam_type_complexType() { daeElement::deleteCMDataArray(_CMData); }
	/**
	 * Overloaded assignment operator
	 */
	virtual domCommon_newparam_type_complexType &operator=( const domCommon_newparam_type_complexType &cpy ) { (void)cpy; return *this; }
};

/**
 * An element of type domCommon_newparam_type_complexType.
 */
class domCommon_newparam_type : public daeElement, public domCommon_newparam_type_complexType
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::COMMON_NEWPARAM_TYPE; }
	static daeInt ID() { return 126; }
	virtual daeInt typeID() const { return ID(); }

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

protected:
	/**
	 * Constructor
	 */
	domCommon_newparam_type(DAE& dae) : daeElement(dae), domCommon_newparam_type_complexType(dae, this) {}
	/**
	 * Destructor
	 */
	virtual ~domCommon_newparam_type() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domCommon_newparam_type &operator=( const domCommon_newparam_type &cpy ) { (void)cpy; return *this; }

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
