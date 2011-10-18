#ifndef __domFx_common_transparent_h__
#define __domFx_common_transparent_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domFx_common_color_or_texture.h>
class DAE;

class domFx_common_transparent : public domFx_common_color_or_texture
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FX_COMMON_TRANSPARENT; }
	static daeInt ID() { return 196; }
	virtual daeInt typeID() const { return ID(); }
protected:  // Attribute
	domFx_opaque attrOpaque;


public:	//Accessors and Mutators
	/**
	 * Gets the opaque attribute.
	 * @return Returns a domFx_opaque of the opaque attribute.
	 */
	domFx_opaque getOpaque() const { return attrOpaque; }
	/**
	 * Sets the opaque attribute.
	 * @param atOpaque The new value for the opaque attribute.
	 */
	void setOpaque( domFx_opaque atOpaque ) { attrOpaque = atOpaque; }

protected:
	/**
	 * Constructor
	 */
	domFx_common_transparent(DAE& dae) : domFx_common_color_or_texture(dae), attrOpaque() {}
	/**
	 * Destructor
	 */
	virtual ~domFx_common_transparent() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domFx_common_transparent &operator=( const domFx_common_transparent &cpy ) { (void)cpy; return *this; }

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
