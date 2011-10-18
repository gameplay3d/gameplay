#ifndef __domOrigin_h__
#define __domOrigin_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

class DAE;

class domOrigin : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::ORIGIN; }
	static daeInt ID() { return 347; }
	virtual daeInt typeID() const { return ID(); }


public:	//Accessors and Mutators
	/**
	 * Gets the value of this element.
	 * @return a domFloat3 of the value.
	 */
	domFloat3& getValue() { return _value; }
	/**
	 * Sets the _value of this element.
	 * @param val The new value for this element.
	 */
	void setValue( const domFloat3& val ) { _value = val; }

protected:  // Value
	/**
	 * The domFloat3 value of the text data of this element. 
	 */
	domFloat3 _value;
protected:
	/**
	 * Constructor
	 */
	domOrigin(DAE& dae) : daeElement(dae), _value() {}
	/**
	 * Destructor
	 */
	virtual ~domOrigin() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domOrigin &operator=( const domOrigin &cpy ) { (void)cpy; return *this; }

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
