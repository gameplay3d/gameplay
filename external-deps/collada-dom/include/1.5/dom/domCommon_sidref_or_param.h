#ifndef __domCommon_sidref_or_param_h__
#define __domCommon_sidref_or_param_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domCommon_param.h>
class DAE;

class domCommon_sidref_or_param : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::COMMON_SIDREF_OR_PARAM; }
	static daeInt ID() { return 430; }
	virtual daeInt typeID() const { return ID(); }
public:
	class domSIDREF;

	typedef daeSmartRef<domSIDREF> domSIDREFRef;
	typedef daeTArray<domSIDREFRef> domSIDREF_Array;

	class domSIDREF : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::SIDREF; }
		static daeInt ID() { return 431; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return Returns a domSidref of the value.
		 */
		domSidref getValue() const { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( domSidref val ) { *(daeStringRef*)&_value = val; }

	protected:  // Value
		/**
		 * The domSidref value of the text data of this element. 
		 */
		domSidref _value;
	protected:
		/**
		 * Constructor
		 */
		domSIDREF(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domSIDREF() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domSIDREF &operator=( const domSIDREF &cpy ) { (void)cpy; return *this; }

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
	domSIDREFRef elemSIDREF;
	domCommon_paramRef elemParam;
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
	 * Gets the SIDREF element.
	 * @return a daeSmartRef to the SIDREF element.
	 */
	const domSIDREFRef getSIDREF() const { return elemSIDREF; }
	/**
	 * Gets the param element.
	 * @return a daeSmartRef to the param element.
	 */
	const domCommon_paramRef getParam() const { return elemParam; }
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
	domCommon_sidref_or_param(DAE& dae) : daeElement(dae), elemSIDREF(), elemParam() {}
	/**
	 * Destructor
	 */
	virtual ~domCommon_sidref_or_param() { daeElement::deleteCMDataArray(_CMData); }
	/**
	 * Overloaded assignment operator
	 */
	virtual domCommon_sidref_or_param &operator=( const domCommon_sidref_or_param &cpy ) { (void)cpy; return *this; }

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
