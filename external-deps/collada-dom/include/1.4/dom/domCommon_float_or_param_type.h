/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 

#ifndef __domCommon_float_or_param_type_h__
#define __domCommon_float_or_param_type_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

class DAE;

class domCommon_float_or_param_type_complexType 
{
public:
	class domFloat;

	typedef daeSmartRef<domFloat> domFloatRef;
	typedef daeTArray<domFloatRef> domFloat_Array;

	class domFloat : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FLOAT; }
		static daeInt ID() { return 113; }
		virtual daeInt typeID() const { return ID(); }
	protected:  // Attribute
		xsNCName attrSid;

	protected:  // Value
		/**
		 * The ::domFloat value of the text data of this element. 
		 */
		::domFloat _value;

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
		domFloat(DAE& dae) : daeElement(dae), attrSid(), _value() {}
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

	class domParam;

	typedef daeSmartRef<domParam> domParamRef;
	typedef daeTArray<domParamRef> domParam_Array;

	class domParam : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::PARAM; }
		static daeInt ID() { return 114; }
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



protected:  // Elements
	domFloatRef elemFloat;
	domParamRef elemParam;
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
	 * Gets the float element.
	 * @return a daeSmartRef to the float element.
	 */
	const domFloatRef getFloat() const { return elemFloat; }
	/**
	 * Gets the param element.
	 * @return a daeSmartRef to the param element.
	 */
	const domParamRef getParam() const { return elemParam; }
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
	domCommon_float_or_param_type_complexType(DAE& dae, daeElement* elt) : elemFloat(), elemParam() {}
	/**
	 * Destructor
	 */
	virtual ~domCommon_float_or_param_type_complexType() { daeElement::deleteCMDataArray(_CMData); }
	/**
	 * Overloaded assignment operator
	 */
	virtual domCommon_float_or_param_type_complexType &operator=( const domCommon_float_or_param_type_complexType &cpy ) { (void)cpy; return *this; }
};

/**
 * An element of type domCommon_float_or_param_type_complexType.
 */
class domCommon_float_or_param_type : public daeElement, public domCommon_float_or_param_type_complexType
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::COMMON_FLOAT_OR_PARAM_TYPE; }
	static daeInt ID() { return 115; }
	virtual daeInt typeID() const { return ID(); }
protected:
	/**
	 * Constructor
	 */
	domCommon_float_or_param_type(DAE& dae) : daeElement(dae), domCommon_float_or_param_type_complexType(dae, this) {}
	/**
	 * Destructor
	 */
	virtual ~domCommon_float_or_param_type() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domCommon_float_or_param_type &operator=( const domCommon_float_or_param_type &cpy ) { (void)cpy; return *this; }

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
