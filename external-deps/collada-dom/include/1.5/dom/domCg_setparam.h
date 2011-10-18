#ifndef __domCg_setparam_h__
#define __domCg_setparam_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domCg_param.h>
class DAE;

/**
 * Assigns a new value to a previously defined parameter.
 */
class domCg_setparam : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::CG_SETPARAM; }
	static daeInt ID() { return 244; }
	virtual daeInt typeID() const { return ID(); }
protected:  // Attribute
	xsToken attrRef;

protected:  // Element
	domCg_paramRef elemCg_param;

public:	//Accessors and Mutators
	/**
	 * Gets the ref attribute.
	 * @return Returns a xsToken of the ref attribute.
	 */
	xsToken getRef() const { return attrRef; }
	/**
	 * Sets the ref attribute.
	 * @param atRef The new value for the ref attribute.
	 */
	void setRef( xsToken atRef ) { *(daeStringRef*)&attrRef = atRef;}

	/**
	 * Gets the cg_param element.
	 * @return a daeSmartRef to the cg_param element.
	 */
	const domCg_paramRef getCg_param() const { return elemCg_param; }
protected:
	/**
	 * Constructor
	 */
	domCg_setparam(DAE& dae) : daeElement(dae), attrRef(), elemCg_param() {}
	/**
	 * Destructor
	 */
	virtual ~domCg_setparam() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domCg_setparam &operator=( const domCg_setparam &cpy ) { (void)cpy; return *this; }

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
