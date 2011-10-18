/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 

#ifndef __domFx_samplerDEPTH_common_h__
#define __domFx_samplerDEPTH_common_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domExtra.h>
class DAE;

/**
 * A texture sampler for depth maps.
 */
class domFx_samplerDEPTH_common_complexType 
{
public:
	class domSource;

	typedef daeSmartRef<domSource> domSourceRef;
	typedef daeTArray<domSourceRef> domSource_Array;

	class domSource : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::SOURCE; }
		static daeInt ID() { return 79; }
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
		static daeInt ID() { return 80; }
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
		static daeInt ID() { return 81; }
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
		static daeInt ID() { return 82; }
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
		static daeInt ID() { return 83; }
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



protected:  // Elements
	domSourceRef elemSource;
	domWrap_sRef elemWrap_s;
	domWrap_tRef elemWrap_t;
	domMinfilterRef elemMinfilter;
	domMagfilterRef elemMagfilter;
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
	domFx_samplerDEPTH_common_complexType(DAE& dae, daeElement* elt) : elemSource(), elemWrap_s(), elemWrap_t(), elemMinfilter(), elemMagfilter(), elemExtra_array() {}
	/**
	 * Destructor
	 */
	virtual ~domFx_samplerDEPTH_common_complexType() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domFx_samplerDEPTH_common_complexType &operator=( const domFx_samplerDEPTH_common_complexType &cpy ) { (void)cpy; return *this; }
};

/**
 * An element of type domFx_samplerDEPTH_common_complexType.
 */
class domFx_samplerDEPTH_common : public daeElement, public domFx_samplerDEPTH_common_complexType
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FX_SAMPLERDEPTH_COMMON; }
	static daeInt ID() { return 84; }
	virtual daeInt typeID() const { return ID(); }
protected:
	/**
	 * Constructor
	 */
	domFx_samplerDEPTH_common(DAE& dae) : daeElement(dae), domFx_samplerDEPTH_common_complexType(dae, this) {}
	/**
	 * Destructor
	 */
	virtual ~domFx_samplerDEPTH_common() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domFx_samplerDEPTH_common &operator=( const domFx_samplerDEPTH_common &cpy ) { (void)cpy; return *this; }

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
