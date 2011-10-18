/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 

#ifndef __domSkin_h__
#define __domSkin_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domSource.h>
#include <dom/domExtra.h>
#include <dom/domInputLocal.h>
#include <dom/domInputLocalOffset.h>
class DAE;

/**
 * The skin element contains vertex and primitive information sufficient to
 * describe blend-weight skinning.
 */
class domSkin : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::SKIN; }
	static daeInt ID() { return 656; }
	virtual daeInt typeID() const { return ID(); }
public:
	class domBind_shape_matrix;

	typedef daeSmartRef<domBind_shape_matrix> domBind_shape_matrixRef;
	typedef daeTArray<domBind_shape_matrixRef> domBind_shape_matrix_Array;

/**
 * This provides extra information about the position and orientation of the
 * base mesh before binding.  If bind_shape_matrix is not specified then an
 * identity matrix may be used as the bind_shape_matrix. The bind_shape_matrix
 * element may occur zero or one times.
 */
	class domBind_shape_matrix : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::BIND_SHAPE_MATRIX; }
		static daeInt ID() { return 657; }
		virtual daeInt typeID() const { return ID(); }

	protected:  // Value
		/**
		 * The domFloat4x4 value of the text data of this element. 
		 */
		domFloat4x4 _value;

	public:	//Accessors and Mutators
		/**
		 * Gets the _value array.
		 * @return Returns a domFloat4x4 reference of the _value array.
		 */
		domFloat4x4 &getValue() { return _value; }
		/**
		 * Gets the _value array.
		 * @return Returns a constant domFloat4x4 reference of the _value array.
		 */
		const domFloat4x4 &getValue() const { return _value; }
		/**
		 * Sets the _value array.
		 * @param val The new value for the _value array.
		 */
		void setValue( const domFloat4x4 &val ) { _value = val; }

	protected:
		/**
		 * Constructor
		 */
		domBind_shape_matrix(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domBind_shape_matrix() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domBind_shape_matrix &operator=( const domBind_shape_matrix &cpy ) { (void)cpy; return *this; }

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

	class domJoints;

	typedef daeSmartRef<domJoints> domJointsRef;
	typedef daeTArray<domJointsRef> domJoints_Array;

/**
 * The joints element associates joint, or skeleton, nodes with attribute
 * data.   In COLLADA, this is specified by the inverse bind matrix of each
 * joint (influence) in the skeleton.
 */
	class domJoints : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::JOINTS; }
		static daeInt ID() { return 658; }
		virtual daeInt typeID() const { return ID(); }

	protected:  // Elements
/**
 * The input element must occur at least twice. These inputs are local inputs.
 * @see domInput
 */
		domInputLocal_Array elemInput_array;
/**
 *  The extra element may appear any number of times.  @see domExtra
 */
		domExtra_Array elemExtra_array;

	public:	//Accessors and Mutators
		/**
		 * Gets the input element array.
		 * @return Returns a reference to the array of input elements.
		 */
		domInputLocal_Array &getInput_array() { return elemInput_array; }
		/**
		 * Gets the input element array.
		 * @return Returns a constant reference to the array of input elements.
		 */
		const domInputLocal_Array &getInput_array() const { return elemInput_array; }
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
		domJoints(DAE& dae) : daeElement(dae), elemInput_array(), elemExtra_array() {}
		/**
		 * Destructor
		 */
		virtual ~domJoints() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domJoints &operator=( const domJoints &cpy ) { (void)cpy; return *this; }

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

	class domVertex_weights;

	typedef daeSmartRef<domVertex_weights> domVertex_weightsRef;
	typedef daeTArray<domVertex_weightsRef> domVertex_weights_Array;

/**
 * The vertex_weights element associates a set of joint-weight pairs with
 * each vertex in the base mesh.
 */
	class domVertex_weights : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::VERTEX_WEIGHTS; }
		static daeInt ID() { return 659; }
		virtual daeInt typeID() const { return ID(); }
	public:
		class domVcount;

		typedef daeSmartRef<domVcount> domVcountRef;
		typedef daeTArray<domVcountRef> domVcount_Array;

