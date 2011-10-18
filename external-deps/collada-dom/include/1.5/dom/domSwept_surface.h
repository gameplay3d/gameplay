#ifndef __domSwept_surface_h__
#define __domSwept_surface_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domCurve.h>
#include <dom/domExtra.h>
class DAE;

/**
 * Describes a surface of linear extrusion ("extruded     surface"), e.g.
 * a generalized cylinder. Such a surface     is obtained by sweeping a curve
 * (called the "extruded     curve" or "basis") in a given direction (referred
 * to as     the "direction of extrusion" and defined by a unit     vector).
 * The u parameter is along the extruded curve.     The v parameter is along
 * the direction of extrusion. The     parameter range for the u parameter
 * is defined by the     reference curve. The parameter range for the v parameter
 * is ] - infinity, + infinity [. The position of the curve     gives the
 * origin of the v parameter. The form of a     surface of linear extrusion
 * is generally a ruled     surface. It can be: - a cylindrical surface, if
 * the     extruded curve is a circle, or a trimmed circle, with an     axis
 * parallel to the direction of extrusion, or - a     planar surface, if the
 * extruded curve is a line.
 */
class domSwept_surface : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::SWEPT_SURFACE; }
	static daeInt ID() { return 352; }
	virtual daeInt typeID() const { return ID(); }
public:
	class domDirection;

	typedef daeSmartRef<domDirection> domDirectionRef;
	typedef daeTArray<domDirectionRef> domDirection_Array;

	class domDirection : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::DIRECTION; }
		static daeInt ID() { return 353; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a domFloat3 of the value.
		 */
		domFloat3& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const domFloat3& val ) { _value = val; }

	protected:  // Value
		/**
		 * The domFloat3 value of the text data of this element. 
		 */
		domFloat3 _value;
	protected:
		/**
		 * Constructor
		 */
		domDirection(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domDirection() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domDirection &operator=( const domDirection &cpy ) { (void)cpy; return *this; }

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

	class domOrigin;

	typedef daeSmartRef<domOrigin> domOriginRef;
	typedef daeTArray<domOriginRef> domOrigin_Array;

	class domOrigin : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::ORIGIN; }
		static daeInt ID() { return 354; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a domFloat3 of the value.
		 */
		domFloat3& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const domFloat3& val ) { _value = val; }

	protected:  // Value
		/**
		 * The domFloat3 value of the text data of this element. 
		 */
		domFloat3 _value;
	protected:
		/**
		 * Constructor
		 */
		domOrigin(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domOrigin() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domOrigin &operator=( const domOrigin &cpy ) { (void)cpy; return *this; }

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

	class domAxis;

	typedef daeSmartRef<domAxis> domAxisRef;
	typedef daeTArray<domAxisRef> domAxis_Array;

	class domAxis : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::AXIS; }
		static daeInt ID() { return 355; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a domFloat3 of the value.
		 */
		domFloat3& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const domFloat3& val ) { _value = val; }

	protected:  // Value
		/**
		 * The domFloat3 value of the text data of this element. 
		 */
		domFloat3 _value;
	protected:
		/**
		 * Constructor
		 */
		domAxis(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domAxis() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domAxis &operator=( const domAxis &cpy ) { (void)cpy; return *this; }

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
	domCurveRef elemCurve;
	domDirectionRef elemDirection;
	domOriginRef elemOrigin;
	domAxisRef elemAxis;
	domExtra_Array elemExtra_array;
	/**
	 * Used to preserve order in elements that do not specify strict sequencing of sub-elements.
	 */
	daeElementRefArray _contents;
	/**
	 * Used to preserve order in elements that have a complex content model.
	 */
	daeUIntArray       _contentsOrder;

	/**
	 * Used to store information needed for some content model objects.
	 */
	daeTArray< daeCharArray * > _CMData;


public:	//Accessors and Mutators
	/**
	 * Gets the curve element.
	 * @return a daeSmartRef to the curve element.
	 */
	const domCurveRef getCurve() const { return elemCurve; }
	/**
	 * Gets the direction element.
	 * @return a daeSmartRef to the direction element.
	 */
	const domDirectionRef getDirection() const { return elemDirection; }
	/**
	 * Gets the origin element.
	 * @return a daeSmartRef to the origin element.
	 */
	const domOriginRef getOrigin() const { return elemOrigin; }
	/**
	 * Gets the axis element.
	 * @return a daeSmartRef to the axis element.
	 */
	const domAxisRef getAxis() const { return elemAxis; }
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
	/**
	 * Gets the _contents array.
	 * @return Returns a reference to the _contents element array.
	 */
	daeElementRefArray &getContents() { return _contents; }
	/**
	 * Gets the _contents array.
	 * @return Returns a constant reference to the _contents element array.
	 */
	const daeElementRefArray &getContents() const { return _contents; }

protected:
	/**
	 * Constructor
	 */
	domSwept_surface(DAE& dae) : daeElement(dae), elemCurve(), elemDirection(), elemOrigin(), elemAxis(), elemExtra_array() {}
	/**
	 * Destructor
	 */
	virtual ~domSwept_surface() { daeElement::deleteCMDataArray(_CMData); }
	/**
	 * Overloaded assignment operator
	 */
	virtual domSwept_surface &operator=( const domSwept_surface &cpy ) { (void)cpy; return *this; }

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
