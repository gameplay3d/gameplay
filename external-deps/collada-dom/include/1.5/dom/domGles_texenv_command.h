#ifndef __domGles_texenv_command_h__
#define __domGles_texenv_command_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domGles_texture_constant.h>
class DAE;

class domGles_texenv_command : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::GLES_TEXENV_COMMAND; }
	static daeInt ID() { return 261; }
	virtual daeInt typeID() const { return ID(); }
protected:  // Attributes
	domGles_texenv_mode attrOperator;
	xsNCName attrSampler;

protected:  // Element
	domGles_texture_constantRef elemConstant;

public:	//Accessors and Mutators
	/**
	 * Gets the operator attribute.
	 * @return Returns a domGles_texenv_mode of the operator attribute.
	 */
	domGles_texenv_mode getOperator() const { return attrOperator; }
	/**
	 * Sets the operator attribute.
	 * @param atOperator The new value for the operator attribute.
	 */
	void setOperator( domGles_texenv_mode atOperator ) { attrOperator = atOperator; }

	/**
	 * Gets the sampler attribute.
	 * @return Returns a xsNCName of the sampler attribute.
	 */
	xsNCName getSampler() const { return attrSampler; }
	/**
	 * Sets the sampler attribute.
	 * @param atSampler The new value for the sampler attribute.
	 */
	void setSampler( xsNCName atSampler ) { *(daeStringRef*)&attrSampler = atSampler;}

	/**
	 * Gets the constant element.
	 * @return a daeSmartRef to the constant element.
	 */
	const domGles_texture_constantRef getConstant() const { return elemConstant; }
protected:
	/**
	 * Constructor
	 */
	domGles_texenv_command(DAE& dae) : daeElement(dae), attrOperator(), attrSampler(), elemConstant() {}
	/**
	 * Destructor
	 */
	virtual ~domGles_texenv_command() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domGles_texenv_command &operator=( const domGles_texenv_command &cpy ) { (void)cpy; return *this; }

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
