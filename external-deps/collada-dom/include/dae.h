/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 

#ifndef __DAE__
#define __DAE__

// We use the boost filesystem library for cross-platform file system support. You'll need
// to have boost on your machine for this to work. For the Windows build boost is provided
// in the external-libs folder, but for Linux it's expected that you'll install a boost
// obtained via your distro's package manager. For example on Debian/Ubuntu, you can run
//   apt-get install libboost-filesystem-dev
// to install the boost filesystem library on your machine.
//
// Disable the warnings we get from Boost
// warning C4180: qualifier applied to function type has no meaning; ignored
// warning C4245: 'argument' : conversion from 'int' to 'boost::filesystem::system_error_type', 
//   signed/unsigned mismatch
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4180 4245)
#endif
#ifndef NO_BOOST
#include <boost/filesystem/convenience.hpp>
#endif
#ifdef _MSC_VER
#pragma warning(pop)
#endif

#include <dae/daeTypes.h>
#include <dae/daeError.h>
#include <dae/daeDatabase.h>
#include <dae/daeIOPlugin.h>
#include <dae/daeAtomicType.h>
#include <dae/daeMetaElement.h>
#include <dae/daeIDRef.h>
#include <dae/daeURI.h>
#include <dae/daeUtils.h>
#include <dae/daeRawResolver.h>
#include <dae/daeSIDResolver.h>

class domCOLLADA;
typedef daeSmartRef<domCOLLADA> domCOLLADARef;
class daeDatabase;

// The DAE class is the core interface via which you interact with the DOM. It
// has methods to load/save documents, get the root element of each document,
// etc. Although internally the DOM works exclusively with URIs, the methods of
// the DAE class that take document paths can take URIs or OS-specific file
// paths.
class DLLSPEC DAE
{
public:
	// Constructor. If no database or IO plugin are provided, a default database and
	// IO plugin will be used.
	DAE(daeDatabase* database = NULL, daeIOPlugin* ioPlugin = NULL)
	  : atomicTypes(*this),
	    baseUri(*this, cdom::getCurrentDirAsUri().c_str())
	{
		// See the end of the thread linked below for an explanation of why we have the DAE
		// constructor set up this way. Basically, I'm going to be changing the build output 
		// location, and when this happens people sometimes continue to link against the old
		// libraries by accident (e.g. if they just do an svn update). By introducing a new
		// function that gets called from a function in a header file, I'm ensuring that someone
		// who tries linking against old libraries will get a link error. This may not sound
		// very nice, but it's certainly better than getting bizarre runtime crashes.
		// https://collada.org/public_forum/viewtopic.php?t=771&sid=f13c34f2d17ca720c5021bccbe5128b7
		init(database, ioPlugin);
		dummyFunction1();
	}

	virtual ~DAE();

	// Release all memory used by the DOM. You never need to call this explicitly. It's
	// called automatically when all DAE objects go out of scope.
    // Deletes directory returned by cdom::getSafeTmpDir().
	static void cleanup();
	
public:
	// Database setup	
	virtual daeDatabase* getDatabase();
	virtual daeInt setDatabase(daeDatabase* database);

	// IO Plugin setup
	virtual daeIOPlugin* getIOPlugin();
	virtual daeInt setIOPlugin(daeIOPlugin* plugin);

	// Creates a new document, returning null on failure.
	virtual domCOLLADA* add(const std::string& path);
	// Opens an existing document, returning null on failure.
	virtual domCOLLADA* open(const std::string& path);
	// Opens a document from memory, returning null on failure.
	virtual domCOLLADA* openFromMemory(const std::string& path, daeString buffer);
	// Write a document to the path specified by the document's URI, returning false on failure.
	virtual bool write(const std::string& path);
	// Write a document to the path specified in the second parameter, returning false on failure.
	virtual bool writeTo(const std::string& docPath, const std::string& pathToWriteTo);
	// Writes all documents, returning false if any document failed to write.
	virtual bool writeAll();
	// Close a specific document, unloading all memory used by the document. Returns false on failure.
	virtual void close(const std::string& path);
	// Remove all loaded documents. Always returns DAE_OK.
	virtual daeInt clear();

	// Returns the total number of documents.
	virtual int getDocCount();
	// Returns the i'th document .
	virtual daeDocument* getDoc(int i);
	// Returns a document matching the path.
	virtual daeDocument* getDoc(const std::string& path);
	
	// Get the root domCOLLADA object corresponding to a particular document.
	virtual domCOLLADA* getRoot(const std::string& path);
	// Set the root domCOLLADA object corresponding to a particular document, returning false on failure.
	virtual bool        setRoot(const std::string& path, domCOLLADA* root);

