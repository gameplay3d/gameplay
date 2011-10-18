#ifndef __domGles2_value_h__
#define __domGles2_value_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domFx_sampler2D.h>
#include <dom/domFx_sampler3D.h>
#include <dom/domFx_samplerCUBE.h>
#include <dom/domFx_samplerDEPTH.h>
#include <dom/domGles2_value.h>
class DAE;

/**
 * New parameters may choose to type themselves by value, providing a default.
 */
class domGles2_value : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::GLES2_VALUE; }
	static daeInt ID() { return 727; }
	virtual daeInt typeID() const { return ID(); }
public:
	class domBool;

	typedef daeSmartRef<domBool> domBoolRef;
	typedef daeTArray<domBoolRef> domBool_Array;

	class domBool : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::BOOL; }
		static daeInt ID() { return 728; }
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

	class domBvec2;

	typedef daeSmartRef<domBvec2> domBvec2Ref;
	typedef daeTArray<domBvec2Ref> domBvec2_Array;

	class domBvec2 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::BVEC2; }
		static daeInt ID() { return 729; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a domBool2 of the value.
		 */
		domBool2& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const domBool2& val ) { _value = val; }

	protected:  // Value
		/**
		 * The domBool2 value of the text data of this element. 
		 */
		domBool2 _value;
	protected:
		/**
		 * Constructor
		 */
		domBvec2(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domBvec2() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domBvec2 &operator=( const domBvec2 &cpy ) { (void)cpy; return *this; }

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

	class domBvec3;

	typedef daeSmartRef<domBvec3> domBvec3Ref;
	typedef daeTArray<domBvec3Ref> domBvec3_Array;

	class domBvec3 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::BVEC3; }
		static daeInt ID() { return 730; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a domBool3 of the value.
		 */
		domBool3& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const domBool3& val ) { _value = val; }

	protected:  // Value
		/**
		 * The domBool3 value of the text data of this element. 
		 */
		domBool3 _value;
	protected:
		/**
		 * Constructor
		 */
		domBvec3(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domBvec3() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domBvec3 &operator=( const domBvec3 &cpy ) { (void)cpy; return *this; }

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

	class domBvec4;

	typedef daeSmartRef<domBvec4> domBvec4Ref;
	typedef daeTArray<domBvec4Ref> domBvec4_Array;

	class domBvec4 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::BVEC4; }
		static daeInt ID() { return 731; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a domBool4 of the value.
		 */
		domBool4& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const domBool4& val ) { _value = val; }

	protected:  // Value
		/**
		 * The domBool4 value of the text data of this element. 
		 */
		domBool4 _value;
	protected:
		/**
		 * Constructor
		 */
		domBvec4(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domBvec4() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domBvec4 &operator=( const domBvec4 &cpy ) { (void)cpy; return *this; }

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
		static daeInt ID() { return 732; }
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

	class domVec2;

	typedef daeSmartRef<domVec2> domVec2Ref;
	typedef daeTArray<domVec2Ref> domVec2_Array;

	class domVec2 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::VEC2; }
		static daeInt ID() { return 733; }
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
		domVec2(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domVec2() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domVec2 &operator=( const domVec2 &cpy ) { (void)cpy; return *this; }

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

	class domVec3;

	typedef daeSmartRef<domVec3> domVec3Ref;
	typedef daeTArray<domVec3Ref> domVec3_Array;

	class domVec3 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::VEC3; }
		static daeInt ID() { return 734; }
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
		domVec3(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domVec3() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domVec3 &operator=( const domVec3 &cpy ) { (void)cpy; return *this; }

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

	class domVec4;

	typedef daeSmartRef<domVec4> domVec4Ref;
	typedef daeTArray<domVec4Ref> domVec4_Array;

	class domVec4 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::VEC4; }
		static daeInt ID() { return 735; }
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
		domVec4(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domVec4() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domVec4 &operator=( const domVec4 &cpy ) { (void)cpy; return *this; }

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

	class domMat2;

	typedef daeSmartRef<domMat2> domMat2Ref;
	typedef daeTArray<domMat2Ref> domMat2_Array;

	class domMat2 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::MAT2; }
		static daeInt ID() { return 736; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a domFloat2x2 of the value.
		 */
		domFloat2x2& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const domFloat2x2& val ) { _value = val; }

	protected:  // Value
		/**
		 * The domFloat2x2 value of the text data of this element. 
		 */
		domFloat2x2 _value;
	protected:
		/**
		 * Constructor
		 */
		domMat2(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domMat2() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domMat2 &operator=( const domMat2 &cpy ) { (void)cpy; return *this; }

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

	class domMat3;

	typedef daeSmartRef<domMat3> domMat3Ref;
	typedef daeTArray<domMat3Ref> domMat3_Array;

	class domMat3 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::MAT3; }
		static daeInt ID() { return 737; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a domFloat3x3 of the value.
		 */
		domFloat3x3& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const domFloat3x3& val ) { _value = val; }

	protected:  // Value
		/**
		 * The domFloat3x3 value of the text data of this element. 
		 */
		domFloat3x3 _value;
	protected:
		/**
		 * Constructor
		 */
		domMat3(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domMat3() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domMat3 &operator=( const domMat3 &cpy ) { (void)cpy; return *this; }

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

	class domMat4;

	typedef daeSmartRef<domMat4> domMat4Ref;
	typedef daeTArray<domMat4Ref> domMat4_Array;

	class domMat4 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::MAT4; }
		static daeInt ID() { return 738; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a domFloat4x4 of the value.
		 */
		domFloat4x4& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const domFloat4x4& val ) { _value = val; }

	protected:  // Value
		/**
		 * The domFloat4x4 value of the text data of this element. 
		 */
		domFloat4x4 _value;
	protected:
		/**
		 * Constructor
		 */
		domMat4(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domMat4() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domMat4 &operator=( const domMat4 &cpy ) { (void)cpy; return *this; }

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
		static daeInt ID() { return 739; }
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

	class domIvec2;

	typedef daeSmartRef<domIvec2> domIvec2Ref;
	typedef daeTArray<domIvec2Ref> domIvec2_Array;

	class domIvec2 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::IVEC2; }
		static daeInt ID() { return 740; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a domInt2 of the value.
		 */
		domInt2& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const domInt2& val ) { _value = val; }

	protected:  // Value
		/**
		 * The domInt2 value of the text data of this element. 
		 */
		domInt2 _value;
	protected:
		/**
		 * Constructor
		 */
		domIvec2(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domIvec2() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domIvec2 &operator=( const domIvec2 &cpy ) { (void)cpy; return *this; }

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

	class domIvec3;

	typedef daeSmartRef<domIvec3> domIvec3Ref;
	typedef daeTArray<domIvec3Ref> domIvec3_Array;

	class domIvec3 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::IVEC3; }
		static daeInt ID() { return 741; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a domInt3 of the value.
		 */
		domInt3& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const domInt3& val ) { _value = val; }

	protected:  // Value
		/**
		 * The domInt3 value of the text data of this element. 
		 */
		domInt3 _value;
	protected:
		/**
		 * Constructor
		 */
		domIvec3(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domIvec3() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domIvec3 &operator=( const domIvec3 &cpy ) { (void)cpy; return *this; }

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

	class domIvec4;

	typedef daeSmartRef<domIvec4> domIvec4Ref;
	typedef daeTArray<domIvec4Ref> domIvec4_Array;

	class domIvec4 : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::IVEC4; }
		static daeInt ID() { return 742; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a domInt4 of the value.
		 */
		domInt4& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const domInt4& val ) { _value = val; }

	protected:  // Value
		/**
		 * The domInt4 value of the text data of this element. 
		 */
		domInt4 _value;
	protected:
		/**
		 * Constructor
		 */
		domIvec4(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domIvec4() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domIvec4 &operator=( const domIvec4 &cpy ) { (void)cpy; return *this; }

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
		static daeInt ID() { return 743; }
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

	class domUsertype;

	typedef daeSmartRef<domUsertype> domUsertypeRef;
	typedef daeTArray<domUsertypeRef> domUsertype_Array;

	class domUsertype : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::USERTYPE; }
		static daeInt ID() { return 744; }
		virtual daeInt typeID() const { return ID(); }
	public:
		class domSetparam;

		typedef daeSmartRef<domSetparam> domSetparamRef;
		typedef daeTArray<domSetparamRef> domSetparam_Array;

