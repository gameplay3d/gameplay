#ifndef __domLink_h__
#define __domLink_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domRotate.h>
#include <dom/domTranslate.h>
#include <dom/domLink.h>
class DAE;

class domLink : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::LINK; }
	static daeInt ID() { return 370; }
	virtual daeInt typeID() const { return ID(); }
public:
	class domAttachment_full;

	typedef daeSmartRef<domAttachment_full> domAttachment_fullRef;
	typedef daeTArray<domAttachment_fullRef> domAttachment_full_Array;

	class domAttachment_full : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::ATTACHMENT_FULL; }
		static daeInt ID() { return 371; }
		virtual daeInt typeID() const { return ID(); }
	protected:  // Attribute
		xsToken attrJoint;

	protected:  // Elements
		domRotate_Array elemRotate_array;
		domTranslate_Array elemTranslate_array;
		domLinkRef elemLink;
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
		 * Gets the joint attribute.
		 * @return Returns a xsToken of the joint attribute.
		 */
		xsToken getJoint() const { return attrJoint; }
		/**
		 * Sets the joint attribute.
		 * @param atJoint The new value for the joint attribute.
		 */
		void setJoint( xsToken atJoint ) { *(daeStringRef*)&attrJoint = atJoint; _validAttributeArray[0] = true; }

		/**
		 * Gets the rotate element array.
		 * @return Returns a reference to the array of rotate elements.
		 */
		domRotate_Array &getRotate_array() { return elemRotate_array; }
		/**
		 * Gets the rotate element array.
		 * @return Returns a constant reference to the array of rotate elements.
		 */
		const domRotate_Array &getRotate_array() const { return elemRotate_array; }
		/**
		 * Gets the translate element array.
		 * @return Returns a reference to the array of translate elements.
		 */
		domTranslate_Array &getTranslate_array() { return elemTranslate_array; }
		/**
		 * Gets the translate element array.
		 * @return Returns a constant reference to the array of translate elements.
		 */
		const domTranslate_Array &getTranslate_array() const { return elemTranslate_array; }
		/**
		 * Gets the link element.
		 * @return a daeSmartRef to the link element.
		 */
		const domLinkRef getLink() const { return elemLink; }
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
		domAttachment_full(DAE& dae) : daeElement(dae), attrJoint(), elemRotate_array(), elemTranslate_array(), elemLink() {}
		/**
		 * Destructor
		 */
		virtual ~domAttachment_full() { daeElement::deleteCMDataArray(_CMData); }
		/**
		 * Overloaded assignment operator
		 */
		virtual domAttachment_full &operator=( const domAttachment_full &cpy ) { (void)cpy; return *this; }

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

	class domAttachment_start;

	typedef daeSmartRef<domAttachment_start> domAttachment_startRef;
	typedef daeTArray<domAttachment_startRef> domAttachment_start_Array;

	class domAttachment_start : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::ATTACHMENT_START; }
		static daeInt ID() { return 372; }
		virtual daeInt typeID() const { return ID(); }
	protected:  // Attribute
		xsToken attrJoint;

	protected:  // Elements
		domRotate_Array elemRotate_array;
		domTranslate_Array elemTranslate_array;
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
		 * Gets the joint attribute.
		 * @return Returns a xsToken of the joint attribute.
		 */
		xsToken getJoint() const { return attrJoint; }
		/**
		 * Sets the joint attribute.
		 * @param atJoint The new value for the joint attribute.
		 */
		void setJoint( xsToken atJoint ) { *(daeStringRef*)&attrJoint = atJoint; _validAttributeArray[0] = true; }

		/**
		 * Gets the rotate element array.
		 * @return Returns a reference to the array of rotate elements.
		 */
		domRotate_Array &getRotate_array() { return elemRotate_array; }
		/**
		 * Gets the rotate element array.
		 * @return Returns a constant reference to the array of rotate elements.
		 */
		const domRotate_Array &getRotate_array() const { return elemRotate_array; }
		/**
		 * Gets the translate element array.
		 * @return Returns a reference to the array of translate elements.
		 */
		domTranslate_Array &getTranslate_array() { return elemTranslate_array; }
		/**
		 * Gets the translate element array.
		 * @return Returns a constant reference to the array of translate elements.
		 */
		const domTranslate_Array &getTranslate_array() const { return elemTranslate_array; }
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
		domAttachment_start(DAE& dae) : daeElement(dae), attrJoint(), elemRotate_array(), elemTranslate_array() {}
		/**
		 * Destructor
		 */
		virtual ~domAttachment_start() { daeElement::deleteCMDataArray(_CMData); }
		/**
		 * Overloaded assignment operator
		 */
		virtual domAttachment_start &operator=( const domAttachment_start &cpy ) { (void)cpy; return *this; }

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

	class domAttachment_end;

	typedef daeSmartRef<domAttachment_end> domAttachment_endRef;
	typedef daeTArray<domAttachment_endRef> domAttachment_end_Array;

	class domAttachment_end : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::ATTACHMENT_END; }
		static daeInt ID() { return 373; }
		virtual daeInt typeID() const { return ID(); }
	protected:  // Attribute
		xsToken attrJoint;

	protected:  // Elements
		domRotate_Array elemRotate_array;
		domTranslate_Array elemTranslate_array;
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
		 * Gets the joint attribute.
		 * @return Returns a xsToken of the joint attribute.
		 */
		xsToken getJoint() const { return attrJoint; }
		/**
		 * Sets the joint attribute.
		 * @param atJoint The new value for the joint attribute.
		 */
		void setJoint( xsToken atJoint ) { *(daeStringRef*)&attrJoint = atJoint; _validAttributeArray[0] = true; }

		/**
		 * Gets the rotate element array.
		 * @return Returns a reference to the array of rotate elements.
		 */
		domRotate_Array &getRotate_array() { return elemRotate_array; }
		/**
		 * Gets the rotate element array.
		 * @return Returns a constant reference to the array of rotate elements.
		 */
		const domRotate_Array &getRotate_array() const { return elemRotate_array; }
		/**
		 * Gets the translate element array.
		 * @return Returns a reference to the array of translate elements.
		 */
		domTranslate_Array &getTranslate_array() { return elemTranslate_array; }
		/**
		 * Gets the translate element array.
		 * @return Returns a constant reference to the array of translate elements.
		 */
		const domTranslate_Array &getTranslate_array() const { return elemTranslate_array; }
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
		domAttachment_end(DAE& dae) : daeElement(dae), attrJoint(), elemRotate_array(), elemTranslate_array() {}
		/**
		 * Destructor
		 */
		virtual ~domAttachment_end() { daeElement::deleteCMDataArray(_CMData); }
		/**
		 * Overloaded assignment operator
		 */
		virtual domAttachment_end &operator=( const domAttachment_end &cpy ) { (void)cpy; return *this; }

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
	domSid attrSid;
	xsToken attrName;

