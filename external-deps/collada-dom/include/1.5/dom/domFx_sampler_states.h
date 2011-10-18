#ifndef __domFx_sampler_states_h__
#define __domFx_sampler_states_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domExtra.h>
class DAE;

class domFx_sampler_states : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FX_SAMPLER_STATES; }
	static daeInt ID() { return 488; }
	virtual daeInt typeID() const { return ID(); }
public:
	class domWrap_s;

	typedef daeSmartRef<domWrap_s> domWrap_sRef;
	typedef daeTArray<domWrap_sRef> domWrap_s_Array;

	class domWrap_s : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::WRAP_S; }
		static daeInt ID() { return 489; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a domFx_sampler_wrap of the value.
		 */
		domFx_sampler_wrap& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const domFx_sampler_wrap& val ) { _value = val; }

	protected:  // Value
		/**
		 * The domFx_sampler_wrap value of the text data of this element. 
		 */
		domFx_sampler_wrap _value;
	protected:
		/**
		 * Constructor
		 */
		domWrap_s(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domWrap_s() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domWrap_s &operator=( const domWrap_s &cpy ) { (void)cpy; return *this; }

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

	class domWrap_t;

	typedef daeSmartRef<domWrap_t> domWrap_tRef;
	typedef daeTArray<domWrap_tRef> domWrap_t_Array;

	class domWrap_t : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::WRAP_T; }
		static daeInt ID() { return 490; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a domFx_sampler_wrap of the value.
		 */
		domFx_sampler_wrap& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const domFx_sampler_wrap& val ) { _value = val; }

	protected:  // Value
		/**
		 * The domFx_sampler_wrap value of the text data of this element. 
		 */
		domFx_sampler_wrap _value;
	protected:
		/**
		 * Constructor
		 */
		domWrap_t(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domWrap_t() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domWrap_t &operator=( const domWrap_t &cpy ) { (void)cpy; return *this; }

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

	class domWrap_p;

	typedef daeSmartRef<domWrap_p> domWrap_pRef;
	typedef daeTArray<domWrap_pRef> domWrap_p_Array;

	class domWrap_p : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::WRAP_P; }
		static daeInt ID() { return 491; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a domFx_sampler_wrap of the value.
		 */
		domFx_sampler_wrap& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const domFx_sampler_wrap& val ) { _value = val; }

	protected:  // Value
		/**
		 * The domFx_sampler_wrap value of the text data of this element. 
		 */
		domFx_sampler_wrap _value;
	protected:
		/**
		 * Constructor
		 */
		domWrap_p(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domWrap_p() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domWrap_p &operator=( const domWrap_p &cpy ) { (void)cpy; return *this; }

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

	class domMinfilter;

	typedef daeSmartRef<domMinfilter> domMinfilterRef;
	typedef daeTArray<domMinfilterRef> domMinfilter_Array;

/**
 * minification
 */
	class domMinfilter : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::MINFILTER; }
		static daeInt ID() { return 492; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a domFx_sampler_min_filter of the value.
		 */
		domFx_sampler_min_filter& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const domFx_sampler_min_filter& val ) { _value = val; }

	protected:  // Value
		/**
		 * The domFx_sampler_min_filter value of the text data of this element. 
		 */
		domFx_sampler_min_filter _value;
	protected:
		/**
		 * Constructor
		 */
		domMinfilter(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domMinfilter() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domMinfilter &operator=( const domMinfilter &cpy ) { (void)cpy; return *this; }

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

	class domMagfilter;

	typedef daeSmartRef<domMagfilter> domMagfilterRef;
	typedef daeTArray<domMagfilterRef> domMagfilter_Array;

/**
 * magnification
 */
	class domMagfilter : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::MAGFILTER; }
		static daeInt ID() { return 493; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a domFx_sampler_mag_filter of the value.
		 */
		domFx_sampler_mag_filter& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const domFx_sampler_mag_filter& val ) { _value = val; }

	protected:  // Value
		/**
		 * The domFx_sampler_mag_filter value of the text data of this element. 
		 */
		domFx_sampler_mag_filter _value;
	protected:
		/**
		 * Constructor
		 */
		domMagfilter(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domMagfilter() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domMagfilter &operator=( const domMagfilter &cpy ) { (void)cpy; return *this; }

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

	class domMipfilter;

	typedef daeSmartRef<domMipfilter> domMipfilterRef;
	typedef daeTArray<domMipfilterRef> domMipfilter_Array;

/**
 * filtering across mips. NONE = no minification NEAREST = bilinear LINEAR
 * = trilinear ANISOTROPIC =  Compensates for distortion caused by the difference
 * in angle between the polygon and the plane of the screen.  Relies on max_anisotropy
 */
	class domMipfilter : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::MIPFILTER; }
		static daeInt ID() { return 494; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a domFx_sampler_mip_filter of the value.
		 */
		domFx_sampler_mip_filter& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const domFx_sampler_mip_filter& val ) { _value = val; }

	protected:  // Value
		/**
		 * The domFx_sampler_mip_filter value of the text data of this element. 
		 */
		domFx_sampler_mip_filter _value;
	protected:
		/**
		 * Constructor
		 */
		domMipfilter(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domMipfilter() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domMipfilter &operator=( const domMipfilter &cpy ) { (void)cpy; return *this; }

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

	class domBorder_color;

	typedef daeSmartRef<domBorder_color> domBorder_colorRef;
	typedef daeTArray<domBorder_colorRef> domBorder_color_Array;

	class domBorder_color : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::BORDER_COLOR; }
		static daeInt ID() { return 495; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a domFx_color of the value.
		 */
		domFx_color& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const domFx_color& val ) { _value = val; }

	protected:  // Value
		/**
		 * The domFx_color value of the text data of this element. 
		 */
		domFx_color _value;
	protected:
		/**
		 * Constructor
		 */
		domBorder_color(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domBorder_color() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domBorder_color &operator=( const domBorder_color &cpy ) { (void)cpy; return *this; }

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

	class domMip_max_level;

	typedef daeSmartRef<domMip_max_level> domMip_max_levelRef;
	typedef daeTArray<domMip_max_levelRef> domMip_max_level_Array;

/**
 * level-of-detail index of largest map to use. Values range from 0 to (n
 * - 1) where 0 is the largest. The default value is zero
 */
	class domMip_max_level : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::MIP_MAX_LEVEL; }
		static daeInt ID() { return 496; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a xsUnsignedByte of the value.
		 */
		xsUnsignedByte& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const xsUnsignedByte& val ) { _value = val; }

	protected:  // Value
		/**
		 * The xsUnsignedByte value of the text data of this element. 
		 */
		xsUnsignedByte _value;
	protected:
		/**
		 * Constructor
		 */
		domMip_max_level(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domMip_max_level() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domMip_max_level &operator=( const domMip_max_level &cpy ) { (void)cpy; return *this; }

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

	class domMip_min_level;

	typedef daeSmartRef<domMip_min_level> domMip_min_levelRef;
	typedef daeTArray<domMip_min_levelRef> domMip_min_level_Array;

/**
 * level-of-detail index of smallest map to use. Values range from 0 to (n
 * - 1) where 0 is the smallest. The default value is zero
 */
	class domMip_min_level : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::MIP_MIN_LEVEL; }
		static daeInt ID() { return 497; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a xsUnsignedByte of the value.
		 */
		xsUnsignedByte& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const xsUnsignedByte& val ) { _value = val; }

	protected:  // Value
		/**
		 * The xsUnsignedByte value of the text data of this element. 
		 */
		xsUnsignedByte _value;
	protected:
		/**
		 * Constructor
		 */
		domMip_min_level(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domMip_min_level() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domMip_min_level &operator=( const domMip_min_level &cpy ) { (void)cpy; return *this; }

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

	class domMip_bias;

	typedef daeSmartRef<domMip_bias> domMip_biasRef;
	typedef daeTArray<domMip_biasRef> domMip_bias_Array;

/**
 * Mipmap level-of-detail bias. The default value is zero
 */
	class domMip_bias : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::MIP_BIAS; }
		static daeInt ID() { return 498; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a xsFloat of the value.
		 */
		xsFloat& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const xsFloat& val ) { _value = val; }

	protected:  // Value
		/**
		 * The xsFloat value of the text data of this element. 
		 */
		xsFloat _value;
	protected:
		/**
		 * Constructor
		 */
		domMip_bias(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domMip_bias() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domMip_bias &operator=( const domMip_bias &cpy ) { (void)cpy; return *this; }

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

	class domMax_anisotropy;

	typedef daeSmartRef<domMax_anisotropy> domMax_anisotropyRef;
	typedef daeTArray<domMax_anisotropyRef> domMax_anisotropy_Array;

/**
 * number of samples that may be used durring anisotropic filtering
 */
	class domMax_anisotropy : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::MAX_ANISOTROPY; }
		static daeInt ID() { return 499; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a xsUnsignedInt of the value.
		 */
		xsUnsignedInt& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const xsUnsignedInt& val ) { _value = val; }

	protected:  // Value
		/**
		 * The xsUnsignedInt value of the text data of this element. 
		 */
		xsUnsignedInt _value;
	protected:
		/**
		 * Constructor
		 */
		domMax_anisotropy(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domMax_anisotropy() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domMax_anisotropy &operator=( const domMax_anisotropy &cpy ) { (void)cpy; return *this; }

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
	domWrap_sRef elemWrap_s;
	domWrap_tRef elemWrap_t;
	domWrap_pRef elemWrap_p;
/**
 * minification @see domMinfilter
 */
	domMinfilterRef elemMinfilter;
/**
 * magnification @see domMagfilter
 */
	domMagfilterRef elemMagfilter;
/**
 * filtering across mips. NONE = no minification NEAREST = bilinear LINEAR
 * = trilinear ANISOTROPIC =  Compensates for distortion caused by the difference
 * in angle between the polygon and the plane of the screen.  Relies on max_anisotropy
 * @see domMipfilter
 */
	domMipfilterRef elemMipfilter;
	domBorder_colorRef elemBorder_color;
/**
 * level-of-detail index of largest map to use. Values range from 0 to (n
 * - 1) where 0 is the largest. The default value is zero @see domMip_max_level
 */
	domMip_max_levelRef elemMip_max_level;
/**
 * level-of-detail index of smallest map to use. Values range from 0 to (n
 * - 1) where 0 is the smallest. The default value is zero @see domMip_min_level
 */
	domMip_min_levelRef elemMip_min_level;
/**
 * Mipmap level-of-detail bias. The default value is zero @see domMip_bias
 */
	domMip_biasRef elemMip_bias;
/**
 * number of samples that may be used durring anisotropic filtering @see domMax_anisotropy
 */
	domMax_anisotropyRef elemMax_anisotropy;
	domExtra_Array elemExtra_array;

public:	//Accessors and Mutators
	/**
	 * Gets the wrap_s element.
	 * @return a daeSmartRef to the wrap_s element.
	 */
	const domWrap_sRef getWrap_s() const { return elemWrap_s; }
	/**
	 * Gets the wrap_t element.
	 * @return a daeSmartRef to the wrap_t element.
	 */
	const domWrap_tRef getWrap_t() const { return elemWrap_t; }
	/**
	 * Gets the wrap_p element.
	 * @return a daeSmartRef to the wrap_p element.
	 */
	const domWrap_pRef getWrap_p() const { return elemWrap_p; }
	/**
	 * Gets the minfilter element.
	 * @return a daeSmartRef to the minfilter element.
	 */
	const domMinfilterRef getMinfilter() const { return elemMinfilter; }
	/**
	 * Gets the magfilter element.
	 * @return a daeSmartRef to the magfilter element.
	 */
	const domMagfilterRef getMagfilter() const { return elemMagfilter; }
	/**
	 * Gets the mipfilter element.
	 * @return a daeSmartRef to the mipfilter element.
	 */
	const domMipfilterRef getMipfilter() const { return elemMipfilter; }
	/**
	 * Gets the border_color element.
	 * @return a daeSmartRef to the border_color element.
	 */
	const domBorder_colorRef getBorder_color() const { return elemBorder_color; }
	/**
	 * Gets the mip_max_level element.
	 * @return a daeSmartRef to the mip_max_level element.
	 */
	const domMip_max_levelRef getMip_max_level() const { return elemMip_max_level; }
	/**
	 * Gets the mip_min_level element.
	 * @return a daeSmartRef to the mip_min_level element.
	 */
	const domMip_min_levelRef getMip_min_level() const { return elemMip_min_level; }
	/**
	 * Gets the mip_bias element.
	 * @return a daeSmartRef to the mip_bias element.
	 */
	const domMip_biasRef getMip_bias() const { return elemMip_bias; }
	/**
	 * Gets the max_anisotropy element.
	 * @return a daeSmartRef to the max_anisotropy element.
	 */
	const domMax_anisotropyRef getMax_anisotropy() const { return elemMax_anisotropy; }
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
	domFx_sampler_states(DAE& dae) : daeElement(dae), elemWrap_s(), elemWrap_t(), elemWrap_p(), elemMinfilter(), elemMagfilter(), elemMipfilter(), elemBorder_color(), elemMip_max_level(), elemMip_min_level(), elemMip_bias(), elemMax_anisotropy(), elemExtra_array() {}
	/**
	 * Destructor
	 */
	virtual ~domFx_sampler_states() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domFx_sampler_states &operator=( const domFx_sampler_states &cpy ) { (void)cpy; return *this; }

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
