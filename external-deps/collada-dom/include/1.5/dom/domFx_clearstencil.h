#ifndef __domFx_clearstencil_h__
#define __domFx_clearstencil_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

class DAE;

class domFx_clearstencil : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FX_CLEARSTENCIL; }
	static daeInt ID() { return 136; }
	virtual daeInt typeID() const { return ID(); }
protected:  // Attribute
	xsNonNegativeInteger attrIndex;


public:	//Accessors and Mutators
	/**
	 * Gets the index attribute.
	 * @return Returns a xsNonNegativeInteger of the index attribute.
	 */
	xsNonNegativeInteger getIndex() const { return attrIndex; }
	/**
	 * Sets the index attribute.
	 * @param atIndex The new value for the index attribute.
	 */
	void setIndex( xsNonNegativeInteger atIndex ) { attrIndex = atIndex; }

	/**
	 * Gets the value of this element.
	 * @return a xsByte of the value.
	 */
	xsByte& getValue() { return _value; }
	/**
	 * Sets the _value of this element.
	 * @param val The new value for this element.
	 */
	void setValue( const xsByte& val ) { _value = val; }

protected:  // Value
	/**
	 * The xsByte value of the text data of this element. 
	 */
	xsByte _value;
protected:
	/**
	 * Constructor
	 */
	domFx_clearstencil(DAE& dae) : daeElement(dae), attrIndex(), _value() {}
	/**
	 * Destructor
	 */
	virtual ~domFx_clearstencil() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domFx_clearstencil &operator=( const domFx_clearstencil &cpy ) { (void)cpy; return *this; }

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
