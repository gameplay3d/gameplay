#ifndef __domHyperbola_h__
#define __domHyperbola_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domExtra.h>
class DAE;

/**
 * Describes a branch of a hyperbola in 3D space. A     hyperbola is defined
 * by its major and minor radii and,     as with any conic curve, is positioned
 * in space with a     right-handed coordinate system where: - the origin
 * is     the center of the hyperbola, - the "X Direction" defines     the
 * major axis, and - the "Y Direction" defines the     minor axis. The origin,
 * "X Direction" and "Y Direction"     of this coordinate system define the
 * plane of the     hyperbola. The coordinate system is the local coordinate
 * system of the hyperbola. The branch of the hyperbola     described is the
 * one located on the positive side of the     major axis. The "main Direction"
 * of the local coordinate     system is a vector normal to the plane of the
 * hyperbola.     The axis, of which the origin and unit vector are     respectively
 * the origin and "main Direction" of the     local coordinate system, is
 * termed the "Axis" or "main     Axis" of the hyperbola. The "main Direction"
 * of the     local coordinate system gives an explicit orientation to   
 * the hyperbola, determining the direction in which the     parameter increases
 * along the hyperbola. The     Geom_Hyperbola hyperbola is parameterized
 * as follows:     P(U) = O + MajRad*Cosh(U)*XDir + MinRad*Sinh(U)*YDir, 
 * where: - P is the point of parameter U, - O, XDir and     YDir are respectively
 * the origin, "X Direction" and "Y     Direction" of its local coordinate
 * system, - MajRad and     MinRad are the major and minor radii of the hyperbola.
 * The "X Axis" of the local coordinate system therefore     defines the origin
 * of the parameter of the hyperbola.     The parameter range is ] -infinite,
 * +infinite [.
 */
class domHyperbola : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::HYPERBOLA; }
	static daeInt ID() { return 318; }
	virtual daeInt typeID() const { return ID(); }
public:
	class domRadius;

	typedef daeSmartRef<domRadius> domRadiusRef;
	typedef daeTArray<domRadiusRef> domRadius_Array;

	class domRadius : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::RADIUS; }
		static daeInt ID() { return 319; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a domFloat2 of the value.
		 */
		domFloat2& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const domFloat2& val ) { _value = val; }

	protected:  // Value
		/**
		 * The domFloat2 value of the text data of this element. 
		 */
		domFloat2 _value;
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
	domHyperbola(DAE& dae) : daeElement(dae), elemRadius(), elemExtra_array() {}
	/**
	 * Destructor
	 */
	virtual ~domHyperbola() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domHyperbola &operator=( const domHyperbola &cpy ) { (void)cpy; return *this; }

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
