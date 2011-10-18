#ifndef __domCg_array_h__
#define __domCg_array_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domCg_param.h>
class DAE;

/**
 * Creates a parameter of a one-dimensional array type.
 */
class domCg_array : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::CG_ARRAY; }
	static daeInt ID() { return 243; }
	virtual daeInt typeID() const { return ID(); }
protected:  // Attributes
/**
 *  The length attribute specifies the length of the array. 
 */
	xsPositiveInteger attrLength;
/**
 * The array may be resized when changed because it is connected to a Cg unsized
 * array.
 */
	xsBoolean attrResizable;

protected:  // Element
/**
 * First element types the array.  If more than one then init entire array
 * and must match length attribute.  If only one then repeat based on length
 * attribute. @see domCg_param
 */
	domCg_param_Array elemCg_param_array;

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
	 * Gets the resizable attribute.
	 * @return Returns a xsBoolean of the resizable attribute.
	 */
	xsBoolean getResizable() const { return attrResizable; }
	/**
	 * Sets the resizable attribute.
	 * @param atResizable The new value for the resizable attribute.
	 */
	void setResizable( xsBoolean atResizable ) { attrResizable = atResizable; }

	/**
	 * Gets the cg_param element array.
	 * @return Returns a reference to the array of cg_param elements.
	 */
	domCg_param_Array &getCg_param_array() { return elemCg_param_array; }
	/**
	 * Gets the cg_param element array.
	 * @return Returns a constant reference to the array of cg_param elements.
	 */
	const domCg_param_Array &getCg_param_array() const { return elemCg_param_array; }
protected:
	/**
	 * Constructor
	 */
	domCg_array(DAE& dae) : daeElement(dae), attrLength(), attrResizable(), elemCg_param_array() {}
	/**
	 * Destructor
	 */
	virtual ~domCg_array() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domCg_array &operator=( const domCg_array &cpy ) { (void)cpy; return *this; }

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
