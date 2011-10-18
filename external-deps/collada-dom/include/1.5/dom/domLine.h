#ifndef __domLine_h__
#define __domLine_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domExtra.h>
class DAE;

/**
 * Describes an infinite line. A line is defined and     positioned in space
 * with an axis which gives it an     origin and a unit vector. The Geom_Line
 * line is     parameterized: P (U) = O + U*Dir, where: - P is the     point
 * of parameter U, - O is the origin and Dir the unit     vector of its positioning
 * axis. The parameter range is ]     -infinite, +infinite [. The orientation
 * of the line is     given by the unit vector of its positioning axis.
 */
class domLine : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::LINE; }
	static daeInt ID() { return 309; }
	virtual daeInt typeID() const { return ID(); }
public:
	class domOrigin;

	typedef daeSmartRef<domOrigin> domOriginRef;
	typedef daeTArray<domOriginRef> domOrigin_Array;

	class domOrigin : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::ORIGIN; }
		static daeInt ID() { return 310; }
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

	class domDirection;

	typedef daeSmartRef<domDirection> domDirectionRef;
	typedef daeTArray<domDirectionRef> domDirection_Array;

	class domDirection : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::DIRECTION; }
		static daeInt ID() { return 311; }
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



protected:  // Elements
	domOriginRef elemOrigin;
	domDirectionRef elemDirection;
	domExtra_Array elemExtra_array;

public:	//Accessors and Mutators
	/**
	 * Gets the origin element.
	 * @return a daeSmartRef to the origin element.
	 */
	const domOriginRef getOrigin() const { return elemOrigin; }
	/**
	 * Gets the direction element.
	 * @return a daeSmartRef to the direction element.
	 */
	const domDirectionRef getDirection() const { return elemDirection; }
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
	domLine(DAE& dae) : daeElement(dae), elemOrigin(), elemDirection(), elemExtra_array() {}
	/**
	 * Destructor
	 */
	virtual ~domLine() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domLine &operator=( const domLine &cpy ) { (void)cpy; return *this; }

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
