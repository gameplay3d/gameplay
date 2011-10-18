#ifndef __domCg_param_h__
#define __domCg_param_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domFx_sampler1D.h>
#include <dom/domFx_sampler2D.h>
#include <dom/domFx_sampler3D.h>
#include <dom/domFx_samplerRECT.h>
#include <dom/domFx_samplerCUBE.h>
#include <dom/domFx_samplerDEPTH.h>
#include <dom/domCg_array.h>
#include <dom/domCg_user.h>
class DAE;

/**
 * A group that specifies the allowable types for CG profile parameters.
 */
class domCg_param : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::CG_PARAM; }
	static daeInt ID() { return 764; }
	virtual daeInt typeID() const { return ID(); }
public:
	class domBool;

	typedef daeSmartRef<domBool> domBoolRef;
	typedef daeTArray<domBoolRef> domBool_Array;

	class domBool : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::BOOL; }
		static daeInt ID() { return 765; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a xsBoolean of the value.
		 */
		xsBoolean& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const xsBoolean& val ) { _value = val; }

	protected:  // Value
		/**
		 * The xsBoolean value of the text data of this element. 
		 */
		xsBoolean _value;
	protected:
		/**
		 * Constructor
		 */
		domBool(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domBool() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domBool &operator=( const domBool &cpy ) { (void)cpy; return *this; }

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

	class domBool2;

	typedef daeSmartRef<domBool2> domBool2Ref;
	typedef daeTArray<domBool2Ref> domBool2_Array;

	class domBool2 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::BOOL2; }
		static daeInt ID() { return 766; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domBool2 of the value.
		 */
		::domBool2& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domBool2& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domBool2 value of the text data of this element. 
		 */
		::domBool2 _value;
	protected:
		/**
		 * Constructor
		 */
		domBool2(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domBool2() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domBool2 &operator=( const domBool2 &cpy ) { (void)cpy; return *this; }

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

	class domBool3;

	typedef daeSmartRef<domBool3> domBool3Ref;
	typedef daeTArray<domBool3Ref> domBool3_Array;

	class domBool3 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::BOOL3; }
		static daeInt ID() { return 767; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domBool3 of the value.
		 */
		::domBool3& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domBool3& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domBool3 value of the text data of this element. 
		 */
		::domBool3 _value;
	protected:
		/**
		 * Constructor
		 */
		domBool3(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domBool3() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domBool3 &operator=( const domBool3 &cpy ) { (void)cpy; return *this; }

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

	class domBool4;

	typedef daeSmartRef<domBool4> domBool4Ref;
	typedef daeTArray<domBool4Ref> domBool4_Array;

	class domBool4 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::BOOL4; }
		static daeInt ID() { return 768; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domBool4 of the value.
		 */
		::domBool4& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domBool4& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domBool4 value of the text data of this element. 
		 */
		::domBool4 _value;
	protected:
		/**
		 * Constructor
		 */
		domBool4(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domBool4() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domBool4 &operator=( const domBool4 &cpy ) { (void)cpy; return *this; }

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

	class domBool2x1;

	typedef daeSmartRef<domBool2x1> domBool2x1Ref;
	typedef daeTArray<domBool2x1Ref> domBool2x1_Array;

	class domBool2x1 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::BOOL2X1; }
		static daeInt ID() { return 769; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domBool2 of the value.
		 */
		::domBool2& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domBool2& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domBool2 value of the text data of this element. 
		 */
		::domBool2 _value;
	protected:
		/**
		 * Constructor
		 */
		domBool2x1(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domBool2x1() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domBool2x1 &operator=( const domBool2x1 &cpy ) { (void)cpy; return *this; }

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

	class domBool2x2;

	typedef daeSmartRef<domBool2x2> domBool2x2Ref;
	typedef daeTArray<domBool2x2Ref> domBool2x2_Array;

	class domBool2x2 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::BOOL2X2; }
		static daeInt ID() { return 770; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domBool2x2 of the value.
		 */
		::domBool2x2& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domBool2x2& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domBool2x2 value of the text data of this element. 
		 */
		::domBool2x2 _value;
	protected:
		/**
		 * Constructor
		 */
		domBool2x2(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domBool2x2() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domBool2x2 &operator=( const domBool2x2 &cpy ) { (void)cpy; return *this; }

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

	class domBool2x3;

	typedef daeSmartRef<domBool2x3> domBool2x3Ref;
	typedef daeTArray<domBool2x3Ref> domBool2x3_Array;

	class domBool2x3 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::BOOL2X3; }
		static daeInt ID() { return 771; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domBool2x3 of the value.
		 */
		::domBool2x3& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domBool2x3& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domBool2x3 value of the text data of this element. 
		 */
		::domBool2x3 _value;
	protected:
		/**
		 * Constructor
		 */
		domBool2x3(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domBool2x3() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domBool2x3 &operator=( const domBool2x3 &cpy ) { (void)cpy; return *this; }

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

	class domBool2x4;

	typedef daeSmartRef<domBool2x4> domBool2x4Ref;
	typedef daeTArray<domBool2x4Ref> domBool2x4_Array;

	class domBool2x4 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::BOOL2X4; }
		static daeInt ID() { return 772; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domBool2x4 of the value.
		 */
		::domBool2x4& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domBool2x4& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domBool2x4 value of the text data of this element. 
		 */
		::domBool2x4 _value;
	protected:
		/**
		 * Constructor
		 */
		domBool2x4(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domBool2x4() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domBool2x4 &operator=( const domBool2x4 &cpy ) { (void)cpy; return *this; }

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

	class domBool3x1;

	typedef daeSmartRef<domBool3x1> domBool3x1Ref;
	typedef daeTArray<domBool3x1Ref> domBool3x1_Array;

	class domBool3x1 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::BOOL3X1; }
		static daeInt ID() { return 773; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domBool3 of the value.
		 */
		::domBool3& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domBool3& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domBool3 value of the text data of this element. 
		 */
		::domBool3 _value;
	protected:
		/**
		 * Constructor
		 */
		domBool3x1(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domBool3x1() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domBool3x1 &operator=( const domBool3x1 &cpy ) { (void)cpy; return *this; }

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

	class domBool3x2;

	typedef daeSmartRef<domBool3x2> domBool3x2Ref;
	typedef daeTArray<domBool3x2Ref> domBool3x2_Array;

	class domBool3x2 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::BOOL3X2; }
		static daeInt ID() { return 774; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domBool3x2 of the value.
		 */
		::domBool3x2& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domBool3x2& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domBool3x2 value of the text data of this element. 
		 */
		::domBool3x2 _value;
	protected:
		/**
		 * Constructor
		 */
		domBool3x2(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domBool3x2() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domBool3x2 &operator=( const domBool3x2 &cpy ) { (void)cpy; return *this; }

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

	class domBool3x3;

	typedef daeSmartRef<domBool3x3> domBool3x3Ref;
	typedef daeTArray<domBool3x3Ref> domBool3x3_Array;

	class domBool3x3 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::BOOL3X3; }
		static daeInt ID() { return 775; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domBool3x3 of the value.
		 */
		::domBool3x3& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domBool3x3& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domBool3x3 value of the text data of this element. 
		 */
		::domBool3x3 _value;
	protected:
		/**
		 * Constructor
		 */
		domBool3x3(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domBool3x3() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domBool3x3 &operator=( const domBool3x3 &cpy ) { (void)cpy; return *this; }

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

	class domBool3x4;

	typedef daeSmartRef<domBool3x4> domBool3x4Ref;
	typedef daeTArray<domBool3x4Ref> domBool3x4_Array;

	class domBool3x4 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::BOOL3X4; }
		static daeInt ID() { return 776; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domBool3x4 of the value.
		 */
		::domBool3x4& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domBool3x4& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domBool3x4 value of the text data of this element. 
		 */
		::domBool3x4 _value;
	protected:
		/**
		 * Constructor
		 */
		domBool3x4(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domBool3x4() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domBool3x4 &operator=( const domBool3x4 &cpy ) { (void)cpy; return *this; }

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

	class domBool4x1;

	typedef daeSmartRef<domBool4x1> domBool4x1Ref;
	typedef daeTArray<domBool4x1Ref> domBool4x1_Array;

	class domBool4x1 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::BOOL4X1; }
		static daeInt ID() { return 777; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domBool4 of the value.
		 */
		::domBool4& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domBool4& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domBool4 value of the text data of this element. 
		 */
		::domBool4 _value;
	protected:
		/**
		 * Constructor
		 */
		domBool4x1(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domBool4x1() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domBool4x1 &operator=( const domBool4x1 &cpy ) { (void)cpy; return *this; }

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

	class domBool4x2;

	typedef daeSmartRef<domBool4x2> domBool4x2Ref;
	typedef daeTArray<domBool4x2Ref> domBool4x2_Array;

	class domBool4x2 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::BOOL4X2; }
		static daeInt ID() { return 778; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domBool4x2 of the value.
		 */
		::domBool4x2& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domBool4x2& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domBool4x2 value of the text data of this element. 
		 */
		::domBool4x2 _value;
	protected:
		/**
		 * Constructor
		 */
		domBool4x2(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domBool4x2() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domBool4x2 &operator=( const domBool4x2 &cpy ) { (void)cpy; return *this; }

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

	class domBool4x3;

	typedef daeSmartRef<domBool4x3> domBool4x3Ref;
	typedef daeTArray<domBool4x3Ref> domBool4x3_Array;

	class domBool4x3 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::BOOL4X3; }
		static daeInt ID() { return 779; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domBool4x3 of the value.
		 */
		::domBool4x3& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domBool4x3& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domBool4x3 value of the text data of this element. 
		 */
		::domBool4x3 _value;
	protected:
		/**
		 * Constructor
		 */
		domBool4x3(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domBool4x3() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domBool4x3 &operator=( const domBool4x3 &cpy ) { (void)cpy; return *this; }

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

	class domBool4x4;

	typedef daeSmartRef<domBool4x4> domBool4x4Ref;
	typedef daeTArray<domBool4x4Ref> domBool4x4_Array;

	class domBool4x4 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::BOOL4X4; }
		static daeInt ID() { return 780; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domBool4x4 of the value.
		 */
		::domBool4x4& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domBool4x4& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domBool4x4 value of the text data of this element. 
		 */
		::domBool4x4 _value;
	protected:
		/**
		 * Constructor
		 */
		domBool4x4(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domBool4x4() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domBool4x4 &operator=( const domBool4x4 &cpy ) { (void)cpy; return *this; }

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

	class domFloat;

	typedef daeSmartRef<domFloat> domFloatRef;
	typedef daeTArray<domFloatRef> domFloat_Array;

	class domFloat : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FLOAT; }
		static daeInt ID() { return 781; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domFloat of the value.
		 */
		::domFloat& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domFloat& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domFloat value of the text data of this element. 
		 */
		::domFloat _value;
	protected:
		/**
		 * Constructor
		 */
		domFloat(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domFloat() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domFloat &operator=( const domFloat &cpy ) { (void)cpy; return *this; }

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

	class domFloat2;

	typedef daeSmartRef<domFloat2> domFloat2Ref;
	typedef daeTArray<domFloat2Ref> domFloat2_Array;

	class domFloat2 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FLOAT2; }
		static daeInt ID() { return 782; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domFloat2 of the value.
		 */
		::domFloat2& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domFloat2& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domFloat2 value of the text data of this element. 
		 */
		::domFloat2 _value;
	protected:
		/**
		 * Constructor
		 */
		domFloat2(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domFloat2() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domFloat2 &operator=( const domFloat2 &cpy ) { (void)cpy; return *this; }

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

	class domFloat3;

	typedef daeSmartRef<domFloat3> domFloat3Ref;
	typedef daeTArray<domFloat3Ref> domFloat3_Array;

	class domFloat3 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FLOAT3; }
		static daeInt ID() { return 783; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domFloat3 of the value.
		 */
		::domFloat3& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domFloat3& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domFloat3 value of the text data of this element. 
		 */
		::domFloat3 _value;
	protected:
		/**
		 * Constructor
		 */
		domFloat3(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domFloat3() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domFloat3 &operator=( const domFloat3 &cpy ) { (void)cpy; return *this; }

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

	class domFloat4;

	typedef daeSmartRef<domFloat4> domFloat4Ref;
	typedef daeTArray<domFloat4Ref> domFloat4_Array;

	class domFloat4 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FLOAT4; }
		static daeInt ID() { return 784; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domFloat4 of the value.
		 */
		::domFloat4& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domFloat4& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domFloat4 value of the text data of this element. 
		 */
		::domFloat4 _value;
	protected:
		/**
		 * Constructor
		 */
		domFloat4(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domFloat4() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domFloat4 &operator=( const domFloat4 &cpy ) { (void)cpy; return *this; }

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

	class domFloat2x1;

	typedef daeSmartRef<domFloat2x1> domFloat2x1Ref;
	typedef daeTArray<domFloat2x1Ref> domFloat2x1_Array;

	class domFloat2x1 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FLOAT2X1; }
		static daeInt ID() { return 785; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domFloat2 of the value.
		 */
		::domFloat2& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domFloat2& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domFloat2 value of the text data of this element. 
		 */
		::domFloat2 _value;
	protected:
		/**
		 * Constructor
		 */
		domFloat2x1(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domFloat2x1() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domFloat2x1 &operator=( const domFloat2x1 &cpy ) { (void)cpy; return *this; }

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

	class domFloat2x2;

	typedef daeSmartRef<domFloat2x2> domFloat2x2Ref;
	typedef daeTArray<domFloat2x2Ref> domFloat2x2_Array;

	class domFloat2x2 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FLOAT2X2; }
		static daeInt ID() { return 786; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domFloat2x2 of the value.
		 */
		::domFloat2x2& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domFloat2x2& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domFloat2x2 value of the text data of this element. 
		 */
		::domFloat2x2 _value;
	protected:
		/**
		 * Constructor
		 */
		domFloat2x2(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domFloat2x2() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domFloat2x2 &operator=( const domFloat2x2 &cpy ) { (void)cpy; return *this; }

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

	class domFloat2x3;

	typedef daeSmartRef<domFloat2x3> domFloat2x3Ref;
	typedef daeTArray<domFloat2x3Ref> domFloat2x3_Array;

	class domFloat2x3 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FLOAT2X3; }
		static daeInt ID() { return 787; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domFloat2x3 of the value.
		 */
		::domFloat2x3& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domFloat2x3& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domFloat2x3 value of the text data of this element. 
		 */
		::domFloat2x3 _value;
	protected:
		/**
		 * Constructor
		 */
		domFloat2x3(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domFloat2x3() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domFloat2x3 &operator=( const domFloat2x3 &cpy ) { (void)cpy; return *this; }

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

	class domFloat2x4;

	typedef daeSmartRef<domFloat2x4> domFloat2x4Ref;
	typedef daeTArray<domFloat2x4Ref> domFloat2x4_Array;

	class domFloat2x4 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FLOAT2X4; }
		static daeInt ID() { return 788; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domFloat2x4 of the value.
		 */
		::domFloat2x4& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domFloat2x4& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domFloat2x4 value of the text data of this element. 
		 */
		::domFloat2x4 _value;
	protected:
		/**
		 * Constructor
		 */
		domFloat2x4(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domFloat2x4() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domFloat2x4 &operator=( const domFloat2x4 &cpy ) { (void)cpy; return *this; }

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

	class domFloat3x1;

	typedef daeSmartRef<domFloat3x1> domFloat3x1Ref;
	typedef daeTArray<domFloat3x1Ref> domFloat3x1_Array;

	class domFloat3x1 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FLOAT3X1; }
		static daeInt ID() { return 789; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domFloat3 of the value.
		 */
		::domFloat3& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domFloat3& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domFloat3 value of the text data of this element. 
		 */
		::domFloat3 _value;
	protected:
		/**
		 * Constructor
		 */
		domFloat3x1(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domFloat3x1() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domFloat3x1 &operator=( const domFloat3x1 &cpy ) { (void)cpy; return *this; }

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

	class domFloat3x2;

	typedef daeSmartRef<domFloat3x2> domFloat3x2Ref;
	typedef daeTArray<domFloat3x2Ref> domFloat3x2_Array;

	class domFloat3x2 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FLOAT3X2; }
		static daeInt ID() { return 790; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domFloat3x2 of the value.
		 */
		::domFloat3x2& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domFloat3x2& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domFloat3x2 value of the text data of this element. 
		 */
		::domFloat3x2 _value;
	protected:
		/**
		 * Constructor
		 */
		domFloat3x2(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domFloat3x2() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domFloat3x2 &operator=( const domFloat3x2 &cpy ) { (void)cpy; return *this; }

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

	class domFloat3x3;

	typedef daeSmartRef<domFloat3x3> domFloat3x3Ref;
	typedef daeTArray<domFloat3x3Ref> domFloat3x3_Array;

	class domFloat3x3 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FLOAT3X3; }
		static daeInt ID() { return 791; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domFloat3x3 of the value.
		 */
		::domFloat3x3& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domFloat3x3& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domFloat3x3 value of the text data of this element. 
		 */
		::domFloat3x3 _value;
	protected:
		/**
		 * Constructor
		 */
		domFloat3x3(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domFloat3x3() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domFloat3x3 &operator=( const domFloat3x3 &cpy ) { (void)cpy; return *this; }

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

	class domFloat3x4;

	typedef daeSmartRef<domFloat3x4> domFloat3x4Ref;
	typedef daeTArray<domFloat3x4Ref> domFloat3x4_Array;

	class domFloat3x4 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FLOAT3X4; }
		static daeInt ID() { return 792; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domFloat3x4 of the value.
		 */
		::domFloat3x4& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domFloat3x4& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domFloat3x4 value of the text data of this element. 
		 */
		::domFloat3x4 _value;
	protected:
		/**
		 * Constructor
		 */
		domFloat3x4(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domFloat3x4() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domFloat3x4 &operator=( const domFloat3x4 &cpy ) { (void)cpy; return *this; }

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

	class domFloat4x1;

	typedef daeSmartRef<domFloat4x1> domFloat4x1Ref;
	typedef daeTArray<domFloat4x1Ref> domFloat4x1_Array;

	class domFloat4x1 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FLOAT4X1; }
		static daeInt ID() { return 793; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domFloat4 of the value.
		 */
		::domFloat4& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domFloat4& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domFloat4 value of the text data of this element. 
		 */
		::domFloat4 _value;
	protected:
		/**
		 * Constructor
		 */
		domFloat4x1(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domFloat4x1() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domFloat4x1 &operator=( const domFloat4x1 &cpy ) { (void)cpy; return *this; }

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

	class domFloat4x2;

	typedef daeSmartRef<domFloat4x2> domFloat4x2Ref;
	typedef daeTArray<domFloat4x2Ref> domFloat4x2_Array;

	class domFloat4x2 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FLOAT4X2; }
		static daeInt ID() { return 794; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domFloat4x2 of the value.
		 */
		::domFloat4x2& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domFloat4x2& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domFloat4x2 value of the text data of this element. 
		 */
		::domFloat4x2 _value;
	protected:
		/**
		 * Constructor
		 */
		domFloat4x2(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domFloat4x2() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domFloat4x2 &operator=( const domFloat4x2 &cpy ) { (void)cpy; return *this; }

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

	class domFloat4x3;

	typedef daeSmartRef<domFloat4x3> domFloat4x3Ref;
	typedef daeTArray<domFloat4x3Ref> domFloat4x3_Array;

	class domFloat4x3 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FLOAT4X3; }
		static daeInt ID() { return 795; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domFloat4x3 of the value.
		 */
		::domFloat4x3& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domFloat4x3& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domFloat4x3 value of the text data of this element. 
		 */
		::domFloat4x3 _value;
	protected:
		/**
		 * Constructor
		 */
		domFloat4x3(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domFloat4x3() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domFloat4x3 &operator=( const domFloat4x3 &cpy ) { (void)cpy; return *this; }

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

	class domFloat4x4;

	typedef daeSmartRef<domFloat4x4> domFloat4x4Ref;
	typedef daeTArray<domFloat4x4Ref> domFloat4x4_Array;

	class domFloat4x4 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FLOAT4X4; }
		static daeInt ID() { return 796; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domFloat4x4 of the value.
		 */
		::domFloat4x4& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domFloat4x4& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domFloat4x4 value of the text data of this element. 
		 */
		::domFloat4x4 _value;
	protected:
		/**
		 * Constructor
		 */
		domFloat4x4(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domFloat4x4() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domFloat4x4 &operator=( const domFloat4x4 &cpy ) { (void)cpy; return *this; }

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

	class domInt;

	typedef daeSmartRef<domInt> domIntRef;
	typedef daeTArray<domIntRef> domInt_Array;

	class domInt : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::INT; }
		static daeInt ID() { return 797; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domInt of the value.
		 */
		::domInt& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domInt& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domInt value of the text data of this element. 
		 */
		::domInt _value;
	protected:
		/**
		 * Constructor
		 */
		domInt(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domInt() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domInt &operator=( const domInt &cpy ) { (void)cpy; return *this; }

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

	class domInt2;

	typedef daeSmartRef<domInt2> domInt2Ref;
	typedef daeTArray<domInt2Ref> domInt2_Array;

	class domInt2 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::INT2; }
		static daeInt ID() { return 798; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domInt2 of the value.
		 */
		::domInt2& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domInt2& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domInt2 value of the text data of this element. 
		 */
		::domInt2 _value;
	protected:
		/**
		 * Constructor
		 */
		domInt2(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domInt2() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domInt2 &operator=( const domInt2 &cpy ) { (void)cpy; return *this; }

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

	class domInt3;

	typedef daeSmartRef<domInt3> domInt3Ref;
	typedef daeTArray<domInt3Ref> domInt3_Array;

	class domInt3 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::INT3; }
		static daeInt ID() { return 799; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domInt3 of the value.
		 */
		::domInt3& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domInt3& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domInt3 value of the text data of this element. 
		 */
		::domInt3 _value;
	protected:
		/**
		 * Constructor
		 */
		domInt3(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domInt3() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domInt3 &operator=( const domInt3 &cpy ) { (void)cpy; return *this; }

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

	class domInt4;

	typedef daeSmartRef<domInt4> domInt4Ref;
	typedef daeTArray<domInt4Ref> domInt4_Array;

	class domInt4 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::INT4; }
		static daeInt ID() { return 800; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domInt4 of the value.
		 */
		::domInt4& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domInt4& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domInt4 value of the text data of this element. 
		 */
		::domInt4 _value;
	protected:
		/**
		 * Constructor
		 */
		domInt4(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domInt4() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domInt4 &operator=( const domInt4 &cpy ) { (void)cpy; return *this; }

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

	class domInt2x1;

	typedef daeSmartRef<domInt2x1> domInt2x1Ref;
	typedef daeTArray<domInt2x1Ref> domInt2x1_Array;

	class domInt2x1 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::INT2X1; }
		static daeInt ID() { return 801; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domInt2 of the value.
		 */
		::domInt2& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domInt2& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domInt2 value of the text data of this element. 
		 */
		::domInt2 _value;
	protected:
		/**
		 * Constructor
		 */
		domInt2x1(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domInt2x1() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domInt2x1 &operator=( const domInt2x1 &cpy ) { (void)cpy; return *this; }

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

	class domInt2x2;

	typedef daeSmartRef<domInt2x2> domInt2x2Ref;
	typedef daeTArray<domInt2x2Ref> domInt2x2_Array;

	class domInt2x2 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::INT2X2; }
		static daeInt ID() { return 802; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domInt2x2 of the value.
		 */
		::domInt2x2& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domInt2x2& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domInt2x2 value of the text data of this element. 
		 */
		::domInt2x2 _value;
	protected:
		/**
		 * Constructor
		 */
		domInt2x2(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domInt2x2() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domInt2x2 &operator=( const domInt2x2 &cpy ) { (void)cpy; return *this; }

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

	class domInt2x3;

	typedef daeSmartRef<domInt2x3> domInt2x3Ref;
	typedef daeTArray<domInt2x3Ref> domInt2x3_Array;

	class domInt2x3 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::INT2X3; }
		static daeInt ID() { return 803; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domInt2x3 of the value.
		 */
		::domInt2x3& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domInt2x3& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domInt2x3 value of the text data of this element. 
		 */
		::domInt2x3 _value;
	protected:
		/**
		 * Constructor
		 */
		domInt2x3(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domInt2x3() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domInt2x3 &operator=( const domInt2x3 &cpy ) { (void)cpy; return *this; }

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

	class domInt2x4;

	typedef daeSmartRef<domInt2x4> domInt2x4Ref;
	typedef daeTArray<domInt2x4Ref> domInt2x4_Array;

	class domInt2x4 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::INT2X4; }
		static daeInt ID() { return 804; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domInt2x4 of the value.
		 */
		::domInt2x4& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domInt2x4& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domInt2x4 value of the text data of this element. 
		 */
		::domInt2x4 _value;
	protected:
		/**
		 * Constructor
		 */
		domInt2x4(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domInt2x4() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domInt2x4 &operator=( const domInt2x4 &cpy ) { (void)cpy; return *this; }

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

	class domInt3x1;

	typedef daeSmartRef<domInt3x1> domInt3x1Ref;
	typedef daeTArray<domInt3x1Ref> domInt3x1_Array;

	class domInt3x1 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::INT3X1; }
		static daeInt ID() { return 805; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domInt3 of the value.
		 */
		::domInt3& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domInt3& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domInt3 value of the text data of this element. 
		 */
		::domInt3 _value;
	protected:
		/**
		 * Constructor
		 */
		domInt3x1(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domInt3x1() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domInt3x1 &operator=( const domInt3x1 &cpy ) { (void)cpy; return *this; }

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

	class domInt3x2;

	typedef daeSmartRef<domInt3x2> domInt3x2Ref;
	typedef daeTArray<domInt3x2Ref> domInt3x2_Array;

	class domInt3x2 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::INT3X2; }
		static daeInt ID() { return 806; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domInt3x2 of the value.
		 */
		::domInt3x2& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domInt3x2& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domInt3x2 value of the text data of this element. 
		 */
		::domInt3x2 _value;
	protected:
		/**
		 * Constructor
		 */
		domInt3x2(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domInt3x2() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domInt3x2 &operator=( const domInt3x2 &cpy ) { (void)cpy; return *this; }

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

	class domInt3x3;

	typedef daeSmartRef<domInt3x3> domInt3x3Ref;
	typedef daeTArray<domInt3x3Ref> domInt3x3_Array;

	class domInt3x3 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::INT3X3; }
		static daeInt ID() { return 807; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domInt3x3 of the value.
		 */
		::domInt3x3& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domInt3x3& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domInt3x3 value of the text data of this element. 
		 */
		::domInt3x3 _value;
	protected:
		/**
		 * Constructor
		 */
		domInt3x3(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domInt3x3() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domInt3x3 &operator=( const domInt3x3 &cpy ) { (void)cpy; return *this; }

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

	class domInt3x4;

	typedef daeSmartRef<domInt3x4> domInt3x4Ref;
	typedef daeTArray<domInt3x4Ref> domInt3x4_Array;

	class domInt3x4 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::INT3X4; }
		static daeInt ID() { return 808; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domInt3x4 of the value.
		 */
		::domInt3x4& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domInt3x4& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domInt3x4 value of the text data of this element. 
		 */
		::domInt3x4 _value;
	protected:
		/**
		 * Constructor
		 */
		domInt3x4(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domInt3x4() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domInt3x4 &operator=( const domInt3x4 &cpy ) { (void)cpy; return *this; }

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

	class domInt4x1;

	typedef daeSmartRef<domInt4x1> domInt4x1Ref;
	typedef daeTArray<domInt4x1Ref> domInt4x1_Array;

	class domInt4x1 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::INT4X1; }
		static daeInt ID() { return 809; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domInt4 of the value.
		 */
		::domInt4& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domInt4& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domInt4 value of the text data of this element. 
		 */
		::domInt4 _value;
	protected:
		/**
		 * Constructor
		 */
		domInt4x1(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domInt4x1() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domInt4x1 &operator=( const domInt4x1 &cpy ) { (void)cpy; return *this; }

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

	class domInt4x2;

	typedef daeSmartRef<domInt4x2> domInt4x2Ref;
	typedef daeTArray<domInt4x2Ref> domInt4x2_Array;

	class domInt4x2 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::INT4X2; }
		static daeInt ID() { return 810; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domInt4x2 of the value.
		 */
		::domInt4x2& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domInt4x2& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domInt4x2 value of the text data of this element. 
		 */
		::domInt4x2 _value;
	protected:
		/**
		 * Constructor
		 */
		domInt4x2(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domInt4x2() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domInt4x2 &operator=( const domInt4x2 &cpy ) { (void)cpy; return *this; }

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

	class domInt4x3;

	typedef daeSmartRef<domInt4x3> domInt4x3Ref;
	typedef daeTArray<domInt4x3Ref> domInt4x3_Array;

	class domInt4x3 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::INT4X3; }
		static daeInt ID() { return 811; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domInt4x3 of the value.
		 */
		::domInt4x3& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domInt4x3& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domInt4x3 value of the text data of this element. 
		 */
		::domInt4x3 _value;
	protected:
		/**
		 * Constructor
		 */
		domInt4x3(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domInt4x3() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domInt4x3 &operator=( const domInt4x3 &cpy ) { (void)cpy; return *this; }

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

	class domInt4x4;

	typedef daeSmartRef<domInt4x4> domInt4x4Ref;
	typedef daeTArray<domInt4x4Ref> domInt4x4_Array;

	class domInt4x4 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::INT4X4; }
		static daeInt ID() { return 812; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domInt4x4 of the value.
		 */
		::domInt4x4& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domInt4x4& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domInt4x4 value of the text data of this element. 
		 */
		::domInt4x4 _value;
	protected:
		/**
		 * Constructor
		 */
		domInt4x4(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domInt4x4() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domInt4x4 &operator=( const domInt4x4 &cpy ) { (void)cpy; return *this; }

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

	class domHalf;

	typedef daeSmartRef<domHalf> domHalfRef;
	typedef daeTArray<domHalfRef> domHalf_Array;

	class domHalf : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::HALF; }
		static daeInt ID() { return 813; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domFloat of the value.
		 */
		::domFloat& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domFloat& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domFloat value of the text data of this element. 
		 */
		::domFloat _value;
	protected:
		/**
		 * Constructor
		 */
		domHalf(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domHalf() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domHalf &operator=( const domHalf &cpy ) { (void)cpy; return *this; }

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

	class domHalf2;

	typedef daeSmartRef<domHalf2> domHalf2Ref;
	typedef daeTArray<domHalf2Ref> domHalf2_Array;

	class domHalf2 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::HALF2; }
		static daeInt ID() { return 814; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domFloat2 of the value.
		 */
		::domFloat2& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domFloat2& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domFloat2 value of the text data of this element. 
		 */
		::domFloat2 _value;
	protected:
		/**
		 * Constructor
		 */
		domHalf2(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domHalf2() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domHalf2 &operator=( const domHalf2 &cpy ) { (void)cpy; return *this; }

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

	class domHalf3;

	typedef daeSmartRef<domHalf3> domHalf3Ref;
	typedef daeTArray<domHalf3Ref> domHalf3_Array;

	class domHalf3 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::HALF3; }
		static daeInt ID() { return 815; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domFloat3 of the value.
		 */
		::domFloat3& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domFloat3& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domFloat3 value of the text data of this element. 
		 */
		::domFloat3 _value;
	protected:
		/**
		 * Constructor
		 */
		domHalf3(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domHalf3() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domHalf3 &operator=( const domHalf3 &cpy ) { (void)cpy; return *this; }

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

	class domHalf4;

	typedef daeSmartRef<domHalf4> domHalf4Ref;
	typedef daeTArray<domHalf4Ref> domHalf4_Array;

	class domHalf4 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::HALF4; }
		static daeInt ID() { return 816; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domFloat4 of the value.
		 */
		::domFloat4& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domFloat4& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domFloat4 value of the text data of this element. 
		 */
		::domFloat4 _value;
	protected:
		/**
		 * Constructor
		 */
		domHalf4(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domHalf4() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domHalf4 &operator=( const domHalf4 &cpy ) { (void)cpy; return *this; }

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

	class domHalf2x1;

	typedef daeSmartRef<domHalf2x1> domHalf2x1Ref;
	typedef daeTArray<domHalf2x1Ref> domHalf2x1_Array;

	class domHalf2x1 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::HALF2X1; }
		static daeInt ID() { return 817; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domFloat2 of the value.
		 */
		::domFloat2& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domFloat2& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domFloat2 value of the text data of this element. 
		 */
		::domFloat2 _value;
	protected:
		/**
		 * Constructor
		 */
		domHalf2x1(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domHalf2x1() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domHalf2x1 &operator=( const domHalf2x1 &cpy ) { (void)cpy; return *this; }

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

	class domHalf2x2;

	typedef daeSmartRef<domHalf2x2> domHalf2x2Ref;
	typedef daeTArray<domHalf2x2Ref> domHalf2x2_Array;

	class domHalf2x2 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::HALF2X2; }
		static daeInt ID() { return 818; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domFloat2x2 of the value.
		 */
		::domFloat2x2& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domFloat2x2& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domFloat2x2 value of the text data of this element. 
		 */
		::domFloat2x2 _value;
	protected:
		/**
		 * Constructor
		 */
		domHalf2x2(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domHalf2x2() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domHalf2x2 &operator=( const domHalf2x2 &cpy ) { (void)cpy; return *this; }

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

	class domHalf2x3;

	typedef daeSmartRef<domHalf2x3> domHalf2x3Ref;
	typedef daeTArray<domHalf2x3Ref> domHalf2x3_Array;

	class domHalf2x3 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::HALF2X3; }
		static daeInt ID() { return 819; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domFloat2x3 of the value.
		 */
		::domFloat2x3& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domFloat2x3& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domFloat2x3 value of the text data of this element. 
		 */
		::domFloat2x3 _value;
	protected:
		/**
		 * Constructor
		 */
		domHalf2x3(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domHalf2x3() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domHalf2x3 &operator=( const domHalf2x3 &cpy ) { (void)cpy; return *this; }

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

	class domHalf2x4;

	typedef daeSmartRef<domHalf2x4> domHalf2x4Ref;
	typedef daeTArray<domHalf2x4Ref> domHalf2x4_Array;

	class domHalf2x4 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::HALF2X4; }
		static daeInt ID() { return 820; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domFloat2x4 of the value.
		 */
		::domFloat2x4& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domFloat2x4& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domFloat2x4 value of the text data of this element. 
		 */
		::domFloat2x4 _value;
	protected:
		/**
		 * Constructor
		 */
		domHalf2x4(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domHalf2x4() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domHalf2x4 &operator=( const domHalf2x4 &cpy ) { (void)cpy; return *this; }

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

	class domHalf3x1;

	typedef daeSmartRef<domHalf3x1> domHalf3x1Ref;
	typedef daeTArray<domHalf3x1Ref> domHalf3x1_Array;

	class domHalf3x1 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::HALF3X1; }
		static daeInt ID() { return 821; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domFloat3 of the value.
		 */
		::domFloat3& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domFloat3& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domFloat3 value of the text data of this element. 
		 */
		::domFloat3 _value;
	protected:
		/**
		 * Constructor
		 */
		domHalf3x1(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domHalf3x1() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domHalf3x1 &operator=( const domHalf3x1 &cpy ) { (void)cpy; return *this; }

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

	class domHalf3x2;

	typedef daeSmartRef<domHalf3x2> domHalf3x2Ref;
	typedef daeTArray<domHalf3x2Ref> domHalf3x2_Array;

	class domHalf3x2 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::HALF3X2; }
		static daeInt ID() { return 822; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domFloat3x2 of the value.
		 */
		::domFloat3x2& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domFloat3x2& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domFloat3x2 value of the text data of this element. 
		 */
		::domFloat3x2 _value;
	protected:
		/**
		 * Constructor
		 */
		domHalf3x2(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domHalf3x2() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domHalf3x2 &operator=( const domHalf3x2 &cpy ) { (void)cpy; return *this; }

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

	class domHalf3x3;

	typedef daeSmartRef<domHalf3x3> domHalf3x3Ref;
	typedef daeTArray<domHalf3x3Ref> domHalf3x3_Array;

	class domHalf3x3 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::HALF3X3; }
		static daeInt ID() { return 823; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domFloat3x3 of the value.
		 */
		::domFloat3x3& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domFloat3x3& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domFloat3x3 value of the text data of this element. 
		 */
		::domFloat3x3 _value;
	protected:
		/**
		 * Constructor
		 */
		domHalf3x3(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domHalf3x3() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domHalf3x3 &operator=( const domHalf3x3 &cpy ) { (void)cpy; return *this; }

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

	class domHalf3x4;

	typedef daeSmartRef<domHalf3x4> domHalf3x4Ref;
	typedef daeTArray<domHalf3x4Ref> domHalf3x4_Array;

	class domHalf3x4 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::HALF3X4; }
		static daeInt ID() { return 824; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domFloat3x4 of the value.
		 */
		::domFloat3x4& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domFloat3x4& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domFloat3x4 value of the text data of this element. 
		 */
		::domFloat3x4 _value;
	protected:
		/**
		 * Constructor
		 */
		domHalf3x4(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domHalf3x4() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domHalf3x4 &operator=( const domHalf3x4 &cpy ) { (void)cpy; return *this; }

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

	class domHalf4x1;

	typedef daeSmartRef<domHalf4x1> domHalf4x1Ref;
	typedef daeTArray<domHalf4x1Ref> domHalf4x1_Array;

	class domHalf4x1 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::HALF4X1; }
		static daeInt ID() { return 825; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domFloat4 of the value.
		 */
		::domFloat4& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domFloat4& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domFloat4 value of the text data of this element. 
		 */
		::domFloat4 _value;
	protected:
		/**
		 * Constructor
		 */
		domHalf4x1(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domHalf4x1() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domHalf4x1 &operator=( const domHalf4x1 &cpy ) { (void)cpy; return *this; }

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

	class domHalf4x2;

	typedef daeSmartRef<domHalf4x2> domHalf4x2Ref;
	typedef daeTArray<domHalf4x2Ref> domHalf4x2_Array;

	class domHalf4x2 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::HALF4X2; }
		static daeInt ID() { return 826; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domFloat4x2 of the value.
		 */
		::domFloat4x2& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domFloat4x2& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domFloat4x2 value of the text data of this element. 
		 */
		::domFloat4x2 _value;
	protected:
		/**
		 * Constructor
		 */
		domHalf4x2(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domHalf4x2() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domHalf4x2 &operator=( const domHalf4x2 &cpy ) { (void)cpy; return *this; }

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

	class domHalf4x3;

	typedef daeSmartRef<domHalf4x3> domHalf4x3Ref;
	typedef daeTArray<domHalf4x3Ref> domHalf4x3_Array;

	class domHalf4x3 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::HALF4X3; }
		static daeInt ID() { return 827; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domFloat4x3 of the value.
		 */
		::domFloat4x3& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domFloat4x3& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domFloat4x3 value of the text data of this element. 
		 */
		::domFloat4x3 _value;
	protected:
		/**
		 * Constructor
		 */
		domHalf4x3(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domHalf4x3() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domHalf4x3 &operator=( const domHalf4x3 &cpy ) { (void)cpy; return *this; }

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

	class domHalf4x4;

	typedef daeSmartRef<domHalf4x4> domHalf4x4Ref;
	typedef daeTArray<domHalf4x4Ref> domHalf4x4_Array;

	class domHalf4x4 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::HALF4X4; }
		static daeInt ID() { return 828; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domFloat4x4 of the value.
		 */
		::domFloat4x4& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domFloat4x4& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domFloat4x4 value of the text data of this element. 
		 */
		::domFloat4x4 _value;
	protected:
		/**
		 * Constructor
		 */
		domHalf4x4(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domHalf4x4() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domHalf4x4 &operator=( const domHalf4x4 &cpy ) { (void)cpy; return *this; }

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

	class domFixed;

	typedef daeSmartRef<domFixed> domFixedRef;
	typedef daeTArray<domFixedRef> domFixed_Array;

	class domFixed : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FIXED; }
		static daeInt ID() { return 829; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domFloat of the value.
		 */
		::domFloat& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domFloat& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domFloat value of the text data of this element. 
		 */
		::domFloat _value;
	protected:
		/**
		 * Constructor
		 */
		domFixed(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domFixed() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domFixed &operator=( const domFixed &cpy ) { (void)cpy; return *this; }

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

	class domFixed2;

	typedef daeSmartRef<domFixed2> domFixed2Ref;
	typedef daeTArray<domFixed2Ref> domFixed2_Array;

	class domFixed2 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FIXED2; }
		static daeInt ID() { return 830; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domFloat2 of the value.
		 */
		::domFloat2& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domFloat2& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domFloat2 value of the text data of this element. 
		 */
		::domFloat2 _value;
	protected:
		/**
		 * Constructor
		 */
		domFixed2(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domFixed2() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domFixed2 &operator=( const domFixed2 &cpy ) { (void)cpy; return *this; }

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

	class domFixed3;

	typedef daeSmartRef<domFixed3> domFixed3Ref;
	typedef daeTArray<domFixed3Ref> domFixed3_Array;

	class domFixed3 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FIXED3; }
		static daeInt ID() { return 831; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domFloat3 of the value.
		 */
		::domFloat3& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domFloat3& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domFloat3 value of the text data of this element. 
		 */
		::domFloat3 _value;
	protected:
		/**
		 * Constructor
		 */
		domFixed3(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domFixed3() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domFixed3 &operator=( const domFixed3 &cpy ) { (void)cpy; return *this; }

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

	class domFixed4;

	typedef daeSmartRef<domFixed4> domFixed4Ref;
	typedef daeTArray<domFixed4Ref> domFixed4_Array;

	class domFixed4 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FIXED4; }
		static daeInt ID() { return 832; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domFloat4 of the value.
		 */
		::domFloat4& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domFloat4& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domFloat4 value of the text data of this element. 
		 */
		::domFloat4 _value;
	protected:
		/**
		 * Constructor
		 */
		domFixed4(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domFixed4() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domFixed4 &operator=( const domFixed4 &cpy ) { (void)cpy; return *this; }

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

	class domFixed2x1;

	typedef daeSmartRef<domFixed2x1> domFixed2x1Ref;
	typedef daeTArray<domFixed2x1Ref> domFixed2x1_Array;

	class domFixed2x1 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FIXED2X1; }
		static daeInt ID() { return 833; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domFloat2 of the value.
		 */
		::domFloat2& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domFloat2& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domFloat2 value of the text data of this element. 
		 */
		::domFloat2 _value;
	protected:
		/**
		 * Constructor
		 */
		domFixed2x1(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domFixed2x1() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domFixed2x1 &operator=( const domFixed2x1 &cpy ) { (void)cpy; return *this; }

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

	class domFixed2x2;

	typedef daeSmartRef<domFixed2x2> domFixed2x2Ref;
	typedef daeTArray<domFixed2x2Ref> domFixed2x2_Array;

	class domFixed2x2 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FIXED2X2; }
		static daeInt ID() { return 834; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domFloat2x2 of the value.
		 */
		::domFloat2x2& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domFloat2x2& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domFloat2x2 value of the text data of this element. 
		 */
		::domFloat2x2 _value;
	protected:
		/**
		 * Constructor
		 */
		domFixed2x2(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domFixed2x2() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domFixed2x2 &operator=( const domFixed2x2 &cpy ) { (void)cpy; return *this; }

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

	class domFixed2x3;

	typedef daeSmartRef<domFixed2x3> domFixed2x3Ref;
	typedef daeTArray<domFixed2x3Ref> domFixed2x3_Array;

	class domFixed2x3 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FIXED2X3; }
		static daeInt ID() { return 835; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domFloat2x3 of the value.
		 */
		::domFloat2x3& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domFloat2x3& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domFloat2x3 value of the text data of this element. 
		 */
		::domFloat2x3 _value;
	protected:
		/**
		 * Constructor
		 */
		domFixed2x3(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domFixed2x3() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domFixed2x3 &operator=( const domFixed2x3 &cpy ) { (void)cpy; return *this; }

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

	class domFixed2x4;

	typedef daeSmartRef<domFixed2x4> domFixed2x4Ref;
	typedef daeTArray<domFixed2x4Ref> domFixed2x4_Array;

	class domFixed2x4 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FIXED2X4; }
		static daeInt ID() { return 836; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domFloat2x4 of the value.
		 */
		::domFloat2x4& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domFloat2x4& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domFloat2x4 value of the text data of this element. 
		 */
		::domFloat2x4 _value;
	protected:
		/**
		 * Constructor
		 */
		domFixed2x4(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domFixed2x4() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domFixed2x4 &operator=( const domFixed2x4 &cpy ) { (void)cpy; return *this; }

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

	class domFixed3x1;

	typedef daeSmartRef<domFixed3x1> domFixed3x1Ref;
	typedef daeTArray<domFixed3x1Ref> domFixed3x1_Array;

	class domFixed3x1 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FIXED3X1; }
		static daeInt ID() { return 837; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domFloat3 of the value.
		 */
		::domFloat3& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domFloat3& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domFloat3 value of the text data of this element. 
		 */
		::domFloat3 _value;
	protected:
		/**
		 * Constructor
		 */
		domFixed3x1(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domFixed3x1() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domFixed3x1 &operator=( const domFixed3x1 &cpy ) { (void)cpy; return *this; }

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

	class domFixed3x2;

	typedef daeSmartRef<domFixed3x2> domFixed3x2Ref;
	typedef daeTArray<domFixed3x2Ref> domFixed3x2_Array;

	class domFixed3x2 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FIXED3X2; }
		static daeInt ID() { return 838; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domFloat3x2 of the value.
		 */
		::domFloat3x2& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domFloat3x2& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domFloat3x2 value of the text data of this element. 
		 */
		::domFloat3x2 _value;
	protected:
		/**
		 * Constructor
		 */
		domFixed3x2(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domFixed3x2() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domFixed3x2 &operator=( const domFixed3x2 &cpy ) { (void)cpy; return *this; }

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

	class domFixed3x3;

	typedef daeSmartRef<domFixed3x3> domFixed3x3Ref;
	typedef daeTArray<domFixed3x3Ref> domFixed3x3_Array;

	class domFixed3x3 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FIXED3X3; }
		static daeInt ID() { return 839; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domFloat3x3 of the value.
		 */
		::domFloat3x3& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domFloat3x3& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domFloat3x3 value of the text data of this element. 
		 */
		::domFloat3x3 _value;
	protected:
		/**
		 * Constructor
		 */
		domFixed3x3(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domFixed3x3() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domFixed3x3 &operator=( const domFixed3x3 &cpy ) { (void)cpy; return *this; }

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

	class domFixed3x4;

	typedef daeSmartRef<domFixed3x4> domFixed3x4Ref;
	typedef daeTArray<domFixed3x4Ref> domFixed3x4_Array;

	class domFixed3x4 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FIXED3X4; }
		static daeInt ID() { return 840; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domFloat3x4 of the value.
		 */
		::domFloat3x4& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domFloat3x4& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domFloat3x4 value of the text data of this element. 
		 */
		::domFloat3x4 _value;
	protected:
		/**
		 * Constructor
		 */
		domFixed3x4(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domFixed3x4() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domFixed3x4 &operator=( const domFixed3x4 &cpy ) { (void)cpy; return *this; }

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

	class domFixed4x1;

	typedef daeSmartRef<domFixed4x1> domFixed4x1Ref;
	typedef daeTArray<domFixed4x1Ref> domFixed4x1_Array;

	class domFixed4x1 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FIXED4X1; }
		static daeInt ID() { return 841; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domFloat4 of the value.
		 */
		::domFloat4& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domFloat4& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domFloat4 value of the text data of this element. 
		 */
		::domFloat4 _value;
	protected:
		/**
		 * Constructor
		 */
		domFixed4x1(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domFixed4x1() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domFixed4x1 &operator=( const domFixed4x1 &cpy ) { (void)cpy; return *this; }

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

	class domFixed4x2;

	typedef daeSmartRef<domFixed4x2> domFixed4x2Ref;
	typedef daeTArray<domFixed4x2Ref> domFixed4x2_Array;

	class domFixed4x2 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FIXED4X2; }
		static daeInt ID() { return 842; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domFloat4x2 of the value.
		 */
		::domFloat4x2& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domFloat4x2& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domFloat4x2 value of the text data of this element. 
		 */
		::domFloat4x2 _value;
	protected:
		/**
		 * Constructor
		 */
		domFixed4x2(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domFixed4x2() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domFixed4x2 &operator=( const domFixed4x2 &cpy ) { (void)cpy; return *this; }

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

	class domFixed4x3;

	typedef daeSmartRef<domFixed4x3> domFixed4x3Ref;
	typedef daeTArray<domFixed4x3Ref> domFixed4x3_Array;

	class domFixed4x3 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FIXED4X3; }
		static daeInt ID() { return 843; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domFloat4x3 of the value.
		 */
		::domFloat4x3& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domFloat4x3& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domFloat4x3 value of the text data of this element. 
		 */
		::domFloat4x3 _value;
	protected:
		/**
		 * Constructor
		 */
		domFixed4x3(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domFixed4x3() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domFixed4x3 &operator=( const domFixed4x3 &cpy ) { (void)cpy; return *this; }

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

	class domFixed4x4;

	typedef daeSmartRef<domFixed4x4> domFixed4x4Ref;
	typedef daeTArray<domFixed4x4Ref> domFixed4x4_Array;

	class domFixed4x4 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FIXED4X4; }
		static daeInt ID() { return 844; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domFloat4x4 of the value.
		 */
		::domFloat4x4& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domFloat4x4& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domFloat4x4 value of the text data of this element. 
		 */
		::domFloat4x4 _value;
	protected:
		/**
		 * Constructor
		 */
		domFixed4x4(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domFixed4x4() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domFixed4x4 &operator=( const domFixed4x4 &cpy ) { (void)cpy; return *this; }

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

	class domString;

	typedef daeSmartRef<domString> domStringRef;
	typedef daeTArray<domStringRef> domString_Array;

	class domString : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::STRING; }
		static daeInt ID() { return 845; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return Returns a ::xsString of the value.
		 */
		::xsString getValue() const { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( ::xsString val ) { *(daeStringRef*)&_value = val; }

	protected:  // Value
		/**
		 * The ::xsString value of the text data of this element. 
		 */
		::xsString _value;
	protected:
		/**
		 * Constructor
		 */
		domString(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domString() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domString &operator=( const domString &cpy ) { (void)cpy; return *this; }

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

	class domEnum;

	typedef daeSmartRef<domEnum> domEnumRef;
	typedef daeTArray<domEnumRef> domEnum_Array;

	class domEnum : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::ENUM; }
		static daeInt ID() { return 846; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a domGl_enumeration of the value.
		 */
		domGl_enumeration& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const domGl_enumeration& val ) { _value = val; }

	protected:  // Value
		/**
		 * The domGl_enumeration value of the text data of this element. 
		 */
		domGl_enumeration _value;
	protected:
		/**
		 * Constructor
		 */
		domEnum(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domEnum() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domEnum &operator=( const domEnum &cpy ) { (void)cpy; return *this; }

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
	domBoolRef elemBool;
	domBool2Ref elemBool2;
	domBool3Ref elemBool3;
	domBool4Ref elemBool4;
	domBool2x1Ref elemBool2x1;
	domBool2x2Ref elemBool2x2;
	domBool2x3Ref elemBool2x3;
	domBool2x4Ref elemBool2x4;
	domBool3x1Ref elemBool3x1;
	domBool3x2Ref elemBool3x2;
	domBool3x3Ref elemBool3x3;
	domBool3x4Ref elemBool3x4;
	domBool4x1Ref elemBool4x1;
	domBool4x2Ref elemBool4x2;
	domBool4x3Ref elemBool4x3;
	domBool4x4Ref elemBool4x4;
	domFloatRef elemFloat;
	domFloat2Ref elemFloat2;
	domFloat3Ref elemFloat3;
	domFloat4Ref elemFloat4;
	domFloat2x1Ref elemFloat2x1;
	domFloat2x2Ref elemFloat2x2;
	domFloat2x3Ref elemFloat2x3;
	domFloat2x4Ref elemFloat2x4;
	domFloat3x1Ref elemFloat3x1;
	domFloat3x2Ref elemFloat3x2;
	domFloat3x3Ref elemFloat3x3;
	domFloat3x4Ref elemFloat3x4;
	domFloat4x1Ref elemFloat4x1;
	domFloat4x2Ref elemFloat4x2;
	domFloat4x3Ref elemFloat4x3;
	domFloat4x4Ref elemFloat4x4;
	domIntRef elemInt;
	domInt2Ref elemInt2;
	domInt3Ref elemInt3;
	domInt4Ref elemInt4;
	domInt2x1Ref elemInt2x1;
	domInt2x2Ref elemInt2x2;
	domInt2x3Ref elemInt2x3;
	domInt2x4Ref elemInt2x4;
	domInt3x1Ref elemInt3x1;
	domInt3x2Ref elemInt3x2;
	domInt3x3Ref elemInt3x3;
	domInt3x4Ref elemInt3x4;
	domInt4x1Ref elemInt4x1;
	domInt4x2Ref elemInt4x2;
	domInt4x3Ref elemInt4x3;
	domInt4x4Ref elemInt4x4;
	domHalfRef elemHalf;
	domHalf2Ref elemHalf2;
	domHalf3Ref elemHalf3;
	domHalf4Ref elemHalf4;
	domHalf2x1Ref elemHalf2x1;
	domHalf2x2Ref elemHalf2x2;
	domHalf2x3Ref elemHalf2x3;
	domHalf2x4Ref elemHalf2x4;
	domHalf3x1Ref elemHalf3x1;
	domHalf3x2Ref elemHalf3x2;
	domHalf3x3Ref elemHalf3x3;
	domHalf3x4Ref elemHalf3x4;
	domHalf4x1Ref elemHalf4x1;
	domHalf4x2Ref elemHalf4x2;
	domHalf4x3Ref elemHalf4x3;
	domHalf4x4Ref elemHalf4x4;
	domFixedRef elemFixed;
	domFixed2Ref elemFixed2;
	domFixed3Ref elemFixed3;
	domFixed4Ref elemFixed4;
	domFixed2x1Ref elemFixed2x1;
	domFixed2x2Ref elemFixed2x2;
	domFixed2x3Ref elemFixed2x3;
	domFixed2x4Ref elemFixed2x4;
	domFixed3x1Ref elemFixed3x1;
	domFixed3x2Ref elemFixed3x2;
	domFixed3x3Ref elemFixed3x3;
	domFixed3x4Ref elemFixed3x4;
	domFixed4x1Ref elemFixed4x1;
	domFixed4x2Ref elemFixed4x2;
	domFixed4x3Ref elemFixed4x3;
	domFixed4x4Ref elemFixed4x4;
	domFx_sampler1DRef elemSampler1D;
	domFx_sampler2DRef elemSampler2D;
	domFx_sampler3DRef elemSampler3D;
	domFx_samplerRECTRef elemSamplerRECT;
	domFx_samplerCUBERef elemSamplerCUBE;
	domFx_samplerDEPTHRef elemSamplerDEPTH;
	domStringRef elemString;
	domEnumRef elemEnum;
	domCg_arrayRef elemArray;
	domCg_userRef elemUsertype;
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
	 * Gets the bool element.
	 * @return a daeSmartRef to the bool element.
	 */
	const domBoolRef getBool() const { return elemBool; }
	/**
	 * Gets the bool2 element.
	 * @return a daeSmartRef to the bool2 element.
	 */
	const domBool2Ref getBool2() const { return elemBool2; }
	/**
	 * Gets the bool3 element.
	 * @return a daeSmartRef to the bool3 element.
	 */
	const domBool3Ref getBool3() const { return elemBool3; }
	/**
	 * Gets the bool4 element.
	 * @return a daeSmartRef to the bool4 element.
	 */
	const domBool4Ref getBool4() const { return elemBool4; }
	/**
	 * Gets the bool2x1 element.
	 * @return a daeSmartRef to the bool2x1 element.
	 */
	const domBool2x1Ref getBool2x1() const { return elemBool2x1; }
	/**
	 * Gets the bool2x2 element.
	 * @return a daeSmartRef to the bool2x2 element.
	 */
	const domBool2x2Ref getBool2x2() const { return elemBool2x2; }
	/**
	 * Gets the bool2x3 element.
	 * @return a daeSmartRef to the bool2x3 element.
	 */
	const domBool2x3Ref getBool2x3() const { return elemBool2x3; }
	/**
	 * Gets the bool2x4 element.
	 * @return a daeSmartRef to the bool2x4 element.
	 */
	const domBool2x4Ref getBool2x4() const { return elemBool2x4; }
	/**
	 * Gets the bool3x1 element.
	 * @return a daeSmartRef to the bool3x1 element.
	 */
	const domBool3x1Ref getBool3x1() const { return elemBool3x1; }
	/**
	 * Gets the bool3x2 element.
	 * @return a daeSmartRef to the bool3x2 element.
	 */
	const domBool3x2Ref getBool3x2() const { return elemBool3x2; }
	/**
	 * Gets the bool3x3 element.
	 * @return a daeSmartRef to the bool3x3 element.
	 */
	const domBool3x3Ref getBool3x3() const { return elemBool3x3; }
	/**
	 * Gets the bool3x4 element.
	 * @return a daeSmartRef to the bool3x4 element.
	 */
	const domBool3x4Ref getBool3x4() const { return elemBool3x4; }
	/**
	 * Gets the bool4x1 element.
	 * @return a daeSmartRef to the bool4x1 element.
	 */
	const domBool4x1Ref getBool4x1() const { return elemBool4x1; }
	/**
	 * Gets the bool4x2 element.
	 * @return a daeSmartRef to the bool4x2 element.
	 */
	const domBool4x2Ref getBool4x2() const { return elemBool4x2; }
	/**
	 * Gets the bool4x3 element.
	 * @return a daeSmartRef to the bool4x3 element.
	 */
	const domBool4x3Ref getBool4x3() const { return elemBool4x3; }
	/**
	 * Gets the bool4x4 element.
	 * @return a daeSmartRef to the bool4x4 element.
	 */
	const domBool4x4Ref getBool4x4() const { return elemBool4x4; }
	/**
	 * Gets the float element.
	 * @return a daeSmartRef to the float element.
	 */
	const domFloatRef getFloat() const { return elemFloat; }
	/**
	 * Gets the float2 element.
	 * @return a daeSmartRef to the float2 element.
	 */
	const domFloat2Ref getFloat2() const { return elemFloat2; }
	/**
	 * Gets the float3 element.
	 * @return a daeSmartRef to the float3 element.
	 */
	const domFloat3Ref getFloat3() const { return elemFloat3; }
	/**
	 * Gets the float4 element.
	 * @return a daeSmartRef to the float4 element.
	 */
	const domFloat4Ref getFloat4() const { return elemFloat4; }
	/**
	 * Gets the float2x1 element.
	 * @return a daeSmartRef to the float2x1 element.
	 */
	const domFloat2x1Ref getFloat2x1() const { return elemFloat2x1; }
	/**
	 * Gets the float2x2 element.
	 * @return a daeSmartRef to the float2x2 element.
	 */
	const domFloat2x2Ref getFloat2x2() const { return elemFloat2x2; }
	/**
	 * Gets the float2x3 element.
	 * @return a daeSmartRef to the float2x3 element.
	 */
	const domFloat2x3Ref getFloat2x3() const { return elemFloat2x3; }
	/**
	 * Gets the float2x4 element.
	 * @return a daeSmartRef to the float2x4 element.
	 */
	const domFloat2x4Ref getFloat2x4() const { return elemFloat2x4; }
	/**
	 * Gets the float3x1 element.
	 * @return a daeSmartRef to the float3x1 element.
	 */
	const domFloat3x1Ref getFloat3x1() const { return elemFloat3x1; }
	/**
	 * Gets the float3x2 element.
	 * @return a daeSmartRef to the float3x2 element.
	 */
	const domFloat3x2Ref getFloat3x2() const { return elemFloat3x2; }
	/**
	 * Gets the float3x3 element.
	 * @return a daeSmartRef to the float3x3 element.
	 */
	const domFloat3x3Ref getFloat3x3() const { return elemFloat3x3; }
	/**
	 * Gets the float3x4 element.
	 * @return a daeSmartRef to the float3x4 element.
	 */
	const domFloat3x4Ref getFloat3x4() const { return elemFloat3x4; }
	/**
	 * Gets the float4x1 element.
	 * @return a daeSmartRef to the float4x1 element.
	 */
	const domFloat4x1Ref getFloat4x1() const { return elemFloat4x1; }
	/**
	 * Gets the float4x2 element.
	 * @return a daeSmartRef to the float4x2 element.
	 */
	const domFloat4x2Ref getFloat4x2() const { return elemFloat4x2; }
	/**
	 * Gets the float4x3 element.
	 * @return a daeSmartRef to the float4x3 element.
	 */
	const domFloat4x3Ref getFloat4x3() const { return elemFloat4x3; }
	/**
	 * Gets the float4x4 element.
	 * @return a daeSmartRef to the float4x4 element.
	 */
	const domFloat4x4Ref getFloat4x4() const { return elemFloat4x4; }
	/**
	 * Gets the int element.
	 * @return a daeSmartRef to the int element.
	 */
	const domIntRef getInt() const { return elemInt; }
	/**
	 * Gets the int2 element.
	 * @return a daeSmartRef to the int2 element.
	 */
	const domInt2Ref getInt2() const { return elemInt2; }
	/**
	 * Gets the int3 element.
	 * @return a daeSmartRef to the int3 element.
	 */
	const domInt3Ref getInt3() const { return elemInt3; }
	/**
	 * Gets the int4 element.
	 * @return a daeSmartRef to the int4 element.
	 */
	const domInt4Ref getInt4() const { return elemInt4; }
	/**
	 * Gets the int2x1 element.
	 * @return a daeSmartRef to the int2x1 element.
	 */
	const domInt2x1Ref getInt2x1() const { return elemInt2x1; }
	/**
	 * Gets the int2x2 element.
	 * @return a daeSmartRef to the int2x2 element.
	 */
	const domInt2x2Ref getInt2x2() const { return elemInt2x2; }
	/**
	 * Gets the int2x3 element.
	 * @return a daeSmartRef to the int2x3 element.
	 */
	const domInt2x3Ref getInt2x3() const { return elemInt2x3; }
	/**
	 * Gets the int2x4 element.
	 * @return a daeSmartRef to the int2x4 element.
	 */
	const domInt2x4Ref getInt2x4() const { return elemInt2x4; }
	/**
	 * Gets the int3x1 element.
	 * @return a daeSmartRef to the int3x1 element.
	 */
	const domInt3x1Ref getInt3x1() const { return elemInt3x1; }
	/**
	 * Gets the int3x2 element.
	 * @return a daeSmartRef to the int3x2 element.
	 */
	const domInt3x2Ref getInt3x2() const { return elemInt3x2; }
	/**
	 * Gets the int3x3 element.
	 * @return a daeSmartRef to the int3x3 element.
	 */
	const domInt3x3Ref getInt3x3() const { return elemInt3x3; }
	/**
	 * Gets the int3x4 element.
	 * @return a daeSmartRef to the int3x4 element.
	 */
	const domInt3x4Ref getInt3x4() const { return elemInt3x4; }
	/**
	 * Gets the int4x1 element.
	 * @return a daeSmartRef to the int4x1 element.
	 */
	const domInt4x1Ref getInt4x1() const { return elemInt4x1; }
	/**
	 * Gets the int4x2 element.
	 * @return a daeSmartRef to the int4x2 element.
	 */
	const domInt4x2Ref getInt4x2() const { return elemInt4x2; }
	/**
	 * Gets the int4x3 element.
	 * @return a daeSmartRef to the int4x3 element.
	 */
	const domInt4x3Ref getInt4x3() const { return elemInt4x3; }
	/**
	 * Gets the int4x4 element.
	 * @return a daeSmartRef to the int4x4 element.
	 */
	const domInt4x4Ref getInt4x4() const { return elemInt4x4; }
	/**
	 * Gets the half element.
	 * @return a daeSmartRef to the half element.
	 */
	const domHalfRef getHalf() const { return elemHalf; }
	/**
	 * Gets the half2 element.
	 * @return a daeSmartRef to the half2 element.
	 */
	const domHalf2Ref getHalf2() const { return elemHalf2; }
	/**
	 * Gets the half3 element.
	 * @return a daeSmartRef to the half3 element.
	 */
	const domHalf3Ref getHalf3() const { return elemHalf3; }
	/**
	 * Gets the half4 element.
	 * @return a daeSmartRef to the half4 element.
	 */
	const domHalf4Ref getHalf4() const { return elemHalf4; }
	/**
	 * Gets the half2x1 element.
	 * @return a daeSmartRef to the half2x1 element.
	 */
	const domHalf2x1Ref getHalf2x1() const { return elemHalf2x1; }
	/**
	 * Gets the half2x2 element.
	 * @return a daeSmartRef to the half2x2 element.
	 */
	const domHalf2x2Ref getHalf2x2() const { return elemHalf2x2; }
	/**
	 * Gets the half2x3 element.
	 * @return a daeSmartRef to the half2x3 element.
	 */
	const domHalf2x3Ref getHalf2x3() const { return elemHalf2x3; }
	/**
	 * Gets the half2x4 element.
	 * @return a daeSmartRef to the half2x4 element.
	 */
	const domHalf2x4Ref getHalf2x4() const { return elemHalf2x4; }
	/**
	 * Gets the half3x1 element.
	 * @return a daeSmartRef to the half3x1 element.
	 */
	const domHalf3x1Ref getHalf3x1() const { return elemHalf3x1; }
	/**
	 * Gets the half3x2 element.
	 * @return a daeSmartRef to the half3x2 element.
	 */
	const domHalf3x2Ref getHalf3x2() const { return elemHalf3x2; }
	/**
	 * Gets the half3x3 element.
	 * @return a daeSmartRef to the half3x3 element.
	 */
	const domHalf3x3Ref getHalf3x3() const { return elemHalf3x3; }
	/**
	 * Gets the half3x4 element.
	 * @return a daeSmartRef to the half3x4 element.
	 */
	const domHalf3x4Ref getHalf3x4() const { return elemHalf3x4; }
	/**
	 * Gets the half4x1 element.
	 * @return a daeSmartRef to the half4x1 element.
	 */
	const domHalf4x1Ref getHalf4x1() const { return elemHalf4x1; }
	/**
	 * Gets the half4x2 element.
	 * @return a daeSmartRef to the half4x2 element.
	 */
	const domHalf4x2Ref getHalf4x2() const { return elemHalf4x2; }
	/**
	 * Gets the half4x3 element.
	 * @return a daeSmartRef to the half4x3 element.
	 */
	const domHalf4x3Ref getHalf4x3() const { return elemHalf4x3; }
	/**
	 * Gets the half4x4 element.
	 * @return a daeSmartRef to the half4x4 element.
	 */
	const domHalf4x4Ref getHalf4x4() const { return elemHalf4x4; }
	/**
	 * Gets the fixed element.
	 * @return a daeSmartRef to the fixed element.
	 */
	const domFixedRef getFixed() const { return elemFixed; }
	/**
	 * Gets the fixed2 element.
	 * @return a daeSmartRef to the fixed2 element.
	 */
	const domFixed2Ref getFixed2() const { return elemFixed2; }
	/**
	 * Gets the fixed3 element.
	 * @return a daeSmartRef to the fixed3 element.
	 */
	const domFixed3Ref getFixed3() const { return elemFixed3; }
	/**
	 * Gets the fixed4 element.
	 * @return a daeSmartRef to the fixed4 element.
	 */
	const domFixed4Ref getFixed4() const { return elemFixed4; }
	/**
	 * Gets the fixed2x1 element.
	 * @return a daeSmartRef to the fixed2x1 element.
	 */
	const domFixed2x1Ref getFixed2x1() const { return elemFixed2x1; }
	/**
	 * Gets the fixed2x2 element.
	 * @return a daeSmartRef to the fixed2x2 element.
	 */
	const domFixed2x2Ref getFixed2x2() const { return elemFixed2x2; }
	/**
	 * Gets the fixed2x3 element.
	 * @return a daeSmartRef to the fixed2x3 element.
	 */
	const domFixed2x3Ref getFixed2x3() const { return elemFixed2x3; }
	/**
	 * Gets the fixed2x4 element.
	 * @return a daeSmartRef to the fixed2x4 element.
	 */
	const domFixed2x4Ref getFixed2x4() const { return elemFixed2x4; }
	/**
	 * Gets the fixed3x1 element.
	 * @return a daeSmartRef to the fixed3x1 element.
	 */
	const domFixed3x1Ref getFixed3x1() const { return elemFixed3x1; }
	/**
	 * Gets the fixed3x2 element.
	 * @return a daeSmartRef to the fixed3x2 element.
	 */
	const domFixed3x2Ref getFixed3x2() const { return elemFixed3x2; }
	/**
	 * Gets the fixed3x3 element.
	 * @return a daeSmartRef to the fixed3x3 element.
	 */
	const domFixed3x3Ref getFixed3x3() const { return elemFixed3x3; }
	/**
	 * Gets the fixed3x4 element.
	 * @return a daeSmartRef to the fixed3x4 element.
	 */
	const domFixed3x4Ref getFixed3x4() const { return elemFixed3x4; }
	/**
	 * Gets the fixed4x1 element.
	 * @return a daeSmartRef to the fixed4x1 element.
	 */
	const domFixed4x1Ref getFixed4x1() const { return elemFixed4x1; }
	/**
	 * Gets the fixed4x2 element.
	 * @return a daeSmartRef to the fixed4x2 element.
	 */
	const domFixed4x2Ref getFixed4x2() const { return elemFixed4x2; }
	/**
	 * Gets the fixed4x3 element.
	 * @return a daeSmartRef to the fixed4x3 element.
	 */
	const domFixed4x3Ref getFixed4x3() const { return elemFixed4x3; }
	/**
	 * Gets the fixed4x4 element.
	 * @return a daeSmartRef to the fixed4x4 element.
	 */
	const domFixed4x4Ref getFixed4x4() const { return elemFixed4x4; }
	/**
	 * Gets the sampler1D element.
	 * @return a daeSmartRef to the sampler1D element.
	 */
	const domFx_sampler1DRef getSampler1D() const { return elemSampler1D; }
	/**
	 * Gets the sampler2D element.
	 * @return a daeSmartRef to the sampler2D element.
	 */
	const domFx_sampler2DRef getSampler2D() const { return elemSampler2D; }
	/**
	 * Gets the sampler3D element.
	 * @return a daeSmartRef to the sampler3D element.
	 */
	const domFx_sampler3DRef getSampler3D() const { return elemSampler3D; }
	/**
	 * Gets the samplerRECT element.
	 * @return a daeSmartRef to the samplerRECT element.
	 */
	const domFx_samplerRECTRef getSamplerRECT() const { return elemSamplerRECT; }
	/**
	 * Gets the samplerCUBE element.
	 * @return a daeSmartRef to the samplerCUBE element.
	 */
	const domFx_samplerCUBERef getSamplerCUBE() const { return elemSamplerCUBE; }
	/**
	 * Gets the samplerDEPTH element.
	 * @return a daeSmartRef to the samplerDEPTH element.
	 */
	const domFx_samplerDEPTHRef getSamplerDEPTH() const { return elemSamplerDEPTH; }
	/**
	 * Gets the string element.
	 * @return a daeSmartRef to the string element.
	 */
	const domStringRef getString() const { return elemString; }
	/**
	 * Gets the enum element.
	 * @return a daeSmartRef to the enum element.
	 */
	const domEnumRef getEnum() const { return elemEnum; }
	/**
	 * Gets the array element.
	 * @return a daeSmartRef to the array element.
	 */
	const domCg_arrayRef getArray() const { return elemArray; }
	/**
	 * Gets the usertype element.
	 * @return a daeSmartRef to the usertype element.
	 */
	const domCg_userRef getUsertype() const { return elemUsertype; }
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
	domCg_param(DAE& dae) : daeElement(dae), elemBool(), elemBool2(), elemBool3(), elemBool4(), elemBool2x1(), elemBool2x2(), elemBool2x3(), elemBool2x4(), elemBool3x1(), elemBool3x2(), elemBool3x3(), elemBool3x4(), elemBool4x1(), elemBool4x2(), elemBool4x3(), elemBool4x4(), elemFloat(), elemFloat2(), elemFloat3(), elemFloat4(), elemFloat2x1(), elemFloat2x2(), elemFloat2x3(), elemFloat2x4(), elemFloat3x1(), elemFloat3x2(), elemFloat3x3(), elemFloat3x4(), elemFloat4x1(), elemFloat4x2(), elemFloat4x3(), elemFloat4x4(), elemInt(), elemInt2(), elemInt3(), elemInt4(), elemInt2x1(), elemInt2x2(), elemInt2x3(), elemInt2x4(), elemInt3x1(), elemInt3x2(), elemInt3x3(), elemInt3x4(), elemInt4x1(), elemInt4x2(), elemInt4x3(), elemInt4x4(), elemHalf(), elemHalf2(), elemHalf3(), elemHalf4(), elemHalf2x1(), elemHalf2x2(), elemHalf2x3(), elemHalf2x4(), elemHalf3x1(), elemHalf3x2(), elemHalf3x3(), elemHalf3x4(), elemHalf4x1(), elemHalf4x2(), elemHalf4x3(), elemHalf4x4(), elemFixed(), elemFixed2(), elemFixed3(), elemFixed4(), elemFixed2x1(), elemFixed2x2(), elemFixed2x3(), elemFixed2x4(), elemFixed3x1(), elemFixed3x2(), elemFixed3x3(), elemFixed3x4(), elemFixed4x1(), elemFixed4x2(), elemFixed4x3(), elemFixed4x4(), elemSampler1D(), elemSampler2D(), elemSampler3D(), elemSamplerRECT(), elemSamplerCUBE(), elemSamplerDEPTH(), elemString(), elemEnum(), elemArray(), elemUsertype() {}
	/**
	 * Destructor
	 */
	virtual ~domCg_param() { daeElement::deleteCMDataArray(_CMData); }
	/**
	 * Overloaded assignment operator
	 */
	virtual domCg_param &operator=( const domCg_param &cpy ) { (void)cpy; return *this; }

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