	// Returns the Collada version, i.e. 1.4, 1.5, etc. Note that this _isn't_ the
	// same as the DOM version (1.3, 2.0, ...).
	virtual daeString getDomVersion();

	// Returns the (modifiable) list of atomic type objects.
	daeAtomicTypeList& getAtomicTypes();

	// Get/set a daeMetaElement object given the meta object's type ID.
	daeMetaElement* getMeta(daeInt typeID);
	void setMeta(daeInt typeID, daeMetaElement& meta);

	// Get all daeMetaElement objects.
	daeMetaElementRefArray& getAllMetas();

	// Returns the list of URI resolvers. You can modify the list to add new resolvers.
	daeURIResolverList& getURIResolvers();

	// The base URI used for resolving relative URI references.
	daeURI& getBaseURI();
	void setBaseURI(const daeURI& uri);
	void setBaseURI(const std::string& uri);

	// Returns the list of ID reference resolvers. You can modify the list to add new
	// resolvers.
	daeIDRefResolverList& getIDRefResolvers();

	// Meant for internal DOM use only.
	daeRawRefCache& getRawRefCache();
	daeSidRefCache& getSidRefCache();

	// These functions specify the client's character encoding for the DOM. The
	// default is Utf8, but if you specify Latin1 then the DOM will use libxml's
	// character conversion functions to convert to Utf8 when writing data and
	// convert to Latin1 when reading data. This can help with the handling of
	// non-ASCII characters on Windows. Only when using libxml for xml I/O does
	// any character conversion occur.
	//
	// Most people can probably just ignore this completely. If you have trouble
	// with non-ASCII characters on Windows, try setting the char encoding to
	// Latin1 to see if that helps.
	//
	// Frankly this certainly isn't the best way of handling non-ASCII character
	// support on Windows, so this interface is a likely target for significant
	// changes in the future.
	//
	// See this Sourceforge thread for more info:
	// http://sourceforge.net/tracker/index.php?func=detail&aid=1818473&group_id=157838&atid=805426
	//
	enum charEncoding {
		Utf8,
		Latin1
	};

	// Global encoding setting. Defaults to Utf8. Set this if you want to make a
	// char encoding change and apply it to all DAE objects.
	static charEncoding getGlobalCharEncoding();
	static void setGlobalCharEncoding(charEncoding encoding);

	// Local encoding setting. If set, overrides the global setting. Useful for setting
	// a specific char encoding for a single DAE object but not for all DAE objects.
	charEncoding getCharEncoding();
	void setCharEncoding(charEncoding encoding);

	// Deprecated. Alternative methods are given.
	virtual daeInt load(daeString uri, daeString docBuffer = NULL); // Use open
	virtual daeInt save(daeString uri, daeBool replace=true); // Use write
	virtual daeInt save(daeUInt documentIndex, daeBool replace=true); // Use write
	virtual daeInt saveAs(daeString uriToSaveTo, daeString docUri, daeBool replace=true); // Use writeTo
	virtual daeInt saveAs(daeString uriToSaveTo, daeUInt documentIndex=0, daeBool replace=true); // Use writeTo
	virtual daeInt unload(daeString uri); // Use close
	virtual domCOLLADA* getDom(daeString uri); // use getRoot
	virtual daeInt      setDom(daeString uri, domCOLLADA* dom); // use setRoot

private:
	void init(daeDatabase* database, daeIOPlugin* ioPlugin);
	void dummyFunction1();
	std::string makeFullUri(const std::string& path);
	domCOLLADA* openCommon(const std::string& path, daeString buffer);
	bool writeCommon(const std::string& docPath, const std::string& pathToWriteTo, bool replace);

	daeDatabase *database;
	daeIOPlugin *plugin;
	bool defaultDatabase;
	bool defaultPlugin;
	daeAtomicTypeList atomicTypes;
	daeMetaElementRefArray metas;
	daeURI baseUri;
	daeURIResolverList uriResolvers;
	daeIDRefResolverList idRefResolvers;
	daeRawRefCache rawRefCache;
	daeSidRefCache sidRefCache;

	std::auto_ptr<charEncoding> localCharEncoding;
	static charEncoding globalCharEncoding;
};


template <typename T> 
inline T *daeSafeCast(daeElement *element)
{ 
	if (element  &&  element->typeID() == T::ID())
		return (T*)element; 
	return NULL; 
}


#endif // __DAE_INTERFACE__
