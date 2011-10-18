#ifndef __domKinematics_limits_h__
#define __domKinematics_limits_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domCommon_float_or_param.h>
class DAE;

class domKinematics_limits : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::KINEMATICS_LIMITS; }
	static daeInt ID() { return 429; }
	virtual daeInt typeID() const { return ID(); }

protected:  // Elements
	domCommon_float_or_paramRef elemMin;
	domCommon_float_or_paramRef elemMax;

public:	//Accessors and Mutators
	/**
	 * Gets the min element.
	 * @return a daeSmartRef to the min element.
	 */
	const domCommon_float_or_paramRef getMin() const { return elemMin; }
	/**
	 * Gets the max element.
	 * @return a daeSmartRef to the max element.
	 */
	const domCommon_float_or_paramRef getMax() const { return elemMax; }
protected:
	/**
	 * Constructor
	 */
	domKinematics_limits(DAE& dae) : daeElement(dae), elemMin(), elemMax() {}
	/**
	 * Destructor
	 */
	virtual ~domKinematics_limits() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domKinematics_limits &operator=( const domKinematics_limits &cpy ) { (void)cpy; return *this; }

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