/**
 * Use a series of these to set the members by name.  The ref attribute will
 * be relative to the struct you are in right now.
 */
		class domSetparam : public daeElement
		{
		public:
			virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::SETPARAM; }
			static daeInt ID() { return 745; }
			virtual daeInt typeID() const { return ID(); }
		protected:  // Attribute
/**
 * Symbol name of the member
 */
			xsToken attrRef;

		protected:  // Element
			domGles2_value_Array elemGles2_value_array;

		public:	//Accessors and Mutators
			/**
			 * Gets the ref attribute.
			 * @return Returns a xsToken of the ref attribute.
			 */
			xsToken getRef() const { return attrRef; }
			/**
			 * Sets the ref attribute.
			 * @param atRef The new value for the ref attribute.
			 */
			void setRef( xsToken atRef ) { *(daeStringRef*)&attrRef = atRef; _validAttributeArray[0] = true; }

			/**
			 * Gets the gles2_value element array.
			 * @return Returns a reference to the array of gles2_value elements.
			 */
			domGles2_value_Array &getGles2_value_array() { return elemGles2_value_array; }
			/**
			 * Gets the gles2_value element array.
			 * @return Returns a constant reference to the array of gles2_value elements.
			 */
			const domGles2_value_Array &getGles2_value_array() const { return elemGles2_value_array; }
		protected:
			/**
			 * Constructor
			 */
			domSetparam(DAE& dae) : daeElement(dae), attrRef(), elemGles2_value_array() {}
			/**
			 * Destructor
			 */
			virtual ~domSetparam() {}
			/**
			 * Overloaded assignment operator
			 */
			virtual domSetparam &operator=( const domSetparam &cpy ) { (void)cpy; return *this; }

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


	protected:  // Attribute
