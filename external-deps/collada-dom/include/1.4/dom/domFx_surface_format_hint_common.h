/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 

#ifndef __domFx_surface_format_hint_common_h__
#define __domFx_surface_format_hint_common_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domExtra.h>
class DAE;

/**
 * If the exact format cannot be resolve via other methods then the format_hint
 * will describe the important features of the format so that the application
 * may select a compatable or close format
 */
class domFx_surface_format_hint_common_complexType 
{
public:
	class domChannels;

	typedef daeSmartRef<domChannels> domChannelsRef;
	typedef daeTArray<domChannelsRef> domChannels_Array;

/**
 * The per-texel layout of the format.  The length of the string indicate
 * how many channels there are and the letter respresents the name of the
 * channel.  There are typically 0 to 4 channels.
 */
	class domChannels : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::CHANNELS; }
		static daeInt ID() { return 6; }
		virtual daeInt typeID() const { return ID(); }

	protected:  // Value
		/**
		 * The domFx_surface_format_hint_channels_enum value of the text data of this element. 
		 */
		domFx_surface_format_hint_channels_enum _value;

	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a domFx_surface_format_hint_channels_enum of the value.
		 */
		domFx_surface_format_hint_channels_enum getValue() const { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( domFx_surface_format_hint_channels_enum val ) { _value = val; }

	protected:
		/**
		 * Constructor
		 */
		domChannels(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domChannels() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domChannels &operator=( const domChannels &cpy ) { (void)cpy; return *this; }

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

	class domRange;

	typedef daeSmartRef<domRange> domRangeRef;
	typedef daeTArray<domRangeRef> domRange_Array;

/**
 * Each channel represents a range of values. Some example ranges are signed
 * or unsigned integers, or between between a clamped range such as 0.0f to
 * 1.0f, or high dynamic range via floating point
 */
	class domRange : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::RANGE; }
		static daeInt ID() { return 7; }
		virtual daeInt typeID() const { return ID(); }

	protected:  // Value
		/**
		 * The domFx_surface_format_hint_range_enum value of the text data of this element. 
		 */
		domFx_surface_format_hint_range_enum _value;

	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a domFx_surface_format_hint_range_enum of the value.
		 */
		domFx_surface_format_hint_range_enum getValue() const { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( domFx_surface_format_hint_range_enum val ) { _value = val; }

	protected:
		/**
		 * Constructor
		 */
		domRange(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domRange() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domRange &operator=( const domRange &cpy ) { (void)cpy; return *this; }

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

	class domPrecision;

	typedef daeSmartRef<domPrecision> domPrecisionRef;
	typedef daeTArray<domPrecisionRef> domPrecision_Array;

/**
 * Each channel of the texel has a precision.  Typically these are all linked
 * together.  An exact format lay lower the precision of an individual channel
 * but applying a higher precision by linking the channels together may still
 * convey the same information.
 */
	class domPrecision : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::PRECISION; }
		static daeInt ID() { return 8; }
		virtual daeInt typeID() const { return ID(); }

	protected:  // Value
		/**
		 * The domFx_surface_format_hint_precision_enum value of the text data of this element. 
		 */
		domFx_surface_format_hint_precision_enum _value;

	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a domFx_surface_format_hint_precision_enum of the value.
		 */
		domFx_surface_format_hint_precision_enum getValue() const { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( domFx_surface_format_hint_precision_enum val ) { _value = val; }

	protected:
		/**
		 * Constructor
		 */
		domPrecision(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domPrecision() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domPrecision &operator=( const domPrecision &cpy ) { (void)cpy; return *this; }

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

	class domOption;

	typedef daeSmartRef<domOption> domOptionRef;
	typedef daeTArray<domOptionRef> domOption_Array;

/**
 * Additional hints about data relationships and other things to help the
 * application pick the best format.
 */
	class domOption : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::OPTION; }
		static daeInt ID() { return 9; }
		virtual daeInt typeID() const { return ID(); }

	protected:  // Value
		/**
		 * The domFx_surface_format_hint_option_enum value of the text data of this element. 
		 */
		domFx_surface_format_hint_option_enum _value;

	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a domFx_surface_format_hint_option_enum of the value.
		 */
		domFx_surface_format_hint_option_enum getValue() const { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( domFx_surface_format_hint_option_enum val ) { _value = val; }

	protected:
		/**
		 * Constructor
		 */
		domOption(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domOption() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domOption &operator=( const domOption &cpy ) { (void)cpy; return *this; }

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
 * The per-texel layout of the format.  The length of the string indicate
 * how many channels there are and the letter respresents the name of the
 * channel.  There are typically 0 to 4 channels. @see domChannels
 */
	domChannelsRef elemChannels;
/**
 * Each channel represents a range of values. Some example ranges are signed
 * or unsigned integers, or between between a clamped range such as 0.0f to
 * 1.0f, or high dynamic range via floating point @see domRange
 */
	domRangeRef elemRange;
/**
 * Each channel of the texel has a precision.  Typically these are all linked
 * together.  An exact format lay lower the precision of an individual channel
 * but applying a higher precision by linking the channels together may still
 * convey the same information. @see domPrecision
 */
	domPrecisionRef elemPrecision;
/**
 * Additional hints about data relationships and other things to help the
 * application pick the best format. @see domOption
 */
	domOption_Array elemOption_array;
	domExtra_Array elemExtra_array;

public:	//Accessors and Mutators
	/**
	 * Gets the channels element.
	 * @return a daeSmartRef to the channels element.
	 */
	const domChannelsRef getChannels() const { return elemChannels; }
	/**
	 * Gets the range element.
	 * @return a daeSmartRef to the range element.
	 */
	const domRangeRef getRange() const { return elemRange; }
	/**
	 * Gets the precision element.
	 * @return a daeSmartRef to the precision element.
	 */
	const domPrecisionRef getPrecision() const { return elemPrecision; }
	/**
	 * Gets the option element array.
	 * @return Returns a reference to the array of option elements.
	 */
	domOption_Array &getOption_array() { return elemOption_array; }
	/**
	 * Gets the option element array.
	 * @return Returns a constant reference to the array of option elements.
	 */
	const domOption_Array &getOption_array() const { return elemOption_array; }
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
	domFx_surface_format_hint_common_complexType(DAE& dae, daeElement* elt) : elemChannels(), elemRange(), elemPrecision(), elemOption_array(), elemExtra_array() {}
	/**
	 * Destructor
	 */
	virtual ~domFx_surface_format_hint_common_complexType() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domFx_surface_format_hint_common_complexType &operator=( const domFx_surface_format_hint_common_complexType &cpy ) { (void)cpy; return *this; }
};

/**
 * An element of type domFx_surface_format_hint_common_complexType.
 */
class domFx_surface_format_hint_common : public daeElement, public domFx_surface_format_hint_common_complexType
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::FX_SURFACE_FORMAT_HINT_COMMON; }
	static daeInt ID() { return 10; }
	virtual daeInt typeID() const { return ID(); }
protected:
	/**
	 * Constructor
	 */
	domFx_surface_format_hint_common(DAE& dae) : daeElement(dae), domFx_surface_format_hint_common_complexType(dae, this) {}
	/**
	 * Destructor
	 */
	virtual ~domFx_surface_format_hint_common() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domFx_surface_format_hint_common &operator=( const domFx_surface_format_hint_common &cpy ) { (void)cpy; return *this; }

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
