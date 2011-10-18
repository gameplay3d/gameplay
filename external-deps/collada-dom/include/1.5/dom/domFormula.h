#ifndef __domFormula_h__
#define __domFormula_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domFormula_newparam.h>
#include <dom/domCommon_float_or_param.h>
#include <dom/domFormula_technique.h>
#include <dom/domTechnique.h>
class DAE;

class domFormula : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FORMULA; }
	static daeInt ID() { return 478; }
	virtual daeInt typeID() const { return ID(); }
protected:  // Attributes
	xsID attrId;
	xsToken attrName;
	domSid attrSid;

protected:  // Elements
	domFormula_newparam_Array elemNewparam_array;
	domCommon_float_or_paramRef elemTarget;
	domFormula_techniqueRef elemTechnique_common;
	domTechnique_Array elemTechnique_array;

public:	//Accessors and Mutators
	/**
	 * Gets the id attribute.
	 * @return Returns a xsID of the id attribute.
	 */
	xsID getId() const { return attrId; }
	/**
	 * Sets the id attribute.
	 * @param atId The new value for the id attribute.
	 */
	void setId( xsID atId ) { *(daeStringRef*)&attrId = atId;
		if( _document != NULL ) _document->changeElementID( this, attrId );
	}

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
	 * Gets the newparam element array.
	 * @return Returns a reference to the array of newparam elements.
	 */
	domFormula_newparam_Array &getNewparam_array() { return elemNewparam_array; }
	/**
	 * Gets the newparam element array.
	 * @return Returns a constant reference to the array of newparam elements.
	 */
	const domFormula_newparam_Array &getNewparam_array() const { return elemNewparam_array; }
	/**
	 * Gets the target element.
	 * @return a daeSmartRef to the target element.
	 */
	const domCommon_float_or_paramRef getTarget() const { return elemTarget; }
	/**
	 * Gets the technique_common element.
	 * @return a daeSmartRef to the technique_common element.
	 */
	const domFormula_techniqueRef getTechnique_common() const { return elemTechnique_common; }
	/**
	 * Gets the technique element array.
	 * @return Returns a reference to the array of technique elements.
	 */
	domTechnique_Array &getTechnique_array() { return elemTechnique_array; }
	/**
	 * Gets the technique element array.
	 * @return Returns a constant reference to the array of technique elements.
	 */
	const domTechnique_Array &getTechnique_array() const { return elemTechnique_array; }
protected:
	/**
	 * Constructor
	 */
	domFormula(DAE& dae) : daeElement(dae), attrId(), attrName(), attrSid(), elemNewparam_array(), elemTarget(), elemTechnique_common(), elemTechnique_array() {}
	/**
	 * Destructor
	 */
	virtual ~domFormula() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domFormula &operator=( const domFormula &cpy ) { (void)cpy; return *this; }

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
