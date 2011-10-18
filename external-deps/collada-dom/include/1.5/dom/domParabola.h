#ifndef __domParabola_h__
#define __domParabola_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domExtra.h>
class DAE;

/**
 * Describes a parabola in 3D space. A parabola is defined     by its focal
 * length (i.e. the distance between its focus     and its apex) and is positioned
 * in space with a     coordinate system where: - the origin is     the apex
 * of the parabola, - the "X Axis"     defines the axis of symmetry; the parabola
 * is on the     positive side of this axis, - the origin, "X     Direction"
 * and "Y Direction" define the     plane of the parabola. This coordinate
 * system is the     local coordinate system of the parabola. The "main  
 * Direction" of this coordinate system is a vector     normal to the plane
 * of the parabola. The axis, of which     the origin and unit vector are
 * respectively the origin     and "main Direction" of the local coordinate
 * system, is termed the "Axis" or "main     Axis" of the parabola. The "main
 * Direction" of the local coordinate system gives an     explicit orientation
 * to the parabola, determining the     direction in which the parameter increases
 * along the     parabola. The Geom_Parabola parabola is parameterized as
 * follows: P(U) = O + U*U/(4.*F)*XDir + U*YDir where: - P     is the point
 * of parameter U, - O, XDir and YDir are     respectively the origin, "X
 * <br>     Direction" and "Y Direction" of its local     coordinate system,
 * - F is the focal length of the     parabola. The parameter of the parabola
 * is therefore its     Y coordinate in the local coordinate system, with
 * the     "X <br> Axis" of the local coordinate     system defining the origin
 * of the parameter. The     parameter range is ] -infinite, +infinite [.
 */
class domParabola : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::PARABOLA; }
	static daeInt ID() { return 316; }
	virtual daeInt typeID() const { return ID(); }
public:
	class domFocal;

	typedef daeSmartRef<domFocal> domFocalRef;
	typedef daeTArray<domFocalRef> domFocal_Array;

	class domFocal : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FOCAL; }
		static daeInt ID() { return 317; }
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
		domFocal(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domFocal() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domFocal &operator=( const domFocal &cpy ) { (void)cpy; return *this; }

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
	domFocalRef elemFocal;
	domExtra_Array elemExtra_array;

public:	//Accessors and Mutators
	/**
	 * Gets the focal element.
	 * @return a daeSmartRef to the focal element.
	 */
	const domFocalRef getFocal() const { return elemFocal; }
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
	domParabola(DAE& dae) : daeElement(dae), elemFocal(), elemExtra_array() {}
	/**
	 * Destructor
	 */
	virtual ~domParabola() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domParabola &operator=( const domParabola &cpy ) { (void)cpy; return *this; }

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
