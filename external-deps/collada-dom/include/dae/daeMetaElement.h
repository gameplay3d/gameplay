/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 

#ifndef __DAE_META_ELEMENT_H__
#define __DAE_META_ELEMENT_H__

#include <dae/daeTypes.h>
#include <dae/daeArrayTypes.h>
#include <dae/daeElement.h>
#include <dae/daeMetaAttribute.h>
#include <dae/daeRefCountedObj.h>

class DAE;
class daeMetaCMPolicy;
class daeMetaElementArrayAttribute;

typedef daeElementRef (*daeElementConstructFunctionPtr)(DAE& dae);

/**
 * Each instance of the @c daeMetaElement class describes a C++ COLLADA dom
 * element type.
 * @par
 * The meta information in @c daeMetaElement is a combination of the information
 * required to create and maintain C++ object instances and
 * the information necessary to parse and construct a hierarchy of COLLADA
 * elements.
 * @par
 * @c daeMetaElement objects also act as factories for C++ COLLADA dom classes where
 * each @c daeElement is capable of creating an instance of the class it describes.
 * Further, each @c daeMetaElement contains references to other @c daeMetaElements
 * for potential XML children elements.  This enables this system to easily
 * create @c daeElements of the appropriate type while navigating through XML
 * recursive parse.
 * @par
 * See @c daeElement for information about the functionality that every @c daeElement implements.
 */
class daeMetaElement : public daeRefCountedObj
{
protected:
	daeStringRef _name;

	daeElementConstructFunctionPtr _createFunc;
	daeInt _elementSize;
	
	daeMetaAttributeRefArray _metaAttributes;
	daeMetaAttributeRef _metaValue;
	daeMetaElementArrayAttribute* _metaContents;
	daeMetaArrayAttribute* _metaContentsOrder;

	daeMetaAttributeRef _metaID;

	daeBool _isTrackableForQueries;
	daeBool _usesStringContents;

	daeBool _isTransparent;
	daeBool _isAbstract;
	daeBool _allowsAny;
	daeBool _innerClass;
	
	daeMetaCMPolicy* _contentModel;	
	daeMetaArrayAttribute* _metaCMData;
	daeUInt _numMetaChoices;

	DAE& dae;

public:
	/**
	 * Constructor
	 */
	DLLSPEC daeMetaElement(DAE& dae);

	/**
	 * Destructor
	 */
	DLLSPEC ~daeMetaElement();

public: // public accessors

	/**
	 * Gets the DAE object that owns this daeMetaElement.
	 * @return Returns the owning DAE.
	 */
	DAE* getDAE();
	
	/**
	 * Determines if elements of this type is an inner class.
	 * @return Returns true if this element type is an inner class.
	 */
	daeBool getIsInnerClass() { return _innerClass; }
	/**
	 * Sets if elements of this type are inner classes.
	 * @param abstract True if this type is an inner class.
	 */
	void setIsInnerClass( daeBool ic ) { _innerClass = ic; }
	/**
	 * Determines if elements of this type can be placed in the object model.
	 * @return Returns true if this element type is abstract, false otherwise.
	 */
	daeBool getIsAbstract() { return _isAbstract; }
	/**
	 * Determines if elements of this type should have an element tag printed when saving.
	 * @return Returns true if this element type should not have a tag, false otherwise.
	 */
	daeBool getIsTransparent() { return _isTransparent; }
	/**
	 * Sets if elements of this type are abstract.
	 * @param abstract True if this type is abstract.
	 */
	void setIsAbstract( daeBool abstract ) { _isAbstract = abstract; }
	/**
	 * Sets whether or not elements of this type should have an element tag printed when saving.
	 * @param transparent True if this type is transparent.
	 */
	void setIsTransparent( daeBool transparent ) { _isTransparent = transparent; }

	/**
	 * Determines if elements of this type should be tracked
	 * for daeDatabase queries.
	 * @return Returns true if this element type should be tracked
	 */
	daeBool getIsTrackableForQueries() { return _isTrackableForQueries; }

	/**
	 * Sets whether elements of this type should be tracked
	 * for @c daeDatabase queries.
	 * @param trackable Indicates whether this element should be tracked.  
	 * A value of true indicates this element type should be tracked and be available for
	 * database queries.
	 */
	void setIsTrackableForQueries(daeBool trackable) {
		_isTrackableForQueries = trackable; }
		
