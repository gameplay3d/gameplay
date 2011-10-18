/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 

#ifndef __DAE_STLDATABASE__
#define __DAE_STLDATABASE__

#include <stdio.h>

#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <functional>

#include <dae/daeElement.h>
#include <dae/daeDatabase.h>

/**
 * The @c daeSTLDatabase class derives from @c daeDatabase and implements
 * the default database.
 */
class DLLSPEC daeSTLDatabase : public daeDatabase
{
public:
	/**
	  * Constructor
	  */
	daeSTLDatabase(DAE& dae);
	/**
	  * Destructor
	  */
	virtual ~daeSTLDatabase();

public:
	// Element Types of all Elements
	virtual daeUInt getTypeCount();
	virtual daeString getTypeName(daeUInt index);
	virtual daeInt setMeta(daeMetaElement *_topMeta);

	// Documents
	virtual daeInt insertDocument(daeString name, daeElement* dom, daeDocument** document = NULL, bool zaeRootDocument = false, const std::string& extractedFileURI = "");
	virtual daeInt insertDocument(daeString name, daeDocument** document = NULL);
	virtual daeInt createDocument(daeString name, daeElement* dom, daeDocument** document = NULL, bool zaeRootDocument = false, const std::string& extractedFileURI = "");
	virtual daeInt createDocument(daeString name, daeDocument** document = NULL);
	virtual daeInt insertDocument( daeDocument *c );

	virtual daeInt removeDocument(daeDocument* document);
	virtual daeUInt getDocumentCount();
	virtual daeDocument* getDocument(daeUInt index);
	virtual daeDocument* getDocument(daeString name, bool skipUriNormalization = false);
	virtual daeString getDocumentName(daeUInt index);
	virtual daeBool isDocumentLoaded(daeString name);

	// Elements 
	virtual daeInt insertElement(daeDocument* document, daeElement* element);
	virtual daeInt removeElement(daeDocument* document, daeElement* element); 
	virtual daeInt changeElementID(daeElement* element, daeString newID);
	virtual daeInt changeElementSID(daeElement* element, daeString newSID); // Not implemented
	virtual daeInt clear();

	virtual std::vector<daeElement*> idLookup(const std::string& id);

	virtual void typeLookup(daeInt typeID,
	                        std::vector<daeElement*>& matchingElements,
	                        daeDocument* doc = NULL);

	// Currently not implemented, but you can uncomment some code in daeSTLDatabase.cpp to get
	// it working.
	virtual void sidLookup(const std::string& sid,
	                       std::vector<daeElement*>& matchingElements,
	                       daeDocument* doc = NULL);

	// Deprecated. Don't use these. Use idLookup or typeLookup instead.
	virtual daeUInt getElementCount(daeString name = NULL,
	                                daeString type = NULL,
	                                daeString file = NULL);
	virtual daeInt getElement(daeElement** pElement, 
	                          daeInt index,
	                          daeString name = NULL,
	                          daeString type = NULL,
	                          daeString file = NULL);

private:

	std::map< std::string, std::vector< daeElement* > > elements; // type name --> element lookup table (deprecated)

	std::multimap<daeInt, daeElement*> typeMap; // type ID --> element lookup table
	typedef std::multimap<daeInt, daeElement*>::iterator typeMapIter;
	typedef std::pair<daeInt, daeElement*> typeMapPair;
	typedef std::pair<typeMapIter, typeMapIter> typeMapRange;

	std::multimap< std::string, daeElement* > elementsIDMap; //map for elements keyed on ID
	typedef std::multimap<std::string, daeElement*>::iterator idMapIter;
	typedef std::pair<std::string, daeElement*> idMapPair;
	typedef std::pair<idMapIter, idMapIter> idMapRange;

	std::multimap< std::string, daeElement* > sidMap; // sid --> element lookup table
	typedef std::multimap<std::string, daeElement*>::iterator sidMapIter;
	typedef std::pair<std::string, daeElement*> sidMapPair;
	typedef std::pair<sidMapIter, sidMapIter> sidMapRange;

	std::vector<daeDocument*> documents;
	daeMetaElement* topMeta;

	daeInt insertChildren( daeDocument *c, daeElement *element );
	daeInt removeChildren( daeDocument *c, daeElement *element );
};

#endif // __DAE_STLDATABASE__
