/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 

#ifndef __DAE_DATABASE__
#define __DAE_DATABASE__

#include <memory>
#include <vector>
#include <dae.h>
#include <dae/daeTypes.h>
#include <dae/daeElement.h>
#include <dae/daeURI.h>
#include <dae/daeDocument.h>


/**
 * The @c daeDatabase class defines the COLLADA runtime database interface.
 */
class DLLSPEC daeDatabase
{
public:
	/**
	 * Constructor
	 */
	daeDatabase(DAE& dae);
	
	/**
	 * Destructor
	 */
	virtual ~daeDatabase() {}

	/**
	 * Get the associated DAE object.
	 * @return The associated DAE object.
	 */
	virtual DAE* getDAE();
	
	/**
	* Creates a new document, defining its root as the <tt><i>dom</i></tt> object; returns an error if the document name already exists.
	* @param name Name of the new document, must be a valid URI.
	* @param dom Existing @c domCOLLADA root element of the document
	* @param document Pointer to a @c daeDocument pointer that receives the document created 
    * @param zaeRootDocument Indicates if the new document is the root document of a ZAE archive.
    * @param extractedFileURI URI to extracted dae file.
	* @return Returns @c DAE_OK if the document was created successfully, otherwise returns a negative value as defined in daeError.h.
	* @note The @c daeElement passed in as <tt><i>dom</i></tt> should always be a @c domCOLLADA object, the API may enforce this in the future.
	* @deprecated This function will be removed in future versions. Please use createDocument.
	*/
	virtual daeInt insertDocument(daeString name, daeElement* dom, daeDocument** document = NULL, bool zaeRootDocument = false, const std::string& extractedFileURI = "") = 0;
	/**
	* Creates a new @c domCOLLADA root element and a new document; returns an error if the document name already exists.
	* @param name Name of the new document, must be a valid URI.
	* @param document Pointer to a @c daeDocument pointer that receives the document created 
	* @return Returns DAE_OK if the document was created successfully, otherwise returns a negative value as defined in daeError.h.
	* @deprecated This function will be removed in future versions. Please use createDocument.
	*/
	virtual daeInt insertDocument(daeString name, daeDocument** document = NULL) = 0;
	/**
	* Creates a new document, defining its root as the <tt><i>dom</i></tt> object; returns an error if the document name already exists.
	* @param name Name of the new document, must be a valid URI.
	* @param dom Existing @c domCOLLADA root element of the document
	* @param document Pointer to a @c daeDocument pointer that receives the document created 
    * @param zaeRootDocument Indicates if the new document is the root document of a ZAE archive.
    * @param extractedFileURI URI to extracted dae file.
	* @return Returns @c DAE_OK if the document was created successfully, otherwise returns a negative value as defined in daeError.h.
	* @note The @c daeElement passed in as <tt><i>dom</i></tt> should always be a @c domCOLLADA object, the API may enforce this in the future.
	*/
	virtual daeInt createDocument(daeString name, daeElement* dom, daeDocument** document = NULL, bool zaeRootDocument = false, const std::string& extractedFileURI = "") = 0;
	/**
	* Creates a new @c domCOLLADA root element and a new document; returns an error if the document name already exists.
	* @param name Name of the new document, must be a valid URI.
	* @param document Pointer to a @c daeDocument pointer that receives the document created 
	* @return Returns DAE_OK if the document was created successfully, otherwise returns a negative value as defined in daeError.h.
	*/
	virtual daeInt createDocument(daeString name, daeDocument** document = NULL) = 0;

	/**
	 * Inserts an already existing document into the database.
	 * @param c The document to insert.
	 * @return Returns DAE_OK if the document was inserted successfully, otherwise returns a negative value as defined in daeError.h.
	 */
	virtual daeInt insertDocument( daeDocument *c ) = 0;

