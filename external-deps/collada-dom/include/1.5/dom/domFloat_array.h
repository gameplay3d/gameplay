#ifndef __domFloat_array_h__
#define __domFloat_array_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

class DAE;

/**
 * The float_array element declares the storage for a homogenous array of
 * floating point values.
 */
class domFloat_array : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FLOAT_ARRAY; }
	static daeInt ID() { return 9; }
	virtual daeInt typeID() const { return ID(); }
protected:  // Attributes
/**
 *  The id attribute is a text string containing the unique identifier of
 * this element. This value  must be unique within the instance document.
 * Optional attribute. 
 */
	xsID attrId;
/**
 *  The name attribute is the text string name of this element. Optional attribute.
 */
	xsToken attrName;
/**
 *  The count attribute indicates the number of values in the array. Required
 * attribute. 
 */
	domUint attrCount;
/**
 *  The digits attribute indicates the number of significant decimal digits
 * of the float values that  can be contained in the array. The default value
 * is 6. Optional attribute. 
 */
	domDigits attrDigits;
/**
 *  The magnitude attribute indicates the largest exponent of the float values
 * that can be contained  in the array. The default value is 38. Optional
 * attribute. 
 */
	domMagnitude attrMagnitude;


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
	 * Gets the digits attribute.
	 * @return Returns a domDigits of the digits attribute.
	 */
	domDigits getDigits() const { return attrDigits; }
	/**
	 * Sets the digits attribute.
	 * @param atDigits The new value for the digits attribute.
	 */
	void setDigits( domDigits atDigits ) { attrDigits = atDigits; }

	/**
	 * Gets the magnitude attribute.
	 * @return Returns a domMagnitude of the magnitude attribute.
	 */
	domMagnitude getMagnitude() const { return attrMagnitude; }
	/**
	 * Sets the magnitude attribute.
	 * @param atMagnitude The new value for the magnitude attribute.
	 */
	void setMagnitude( domMagnitude atMagnitude ) { attrMagnitude = atMagnitude; }

	/**
	 * Gets the value of this element.
	 * @return a domList_of_floats of the value.
	 */
	domList_of_floats& getValue() { return _value; }
	/**
	 * Sets the _value of this element.
	 * @param val The new value for this element.
	 */
	void setValue( const domList_of_floats& val ) { _value = val; }

protected:  // Value
	/**
	 * The domList_of_floats value of the text data of this element. 
	 */
	domList_of_floats _value;
protected:
	/**
	 * Constructor
	 */
	domFloat_array(DAE& dae) : daeElement(dae), attrId(), attrName(), attrCount(), attrDigits(), attrMagnitude(), _value() {}
	/**
	 * Destructor
	 */
	virtual ~domFloat_array() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domFloat_array &operator=( const domFloat_array &cpy ) { (void)cpy; return *this; }

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
