#ifndef __domFx_sources_h__
#define __domFx_sources_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

class DAE;

class domFx_sources : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FX_SOURCES; }
	static daeInt ID() { return 182; }
	virtual daeInt typeID() const { return ID(); }
public:
	class domInline;

	typedef daeSmartRef<domInline> domInlineRef;
	typedef daeTArray<domInlineRef> domInline_Array;

/**
 * this is allows you to include some code right here....like a #define for
 * an uber shader that is imported
 */
	class domInline : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::INLINE; }
		static daeInt ID() { return 183; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return Returns a xsString of the value.
		 */
		xsString getValue() const { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( xsString val ) { *(daeStringRef*)&_value = val; }

	protected:  // Value
		/**
		 * The xsString value of the text data of this element. 
		 */
		xsString _value;
	protected:
		/**
		 * Constructor
		 */
		domInline(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domInline() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domInline &operator=( const domInline &cpy ) { (void)cpy; return *this; }

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

	class domImport;

	typedef daeSmartRef<domImport> domImportRef;
	typedef daeTArray<domImportRef> domImport_Array;

/**
 * Ref attribute identifies the sid of a code or include element
 */
	class domImport : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::IMPORT; }
		static daeInt ID() { return 184; }
		virtual daeInt typeID() const { return ID(); }
	protected:  // Attribute
		xsToken attrRef;


	public:	//Accessors and Mutators
		/**
		 * Gets the ref attribute.
		 * @return Returns a xsToken of the ref attribute.
		 */
		xsToken getRef() const { return attrRef; }
		/**
		 * Sets the ref attribute.
		 * @param atRef The new value for the ref attribute.
		 */
		void setRef( xsToken atRef ) { *(daeStringRef*)&attrRef = atRef; _validAttributeArray[0] = true; }

	protected:
		/**
		 * Constructor
		 */
		domImport(DAE& dae) : daeElement(dae), attrRef() {}
		/**
		 * Destructor
		 */
		virtual ~domImport() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domImport &operator=( const domImport &cpy ) { (void)cpy; return *this; }

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
/**
 * this is allows you to include some code right here....like a #define for
 * an uber shader that is imported @see domInline
 */
	domInline_Array elemInline_array;
/**
 * Ref attribute identifies the sid of a code or include element @see domImport
 */
	domImport_Array elemImport_array;
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
	 * Gets the inline element array.
	 * @return Returns a reference to the array of inline elements.
	 */
	domInline_Array &getInline_array() { return elemInline_array; }
	/**
	 * Gets the inline element array.
	 * @return Returns a constant reference to the array of inline elements.
	 */
	const domInline_Array &getInline_array() const { return elemInline_array; }
	/**
	 * Gets the import element array.
	 * @return Returns a reference to the array of import elements.
	 */
	domImport_Array &getImport_array() { return elemImport_array; }
	/**
	 * Gets the import element array.
	 * @return Returns a constant reference to the array of import elements.
	 */
	const domImport_Array &getImport_array() const { return elemImport_array; }
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
	domFx_sources(DAE& dae) : daeElement(dae), elemInline_array(), elemImport_array() {}
	/**
	 * Destructor
	 */
	virtual ~domFx_sources() { daeElement::deleteCMDataArray(_CMData); }
	/**
	 * Overloaded assignment operator
	 */
	virtual domFx_sources &operator=( const domFx_sources &cpy ) { (void)cpy; return *this; }

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