	/**
	* Removes a document from the database.
	* @param document Document to remove from the database
	* @return Returns DAE_OK if the document was successfully removed, otherwise returns a negative value as defined in daeError.h. 
	*/
	virtual daeInt removeDocument(daeDocument* document) = 0;
	/**
	* Gets the number of documents.
	* @return Returns the number of documents.
	*/
	virtual daeUInt getDocumentCount() = 0;
	/**
	* Gets a document based on the document index.
	* @param index Index of the document to get.
	* @return Returns a pointer on the document, or NULL if not found. 
	*/
	virtual daeDocument* getDocument(daeUInt index) = 0;
	/**
	* Gets a document based on the document index.
	* @param index Index of the document to get.
	* @return Returns a pointer on the document, or NULL if not found. 
	*/
	daeDocument* getDoc(daeUInt index);
	/**
	* Gets a document based on the document name.
	* @param name The name of the document as a URI.
	* @param skipUriNormalization Use the document name as is; don't normalize it first.
	*   This is mostly for improved performance.
	* @return Returns a pointer to the document, or NULL if not found. 
	* @note If the URI contains a fragment, the fragment is stripped off.
	*/
	virtual daeDocument* getDocument(daeString name, bool skipUriNormalization = false) = 0;
	/**
	* Gets a document name.
	* @param index Index of the document to get.
	* @return Returns the name of the document at the given index. 
	*/
	virtual daeString getDocumentName(daeUInt index) = 0;
	/**
	* Indicates if a document is loaded or not.
	* @param name Name of the document  as a URI.
	* @return Returns true if the document is loaded, false otherwise.
	* @note If the URI contains a fragment, the fragment is stripped off.
	*/
	virtual daeBool isDocumentLoaded(daeString name) = 0;
	
	/**
	* Inserts a @c daeElement into the runtime database.
	* @param document Document in which the @c daeElement lives.
	* @param element @c daeElement to insert in the database
	* @return Returns @c DAE_OK if element successfully inserted, otherwise returns a negative value as defined in daeError.h.
	*/
	virtual daeInt insertElement(daeDocument* document,
	                             daeElement* element) = 0;
	/**
	* Removes a @c daeElement from the runtime database; not implemented in the reference STL implementation.
	* @param document Document in which the @c daeElement lives.
	* @param element Element to remove.
	* @return Returns @c DAE_OK if element successfully removed, otherwise returns a negative value as defined in daeError.h.
	*/
	virtual daeInt removeElement(daeDocument* document,
	                             daeElement* element) = 0;
	/**
	 * Updates the database to reflect a change to the ID of a @c daeElement.
	 * @param element @c daeElement whose ID is going to change.
	 * @param newID The ID that will be assigned to the element.
	 * @return Returns @c DAE_OK if the database was successfully updated, otherwise returns a negative value as defined in daeError.h.
	 * @note The database doesn't actually change the ID of the element, it
	 * merely updates its internal structures to reflect the change. It's
	 * expected that the ID will be assigned to the element by someone else.
	 */
	virtual daeInt changeElementID(daeElement* element,
	                               daeString newID) = 0;

	/**
	 * Updates the database to reflect a change to the sid of a @c daeElement.
	 * @param element @c daeElement whose sid is going to change.
	 * @param newSID The sid that will be assigned to the element.
	 * @return Returns @c DAE_OK if the database was successfully updated, otherwise returns a negative value as defined in daeError.h.
	 * @note The database doesn't actually change the sid of the element, it
	 * merely updates its internal structures to reflect the change. It's
	 * expected that the sid will be assigned to the element by someone else.
 	 * Note - This function currently isn't implemented in the default database.
	 */
	virtual daeInt changeElementSID(daeElement* element,
	                                daeString newSID) = 0;

	/**
	* Unloads all of the documents of the runtime database.
	* This function frees all the @c dom* objects created so far,
	* except any objects on which you still have a smart pointer reference (@c daeSmartRef).
	* @return Returns @c DAE_OK if all documents successfully unloaded, otherwise returns a negative value as defined in daeError.h.
	*/
	virtual daeInt clear() = 0;

	/**
	 * Lookup elements by ID, searching through all documents.
	 * @param id The ID to match on.
	 * @return The array of matching elements.
	 */
	virtual std::vector<daeElement*> idLookup(const std::string& id) = 0;

	/**
	 * Find an element with the given ID in a specific document.
	 * @param id The ID to match on.
	 * @param doc The document to search in.
	 * @return The matching element if one is found, NULL otherwise.
	 */
	daeElement* idLookup(const std::string& id, daeDocument* doc);

	/**
	 * Lookup elements by type ID.
	 * @param typeID The type to match on, e.g. domNode::ID().
	 * @param doc The document to search in, or NULL to search in all documents.
	 * @return The array of matching elements.
	 */
	std::vector<daeElement*> typeLookup(daeInt typeID, daeDocument* doc = NULL);

