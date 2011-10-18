#ifndef __domAxis_constraint_h__
#define __domAxis_constraint_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domAxis.h>
#include <dom/domJoint_limits.h>
class DAE;

class domAxis_constraint : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::AXIS_CONSTRAINT; }
	static daeInt ID() { return 461; }
	virtual daeInt typeID() const { return ID(); }
protected:  // Attribute
	domSid attrSid;

protected:  // Elements
	domAxisRef elemAxis;
	domJoint_limitsRef elemLimits;

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
	 * Gets the axis element.
	 * @return a daeSmartRef to the axis element.
	 */
	const domAxisRef getAxis() const { return elemAxis; }
	/**
	 * Gets the limits element.
	 * @return a daeSmartRef to the limits element.
	 */
	const domJoint_limitsRef getLimits() const { return elemLimits; }
protected:
	/**
	 * Constructor
	 */
	domAxis_constraint(DAE& dae) : daeElement(dae), attrSid(), elemAxis(), elemLimits() {}
	/**
	 * Destructor
	 */
	virtual ~domAxis_constraint() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domAxis_constraint &operator=( const domAxis_constraint &cpy ) { (void)cpy; return *this; }

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
