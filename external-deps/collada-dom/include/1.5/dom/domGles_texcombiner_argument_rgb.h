#ifndef __domGles_texcombiner_argument_rgb_h__
#define __domGles_texcombiner_argument_rgb_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

class DAE;

class domGles_texcombiner_argument_rgb : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::GLES_TEXCOMBINER_ARGUMENT_RGB; }
	static daeInt ID() { return 262; }
	virtual daeInt typeID() const { return ID(); }
protected:  // Attributes
	domGles_texcombiner_source attrSource;
	domGles_texcombiner_operand_rgb attrOperand;
	xsNCName attrSampler;


public:	//Accessors and Mutators
	/**
	 * Gets the source attribute.
	 * @return Returns a domGles_texcombiner_source of the source attribute.
	 */
	domGles_texcombiner_source getSource() const { return attrSource; }
	/**
	 * Sets the source attribute.
	 * @param atSource The new value for the source attribute.
	 */
	void setSource( domGles_texcombiner_source atSource ) { attrSource = atSource; }

	/**
	 * Gets the operand attribute.
	 * @return Returns a domGles_texcombiner_operand_rgb of the operand attribute.
	 */
	domGles_texcombiner_operand_rgb getOperand() const { return attrOperand; }
	/**
	 * Sets the operand attribute.
	 * @param atOperand The new value for the operand attribute.
	 */
	void setOperand( domGles_texcombiner_operand_rgb atOperand ) { attrOperand = atOperand; }

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

protected:
	/**
	 * Constructor
	 */
	domGles_texcombiner_argument_rgb(DAE& dae) : daeElement(dae), attrSource(), attrOperand(), attrSampler() {}
	/**
	 * Destructor
	 */
	virtual ~domGles_texcombiner_argument_rgb() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domGles_texcombiner_argument_rgb &operator=( const domGles_texcombiner_argument_rgb &cpy ) { (void)cpy; return *this; }

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
