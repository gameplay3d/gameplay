#ifndef __domTargetable_float4_h__
#define __domTargetable_float4_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

class DAE;

class domTargetable_float4 : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::TARGETABLE_FLOAT4; }
	static daeInt ID() { return 368; }
	virtual daeInt typeID() const { return ID(); }
protected:  // Attribute
	domSid attrSid;


public:	//Accessors and Mutators
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
	 * @return a domFloat4 of the value.
	 */
	domFloat4& getValue() { return _value; }
	/**
	 * Sets the _value of this element.
	 * @param val The new value for this element.
	 */
	void setValue( const domFloat4& val ) { _value = val; }

protected:  // Value
	/**
	 * The domFloat4 value of the text data of this element. 
	 */
	domFloat4 _value;
protected:
	/**
	 * Constructor
	 */
	domTargetable_float4(DAE& dae) : daeElement(dae), attrSid(), _value() {}
	/**
	 * Destructor
	 */
	virtual ~domTargetable_float4() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domTargetable_float4 &operator=( const domTargetable_float4 &cpy ) { (void)cpy; return *this; }

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
