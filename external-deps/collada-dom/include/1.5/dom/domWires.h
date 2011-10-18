#ifndef __domWires_h__
#define __domWires_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domInput_local_offset.h>
#include <dom/domP.h>
#include <dom/domExtra.h>
class DAE;

/**
 * This element defines all the wires used in the brep     structure.
 */
class domWires : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::WIRES; }
	static daeInt ID() { return 301; }
	virtual daeInt typeID() const { return ID(); }
public:
	class domVcount;

	typedef daeSmartRef<domVcount> domVcountRef;
	typedef daeTArray<domVcountRef> domVcount_Array;

	class domVcount : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::VCOUNT; }
		static daeInt ID() { return 302; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a domList_of_uints of the value.
		 */
		domList_of_uints& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const domList_of_uints& val ) { _value = val; }

	protected:  // Value
		/**
		 * The domList_of_uints value of the text data of this element. 
		 */
		domList_of_uints _value;
	protected:
		/**
		 * Constructor
		 */
		domVcount(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domVcount() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domVcount &operator=( const domVcount &cpy ) { (void)cpy; return *this; }

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
	xsID attrId;
	xsToken attrName;
	domUint attrCount;

protected:  // Elements
/**
 * One input elements is needed to define a wire by its edges. The second
 * is needed for orientation of the edges @see domInput
 */
	domInput_local_offset_Array elemInput_array;
	domVcountRef elemVcount;
/**
 * The indices of the edges for each wire. @see domP
 */
	domPRef elemP;
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
	void setId( xsID atId ) { *(daeStringRef*)&attrId = atId;
		if( _document != NULL ) _document->changeElementID( this, attrId );
	}

	/**
	 * Gets the name attribute.
	 * @return Returns a xsToken of the name attribute.
	 */
	xsToken getName() const { return attrName; }
	/**
	 * Sets the name attribute.
	 * @param atName The new value for the name attribute.
	 */
	void setName( xsToken atName ) { *(daeStringRef*)&attrName = atName;}

	/**
	 * Gets the count attribute.
	 * @return Returns a domUint of the count attribute.
	 */
	domUint getCount() const { return attrCount; }
	/**
	 * Sets the count attribute.
	 * @param atCount The new value for the count attribute.
	 */
	void setCount( domUint atCount ) { attrCount = atCount; }

	/**
	 * Gets the input element array.
	 * @return Returns a reference to the array of input elements.
	 */
	domInput_local_offset_Array &getInput_array() { return elemInput_array; }
	/**
	 * Gets the input element array.
	 * @return Returns a constant reference to the array of input elements.
	 */
	const domInput_local_offset_Array &getInput_array() const { return elemInput_array; }
	/**
	 * Gets the vcount element.
	 * @return a daeSmartRef to the vcount element.
	 */
	const domVcountRef getVcount() const { return elemVcount; }
	/**
	 * Gets the p element.
	 * @return a daeSmartRef to the p element.
	 */
	const domPRef getP() const { return elemP; }
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
	domWires(DAE& dae) : daeElement(dae), attrId(), attrName(), attrCount(), elemInput_array(), elemVcount(), elemP(), elemExtra_array() {}
	/**
	 * Destructor
	 */
	virtual ~domWires() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domWires &operator=( const domWires &cpy ) { (void)cpy; return *this; }

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
