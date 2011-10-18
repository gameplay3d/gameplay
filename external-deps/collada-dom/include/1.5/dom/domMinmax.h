#ifndef __domMinmax_h__
#define __domMinmax_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

class DAE;

class domMinmax : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::MINMAX; }
	static daeInt ID() { return 458; }
	virtual daeInt typeID() const { return ID(); }
protected:  // Attributes
	xsToken attrName;
	domSid attrSid;


public:	//Accessors and Mutators
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
	 * Gets the sid attribute.
	 * @return Returns a domSid of the sid attribute.
	 */
	domSid getSid() const { return attrSid; }
	/**
	 * Sets the sid attribute.
	 * @param atSid The new value for the sid attribute.
	 */
	void setSid( domSid atSid ) { *(daeStringRef*)&attrSid = atSid;}

	/**
	 * Gets the value of this element.
	 * @return a domFloat of the value.
	 */
	domFloat& getValue() { return _value; }
	/**
	 * Sets the _value of this element.
	 * @param val The new value for this element.
	 */
	void setValue( const domFloat& val ) { _value = val; }

protected:  // Value
	/**
	 * The domFloat value of the text data of this element. 
	 */
	domFloat _value;
protected:
	/**
	 * Constructor
	 */
	domMinmax(DAE& dae) : daeElement(dae), attrName(), attrSid(), _value() {}
	/**
	 * Destructor
	 */
	virtual ~domMinmax() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domMinmax &operator=( const domMinmax &cpy ) { (void)cpy; return *this; }

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
