/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 

#ifndef __DAE_IO_PLUGIN_COMMON__
#define __DAE_IO_PLUGIN_COMMON__

#include <vector>
#include <dae/daeElement.h>
#include <dae/daeURI.h>
#include <dae/daeMetaAttribute.h>
#include <dae/daeIOPlugin.h>

class daeMetaElement;
class daeDocument;

/**
 * The @c daeIOPluginCommon class was created to serve as a base class for the common functionality
 * between the daeLIBXMLPlugin and daeTinyXMLPlugin classes.
 */
class DLLSPEC daeIOPluginCommon : public daeIOPlugin {
public:
	/**
	 * Constructor.
	 */
	daeIOPluginCommon();
	/**
	 * Destructor.
	 */
	virtual ~daeIOPluginCommon();

	virtual daeInt setMeta(daeMetaElement *topMeta);

	// Database setup	
	virtual void setDatabase(daeDatabase* database);

	// Operations
	virtual daeInt read(const daeURI& uri, daeString docBuffer);

protected:
	daeDatabase* database;

	// On failure, these functions return NULL
	virtual daeElementRef readFromFile(const daeURI& uri) = 0;
	virtual daeElementRef readFromMemory(daeString buffer, const daeURI& baseUri) = 0;

	// Reading support for subclasses
	typedef std::pair<daeString, daeString> attrPair;
	daeElementRef beginReadElement(daeElement* parentElement, 
	                               daeString elementName, 
	                               const std::vector<attrPair>& attributes,
	                               daeInt lineNumber);
	bool readElementText(daeElement* element, daeString text, daeInt elementLineNumber);

private:
	daeMetaElement* topMeta;
};

#endif //__DAE_IO_PLUGIN_COMMON__
