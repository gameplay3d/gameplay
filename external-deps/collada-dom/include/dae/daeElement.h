/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 

#ifndef __DAE_ELEMENT_H__
#define __DAE_ELEMENT_H__
#include <string>
#include <dae/daeTypes.h>
#include <dae/daeMemorySystem.h>
#include <wchar.h>
#include <dae/daeArray.h>
#include <dae/daeRefCountedObj.h>
#include <dae/daeSmartRef.h>

//#ifndef NO_MALLOC_HEADER
//#include <malloc.h>
//#endif

namespace COLLADA_TYPE
{
	typedef int TypeEnum;
}

class DAE;
class daeMetaElement;
class daeMetaAttribute;
class daeDocument;
class daeURI;

/**
 * The @c daeElement class represents an instance of a COLLADA "Element";
 * it is the main base class for the COLLADA Dom.
 * Features of this class include:
 * - Uses factory concepts defined via daeMetaElement
 * - Composed of attributes, content elements and content values
 * - Reference counted via daeSmartRef
 * - Contains information for XML base URI, and XML containing element
 */
class DLLSPEC daeElement : public daeRefCountedObj
{
public:
	/**
	 * Macro that defines new and delete overrides for this class
	 */
	DAE_ALLOC
protected:
	daeElement* _parent;
	daeDocument* _document;
	daeMetaElement* _meta;
	daeString _elementName;
	daeBoolArray _validAttributeArray; // This is now obsolete and can be removed
	void* _userData;

protected:
	daeElement( const daeElement &cpy ) : daeRefCountedObj() { (void)cpy; };
	virtual daeElement &operator=( const daeElement &cpy ) { (void)cpy; return *this; }

	void init();

	// These functions are called internally.
	void setDocument( daeDocument* c, bool notifyDocument );
	daeElement* simpleAdd(daeString name, int index = -1);

public:
	/**
	 * Element Constructor.
	 * @note This should not be used externally.
	 * Use factories to create elements
	 */
	daeElement();
	/**
	 * Element Constructor.
	 * @note This should not be used externally.
	 * Use factories to create elements
	 */
	daeElement(DAE& dae);

	/**
	 * Element Destructor.
	 * @note This should not be used externally, 
	 * if daeSmartRefs are being used.
	 */
	virtual ~daeElement();

	/**
	 * Sets up a @c daeElement. Called on all @c daeElements as part of their initialization.
	 * @param meta Meta element to use to configure this element.
	 * @note Should not be called externally.
	 */
	void setup(daeMetaElement* meta);

	// These functions are for adding child elements. They return null if adding
	// the element failed.
	daeElement* add(daeString name, int index = -1);
	daeElement* add(daeElement* elt, int index = -1);
	daeElement* addBefore(daeElement* elt, daeElement* index);
	daeElement* addAfter(daeElement* elt, daeElement* index);

	// These functions are deprecated. Use 'add' instead.
	daeElement* createAndPlace(daeString elementName);
	daeElement* createAndPlaceAt(daeInt index, daeString elementName);
	daeBool placeElement(daeElement* element);
	daeBool placeElementAt(daeInt index, daeElement* element);
	daeBool placeElementBefore( daeElement* marker, daeElement *element );
	daeBool placeElementAfter( daeElement* marker, daeElement *element );

	/**
	 * Finds the last index into the array of children of the name specified.
	 * @param elementName The name to look for.
	 * @return Returns the index into the children array of the last element with name elementName. -1 if
	 *         there are no children of name elementName.
	 */
	daeInt findLastIndexOf( daeString elementName );

	/**
	 * Removes the specified element from it parent, the @c this element.
	 * This function is the opposite of @c placeElement().  It removes the specified
	 * element from the <tt><i> _contents </i></tt> array, and from wherever else it appears
	 * inside of the @c this element.  Use this function instead of @c clear(), @c remove() or @c delete()
	 * if you want to keep the <tt><i> _contents </i></tt> field up-to-date.
	 *
	 * @param element Element to be removed in the @c this container.
	 * @return Returns true if the element was successfully removed, false otherwise.
	 */
	daeBool removeChildElement(daeElement* element);
	
	/**
	 * Removes the specified element from its parent element. 
	 * This function is the opposite of @c placeElement().  It removes the specified
	 * element from both the <tt><i> _contents </i></tt> array and from wherever else it appears
	 * inside of its parent. The function itself finds the parent, and is defined as a static method,
	 * since removing the element from its parent may result in the deletion of the element.
	 * If the element has no parent, nothing is done.
	 *
	 * Use this function instead of @c clear(), @c remove() or @c delete()
	 * if you want to keep <tt><i> _contents </i></tt> up-to-date.
	 *
	 * @param element Element to remove from its parent container, the function finds the parent element.
	 * @return Returns true if the element was successfully removed, false otherwise.
	 */
	static daeBool removeFromParent(daeElement* element)
	{
		if(element != NULL && element->_parent != NULL) 
			return(element->_parent->removeChildElement(element));
		return false;
	};