	/**
	 * Determines if elements of this type allow for any element as a child.
	 * @return Returns true if this element can have any child element, false otherwise.
	 */
	daeBool getAllowsAny() { return _allowsAny; }
	/**
	 * Sets if elements of this type allow for any element as a child.
	 * @param allows True if this element allows for any child element, false otherwise.
	 */
	void setAllowsAny( daeBool allows ) { _allowsAny = allows; }

	/**
	 * Gets the @c daeMetaAttribute for the non-element contents of a @c daeElement.
	 * This corresponds to a @c daeMetaFloatAttribute, @c daeMetaFloatArrayAttribute,
	 * et cetera.
	 * @return Returns the @c daeMetaAttribute pointer for the non-element contents of
	 * this element type.
	 */
	daeMetaAttribute* getValueAttribute() { return _metaValue; }

	/**
	 * Gets the @c daeMetaAttribute for the ID attribute of a @c daeElement.
	 * @return Returns the ID @c daeMetaAttribute, or NULL if the element type
	 * does not have an ID attribute.
	 */
	daeMetaAttribute* getIDAttribute() { return _metaID; }

	/**
	 * Gets the name of this element type.
	 * @return Returns the name of this element type.
	 */
	daeStringRef getName() { return _name; } 

	/**
	 * Sets the name of this element type.
	 * @param s String name	to set.
	 */
	void setName(daeString s) { _name = s; }

	/**
	 * Gets the array of all known attributes on this element type.
	 * This includes all meta attributes except those describing child
	 * elements. It does include the value element.
	 * @return Returns the array of @c daeMetaAttributeRefs.
	 */
	daeMetaAttributeRefArray& getMetaAttributes() {
		return _metaAttributes; }

	/**
	 * Gets the attribute which has a name as provided by the <tt><i>s</i></tt> parameter. 
	 * @param s String containing the  desired attribute's name.
	 * @return Returns the corresponding @c daeMetaAttribute, or NULL if none found.
	 */
	DLLSPEC daeMetaAttribute* getMetaAttribute(daeString s);

	/**
	 * Sets the size in bytes of each instance of this element type.
	 * Used for factory element creation.
	 * @param size Number of bytes for each C++ element instance.
	 */
	void setElementSize(daeInt size) {_elementSize = size;}

	/**
	 * Gets the size in bytes of each instance of this element type.
	 * Used for factory element creation.
	 * @return Returns the number of bytes for each C++ element instance.
	 */
	daeInt getElementSize() { return _elementSize;}
	
public: 
	/**
	 * Registers with the reflective object system that the dom class described by this @c daeMetaElement
	 * contains a <tt><i>_contents</i></tt> array. This method is @em only for @c daeMetaElement contstuction, and
	 * should only be called by the system as it sets up the Reflective Object System.
	 * @param offset Byte offset for the contents field in the C++ element class.
	 */
	DLLSPEC void addContents(daeInt offset);
	/**
	 * Registers with the reflective object system the array that stores the _contents ordering. This method is @em 
	 * only for @c daeMetaElement contstuction, and should only be called by the system as it sets up the Reflective 
	 * Object System.
	 * @param offset Byte offset for the contents order array in the C++ element class.
	 */
	DLLSPEC void addContentsOrder( daeInt offset );
	/**
	 * Registers with the reflective object system that the dom class described by this @c daeMetaElement
	 * contains at least one choice group in the content model for this element. This method is @em only 
	 * for @c daeMetaElement contstuction, and should only be called by the system as it sets up the 
	 * Reflective Object System.
	 * @param offset Byte offset for the contents field in the C++ element class.
	 * @param numChoices The number of choice content model blocks there are for this element type.
	 */
	DLLSPEC void addCMDataArray( daeInt offset, daeUInt numChoices );

	/**
	 * Gets the attribute associated with the contents meta information.
	 * @see @c addContents()
	 * @return Returns the @c daeMetaElementArrayAttribute.
	 */
	daeMetaElementArrayAttribute* getContents() { return _metaContents; }
	/**
	 * Gets the attribute associated with the CMData array meta information.
	 * @see @c addCMDataArray()
	 * @return Returns the @c daeMetaArrayAttribute for the CMData of an element.
	 */
	daeMetaArrayAttribute* getMetaCMData() { return _metaCMData; }
	/**
	 * Gets the number of choice content model blocks there are for this element type.
	 * @return Returns the number of daeMetaChoice's there are in the content model.
	 */
	daeUInt getNumChoices() const { return _numMetaChoices; }

