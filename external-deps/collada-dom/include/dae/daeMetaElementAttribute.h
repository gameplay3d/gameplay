/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 

#ifndef __DAE_META_ELEMENT_ATTRIBUTE_H__
#define __DAE_META_ELEMENT_ATTRIBUTE_H__

#include <dae/daeTypes.h>
#include <dae/daeMetaAttribute.h>
#include <dae/daeMetaCMPolicy.h>

class daeMetaElement;
class daeElement;
class daeDocument;

/**
* The @c daeMetaElementAttribute class represents a content model object that is an element.
*/
class daeMetaElementAttribute : public daeMetaAttribute, public daeMetaCMPolicy
{
public:
	/** The metaElement that describes the element type of this attribute */
	daeMetaElement* 		_elementType;

public:
	/**
	 * Constructor.
	 * @param container The daeMetaElement that this policy object belongs to.
	 * @param parent The daeMetaCMPolicy parent of this policy object.
	 * @param odinal The ordinal value offset of this specific policy object. Used for maintaining the 
	 * correct order of child elements.
	 * @param minO The minimum number of times this CMPolicy object must appear. This value comes from the COLLADA schema.
	 * @param maxO The maximum number of times this CMPolicy object may appear. This value comes from the COLLADA schema.
	 */
	daeMetaElementAttribute( daeMetaElement *container, daeMetaCMPolicy *parent = NULL, daeUInt ordinal = 0, daeInt minO = 1, daeInt maxO = 1);
	/**
	 * Destructor
	 */
	virtual ~daeMetaElementAttribute();

public:

	virtual daeElement *placeElement(daeElement* parent, daeElement* child, daeUInt &ordinal, daeInt offset = 0, daeElement* before = NULL, daeElement *after = NULL);
	virtual daeBool removeElement(daeElement* parent, daeElement* child);

	daeMetaElement *findChild( daeString elementName );

	virtual void getChildren( daeElement* parent, daeElementRefArray &array );

public:
	/**
	 * Sets the element type for the element that this attribute points to.
	 * @param elementType @c daeMetaElement representing the type.
	 */
	void setElementType(daeMetaElement *elementType) {
		_elementType = elementType; }

	/**
	 * Gets the element type for the element that this attribute points to.
	 * @return Returns the @c daeMetaElement representing the type.
	 */
	daeMetaElement* getElementType() { return _elementType; }
	
	/**
	 * Sets the database document associated with this element.
	 * @param parent The daeElement to set the document.
	 * @param c The @c daeDocument to associate with this element.
	 */
	virtual void setDocument(daeElement *parent, daeDocument* c );
	inline void setCollection(daeElement *parent, daeDocument* c ) {
		setDocument( parent, c );
	}

	/**
	 * Gets the number of elements associated with this attribute in instance <tt><i>e.</i></tt> 
	 * @param e Containing element to run the operation on.
	 * @return Returns the number of elements associated with this attribute
	 * in instance <tt><i>e.</i></tt> 
	 */
	virtual daeInt getCount(daeElement* e);

	/**
	 * Gets an element from containing element <tt><i>e</i></tt> based on <tt><i>index.</i></tt> 
	 * @param e Containing element from which to get the element.
	 * @param index Index of the element to retrieve if indeed
	 * there is an array of elements rather than a singleton.
	 * @return Returns the associated element out of parent element e, based on index, if necessary.
	 */
	virtual daeMemoryRef get(daeElement* e, daeInt index);

	/**
	 * Defines the override version of base method.
	 * @param element Element on which to set this attribute.
	 * @param s String containing the value to be converted via the
	 * atomic type system.
	 */
	virtual void set(daeElement* element, daeString s);
	/**
	 * Defines the override version of base method.
	 * @param toElement Pointer to a @c daeElement to copy this attribute to.
	 * @param fromElement Pointer to a @c daeElement to copy this attribute from.
	 */
	virtual void copy(daeElement* toElement, daeElement* fromElement);

	/**
	 * Gets if this attribute is an array attribute.
	 * @return Returns true if this attribute is an array type.
	 */
	virtual daeBool isArrayAttribute()		{ return false; }
};
typedef daeSmartRef<daeMetaElementAttribute> daeMetaElementAttributeRef;
typedef daeTArray<daeMetaElementAttributeRef> daeMetaElementAttributeArray;


/**
 * The @c daeMetaElementArrayAttribute class is similar to daeMetaElementAttribute 
 * except that this meta attribute describes an array of elements rather than a singleton.
 */
class daeMetaElementArrayAttribute : public daeMetaElementAttribute
{
public:
	/**
	 * Constructor.
	 * @param container The daeMetaElement that this policy object belongs to.
	 * @param parent The daeMetaCMPolicy parent of this policy object.
	 * @param odinal The ordinal value offset of this specific policy object. Used for maintaining the 
	 * correct order of child elements.
	 * @param minO The minimum number of times this CMPolicy object must appear. This value comes from the COLLADA schema.
	 * @param maxO The maximum number of times this CMPolicy object may appear. This value comes from the COLLADA schema.
	 */
	daeMetaElementArrayAttribute(daeMetaElement *container, daeMetaCMPolicy *parent = NULL, daeUInt ordinal = 0, daeInt minO = 1, daeInt maxO = 1);
	~daeMetaElementArrayAttribute();
public:
	virtual daeElement *placeElement(daeElement* parent, daeElement* child, daeUInt &ordinal, daeInt offset = 0, daeElement* before = NULL, daeElement *after = NULL);
	virtual daeBool removeElement(daeElement* parent, daeElement* child);

	void getChildren( daeElement* parent, daeElementRefArray &array );

	/**
	 * Sets the database document associated with this element.
	 * @param c The @c daeDocument to associate with this element.
	 */
	virtual void setDocument(daeElement *parent, daeDocument* c );
	inline void setCollection(daeElement *parent, daeDocument* c ) {
		setDocument( parent, c );
	}

	/**
	 * Defines the override version of this method from @c daeMetaElement.
	 * @param e Containing element to run the operation on.
	 * @return Returns the number of particles associated with this attribute
	 * in instance <tt><i>e.</i></tt> 
	 */
	virtual daeInt getCount(daeElement* e);
	/**
	 * Defines the override version of this method from @c daeMetaElement.
	 * @param e Containing element from which to get the element.
	 * @param index Index of the particle to retrieve if indeed
	 * there is an array of elements rather than a singleton.
	 * @return Returns the associated particle out of parent element e, based on index, if necessary.
	 */
	virtual daeMemoryRef get(daeElement* e, daeInt index);
	/**
	 * Defines the override version of this method from @c daeMetaElement.
	 * @param toElement Pointer to a @c daeElement to copy this attribute to.
	 * @param fromElement Pointer to a @c daeElement to copy this attribute from.
	 */
	virtual void copy(daeElement* toElement, daeElement* fromElement);

	/**
	 * Gets if this attribute is an array attribute.
	 * @return Returns true if this attribute is an array type.
	 */
	virtual daeBool isArrayAttribute()		{ return true; }
};
typedef daeSmartRef<daeMetaElementArrayAttribute> daeMetaElementArrayAttributeRef;
typedef daeTArray<daeMetaElementArrayAttributeRef> daeMetaElementArrayAttributeArray;

#endif