	/**
	 * Returns the number of attributes in this element.
	 * @return The number of attributes this element has.
	 */
	size_t getAttributeCount();
	
	/**
	 * Returns the daeMetaAttribute object corresponding to the attribute specified.
	 * @param name The name of the attribute to find.
	 * @return Returns the corresponding daeMetaAttribute object or NULL if this element
	 * doesn't have the specified attribute.
	 */
	daeMetaAttribute* getAttributeObject(daeString name);

	/**
	 * Returns the daeMetaAttribute object corresponding to attribute i.
	 * @param i The index of the attribute to find.
	 * @return Returns the corresponding daeMetaAttribute object
	 */
	daeMetaAttribute* getAttributeObject(size_t i);

	/**
	 * Returns the name of the attribute at the specified index.
	 * @param i The index of the attribute whose name should be retrieved.
	 * @return Returns the name of the attribute, or "" if the index is out of range.
	 */
	std::string getAttributeName(size_t i);

	/**
	 * Checks if this element can have the attribute specified.
	 * @param name The name of the attribute to look for.
	 * @return Returns true is this element can have an attribute with the name specified. False otherwise.
	 */
	daeBool hasAttribute(daeString name);

	/**
	 * Checks if an attribute has been set either by being loaded from the COLLADA document or set
	 * programmatically.
	 * @param name The name of the attribute to check.
	 * @return Returns true if the attribute has been set. False if the attribute hasn't been set 
	 * or doesn't exist for this element.
	 */
	daeBool isAttributeSet(daeString name);

	/**
	 * Gets an attribute's value as a string.
	 * @param name The name of the attribute.
	 * @return The value of the attribute. Returns an empty string if this element doesn't
	 * have the specified attribute.
	 */
	std::string getAttribute(daeString name);

	/**
	 * Just like the previous method, this method gets an attribute's value as a string. It
	 * takes the string as a reference parameter instead of returning it, for extra efficiency.
	 * @param name The name of the attribute.
	 * @param A string in which to store the value of the attribute. This will be set to an empty 
	 * string if this element doesn't have the specified attribute.
	 */
	void getAttribute(daeString name, std::string& value);

	/**
	 * Gets an attribute's value as a string.
	 * @param i The index of the attribute to retrieve.
	 * @return The value of the attribute.
	 */
	std::string getAttribute(size_t i);

	/**
	 * Just like the previous method, this method gets an attribute's value as a string. It
	 * takes the string as a reference parameter instead of returning it, for extra efficiency.
	 * @param i The index of the attribute to retrieve.
	 * @param A string in which to store the value of the attribute.
	 */
	void getAttribute(size_t i, std::string& value);

	struct DLLSPEC attr {
		attr();
		attr(const std::string& name, const std::string& value);

		std::string name;
		std::string value;
	};
	
	/**
	 * Returns an array containing all the attributes of this element.
	 * @return A daeArray of attr objects.
	 */
	daeTArray<attr> getAttributes();

	/**
	 * Just like the previous method, this method returns an array containing all the attributes
	 * of this element. It returns the result via a reference parameter for extra efficiency.
	 * @param attrs The array of attr objects to return.
	 */
	void getAttributes(daeTArray<attr>& attrs);

	/**
	 * Sets the attribute to the specified value.
	 * @param name Attribute to set.
	 * @param value Value to apply to the attribute.
	 * @return Returns true if the attribute was found and the value was set, false otherwise.
	 */
	virtual daeBool setAttribute(daeString name, daeString value);

	/**
	 * Sets the attribute at the specified index to the given value.
	 * @param i Index of the attribute to set.
	 * @param value Value to apply to the attribute.
	 * @return Returns true if the attribute was found and the value was set, false otherwise.
	 */
	virtual daeBool setAttribute(size_t i, daeString value);

	/**
	 * Returns the daeMetaAttribute object corresponding to the character data for this element.
	 * @return Returns a daeMetaAttribute object or NULL if this element doesn't have
	 * character data.
	 */
	daeMetaAttribute* getCharDataObject();

	/**
	 * Checks if this element can have character data.
	 * @return Returns true if this element can have character data, false otherwise.
	 */
	daeBool hasCharData();

	/**
	 * Returns this element's character data as a string.
	 * @return A string containing this element's character data, or an empty string
	 * if this element can't have character data.
	 */
	std::string getCharData();

