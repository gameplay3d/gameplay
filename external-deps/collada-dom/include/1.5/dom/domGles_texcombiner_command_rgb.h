#ifndef __domGles_texcombiner_command_rgb_h__
#define __domGles_texcombiner_command_rgb_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domGles_texcombiner_argument_rgb.h>
class DAE;

/**
 * Defines the RGB portion of a texture_pipeline command. This is a combiner-mode
 * texturing operation.
 */
class domGles_texcombiner_command_rgb : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::GLES_TEXCOMBINER_COMMAND_RGB; }
	static daeInt ID() { return 264; }
	virtual daeInt typeID() const { return ID(); }
protected:  // Attributes
	domGles_texcombiner_operator_rgb attrOperator;
	xsFloat attrScale;

protected:  // Element
	domGles_texcombiner_argument_rgb_Array elemArgument_array;

public:	//Accessors and Mutators
	/**
	 * Gets the operator attribute.
	 * @return Returns a domGles_texcombiner_operator_rgb of the operator attribute.
	 */
	domGles_texcombiner_operator_rgb getOperator() const { return attrOperator; }
	/**
	 * Sets the operator attribute.
	 * @param atOperator The new value for the operator attribute.
	 */
	void setOperator( domGles_texcombiner_operator_rgb atOperator ) { attrOperator = atOperator; }

	/**
	 * Gets the scale attribute.
	 * @return Returns a xsFloat of the scale attribute.
	 */
	xsFloat getScale() const { return attrScale; }
	/**
	 * Sets the scale attribute.
	 * @param atScale The new value for the scale attribute.
	 */
	void setScale( xsFloat atScale ) { attrScale = atScale; }

	/**
	 * Gets the argument element array.
	 * @return Returns a reference to the array of argument elements.
	 */
	domGles_texcombiner_argument_rgb_Array &getArgument_array() { return elemArgument_array; }
	/**
	 * Gets the argument element array.
	 * @return Returns a constant reference to the array of argument elements.
	 */
	const domGles_texcombiner_argument_rgb_Array &getArgument_array() const { return elemArgument_array; }
protected:
	/**
	 * Constructor
	 */
	domGles_texcombiner_command_rgb(DAE& dae) : daeElement(dae), attrOperator(), attrScale(), elemArgument_array() {}
	/**
	 * Destructor
	 */
	virtual ~domGles_texcombiner_command_rgb() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domGles_texcombiner_command_rgb &operator=( const domGles_texcombiner_command_rgb &cpy ) { (void)cpy; return *this; }

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
