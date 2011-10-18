#ifndef __domEllipse_h__
#define __domEllipse_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domExtra.h>
class DAE;

/**
 * Describes an ellipse in 3D space. An ellipse is defined     by its major
 * and minor radii and, as with any conic     curve, is positioned in space
 * with a right-handed     coordinate system where: - the origin is the center
 * of     the ellipse, - the "X Direction" defines the     major axis, and
 * - the "Y Direction" defines     the minor axis. The origin, "X Direction"
 * and     "Y Direction" of this coordinate system define     the plane of
 * the ellipse. The coordinate system is the     local coordinate system of
 * the ellipse. The "main     Direction" of this coordinate system is the
 * vector     normal to the plane of the ellipse. The axis, of which     the
 * origin and unit vector are respectively the origin     and "main Direction"
 * of the local coordinate     system, is termed the "Axis" or "main     Axis"
 * of the ellipse. The "main     Direction" of the local coordinate system
 * gives an     explicit orientation to the ellipse (definition of the   
 * trigonometric sense), determining the direction in which     the parameter
 * increases along the ellipse. The     Geom_Ellipse ellipse is parameterized
 * by an angle: P(U)     = O + MajorRad*Cos(U)*XDir + MinorRad*Sin(U)*YDir
 * where:     - P is the point of parameter U, - O, XDir and YDir are    
 * respectively the origin, "X <br>     Direction" and "Y Direction" of its
 * local     coordinate system, - MajorRad and MinorRad are the major    
 * and minor radii of the ellipse. The "X Axis"     of the local coordinate
 * system therefore defines the     origin of the parameter of the ellipse.
 * An ellipse is a     closed and periodic curve. The period is 2.*Pi and
 * the     parameter range is [ 0, 2.*Pi [.
 */
class domEllipse : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::ELLIPSE; }
	static daeInt ID() { return 314; }
	virtual daeInt typeID() const { return ID(); }
public:
	class domRadius;

	typedef daeSmartRef<domRadius> domRadiusRef;
	typedef daeTArray<domRadiusRef> domRadius_Array;

	class domRadius : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::RADIUS; }
		static daeInt ID() { return 315; }
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
	domEllipse(DAE& dae) : daeElement(dae), elemRadius(), elemExtra_array() {}
	/**
	 * Destructor
	 */
	virtual ~domEllipse() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domEllipse &operator=( const domEllipse &cpy ) { (void)cpy; return *this; }

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