	/**
	 * Similar to the previous method, but fills a string passed in by the user for efficiency.
	 * @param data The string to be filled with this element's character content. The
	 * string is set to an empty string if this element can't have character data.
	 */
	void getCharData(std::string& data);

	/**
	 * Sets this element's character data.
	 * @param data The new character data of this element.
	 * @return Returns true if this element can have character data and the character data
	 * was successfully changed, false otherwise.
	 */
	daeBool setCharData(const std::string& data);

	// These functions are deprecated.
	daeMemoryRef getAttributeValue(daeString name); // Use getAttribute or getAttributeObject instead.
	daeBool hasValue(); // Use hasCharData instead.
	daeMemoryRef getValuePointer(); // Use getCharData or getCharDataObject instead.

	/**
	 * Finds the database document associated with @c this element.
	 * @return Returns the @c daeDocument representing the containing file or database
	 * group.
	 */
	daeDocument* getDocument() const { return _document; }

	/**
	 * Deprecated.
	 */
	daeDocument* getCollection() const { return _document; }

	/**
	 * Get the associated DAE object.
	 * @return The associated DAE object.
	 */
	DAE* getDAE();
	
	/**
	 * Sets the database document associated with this element.
	 * @param c The daeDocument to associate with this element.
	 */
	void setDocument(daeDocument* c) { setDocument( c, true ); }
	/**
	 * Deprecated.
	 */
	void setCollection(daeDocument* c );

	/**
	 * Gets the URI of the document containing this element, note that this is NOT the URI of the element.
	 * @return Returns a pointer to the daeURI of the document containing this element.
	 */
	daeURI*	getDocumentURI() const;

	/**
	 * Creates an element via the element factory system.  This creation
	 * is based @em only on potential child elements of this element.
	 * @param elementName Class name of the subelement to create.
	 * @return Returns the created @c daeElement, if it was successfully created.
	 */
	daeSmartRef<daeElement> createElement(daeString elementName);

	/**
	 * Gets the container element for @c this element.
	 * If @c createAndPlace() was used to create the element, its parent is the the caller of @c createAndPlace().
	 * @return Returns the parent element, if @c this is not the top level element.
	 */
	daeElement* getParentElement() { return _parent;}
	/**
	 * Deprecated. Use getParentElement()
	 * @deprecated
	 */
	daeElement* getXMLParentElement() { return _parent;}
	/**
	 * Sets the parent element for this element.
	 * @param newParent The element which is the new parent element for this element.
	 * @note This function is called internally and not meant to be called form the client application. 
	 */
	void setParentElement( daeElement *parent ) { _parent = parent; }

	// These are helper structures to let the xml hierarchy search functions know when we've
	// found a match. You can implement a custom matcher by inheriting from this structure,
	// just like matchName and matchType.
	struct DLLSPEC matchElement {
		virtual bool operator()(daeElement* elt) const = 0;
		virtual ~matchElement() { };
	};

	// Matches an element by name
	struct DLLSPEC matchName : public matchElement {
		matchName(daeString name);
		virtual bool operator()(daeElement* elt) const;
		std::string name;
	};

	// Matches an element by schema type
	struct DLLSPEC matchType : public matchElement {
		matchType(daeInt typeID);
		virtual bool operator()(daeElement* elt) const;
		daeInt typeID;
	};

	// Returns a matching child element. By "child", I mean one hierarchy level beneath the
	// current element. This function is basically the same as getDescendant, except that it
	// only goes one level deep.
	daeElement* getChild(const matchElement& matcher);

	// Performs a breadth-first search and returns a matching descendant element. A "descendant
	// element" is an element beneath the current element in the xml hierarchy.
	daeElement* getDescendant(const matchElement& matcher);

	// Returns the parent element.
	daeElement* getParent();

	// Searches up through the xml hiearchy and returns a matching element.
	daeElement* getAncestor(const matchElement& matcher);

	// These functions perform the same as the functions above, except that they take the element
	// name to match as a string. This makes these functions a little simpler to use if you're
	// matching based on element name, which is assumed to be the most common case. Instead of
	// "getChild(matchName(eltName))", you can just write "getChild(eltName)".
	daeElement* getChild(daeString eltName);
	daeElement* getDescendant(daeString eltName);
	daeElement* getAncestor(daeString eltName);

	/**
	 * Gets the associated Meta information for this element.  This
	 * Meta also acts as a factory.  See @c daeMetaElement documentation for more
	 * information.
	 * @return Returns the associated meta information.
	 */
	inline daeMetaElement* getMeta() { return _meta; }

