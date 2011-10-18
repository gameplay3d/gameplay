/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 

#ifndef __domCg_newparam_h__
#define __domCg_newparam_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domCg_param_type.h>
#include <dom/domFx_annotate_common.h>
#include <dom/domCg_setuser_type.h>
#include <dom/domCg_newarray_type.h>
class DAE;

/**
 * Create a new, named param object in the CG Runtime, assign it a type, an
 * initial value, and additional attributes at declaration time.
 */
class domCg_newparam_complexType 
{
public:
	class domSemantic;

	typedef daeSmartRef<domSemantic> domSemanticRef;
	typedef daeTArray<domSemanticRef> domSemantic_Array;

/**
 * The semantic element allows you to specify a semantic for this new param.
 */
	class domSemantic : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::SEMANTIC; }
		static daeInt ID() { return 140; }
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

	class domModifier;

	typedef daeSmartRef<domModifier> domModifierRef;
	typedef daeTArray<domModifierRef> domModifier_Array;

/**
 * The modifier element allows you to specify a modifier for this new param.
 */
	class domModifier : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::MODIFIER; }
		static daeInt ID() { return 141; }
		virtual daeInt typeID() const { return ID(); }

	protected:  // Value
		/**
		 * The domFx_modifier_enum_common value of the text data of this element. 
		 */
		domFx_modifier_enum_common _value;

	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a domFx_modifier_enum_common of the value.
		 */
		domFx_modifier_enum_common getValue() const { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( domFx_modifier_enum_common val ) { _value = val; }

	protected:
		/**
		 * Constructor
		 */
		domModifier(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domModifier() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domModifier &operator=( const domModifier &cpy ) { (void)cpy; return *this; }

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
	domCg_identifier attrSid;

protected:  // Elements
/**
 * The annotate element allows you to specify an annotation for this new param.
 * @see domAnnotate
 */
	domFx_annotate_common_Array elemAnnotate_array;
/**
 * The semantic element allows you to specify a semantic for this new param.
 * @see domSemantic
 */
	domSemanticRef elemSemantic;
/**
 * The modifier element allows you to specify a modifier for this new param.
 * @see domModifier
 */
	domModifierRef elemModifier;
	domCg_param_typeRef elemCg_param_type;
	domCg_setuser_typeRef elemUsertype;
	domCg_newarray_typeRef elemArray;
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
	 * @return Returns a domCg_identifier of the sid attribute.
	 */
	domCg_identifier getSid() const { return attrSid; }
	/**
	 * Sets the sid attribute.
	 * @param atSid The new value for the sid attribute.
	 */
	void setSid( domCg_identifier atSid ) { attrSid = atSid; }

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
	 * Gets the semantic element.
	 * @return a daeSmartRef to the semantic element.
	 */
	const domSemanticRef getSemantic() const { return elemSemantic; }
	/**
	 * Gets the modifier element.
	 * @return a daeSmartRef to the modifier element.
	 */
	const domModifierRef getModifier() const { return elemModifier; }
	/**
	 * Gets the cg_param_type element.
	 * @return a daeSmartRef to the cg_param_type element.
	 */
	const domCg_param_typeRef getCg_param_type() const { return elemCg_param_type; }
	/**
	 * Gets the usertype element.
	 * @return a daeSmartRef to the usertype element.
	 */
	const domCg_setuser_typeRef getUsertype() const { return elemUsertype; }
	/**
	 * Gets the array element.
	 * @return a daeSmartRef to the array element.
	 */
	const domCg_newarray_typeRef getArray() const { return elemArray; }
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
	domCg_newparam_complexType(DAE& dae, daeElement* elt) : attrSid(), elemAnnotate_array(), elemSemantic(), elemModifier(), elemCg_param_type(), elemUsertype(), elemArray() {}
	/**
	 * Destructor
	 */
	virtual ~domCg_newparam_complexType() { daeElement::deleteCMDataArray(_CMData); }
	/**
	 * Overloaded assignment operator
	 */
	virtual domCg_newparam_complexType &operator=( const domCg_newparam_complexType &cpy ) { (void)cpy; return *this; }
};

/**
 * An element of type domCg_newparam_complexType.
 */
class domCg_newparam : public daeElement, public domCg_newparam_complexType
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::CG_NEWPARAM; }
	static daeInt ID() { return 142; }
	virtual daeInt typeID() const { return ID(); }

public:	//Accessors and Mutators
	/**
	 * Gets the sid attribute.
	 * @return Returns a domCg_identifier of the sid attribute.
	 */
	domCg_identifier getSid() const { return attrSid; }
	/**
	 * Sets the sid attribute.
	 * @param atSid The new value for the sid attribute.
	 */
	void setSid( domCg_identifier atSid ) { attrSid = atSid; _validAttributeArray[0] = true; }

protected:
	/**
	 * Constructor
	 */
	domCg_newparam(DAE& dae) : daeElement(dae), domCg_newparam_complexType(dae, this) {}
	/**
	 * Destructor
	 */
	virtual ~domCg_newparam() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domCg_newparam &operator=( const domCg_newparam &cpy ) { (void)cpy; return *this; }

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