protected:  // Elements
	domRotate_Array elemRotate_array;
	domTranslate_Array elemTranslate_array;
	domAttachment_full_Array elemAttachment_full_array;
	domAttachment_start_Array elemAttachment_start_array;
	domAttachment_end_Array elemAttachment_end_array;
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
	 * Gets the sid attribute.
	 * @return Returns a domSid of the sid attribute.
	 */
	domSid getSid() const { return attrSid; }
	/**
	 * Sets the sid attribute.
	 * @param atSid The new value for the sid attribute.
	 */
	void setSid( domSid atSid ) { *(daeStringRef*)&attrSid = atSid;}

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
	 * Gets the rotate element array.
	 * @return Returns a reference to the array of rotate elements.
	 */
	domRotate_Array &getRotate_array() { return elemRotate_array; }
	/**
	 * Gets the rotate element array.
	 * @return Returns a constant reference to the array of rotate elements.
	 */
	const domRotate_Array &getRotate_array() const { return elemRotate_array; }
	/**
	 * Gets the translate element array.
	 * @return Returns a reference to the array of translate elements.
	 */
	domTranslate_Array &getTranslate_array() { return elemTranslate_array; }
	/**
	 * Gets the translate element array.
	 * @return Returns a constant reference to the array of translate elements.
	 */
	const domTranslate_Array &getTranslate_array() const { return elemTranslate_array; }
	/**
	 * Gets the attachment_full element array.
	 * @return Returns a reference to the array of attachment_full elements.
	 */
	domAttachment_full_Array &getAttachment_full_array() { return elemAttachment_full_array; }
	/**
	 * Gets the attachment_full element array.
	 * @return Returns a constant reference to the array of attachment_full elements.
	 */
	const domAttachment_full_Array &getAttachment_full_array() const { return elemAttachment_full_array; }
	/**
	 * Gets the attachment_start element array.
	 * @return Returns a reference to the array of attachment_start elements.
	 */
	domAttachment_start_Array &getAttachment_start_array() { return elemAttachment_start_array; }
	/**
	 * Gets the attachment_start element array.
	 * @return Returns a constant reference to the array of attachment_start elements.
	 */
	const domAttachment_start_Array &getAttachment_start_array() const { return elemAttachment_start_array; }
	/**
	 * Gets the attachment_end element array.
	 * @return Returns a reference to the array of attachment_end elements.
	 */
	domAttachment_end_Array &getAttachment_end_array() { return elemAttachment_end_array; }
	/**
	 * Gets the attachment_end element array.
	 * @return Returns a constant reference to the array of attachment_end elements.
	 */
	const domAttachment_end_Array &getAttachment_end_array() const { return elemAttachment_end_array; }
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
	domLink(DAE& dae) : daeElement(dae), attrSid(), attrName(), elemRotate_array(), elemTranslate_array(), elemAttachment_full_array(), elemAttachment_start_array(), elemAttachment_end_array() {}
	/**
	 * Destructor
	 */
	virtual ~domLink() { daeElement::deleteCMDataArray(_CMData); }
	/**
	 * Overloaded assignment operator
	 */
	virtual domLink &operator=( const domLink &cpy ) { (void)cpy; return *this; }

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
