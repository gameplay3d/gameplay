#ifndef __domCircle_h__
#define __domCircle_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domExtra.h>
class DAE;

/**
 * Describes a circle. A circle is defined by its radius     and, as with
 * any conic curve, is positioned in space     with a right-handed coordinate
 * system where: - the     origin is the center of the circle, and - the origin,
 * "X     Direction" and "Y Direction" define the plane of the     circle.
 * This coordinate system is the local coordinate     system of the circle.
 * The "main Direction" of this     coordinate system is the vector normal
 * to the plane of     the circle. The axis, of which the origin and unit
 * vector are respectively the origin and "main Direction"     of the local
 * coordinate system, is termed the "Axis" or     "main Axis" of the circle.
 * The "main Direction" of the     local coordinate system gives an explicit
 * orientation to     the circle (definition of the trigonometric sense),
 * determining the direction in which the parameter     increases along the
 * circle. The Geom_Circle circle is     parameterized by an angle: P(U) =
 * O + R*Cos(U)*XDir +     R*Sin(U)*YDir, where: - P is the point of parameter
 * U, -     O, XDir and YDir are respectively the origin, "X     Direction"
 * and "Y Direction" of its local coordinate     system, - R is the radius
 * of the circle. The "X     Axis" of the local coordinate system therefore
 * defines the origin of the parameter of the circle.     The parameter is
 * the angle with this "X Direction".     A circle is a closed and periodic
 * curve. The period     is 2.*Pi and the parameter range is [ 0, 2.*Pi [.
 */
class domCircle : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::CIRCLE; }
	static daeInt ID() { return 312; }
	virtual daeInt typeID() const { return ID(); }
public:
	class domRadius;

	typedef daeSmartRef<domRadius> domRadiusRef;
	typedef daeTArray<domRadiusRef> domRadius_Array;

	class domRadius : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::RADIUS; }
		static daeInt ID() { return 313; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a domFloat of the value.
		 */
		domFloat& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const domFloat& val ) { _value = val; }

	protected:  // Value
		/**
		 * The domFloat value of the text data of this element. 
		 */
		domFloat _value;
	protected:
		/**
		 * Constructor
		 */
		domRadius(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domRadius() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domRadius &operator=( const domRadius &cpy ) { (void)cpy; return *this; }

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
	domRadiusRef elemRadius;
	domExtra_Array elemExtra_array;

public:	//Accessors and Mutators
	/**
	 * Gets the radius element.
	 * @return a daeSmartRef to the radius element.
	 */
	const domRadiusRef getRadius() const { return elemRadius; }
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
	domCircle(DAE& dae) : daeElement(dae), elemRadius(), elemExtra_array() {}
	/**
	 * Destructor
	 */
	virtual ~domCircle() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domCircle &operator=( const domCircle &cpy ) { (void)cpy; return *this; }

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
