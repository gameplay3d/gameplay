/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 

#ifndef __domFx_sampler2D_common_h__
#define __domFx_sampler2D_common_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domExtra.h>
class DAE;

/**
 * A two-dimensional texture sampler.
 */
class domFx_sampler2D_common_complexType 
{
public:
	class domSource;

	typedef daeSmartRef<domSource> domSourceRef;
	typedef daeTArray<domSourceRef> domSource_Array;

	class domSource : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::SOURCE; }
		static daeInt ID() { return 37; }
		virtual daeInt typeID() const { return ID(); }

	protected:  // Value
		/**
		 * The xsNCName value of the text data of this element. 
		 */
		xsNCName _value;

	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return Returns a xsNCName of the value.
		 */
		xsNCName getValue() const { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( xsNCName val ) { *(daeStringRef*)&_value = val; }

	protected:
		/**
		 * Constructor
		 */
		domSource(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domSource() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domSource &operator=( const domSource &cpy ) { (void)cpy; return *this; }

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

	class domWrap_s;

	typedef daeSmartRef<domWrap_s> domWrap_sRef;
	typedef daeTArray<domWrap_sRef> domWrap_s_Array;

	class domWrap_s : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::WRAP_S; }
		static daeInt ID() { return 38; }
		virtual daeInt typeID() const { return ID(); }

	protected:  // Value
		/**
		 * The domFx_sampler_wrap_common value of the text data of this element. 
		 */
		domFx_sampler_wrap_common _value;

	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a domFx_sampler_wrap_common of the value.
		 */
		domFx_sampler_wrap_common getValue() const { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( domFx_sampler_wrap_common val ) { _value = val; }

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
		static daeInt ID() { return 39; }
		virtual daeInt typeID() const { return ID(); }

	protected:  // Value
		/**
		 * The domFx_sampler_wrap_common value of the text data of this element. 
		 */
		domFx_sampler_wrap_common _value;

	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a domFx_sampler_wrap_common of the value.
		 */
		domFx_sampler_wrap_common getValue() const { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( domFx_sampler_wrap_common val ) { _value = val; }

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

	class domMinfilter;

	typedef daeSmartRef<domMinfilter> domMinfilterRef;
	typedef daeTArray<domMinfilterRef> domMinfilter_Array;

	class domMinfilter : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::MINFILTER; }
		static daeInt ID() { return 40; }
		virtual daeInt typeID() const { return ID(); }

	protected:  // Value
		/**
		 * The domFx_sampler_filter_common value of the text data of this element. 
		 */
		domFx_sampler_filter_common _value;

	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a domFx_sampler_filter_common of the value.
		 */
		domFx_sampler_filter_common getValue() const { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( domFx_sampler_filter_common val ) { _value = val; }

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

	class domMagfilter : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::MAGFILTER; }
		static daeInt ID() { return 41; }
		virtual daeInt typeID() const { return ID(); }

	protected:  // Value
		/**
		 * The domFx_sampler_filter_common value of the text data of this element. 
		 */
		domFx_sampler_filter_common _value;

	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a domFx_sampler_filter_common of the value.
		 */
		domFx_sampler_filter_common getValue() const { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( domFx_sampler_filter_common val ) { _value = val; }

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

	class domMipfilter : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::MIPFILTER; }
		static daeInt ID() { return 42; }
		virtual daeInt typeID() const { return ID(); }

	protected:  // Value
		/**
		 * The domFx_sampler_filter_common value of the text data of this element. 
		 */
		domFx_sampler_filter_common _value;

	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a domFx_sampler_filter_common of the value.
		 */
		domFx_sampler_filter_common getValue() const { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( domFx_sampler_filter_common val ) { _value = val; }

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
		static daeInt ID() { return 43; }
		virtual daeInt typeID() const { return ID(); }

	protected:  // Value
		/**
		 * The domFx_color_common value of the text data of this element. 
		 */
		domFx_color_common _value;

	public:	//Accessors and Mutators
		/**
		 * Gets the _value array.
		 * @return Returns a domFx_color_common reference of the _value array.
		 */
		domFx_color_common &getValue() { return _value; }
		/**
		 * Gets the _value array.
		 * @return Returns a constant domFx_color_common reference of the _value array.
		 */
		const domFx_color_common &getValue() const { return _value; }
		/**
		 * Sets the _value array.
		 * @param val The new value for the _value array.
		 */
		void setValue( const domFx_color_common &val ) { _value = val; }

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

	class domMipmap_maxlevel;

	typedef daeSmartRef<domMipmap_maxlevel> domMipmap_maxlevelRef;
	typedef daeTArray<domMipmap_maxlevelRef> domMipmap_maxlevel_Array;

	class domMipmap_maxlevel : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::MIPMAP_MAXLEVEL; }
		static daeInt ID() { return 44; }
		virtual daeInt typeID() const { return ID(); }

	protected:  // Value
		/**
		 * The xsUnsignedByte value of the text data of this element. 
		 */
		xsUnsignedByte _value;

	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a xsUnsignedByte of the value.
		 */
		xsUnsignedByte getValue() const { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( xsUnsignedByte val ) { _value = val; }

	protected:
		/**
		 * Constructor
		 */
		domMipmap_maxlevel(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domMipmap_maxlevel() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domMipmap_maxlevel &operator=( const domMipmap_maxlevel &cpy ) { (void)cpy; return *this; }

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

	class domMipmap_bias;

	typedef daeSmartRef<domMipmap_bias> domMipmap_biasRef;
	typedef daeTArray<domMipmap_biasRef> domMipmap_bias_Array;

	class domMipmap_bias : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::MIPMAP_BIAS; }
		static daeInt ID() { return 45; }
		virtual daeInt typeID() const { return ID(); }

	protected:  // Value
		/**
		 * The xsFloat value of the text data of this element. 
		 */
		xsFloat _value;

	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a xsFloat of the value.
		 */
		xsFloat getValue() const { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( xsFloat val ) { _value = val; }

	protected:
		/**
		 * Constructor
		 */
		domMipmap_bias(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domMipmap_bias() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domMipmap_bias &operator=( const domMipmap_bias &cpy ) { (void)cpy; return *this; }

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
	domSourceRef elemSource;
	domWrap_sRef elemWrap_s;
	domWrap_tRef elemWrap_t;
	domMinfilterRef elemMinfilter;
	domMagfilterRef elemMagfilter;
	domMipfilterRef elemMipfilter;
	domBorder_colorRef elemBorder_color;
	domMipmap_maxlevelRef elemMipmap_maxlevel;
	domMipmap_biasRef elemMipmap_bias;
	domExtra_Array elemExtra_array;

public:	//Accessors and Mutators
	/**
	 * Gets the source element.
	 * @return a daeSmartRef to the source element.
	 */
	const domSourceRef getSource() const { return elemSource; }
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
	 * Gets the mipmap_maxlevel element.
	 * @return a daeSmartRef to the mipmap_maxlevel element.
	 */
	const domMipmap_maxlevelRef getMipmap_maxlevel() const { return elemMipmap_maxlevel; }
	/**
	 * Gets the mipmap_bias element.
	 * @return a daeSmartRef to the mipmap_bias element.
	 */
	const domMipmap_biasRef getMipmap_bias() const { return elemMipmap_bias; }
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
	domFx_sampler2D_common_complexType(DAE& dae, daeElement* elt) : elemSource(), elemWrap_s(), elemWrap_t(), elemMinfilter(), elemMagfilter(), elemMipfilter(), elemBorder_color(), elemMipmap_maxlevel(), elemMipmap_bias(), elemExtra_array() {}
	/**
	 * Destructor
	 */
	virtual ~domFx_sampler2D_common_complexType() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domFx_sampler2D_common_complexType &operator=( const domFx_sampler2D_common_complexType &cpy ) { (void)cpy; return *this; }
};

/**
 * An element of type domFx_sampler2D_common_complexType.
 */
class domFx_sampler2D_common : public daeElement, public domFx_sampler2D_common_complexType
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FX_SAMPLER2D_COMMON; }
	static daeInt ID() { return 46; }
	virtual daeInt typeID() const { return ID(); }
protected:
	/**
	 * Constructor
	 */
	domFx_sampler2D_common(DAE& dae) : daeElement(dae), domFx_sampler2D_common_complexType(dae, this) {}
	/**
	 * Destructor
	 */
	virtual ~domFx_sampler2D_common() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domFx_sampler2D_common &operator=( const domFx_sampler2D_common &cpy ) { (void)cpy; return *this; }

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
