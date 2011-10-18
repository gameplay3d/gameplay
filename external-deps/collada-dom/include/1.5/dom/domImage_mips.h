#ifndef __domImage_mips_h__
#define __domImage_mips_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

class DAE;

class domImage_mips : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::IMAGE_MIPS; }
	static daeInt ID() { return 22; }
	virtual daeInt typeID() const { return ID(); }
protected:  // Attributes
/**
 * Zero is max levels = 1 + floor(log2(max(w, h, d))) in both OpenGL and DirectX.
 * One is no mips.
 */
	xsUnsignedInt attrLevels;
/**
 * regenerate the mipmaps based on information loaded into mip0
 */
	xsBoolean attrAuto_generate;


public:	//Accessors and Mutators
	/**
	 * Gets the levels attribute.
	 * @return Returns a xsUnsignedInt of the levels attribute.
	 */
	xsUnsignedInt getLevels() const { return attrLevels; }
	/**
	 * Sets the levels attribute.
	 * @param atLevels The new value for the levels attribute.
	 */
	void setLevels( xsUnsignedInt atLevels ) { attrLevels = atLevels; }

	/**
	 * Gets the auto_generate attribute.
	 * @return Returns a xsBoolean of the auto_generate attribute.
	 */
	xsBoolean getAuto_generate() const { return attrAuto_generate; }
	/**
	 * Sets the auto_generate attribute.
	 * @param atAuto_generate The new value for the auto_generate attribute.
	 */
	void setAuto_generate( xsBoolean atAuto_generate ) { attrAuto_generate = atAuto_generate; }

protected:
	/**
	 * Constructor
	 */
	domImage_mips(DAE& dae) : daeElement(dae), attrLevels(), attrAuto_generate() {}
	/**
	 * Destructor
	 */
	virtual ~domImage_mips() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domImage_mips &operator=( const domImage_mips &cpy ) { (void)cpy; return *this; }

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
