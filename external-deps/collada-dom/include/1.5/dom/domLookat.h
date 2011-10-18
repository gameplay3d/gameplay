#ifndef __domLookat_h__
#define __domLookat_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

class DAE;

/**
 * The lookat element contains a position and orientation transformation suitable
 * for aiming a camera. The lookat element contains three mathematical vectors
 * within it that describe:  1.The position of the object; 2.The position
 * of the interest point; 3.The direction that points up.
 */
class domLookat : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::LOOKAT; }
	static daeInt ID() { return 14; }
	virtual daeInt typeID() const { return ID(); }
protected:  // Attribute
/**
 *  The sid attribute is a text string value containing the sub-identifier
 * of this element.  This value must be unique within the scope of the parent
 * element. Optional attribute. 
 */
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
	 * @return a domFloat3x3 of the value.
	 */
	domFloat3x3& getValue() { return _value; }
	/**
	 * Sets the _value of this element.
	 * @param val The new value for this element.
	 */
	void setValue( const domFloat3x3& val ) { _value = val; }

protected:  // Value
	/**
	 * The domFloat3x3 value of the text data of this element. 
	 */
	domFloat3x3 _value;
protected:
	/**
	 * Constructor
	 */
	domLookat(DAE& dae) : daeElement(dae), attrSid(), _value() {}
	/**
	 * Destructor
	 */
	virtual ~domLookat() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domLookat &operator=( const domLookat &cpy ) { (void)cpy; return *this; }

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
