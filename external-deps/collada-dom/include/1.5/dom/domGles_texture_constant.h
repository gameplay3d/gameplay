#ifndef __domGles_texture_constant_h__
#define __domGles_texture_constant_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

class DAE;

class domGles_texture_constant : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::GLES_TEXTURE_CONSTANT; }
	static daeInt ID() { return 260; }
	virtual daeInt typeID() const { return ID(); }
protected:  // Attributes
	domFloat4 attrValue;
	xsNCName attrParam;


public:	//Accessors and Mutators
	/**
	 * Gets the value attribute.
	 * @return Returns a domFloat4 of the value attribute.
	 */
	domFloat4 getValue() const { return attrValue; }
	/**
	 * Sets the value attribute.
	 * @param atValue The new value for the value attribute.
	 */
	void setValue( domFloat4 atValue ) { attrValue = atValue; }

	/**
	 * Gets the param attribute.
	 * @return Returns a xsNCName of the param attribute.
	 */
	xsNCName getParam() const { return attrParam; }
	/**
	 * Sets the param attribute.
	 * @param atParam The new value for the param attribute.
	 */
	void setParam( xsNCName atParam ) { *(daeStringRef*)&attrParam = atParam;}

protected:
	/**
	 * Constructor
	 */
	domGles_texture_constant(DAE& dae) : daeElement(dae), attrValue(), attrParam() {}
	/**
	 * Destructor
	 */
	virtual ~domGles_texture_constant() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domGles_texture_constant &operator=( const domGles_texture_constant &cpy ) { (void)cpy; return *this; }

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