/**
 * The vcount element contains a list of integers describing the number of
 * influences for each vertex. The vcount element may occur once.
 */
		class domVcount : public daeElement
		{
		public:
			virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::VCOUNT; }
			static daeInt ID() { return 660; }
			virtual daeInt typeID() const { return ID(); }

		protected:  // Value
			/**
			 * The domListOfUInts value of the text data of this element. 
			 */
			domListOfUInts _value;

		public:	//Accessors and Mutators
			/**
			 * Gets the _value array.
			 * @return Returns a domListOfUInts reference of the _value array.
			 */
			domListOfUInts &getValue() { return _value; }
			/**
			 * Gets the _value array.
			 * @return Returns a constant domListOfUInts reference of the _value array.
			 */
			const domListOfUInts &getValue() const { return _value; }
			/**
			 * Sets the _value array.
			 * @param val The new value for the _value array.
			 */
			void setValue( const domListOfUInts &val ) { _value = val; }

		protected:
			/**
			 * Constructor
			 */
			domVcount(DAE& dae) : daeElement(dae), _value() {}
			/**
			 * Destructor
			 */
			virtual ~domVcount() {}
			/**
			 * Overloaded assignment operator
			 */
			virtual domVcount &operator=( const domVcount &cpy ) { (void)cpy; return *this; }

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

		class domV;

		typedef daeSmartRef<domV> domVRef;
		typedef daeTArray<domVRef> domV_Array;

/**
 * The v element describes which bones and attributes are associated with
 * each vertex.  An index  of –1 into the array of joints refers to the
 * bind shape.  Weights should be normalized before use. The v element must
 * occur zero or one times.
 */
		class domV : public daeElement
		{
		public:
			virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::V; }
			static daeInt ID() { return 661; }
			virtual daeInt typeID() const { return ID(); }

		protected:  // Value
			/**
			 * The domListOfInts value of the text data of this element. 
			 */
			domListOfInts _value;

		public:	//Accessors and Mutators
			/**
			 * Gets the _value array.
			 * @return Returns a domListOfInts reference of the _value array.
			 */
			domListOfInts &getValue() { return _value; }
			/**
			 * Gets the _value array.
			 * @return Returns a constant domListOfInts reference of the _value array.
			 */
			const domListOfInts &getValue() const { return _value; }
			/**
			 * Sets the _value array.
			 * @param val The new value for the _value array.
			 */
			void setValue( const domListOfInts &val ) { _value = val; }

		protected:
			/**
			 * Constructor
			 */
			domV(DAE& dae) : daeElement(dae), _value() {}
			/**
			 * Destructor
			 */
			virtual ~domV() {}
			/**
			 * Overloaded assignment operator
			 */
			virtual domV &operator=( const domV &cpy ) { (void)cpy; return *this; }

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
 *  The count attribute describes the number of vertices in the base mesh.
 * Required element.  
 */
		domUint attrCount;

	protected:  // Elements
/**
 * The input element must occur at least twice. @see domInput
 */
		domInputLocalOffset_Array elemInput_array;
/**
 * The vcount element contains a list of integers describing the number of
 * influences for each vertex. The vcount element may occur once. @see domVcount
 */
		domVcountRef elemVcount;
/**
 * The v element describes which bones and attributes are associated with
 * each vertex.  An index  of –1 into the array of joints refers to the
 * bind shape.  Weights should be normalized before use. The v element must
 * occur zero or one times. @see domV
 */
		domVRef elemV;