/**
 * Symbol for type or structure in shader code.   This aids in type-checking,
 * resolution of bindings, and support interfaces
 */
		xsToken attrTypename;

	protected:  // Element
/**
 * Use a series of these to set the members by name.  The ref attribute will
 * be relative to the struct you are in right now. @see domSetparam
 */
		domSetparam_Array elemSetparam_array;

	public:	//Accessors and Mutators
		/**
		 * Gets the typename attribute.
		 * @return Returns a xsToken of the typename attribute.
		 */
		xsToken getTypename() const { return attrTypename; }
		/**
		 * Sets the typename attribute.
		 * @param atTypename The new value for the typename attribute.
		 */
		void setTypename( xsToken atTypename ) { *(daeStringRef*)&attrTypename = atTypename; _validAttributeArray[0] = true; }

		/**
		 * Gets the setparam element array.
		 * @return Returns a reference to the array of setparam elements.
		 */
		domSetparam_Array &getSetparam_array() { return elemSetparam_array; }
		/**
		 * Gets the setparam element array.
		 * @return Returns a constant reference to the array of setparam elements.
		 */
		const domSetparam_Array &getSetparam_array() const { return elemSetparam_array; }
	protected:
		/**
		 * Constructor
		 */
		domUsertype(DAE& dae) : daeElement(dae), attrTypename(), elemSetparam_array() {}
		/**
		 * Destructor
		 */
		virtual ~domUsertype() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domUsertype &operator=( const domUsertype &cpy ) { (void)cpy; return *this; }

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

	class domArray;

	typedef daeSmartRef<domArray> domArrayRef;
	typedef daeTArray<domArrayRef> domArray_Array;

	class domArray : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::ARRAY; }
		static daeInt ID() { return 746; }
		virtual daeInt typeID() const { return ID(); }
	protected:  // Attribute
/**
 * Enable applications to validate element count without shader having been
 * compiled
 */
		xsPositiveInteger attrLength;

	protected:  // Element
		domGles2_value_Array elemGles2_value_array;

	public:	//Accessors and Mutators
		/**
		 * Gets the length attribute.
		 * @return Returns a xsPositiveInteger of the length attribute.
		 */
		xsPositiveInteger getLength() const { return attrLength; }
		/**
		 * Sets the length attribute.
		 * @param atLength The new value for the length attribute.
		 */
		void setLength( xsPositiveInteger atLength ) { attrLength = atLength; _validAttributeArray[0] = true; }

		/**
		 * Gets the gles2_value element array.
		 * @return Returns a reference to the array of gles2_value elements.
		 */
		domGles2_value_Array &getGles2_value_array() { return elemGles2_value_array; }
		/**
		 * Gets the gles2_value element array.
		 * @return Returns a constant reference to the array of gles2_value elements.
		 */
		const domGles2_value_Array &getGles2_value_array() const { return elemGles2_value_array; }
	protected:
		/**
		 * Constructor
		 */
		domArray(DAE& dae) : daeElement(dae), attrLength(), elemGles2_value_array() {}
		/**
		 * Destructor
		 */
		virtual ~domArray() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domArray &operator=( const domArray &cpy ) { (void)cpy; return *this; }

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
	domBvec2Ref elemBvec2;
	domBvec3Ref elemBvec3;
	domBvec4Ref elemBvec4;
	domFloatRef elemFloat;
	domVec2Ref elemVec2;
	domVec3Ref elemVec3;
	domVec4Ref elemVec4;
	domMat2Ref elemMat2;
	domMat3Ref elemMat3;
	domMat4Ref elemMat4;
	domIntRef elemInt;
	domIvec2Ref elemIvec2;
	domIvec3Ref elemIvec3;
	domIvec4Ref elemIvec4;
	domEnumRef elemEnum;
	domFx_sampler2DRef elemSampler2D;
