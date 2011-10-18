/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 

#ifndef __domFx_surface_init_planar_common_h__
#define __domFx_surface_init_planar_common_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

class DAE;

/**
 * For 1D, 2D, RECT surface types
 */
class domFx_surface_init_planar_common_complexType 
{
public:
	class domAll;

	typedef daeSmartRef<domAll> domAllRef;
	typedef daeTArray<domAllRef> domAll_Array;

/**
 * Init the entire surface with one compound image such as DDS
 */
	class domAll : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::ALL; }
		static daeInt ID() { return 11; }
		virtual daeInt typeID() const { return ID(); }
	protected:  // Attribute
		xsIDREF attrRef;


	public:	//Accessors and Mutators
		/**
		 * Gets the ref attribute.
		 * @return Returns a xsIDREF reference of the ref attribute.
		 */
		xsIDREF &getRef() { return attrRef; }
		/**
		 * Gets the ref attribute.
		 * @return Returns a constant xsIDREF reference of the ref attribute.
		 */
		const xsIDREF &getRef() const{ return attrRef; }
		/**
		 * Sets the ref attribute.
		 * @param atRef The new value for the ref attribute.
		 */
		void setRef( const xsIDREF &atRef ) { attrRef = atRef; _validAttributeArray[0] = true; }

	protected:
		/**
		 * Constructor
		 */
		domAll(DAE& dae) : daeElement(dae), attrRef(*this) {}
		/**
		 * Destructor
		 */
		virtual ~domAll() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domAll &operator=( const domAll &cpy ) { (void)cpy; return *this; }

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



protected:  // Element
/**
 * Init the entire surface with one compound image such as DDS @see domAll
 */
	domAllRef elemAll;
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
	 * Gets the all element.
	 * @return a daeSmartRef to the all element.
	 */
	const domAllRef getAll() const { return elemAll; }
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
	domFx_surface_init_planar_common_complexType(DAE& dae, daeElement* elt) : elemAll() {}
	/**
	 * Destructor
	 */
	virtual ~domFx_surface_init_planar_common_complexType() { daeElement::deleteCMDataArray(_CMData); }
	/**
	 * Overloaded assignment operator
	 */
	virtual domFx_surface_init_planar_common_complexType &operator=( const domFx_surface_init_planar_common_complexType &cpy ) { (void)cpy; return *this; }
};

/**
 * An element of type domFx_surface_init_planar_common_complexType.
 */
class domFx_surface_init_planar_common : public daeElement, public domFx_surface_init_planar_common_complexType
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FX_SURFACE_INIT_PLANAR_COMMON; }
	static daeInt ID() { return 12; }
	virtual daeInt typeID() const { return ID(); }
protected:
	/**
	 * Constructor
	 */
	domFx_surface_init_planar_common(DAE& dae) : daeElement(dae), domFx_surface_init_planar_common_complexType(dae, this) {}
	/**
	 * Destructor
	 */
	virtual ~domFx_surface_init_planar_common() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domFx_surface_init_planar_common &operator=( const domFx_surface_init_planar_common &cpy ) { (void)cpy; return *this; }

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