	// These functions are deprecated. Use typeID instead.
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return (COLLADA_TYPE::TypeEnum)0; }
	daeString getTypeName() const;

	/**
	 * Returns this element's type ID. Every element is an instance of a type specified in
	 * the Collada schema, and every schema type has a unique ID.
	 * @return The element's type ID.
	 */
	virtual daeInt typeID() const = 0;

	/**
	 * Gets this element's name.
	 * @return Returns the string for the name.
	 * @remarks This function returns NULL if the element's name is identical to it's type's name.
	 */
	daeString getElementName() const;
	/**
	 * Sets this element's name.
	 * @param nm Specifies the string to use as the element's name.
	 * @remarks Use caution when using this function since you can easily create invalid COLLADA documents.
	 */
	void setElementName( daeString nm );
	
	/**
	 * Gets the element ID if it exists.
	 * @return Returns the value of the ID attribute, if there is such
	 * an attribute on this element type.
	 * @return the string for the element ID if it exists.
	 */
	daeString getID() const;

	/**
	 * Gets the children/sub-elements of this element.
	 * This is a helper function used to easily access an element's children without the use of the
	 * _meta objects.  This function adds the convenience of the _contents array to elements that do
	 * not contain a _contents array.
	 * @return The return value.  An elementref array to append this element's children to.
	 */
	daeTArray< daeSmartRef<daeElement> > getChildren();

	/**
	 * Same as the previous function, but returns the result via a parameter instead
	 * of a return value, for extra efficiency.
	 * @param array The return value.  An elementref array to append this element's children to.
	 */
	//void getChildren( daeElementRefArray &array );
	void getChildren( daeTArray<daeSmartRef<daeElement> > &array );

	/**
	 * Gets all the children of a particular type.
	 * @return An array containing the matching child elements.
	 */
	template<typename T>
	daeTArray< daeSmartRef<T> > getChildrenByType() {
		daeTArray< daeSmartRef<T> > result;
		getChildrenByType(result);
		return result;
	}

	/**
	 * Same as the previous function, but returns the result via a parameter instead
	 * of a return value, for extra efficiency.
	 * @return An array containing the matching child elements.
	 */
	template<typename T>
	void getChildrenByType(daeTArray< daeSmartRef<T> >& matchingChildren) {
		matchingChildren.setCount(0);
		daeTArray< daeSmartRef<daeElement> > children;
		getChildren(children);
		for (size_t i = 0; i < children.getCount(); i++)
			if (children[i]->typeID() == T::ID())
				matchingChildren.append((T*)children[i].cast());
	}

	/**
	 * Clones/deep copies this @c daeElement and all of it's subtree.
	 * @param idSuffix A string to append to the copied element's ID, if one exists.
	 *        Default is no ID mangling.
	 * @param nameSuffix A string to append to the copied element's name, if one exists.
	 *        Default is no name mangling.
	 * @return Returns a @c daeElement smartref of the copy of this element.
	 */
	daeSmartRef<daeElement> clone( daeString idSuffix = NULL, daeString nameSuffix = NULL );

	// Class for reporting info about element comparisons
	struct DLLSPEC compareResult {
		int compareValue; // > 0 if elt1 > elt2,
		                  // < 0 if elt1 < elt2,
		                  // = 0 if elt1 = elt2
		daeElement* elt1;
		daeElement* elt2;
		bool nameMismatch; // true if the names didn't match
		std::string attrMismatch; // The name of the mismatched attribute, or "" if there was no attr mismatch
		bool charDataMismatch; // true if the char data didn't match
		bool childCountMismatch; // true if the number of children didn't match

		compareResult();
		std::string format(); // Write to a string
	};

	// Function for doing a generic, recursive comparison of two xml elements. It
	// also provides a full element ordering, so that you could store elements in
	// a map or a set. Return val is > 0 if elt1 > elt2, < 0 if elt1 < elt2, and 0
	// if elt1 == elt2.
	static int compare(daeElement& elt1, daeElement& elt2);

	// Same as the previous function, but returns a full compareResult object.
	static compareResult compareWithFullResult(daeElement& elt1, daeElement& elt2);

	/**
	 * Sets the user data pointer attached to this element.
	 * @param data User's custom data to store.
	 */
	void setUserData(void* data);

	/**
	 * Gets the user data pointer attached to this element.
	 * @return User data pointer previously set with setUserData.
	 */
	void* getUserData();
	
public:
	// This function is called internally
	static void deleteCMDataArray(daeTArray<daeCharArray*>& cmData);
};

#include <dae/daeSmartRef.h>
typedef daeSmartRef<daeElement> daeElementRef;
typedef daeSmartRef<const daeElement> daeElementConstRef;
//#include <dae/daeArray.h>
typedef daeTArray<daeElementRef> daeElementRefArray;

#endif //__DAE_ELEMENT_H__
