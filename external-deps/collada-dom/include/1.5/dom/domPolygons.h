#ifndef __domPolygons_h__
#define __domPolygons_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domInput_local_offset.h>
#include <dom/domP.h>
#include <dom/domExtra.h>
class DAE;

/**
 * The polygons element provides the information needed to bind vertex attributes
 * together and  then organize those vertices into individual polygons. The
 * polygons described can contain  arbitrary numbers of vertices. These polygons
 * may be self intersecting and may also contain holes.
 */
class domPolygons : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::POLYGONS; }
	static daeInt ID() { return 29; }
	virtual daeInt typeID() const { return ID(); }
public:
	class domPh;

	typedef daeSmartRef<domPh> domPhRef;
	typedef daeTArray<domPhRef> domPh_Array;

/**
 * The ph element may occur any number of times. It describes a polygon with
 * holes.
 */
	class domPh : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::PH; }
		static daeInt ID() { return 30; }
		virtual daeInt typeID() const { return ID(); }
	public:
		class domH;

		typedef daeSmartRef<domH> domHRef;
		typedef daeTArray<domHRef> domH_Array;

/**
 * The h element represents a hole in the polygon specified. There must be
 * at least one h element.
 */
		class domH : public daeElement
		{
		public:
			virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::H; }
			static daeInt ID() { return 31; }
			virtual daeInt typeID() const { return ID(); }


		public:	//Accessors and Mutators
			/**
			 * Gets the value of this element.
			 * @return a domList_of_uints of the value.
			 */
			domList_of_uints& getValue() { return _value; }
			/**
			 * Sets the _value of this element.
			 * @param val The new value for this element.
			 */
			void setValue( const domList_of_uints& val ) { _value = val; }

		protected:  // Value
			/**
			 * The domList_of_uints value of the text data of this element. 
			 */
			domList_of_uints _value;
		protected:
			/**
			 * Constructor
			 */
			domH(DAE& dae) : daeElement(dae), _value() {}
			/**
			 * Destructor
			 */
			virtual ~domH() {}
			/**
			 * Overloaded assignment operator
			 */
			virtual domH &operator=( const domH &cpy ) { (void)cpy; return *this; }

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
 * There may only be one p element. @see domP
 */
		domPRef elemP;
/**
 * The h element represents a hole in the polygon specified. There must be
 * at least one h element. @see domH
 */
		domH_Array elemH_array;

	public:	//Accessors and Mutators
		/**
		 * Gets the p element.
		 * @return a daeSmartRef to the p element.
		 */
		const domPRef getP() const { return elemP; }
		/**
		 * Gets the h element array.
		 * @return Returns a reference to the array of h elements.
		 */
		domH_Array &getH_array() { return elemH_array; }
		/**
		 * Gets the h element array.
		 * @return Returns a constant reference to the array of h elements.
		 */
		const domH_Array &getH_array() const { return elemH_array; }
	protected:
		/**
		 * Constructor
		 */
		domPh(DAE& dae) : daeElement(dae), elemP(), elemH_array() {}
		/**
		 * Destructor
		 */
		virtual ~domPh() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domPh &operator=( const domPh &cpy ) { (void)cpy; return *this; }

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


protected:  // Attributes
/**
 *  The name attribute is the text string name of this element. Optional attribute.
 */
	xsToken attrName;
/**
 *  The count attribute indicates the number of polygon primitives. Required
 * attribute. 
 */
	domUint attrCount;
/**
 *  The material attribute declares a symbol for a material. This symbol is
 * bound to a material  at the time of instantiation. If the material attribute
 * is not specified then the lighting  and shading results are application
 * defined. Optional attribute.  
 */
	xsNCName attrMaterial;

protected:  // Elements
/**
 * The input element may occur any number of times. This input is a local
 * input with the  offset and set attributes. @see domInput
 */
	domInput_local_offset_Array elemInput_array;
/**
 * The p element may occur any number of times. @see domP
 */
	domP_Array elemP_array;
/**
 * The ph element may occur any number of times. It describes a polygon with
 * holes. @see domPh
 */
	domPh_Array elemPh_array;
/**
 * The extra element may appear any number of times. @see domExtra
 */
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
	 * Gets the name attribute.
	 * @return Returns a xsToken of the name attribute.
	 */
	xsToken getName() const { return attrName; }
	/**
	 * Sets the name attribute.
	 * @param atName The new value for the name attribute.
	 */
	void setName( xsToken atName ) { *(daeStringRef*)&attrName = atName;}

	/**
	 * Gets the count attribute.
	 * @return Returns a domUint of the count attribute.
	 */
	domUint getCount() const { return attrCount; }
	/**
	 * Sets the count attribute.
	 * @param atCount The new value for the count attribute.
	 */
	void setCount( domUint atCount ) { attrCount = atCount; }

	/**
	 * Gets the material attribute.
	 * @return Returns a xsNCName of the material attribute.
	 */
	xsNCName getMaterial() const { return attrMaterial; }
	/**
	 * Sets the material attribute.
	 * @param atMaterial The new value for the material attribute.
	 */
	void setMaterial( xsNCName atMaterial ) { *(daeStringRef*)&attrMaterial = atMaterial;}

	/**
	 * Gets the input element array.
	 * @return Returns a reference to the array of input elements.
	 */
	domInput_local_offset_Array &getInput_array() { return elemInput_array; }
	/**
	 * Gets the input element array.
	 * @return Returns a constant reference to the array of input elements.
	 */
	const domInput_local_offset_Array &getInput_array() const { return elemInput_array; }
	/**
	 * Gets the p element array.
	 * @return Returns a reference to the array of p elements.
	 */
	domP_Array &getP_array() { return elemP_array; }
	/**
	 * Gets the p element array.
	 * @return Returns a constant reference to the array of p elements.
	 */
	const domP_Array &getP_array() const { return elemP_array; }
	/**
	 * Gets the ph element array.
	 * @return Returns a reference to the array of ph elements.
	 */
	domPh_Array &getPh_array() { return elemPh_array; }
	/**
	 * Gets the ph element array.
	 * @return Returns a constant reference to the array of ph elements.
	 */
	const domPh_Array &getPh_array() const { return elemPh_array; }
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
	domPolygons(DAE& dae) : daeElement(dae), attrName(), attrCount(), attrMaterial(), elemInput_array(), elemP_array(), elemPh_array(), elemExtra_array() {}
	/**
	 * Destructor
	 */
	virtual ~domPolygons() { daeElement::deleteCMDataArray(_CMData); }
	/**
	 * Overloaded assignment operator
	 */
	virtual domPolygons &operator=( const domPolygons &cpy ) { (void)cpy; return *this; }

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