	/**
	 * Same as the previous method, but returns the array of matching elements via a
	 * reference parameter for additional efficiency.
	 * @param typeID The type to match on, e.g. domNode::ID().
	 * @param matchingElements The array of matching elements.
	 * @param doc The document to search in, or NULL to search in all documents.
	 */
	virtual void typeLookup(daeInt typeID,
	                        std::vector<daeElement*>& matchingElements,
	                        daeDocument* doc = NULL) = 0;

	/**
	 * Lookup elements by type ID.
	 * @param doc The document to search in, or NULL to search in all documents.
	 * @return The array of matching elements.
	 */
	template<typename T>
	std::vector<T*> typeLookup(daeDocument* doc = NULL) {
		std::vector<T*> result;
		typeLookup(result, doc);
		return result;
	}

	/**
	 * Same as the previous method, but returns the array of matching elements via a
	 * reference parameter for additional efficiency.
	 * @param matchingElements The array of matching elements.
	 * @param doc The document to search in, or NULL to search in all documents.
	 */
	template<typename T> void
	typeLookup(std::vector<T*>& matchingElements, daeDocument* doc = NULL) {
		std::vector<daeElement*> elts;
		typeLookup(T::ID(), elts, doc);
		matchingElements.clear();
		matchingElements.reserve(elts.size());
		for (size_t i = 0; i < elts.size(); i++)
			matchingElements.push_back((T*)elts[i]);
	}
		
	/**
	 * Lookup elements by sid.
	 * @param sid The sid to match on.
	 * @param doc The document to search in, or NULL to search in all documents.
	 * @return The array of matching elements.
	 * Note - This function currently isn't implemented in the default database.
	 */
	std::vector<daeElement*> sidLookup(const std::string& sid, daeDocument* doc = NULL);

	/**
	 * Same as the previous method, but the results are returned via a parameter instead
	 * of a return value, for extra efficiency.
	 * @param sid The sid to match on.
	 * @param matchingElements The array of matching elements.
	 * @param doc The document to search in, or NULL to search in all documents.
	 * Note - This function currently isn't implemented in the default database.
	 */
	virtual void sidLookup(const std::string& sid,
	                       std::vector<daeElement*>& matchingElements,
	                       daeDocument* doc = NULL) = 0;

	/** 
	* Sets the top meta object.
	* Called by @c dae::setDatabase() when the database changes. It passes to this function the
	* top meta object, which is the root of a 
    * hierarchy of @c daeMetaElement objects. This top meta object is capable of creating
	* any of the root objects in the DOM tree.
	* @param _topMeta Top meta object to use to create objects to fill the database.
	* @return Returns DAE_OK if successful, otherwise returns a negative value defined in daeError.h.
	*/
	virtual daeInt setMeta(daeMetaElement *_topMeta) = 0;

public:
	// The following methods are deprecated, and it's recommended that you don't use them.
	// Where appropriate, alternative methods are specified.

	virtual daeUInt getTypeCount() = 0;
	virtual daeString getTypeName(daeUInt index) = 0;

	// Instead of the following two methods, use idLookup or typeLookup.
	virtual daeUInt getElementCount(daeString name = NULL,
	                                daeString type = NULL,
	                                daeString file = NULL) = 0;
	virtual daeInt getElement(daeElement** pElement,
	                          daeInt index,
	                          daeString name = NULL,
	                          daeString type = NULL,
	                          daeString file = NULL ) = 0;

	inline daeInt insertCollection(daeString name, daeElement* dom, daeDocument** document = NULL) {
		return insertDocument( name, dom, document );
	}
	inline daeInt insertCollection(daeString name, daeDocument** document = NULL) {
		return insertDocument( name, document );
	}
	inline daeInt createCollection(daeString name, daeElement* dom, daeDocument** document = NULL) {
		return createDocument( name, dom, document );
	}
	inline daeInt createCollection(daeString name, daeDocument** document = NULL) {
		return createDocument( name, document );
	}
	inline daeInt insertCollection( daeDocument *c ) {
		return insertDocument( c );
	}
	inline daeInt removeCollection(daeDocument* document) {
		return removeDocument( document );
	}
	inline daeUInt getCollectionCount() {
		return getDocumentCount();
	}
	inline daeDocument* getCollection(daeUInt index) {
		return getDocument( index );
	}
	inline daeDocument* getCollection(daeString name) {
		return getDocument( name );
	}
	inline daeString getCollectionName(daeUInt index) {
		return getDocumentName( index );
	}
	inline daeBool isCollectionLoaded(daeString name) {
		return isDocumentLoaded( name );
	}

protected:
	DAE& dae;
};

#endif //__DAE_DATABASE__

