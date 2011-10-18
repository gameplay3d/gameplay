#ifndef __domPlane_h__
#define __domPlane_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domExtra.h>
class DAE;

/**
 * An infinite plane primitive.
 */
class domPlane : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::PLANE; }
	static daeInt ID() { return 283; }
	virtual daeInt typeID() const { return ID(); }
public:
	class domEquation;

	typedef daeSmartRef<domEquation> domEquationRef;
	typedef daeTArray<domEquationRef> domEquation_Array;

/**
 * 4 float values that represent the coefficients for the plane's equation:
 * Ax + By + Cz + D = 0
 */
	class domEquation : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::EQUATION; }
		static daeInt ID() { return 284; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a domFloat4 of the value.
		 */
		domFloat4& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const domFloat4& val ) { _value = val; }

	protected:  // Value
		/**
		 * The domFloat4 value of the text data of this element. 
		 */
		domFloat4 _value;
	protected:
		/**
		 * Constructor
		 */
		domEquation(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domEquation() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domEquation &operator=( const domEquation &cpy ) { (void)cpy; return *this; }

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



protected:  // Elements
/**
 * 4 float values that represent the coefficients for the plane's equation:
 * Ax + By + Cz + D = 0 @see domEquation
 */
	domEquationRef elemEquation;
/**
 * The extra element may appear any number of times. @see domExtra
 */
	domExtra_Array elemExtra_array;

public:	//Accessors and Mutators
	/**
	 * Gets the equation element.
	 * @return a daeSmartRef to the equation element.
	 */
	const domEquationRef getEquation() const { return elemEquation; }
	/**
	 * Gets the extra element array.
	 * @return Returns a reference to the array of extra elements.
	 */
	domExtra_Array &getExtra_array() { return elemExtra_array; }
	/**
	 * Gets the extra element array.
	 * @return Returns a constant reference to the array of extra elements.
	 */
	const domExtra_Array &getExtra_array() const { return elemExtra_array; }
protected:
	/**
	 * Constructor
	 */
	domPlane(DAE& dae) : daeElement(dae), elemEquation(), elemExtra_array() {}
	/**
	 * Destructor
	 */
	virtual ~domPlane() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domPlane &operator=( const domPlane &cpy ) { (void)cpy; return *this; }

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
