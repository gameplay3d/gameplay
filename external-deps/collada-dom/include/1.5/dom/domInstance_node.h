#ifndef __domInstance_node_h__
#define __domInstance_node_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domInstance_with_extra.h>
class DAE;

/**
 * The instance_node element declares the instantiation of a COLLADA node
 * resource.
 */
class domInstance_node : public domInstance_with_extra
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::INSTANCE_NODE; }
	static daeInt ID() { return 111; }
	virtual daeInt typeID() const { return ID(); }
protected:  // Attribute
	xsAnyURI attrProxy;


public:	//Accessors and Mutators
	/**
	 * Gets the url attribute.
	 * @return Returns a xsAnyURI reference of the url attribute.
	 */
	xsAnyURI &getUrl() { return attrUrl; }
	/**
	 * Gets the url attribute.
	 * @return Returns a constant xsAnyURI reference of the url attribute.
	 */
	const xsAnyURI &getUrl() const { return attrUrl; }
	/**
	 * Sets the url attribute.
	 * @param atUrl The new value for the url attribute.
	 */
	void setUrl( const xsAnyURI &atUrl ) { attrUrl = atUrl; }
	/**
	 * Sets the url attribute.
	 * @param atUrl The new value for the url attribute.
	 */
	void setUrl( xsString atUrl ) { attrUrl = atUrl; }

	/**
	 * Gets the sid attribute.
	 * @return Returns a domSid of the sid attribute.
	 */
	domSid getSid() const { return attrSid; }
	/**
	 * Sets the sid attribute.
	 * @param atSid The new value for the sid attribute.
	 */
	void setSid( domSid atSid ) { *(daeStringRef*)&attrSid = atSid;}

	/**
	 * Gets the name attribute.
	 * @return Returns a xsToken of the name attribute.
	 */
	xsToken getName() const { return attrName; }
	/**
	 * Sets the name attribute.
	 * @param atName The new value for the name attribute.
	 */
	void setName( xsToken atName ) { *(daeStringRef*)&attrName = atName;}

	/**
	 * Gets the proxy attribute.
	 * @return Returns a xsAnyURI reference of the proxy attribute.
	 */
	xsAnyURI &getProxy() { return attrProxy; }
	/**
	 * Gets the proxy attribute.
	 * @return Returns a constant xsAnyURI reference of the proxy attribute.
	 */
	const xsAnyURI &getProxy() const { return attrProxy; }
	/**
	 * Sets the proxy attribute.
	 * @param atProxy The new value for the proxy attribute.
	 */
	void setProxy( const xsAnyURI &atProxy ) { attrProxy = atProxy; }
	/**
	 * Sets the proxy attribute.
	 * @param atProxy The new value for the proxy attribute.
	 */
	void setProxy( xsString atProxy ) { attrProxy = atProxy; }

protected:
	/**
	 * Constructor
	 */
	domInstance_node(DAE& dae) : domInstance_with_extra(dae), attrProxy(dae, *this) {}
	/**
	 * Destructor
	 */
	virtual ~domInstance_node() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domInstance_node &operator=( const domInstance_node &cpy ) { (void)cpy; return *this; }

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
