/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 

#ifndef __DAE_DOCUMENT__
#define __DAE_DOCUMENT__

#include <dae/daeTypes.h>
#include <dae/daeElement.h>
#include <dae/daeURI.h>
#include <dae/daeStringRef.h>

class DAE;
class daeDatabase;

/**
 * The @c daeDocument class implements a COLLADA runtime database entry.
 */
class DLLSPEC daeDocument
{
public:
	/**
	 * Constructor
	 * @param dae The dae that owns this document. 
     * @param zaeRootDocument Indicates if the new document is the root document of a ZAE archive.
     * @param extractedFileURI URI to extracted dae file.
	 */
    daeDocument(DAE& dae, bool zaeRootDocument = false, const std::string& extractedFileURI = "");

	/**
	 * Destructor
	 */
	~daeDocument();

	/**
	* Accessor to get the @c domCollada associated with this document.
	* @return A @c daeElementRef for the @c domCollada that is the root of this document.
	* @note This function should really return a domColladaRef,
	* but we're trying to avoid having @c dae classes depend on generated dom classes.
	*/
	daeElement* getDomRoot() const {return(dom);}
	/**
	* Accessor to set the domCollada associated with this document
	* @param domRoot the domCollada that is the root of this document
	* @remarks Should really require a domColladaRef but we're trying to avoid having dae classes depend on generated dom classes.
	*/
	void setDomRoot(daeElement* domRoot) {dom = domRoot; domRoot->setDocument(this); }
	/**
	* Accessor to get the URI associated with the document in this document; 
	* this is currently set to the URI from which the document was loaded, but
	* is blank if the document was created with @c insertDocument().
	* @return Returns a pointer to the URI for this document.
	* @note This is the full URI of the document and not the document base URI.
	*/
	daeURI* getDocumentURI() {return (&uri);}

	/**
	* Const accessor to get the URI associated with the document in this collection; 
	* this is currently set to the URI from which the collection was loaded, but
	* is blank if the collection was created with @c insertCollection().
	* @return Returns a pointer to the URI for this collection.
	* @note This is the full URI of the document and not the document base URI.
	*/
	const daeURI* getDocumentURI() const {return (&uri);}

	/**
	 * Accessor to get the DAE that owns this document.
	 * @return Returns the DAE that owns this document.
	 */
	DAE* getDAE();

	/**
	 * Accessor to get the database associated with this document.
	 * @return Returns the database associated with this document.
	 */
	daeDatabase* getDatabase();

	/**
	 * This function is used to track how a document gets modified. It gets called internally.
	 * @param element The element that was added to this document.
	 * @note This function is called internally and not meant to be called by the client application.
	 * Calling this function from the client application may result in unexpected behavior.
	 */
	void insertElement( daeElementRef element );
	/**
	 * This function is used to track how a document gets modified. It gets called internally.
	 * @param element The element that was removed from this document.
	 * @note This function is called internally and not meant to be called by the client application.
	 * Calling this function from the client application may result in unexpected behavior.
	 */
	void removeElement( daeElementRef element );
	/**
	 * This function is used to track how a document gets modified. It gets called internally.
	 * @param element The element whose ID is about to be changed.
	 * @param newID The ID that is going to be assigned to the element.
	 * @note This function is called internally and not meant to be called by the client application.
	 * Calling this function from the client application may result in unexpected behavior.
	 */
	void changeElementID( daeElementRef element, daeString newID );
	/**
	 * This function is just like changeElementID, except it keeps track of sids instead of IDs.
	 * @param element The element whose sid is about to be changed.
	 * @param newSID The sid that is going to be assigned to the element.
	 * @note This function is called internally and not meant to be called by the client application.
	 * Calling this function from the client application may result in unexpected behavior.
	 */
	void changeElementSID( daeElementRef element, daeString newSID );

    /**
     * Returns true if this document is the root of a ZAE archive.
     * In that case getExtractedFileURI() can be used to parse
     * this document and for URI resolving.
     * @note This function is called internally and not meant to be called by the client application.
     */
    bool isZAERootDocument() {return mZAERootDocument;}

    /**
     * If this document is the root of a ZAE archive, this method can be used
     * to get the extracted file. Return value is only valid if isZAERootDocument()
     * returns true.
     * @note This function is called internally and not meant to be called by the client application.
     */
    const daeURI& getExtractedFileURI() {return mExtractedFileURI;}

private:
	/**
	 * The DAE that owns this document. The DAE's database is notified by the document when
	 * elements are inserted, removed, or have their ID changed.
	 */
	DAE* dae;

	/**
	 * Top Level element for of the document, always a domCollada
	 * @remarks This member will eventually be taken private, use getDomRoot() to access it.
	 */
	daeElementRef dom;
	
	/** 
	 * The URI of the document, may be blank if the document wasn't loaded from a URI
	 * @remarks This member will eventually be taken private, use getDocumentURI() to access it.
	 */
	daeURI uri;

    /**
     * Indicates if this document is the root of a ZAE archive.
     */
    bool mZAERootDocument;

    /**
     * URI pointing to the extracted root DAE if mZAERootDocument is true.
     * Otherwise it is not valid.
     */
    daeURI mExtractedFileURI;
};

typedef daeDocument daeCollection;

#endif

