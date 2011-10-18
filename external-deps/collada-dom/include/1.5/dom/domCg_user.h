#ifndef __domCg_user_h__
#define __domCg_user_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domCg_setparam.h>
class DAE;

/**
 * Creates an instance of a structured class.
 */
class domCg_user : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::CG_USER; }
	static daeInt ID() { return 245; }
	virtual daeInt typeID() const { return ID(); }
protected:  // Attributes
	xsToken attrTypename;
/**
 * Reference a code or include element which defines the usertype 
 */
	xsNCName attrSource;

protected:  // Element
/**
 * Use a series of these to set the members by name.  The ref attribute will
 * be relative to the usertype you are in right now. @see domSetparam
 */
	domCg_setparam_Array elemSetparam_array;

public:	//Accessors and Mutators
	/**
	 * Gets the typename attribute.
	 * @return Returns a xsToken of the typename attribute.
	 */
	xsToken getTypename() const { return attrTypename; }
	/**
	 * Sets the typename attribute.
	 * @param atTypename The new value for the typename attribute.
	 */
	void setTypename( xsToken atTypename ) { *(daeStringRef*)&attrTypename = atTypename;}

	/**
	 * Gets the source attribute.
	 * @return Returns a xsNCName of the source attribute.
	 */
	xsNCName getSource() const { return attrSource; }
	/**
	 * Sets the source attribute.
	 * @param atSource The new value for the source attribute.
	 */
	void setSource( xsNCName atSource ) { *(daeStringRef*)&attrSource = atSource;}

	/**
	 * Gets the setparam element array.
	 * @return Returns a reference to the array of setparam elements.
	 */
	domCg_setparam_Array &getSetparam_array() { return elemSetparam_array; }
	/**
	 * Gets the setparam element array.
	 * @return Returns a constant reference to the array of setparam elements.
	 */
	const domCg_setparam_Array &getSetparam_array() const { return elemSetparam_array; }
protected:
	/**
	 * Constructor
	 */
	domCg_user(DAE& dae) : daeElement(dae), attrTypename(), attrSource(), elemSetparam_array() {}
	/**
	 * Destructor
	 */
	virtual ~domCg_user() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domCg_user &operator=( const domCg_user &cpy ) { (void)cpy; return *this; }

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
