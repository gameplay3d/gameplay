#ifndef __domGlsl_array_h__
#define __domGlsl_array_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domGlsl_value.h>
class DAE;

/**
 * The glsl_newarray_type element is used to creates a parameter of a one-dimensional
 * array type.
 */
class domGlsl_array : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::GLSL_ARRAY; }
	static daeInt ID() { return 227; }
	virtual daeInt typeID() const { return ID(); }
protected:  // Attribute
/**
 *  The length attribute specifies the length of the array. 
 */
	xsPositiveInteger attrLength;

protected:  // Element
	domGlsl_value_Array elemGlsl_value_array;

public:	//Accessors and Mutators
	/**
	 * Gets the length attribute.
	 * @return Returns a xsPositiveInteger of the length attribute.
	 */
	xsPositiveInteger getLength() const { return attrLength; }
	/**
	 * Sets the length attribute.
	 * @param atLength The new value for the length attribute.
	 */
	void setLength( xsPositiveInteger atLength ) { attrLength = atLength; }

	/**
	 * Gets the glsl_value element array.
	 * @return Returns a reference to the array of glsl_value elements.
	 */
	domGlsl_value_Array &getGlsl_value_array() { return elemGlsl_value_array; }
	/**
	 * Gets the glsl_value element array.
	 * @return Returns a constant reference to the array of glsl_value elements.
	 */
	const domGlsl_value_Array &getGlsl_value_array() const { return elemGlsl_value_array; }
protected:
	/**
	 * Constructor
	 */
	domGlsl_array(DAE& dae) : daeElement(dae), attrLength(), elemGlsl_value_array() {}
	/**
	 * Destructor
	 */
	virtual ~domGlsl_array() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domGlsl_array &operator=( const domGlsl_array &cpy ) { (void)cpy; return *this; }

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