	/**
	 * Appends a @c daeMetaAttribute that represents a field corresponding to an
	 * XML attribute to the C++ version of this element type.
	 * @param attr Attribute to append to this element types list
	 * of potential attributes.
	 */
	DLLSPEC void appendAttribute(daeMetaAttribute* attr);

	/**
	 * Registers the function that can construct a C++ instance of this class.
	 * Necessary for the factory system such that C++ can still call @c new and the
	 * @c vptr will still be initialized even when constructed via the factory system.
	 * @param func Pointer to a function that does object construction.
	 */
	void registerClass(daeElementConstructFunctionPtr func) {
		_createFunc = func; }

	/**
	 * Validates this class to be used by the runtime c++ object model
	 * including factory creation.
	 */
	DLLSPEC void validate();
	
	/**
	 * Places a child element into the <tt><i>parent</i></tt> element where the
	 * calling object is the @c daeMetaElement for the parent element.
	 * @param parent Element to act as the container.
	 * @param child Child element to place in the parent.
	 * @return Returns true if the operation was successful, false otherwise.
	 */
	DLLSPEC daeBool place(daeElement *parent, daeElement *child, daeUInt *ordinal = NULL);
	/**
	 * Places a child element into the <tt><i>parent</i></tt> element at a specific location
	 * where the calling object is the @c daeMetaElement for the parent element.
	 * @param index The location in the contents array to insert.
	 * @param parent Element to act as the container.
	 * @param child Child element to place in the parent.
	 * @return Returns true if the operation was successful, false otherwise.
	 * @note This should only be called on elements that have a _contents array. Elements without
	 * a _contents array will be placed normally.
	 */
	DLLSPEC daeBool placeAt( daeInt index, daeElement *parent, daeElement *child );
	/**
	 * Places a child element into the <tt><i>parent</i></tt> element at a specific location which is right
	 * before the marker element.
	 * @param marker The element location in the contents array to insert before.
	 * @param parent Element to act as the container.
	 * @param child Child element to place in the parent.
	 * @return Returns true if the operation was successful, false otherwise.
	 */
	DLLSPEC daeBool placeBefore( daeElement* marker, daeElement *parent, daeElement *child, daeUInt *ordinal = NULL );
	/**
	 * Places a child element into the <tt><i>parent</i></tt> element at a specific location which is right
	 * after the marker element.
	 * @param marker The element location in the contents array to insert after.
	 * @param parent Element to act as the container.
	 * @param child Child element to place in the parent.
	 * @return Returns true if the operation was successful, false otherwise.
	 */
	DLLSPEC daeBool placeAfter( daeElement* marker, daeElement *parent, daeElement *child, daeUInt *ordinal = NULL );

	/**
	 * Removes a child element from its parent element.
	 * @param parent Element That is the parent.
	 * @param child Child element to remove.
	 * @return Returns true if the operation was successful, false otherwise.
	 */
	DLLSPEC daeBool remove( daeElement *parent, daeElement *child );
	/**
	 * Gets all of the children from an element of this type.
	 * @param parent The element that you want to get the children from.
	 * @param array The return value.  An elementref array to append this element's children to.
	 */
	DLLSPEC void getChildren( daeElement* parent, daeElementRefArray &array );

	/**
	 * Invokes the factory element creation routine set by @c registerConstructor() 
	 * to return a C++ COLLADA Object Model instance of this element type.
	 * @return Returns a created @c daeElement of appropriate type via the
	 * object creation function and the <tt> daeElement::setup() </tt> function.
	 */
	DLLSPEC daeElementRef create();

	/**
	 * Looks through the list of potential child elements
	 * for this element type finding the corresponding element type; if a corresponding element type 
	 * is found, use that type as a factory and return an instance of that
	 * child type.  Typically @c place() is called after @c create(childelementname)
	 * @param childElementTypeName Type name to create.
	 * @return Returns the created element if the type was found as a potential child element.
	 */
	DLLSPEC daeElementRef create(daeString childElementTypeName);

	/**
	 * Gets the root of the content model policy tree.
	 * @return Returns the root element of the tree of content model policy elements.
	 */
	daeMetaCMPolicy *getCMRoot()  { return _contentModel; }
	/**
	 * Sets the root of the content model policy tree.
	 * @param cm The root element of the tree of content model policy elements.
	 */
	DLLSPEC void setCMRoot( daeMetaCMPolicy *cm );
};

typedef daeSmartRef<daeMetaElement> daeMetaElementRef;
typedef daeTArray<daeMetaElementRef> daeMetaElementRefArray;

#endif //__DAE_META_ELEMENT_H__





