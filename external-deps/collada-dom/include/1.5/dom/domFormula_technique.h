#ifndef __domFormula_technique_h__
#define __domFormula_technique_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

class DAE;

class domFormula_technique : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FORMULA_TECHNIQUE; }
	static daeInt ID() { return 464; }
	virtual daeInt typeID() const { return ID(); }

protected:  // Element
	/**
	 * Used to preserve order in elements that do not specify strict sequencing of sub-elements.
	 */
	daeElementRefArray _contents;
	/**
	 * Used to preserve order in elements that have a complex content model.
	 */
	daeUIntArray       _contentsOrder;

protected:
	/**
	 * Constructor
	 */
	domFormula_technique(DAE& dae) : daeElement(dae) {}
	/**
	 * Destructor
	 */
	virtual ~domFormula_technique() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domFormula_technique &operator=( const domFormula_technique &cpy ) { (void)cpy; return *this; }

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
