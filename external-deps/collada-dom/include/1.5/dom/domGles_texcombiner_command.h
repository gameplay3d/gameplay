#ifndef __domGles_texcombiner_command_h__
#define __domGles_texcombiner_command_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domGles_texture_constant.h>
#include <dom/domGles_texcombiner_command_rgb.h>
#include <dom/domGles_texcombiner_command_alpha.h>
class DAE;

class domGles_texcombiner_command : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::GLES_TEXCOMBINER_COMMAND; }
	static daeInt ID() { return 266; }
	virtual daeInt typeID() const { return ID(); }

protected:  // Elements
	domGles_texture_constantRef elemConstant;
	domGles_texcombiner_command_rgbRef elemRGB;
	domGles_texcombiner_command_alphaRef elemAlpha;

public:	//Accessors and Mutators
	/**
	 * Gets the constant element.
	 * @return a daeSmartRef to the constant element.
	 */
	const domGles_texture_constantRef getConstant() const { return elemConstant; }
	/**
	 * Gets the RGB element.
	 * @return a daeSmartRef to the RGB element.
	 */
	const domGles_texcombiner_command_rgbRef getRGB() const { return elemRGB; }
	/**
	 * Gets the alpha element.
	 * @return a daeSmartRef to the alpha element.
	 */
	const domGles_texcombiner_command_alphaRef getAlpha() const { return elemAlpha; }
protected:
	/**
	 * Constructor
	 */
	domGles_texcombiner_command(DAE& dae) : daeElement(dae), elemConstant(), elemRGB(), elemAlpha() {}
	/**
	 * Destructor
	 */
	virtual ~domGles_texcombiner_command() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domGles_texcombiner_command &operator=( const domGles_texcombiner_command &cpy ) { (void)cpy; return *this; }

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
