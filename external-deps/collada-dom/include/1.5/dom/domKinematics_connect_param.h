#ifndef __domKinematics_connect_param_h__
#define __domKinematics_connect_param_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

class DAE;

class domKinematics_connect_param : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::KINEMATICS_CONNECT_PARAM; }
	static daeInt ID() { return 434; }
	virtual daeInt typeID() const { return ID(); }
protected:  // Attribute
	xsToken attrRef;


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

protected:
	/**
	 * Constructor
	 */
	domKinematics_connect_param(DAE& dae) : daeElement(dae), attrRef() {}
	/**
	 * Destructor
	 */
	virtual ~domKinematics_connect_param() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domKinematics_connect_param &operator=( const domKinematics_connect_param &cpy ) { (void)cpy; return *this; }

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
