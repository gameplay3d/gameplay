#ifndef __domInstance_formula_h__
#define __domInstance_formula_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domFormula_setparam.h>
class DAE;

class domInstance_formula : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::INSTANCE_FORMULA; }
	static daeInt ID() { return 475; }
	virtual daeInt typeID() const { return ID(); }
protected:  // Attributes
	domSid attrSid;
	xsToken attrName;
	xsAnyURI attrUrl;

protected:  // Element
	domFormula_setparam_Array elemSetparam_array;

public:	//Accessors and Mutators
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
	 * Gets the setparam element array.
	 * @return Returns a reference to the array of setparam elements.
	 */
	domFormula_setparam_Array &getSetparam_array() { return elemSetparam_array; }
	/**
	 * Gets the setparam element array.
	 * @return Returns a constant reference to the array of setparam elements.
	 */
	const domFormula_setparam_Array &getSetparam_array() const { return elemSetparam_array; }
protected:
	/**
	 * Constructor
	 */
	domInstance_formula(DAE& dae) : daeElement(dae), attrSid(), attrName(), attrUrl(dae, *this), elemSetparam_array() {}
	/**
	 * Destructor
	 */
	virtual ~domInstance_formula() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domInstance_formula &operator=( const domInstance_formula &cpy ) { (void)cpy; return *this; }

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