/**
 *  The extra element may appear any number of times.  @see domExtra
 */
		domExtra_Array elemExtra_array;

	public:	//Accessors and Mutators
		/**
		 * Gets the count attribute.
		 * @return Returns a domUint of the count attribute.
		 */
		domUint getCount() const { return attrCount; }
		/**
		 * Sets the count attribute.
		 * @param atCount The new value for the count attribute.
		 */
		void setCount( domUint atCount ) { attrCount = atCount; _validAttributeArray[0] = true; }

		/**
		 * Gets the input element array.
		 * @return Returns a reference to the array of input elements.
		 */
		domInputLocalOffset_Array &getInput_array() { return elemInput_array; }
		/**
		 * Gets the input element array.
		 * @return Returns a constant reference to the array of input elements.
		 */
		const domInputLocalOffset_Array &getInput_array() const { return elemInput_array; }
		/**
		 * Gets the vcount element.
		 * @return a daeSmartRef to the vcount element.
		 */
		const domVcountRef getVcount() const { return elemVcount; }
		/**
		 * Gets the v element.
		 * @return a daeSmartRef to the v element.
		 */
		const domVRef getV() const { return elemV; }
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
		domVertex_weights(DAE& dae) : daeElement(dae), attrCount(), elemInput_array(), elemVcount(), elemV(), elemExtra_array() {}
		/**
		 * Destructor
		 */
		virtual ~domVertex_weights() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domVertex_weights &operator=( const domVertex_weights &cpy ) { (void)cpy; return *this; }

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
 *  The source attribute contains a URI reference to the base mesh, (a static
 * mesh or a morphed mesh). This also provides the bind-shape of the skinned
 * mesh.  Required attribute. 
 */
	xsAnyURI attrSource;

protected:  // Elements
/**
 * This provides extra information about the position and orientation of the
 * base mesh before binding.  If bind_shape_matrix is not specified then an
 * identity matrix may be used as the bind_shape_matrix. The bind_shape_matrix
 * element may occur zero or one times. @see domBind_shape_matrix
 */
	domBind_shape_matrixRef elemBind_shape_matrix;
/**
 *  The skin element must contain at least three source elements.  @see domSource
 */
	domSource_Array elemSource_array;
/**
 * The joints element associates joint, or skeleton, nodes with attribute
 * data.   In COLLADA, this is specified by the inverse bind matrix of each
 * joint (influence) in the skeleton. @see domJoints
 */
	domJointsRef elemJoints;
/**
 * The vertex_weights element associates a set of joint-weight pairs with
 * each vertex in the base mesh. @see domVertex_weights
 */
	domVertex_weightsRef elemVertex_weights;
/**
 *  The extra element may appear any number of times.  @see domExtra
 */
	domExtra_Array elemExtra_array;

public:	//Accessors and Mutators
	/**
	 * Gets the source attribute.
	 * @return Returns a xsAnyURI reference of the source attribute.
	 */
	xsAnyURI &getSource() { return attrSource; }
	/**
	 * Gets the source attribute.
	 * @return Returns a constant xsAnyURI reference of the source attribute.
	 */
	const xsAnyURI &getSource() const { return attrSource; }
	/**
	 * Sets the source attribute.
	 * @param atSource The new value for the source attribute.
	 */
	void setSource( const xsAnyURI &atSource ) { attrSource = atSource; _validAttributeArray[0] = true; }
	/**
	 * Sets the source attribute.
	 * @param atSource The new value for the source attribute.
	 */
	void setSource( xsString atSource ) { attrSource = atSource; _validAttributeArray[0] = true; }

	/**
	 * Gets the bind_shape_matrix element.
	 * @return a daeSmartRef to the bind_shape_matrix element.
	 */
	const domBind_shape_matrixRef getBind_shape_matrix() const { return elemBind_shape_matrix; }
	/**
	 * Gets the source element array.
	 * @return Returns a reference to the array of source elements.
	 */
	domSource_Array &getSource_array() { return elemSource_array; }
	/**
	 * Gets the source element array.
	 * @return Returns a constant reference to the array of source elements.
	 */
	const domSource_Array &getSource_array() const { return elemSource_array; }
	/**
	 * Gets the joints element.
	 * @return a daeSmartRef to the joints element.
	 */
	const domJointsRef getJoints() const { return elemJoints; }
	/**
	 * Gets the vertex_weights element.
	 * @return a daeSmartRef to the vertex_weights element.
	 */
	const domVertex_weightsRef getVertex_weights() const { return elemVertex_weights; }
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
	domSkin(DAE& dae) : daeElement(dae), attrSource(dae, *this), elemBind_shape_matrix(), elemSource_array(), elemJoints(), elemVertex_weights(), elemExtra_array() {}
	/**
	 * Destructor
	 */
	virtual ~domSkin() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domSkin &operator=( const domSkin &cpy ) { (void)cpy; return *this; }

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