/**
 * not in baseline spec but probably available in extensions @see domSampler3D
 */
	domFx_sampler3DRef elemSampler3D;
	domFx_samplerCUBERef elemSamplerCUBE;
/**
 * not in baseline spec but probably available in extensions @see domSamplerDEPTH
 */
	domFx_samplerDEPTHRef elemSamplerDEPTH;
	domUsertypeRef elemUsertype;
	domArrayRef elemArray;
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
	 * Gets the bvec2 element.
	 * @return a daeSmartRef to the bvec2 element.
	 */
	const domBvec2Ref getBvec2() const { return elemBvec2; }
	/**
	 * Gets the bvec3 element.
	 * @return a daeSmartRef to the bvec3 element.
	 */
	const domBvec3Ref getBvec3() const { return elemBvec3; }
	/**
	 * Gets the bvec4 element.
	 * @return a daeSmartRef to the bvec4 element.
	 */
	const domBvec4Ref getBvec4() const { return elemBvec4; }
	/**
	 * Gets the float element.
	 * @return a daeSmartRef to the float element.
	 */
	const domFloatRef getFloat() const { return elemFloat; }
	/**
	 * Gets the vec2 element.
	 * @return a daeSmartRef to the vec2 element.
	 */
	const domVec2Ref getVec2() const { return elemVec2; }
	/**
	 * Gets the vec3 element.
	 * @return a daeSmartRef to the vec3 element.
	 */
	const domVec3Ref getVec3() const { return elemVec3; }
	/**
	 * Gets the vec4 element.
	 * @return a daeSmartRef to the vec4 element.
	 */
	const domVec4Ref getVec4() const { return elemVec4; }
	/**
	 * Gets the mat2 element.
	 * @return a daeSmartRef to the mat2 element.
	 */
	const domMat2Ref getMat2() const { return elemMat2; }
	/**
	 * Gets the mat3 element.
	 * @return a daeSmartRef to the mat3 element.
	 */
	const domMat3Ref getMat3() const { return elemMat3; }
	/**
	 * Gets the mat4 element.
	 * @return a daeSmartRef to the mat4 element.
	 */
	const domMat4Ref getMat4() const { return elemMat4; }
	/**
	 * Gets the int element.
	 * @return a daeSmartRef to the int element.
	 */
	const domIntRef getInt() const { return elemInt; }
	/**
	 * Gets the ivec2 element.
	 * @return a daeSmartRef to the ivec2 element.
	 */
	const domIvec2Ref getIvec2() const { return elemIvec2; }
	/**
	 * Gets the ivec3 element.
	 * @return a daeSmartRef to the ivec3 element.
	 */
	const domIvec3Ref getIvec3() const { return elemIvec3; }
	/**
	 * Gets the ivec4 element.
	 * @return a daeSmartRef to the ivec4 element.
	 */
	const domIvec4Ref getIvec4() const { return elemIvec4; }
	/**
	 * Gets the enum element.
	 * @return a daeSmartRef to the enum element.
	 */
	const domEnumRef getEnum() const { return elemEnum; }
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
	 * Gets the usertype element.
	 * @return a daeSmartRef to the usertype element.
	 */
	const domUsertypeRef getUsertype() const { return elemUsertype; }
	/**
	 * Gets the array element.
	 * @return a daeSmartRef to the array element.
	 */
	const domArrayRef getArray() const { return elemArray; }
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
	domGles2_value(DAE& dae) : daeElement(dae), elemBool(), elemBvec2(), elemBvec3(), elemBvec4(), elemFloat(), elemVec2(), elemVec3(), elemVec4(), elemMat2(), elemMat3(), elemMat4(), elemInt(), elemIvec2(), elemIvec3(), elemIvec4(), elemEnum(), elemSampler2D(), elemSampler3D(), elemSamplerCUBE(), elemSamplerDEPTH(), elemUsertype(), elemArray() {}
	/**
	 * Destructor
	 */
	virtual ~domGles2_value() { daeElement::deleteCMDataArray(_CMData); }
	/**
	 * Overloaded assignment operator
	 */
	virtual domGles2_value &operator=( const domGles2_value &cpy ) { (void)cpy; return *this; }

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
