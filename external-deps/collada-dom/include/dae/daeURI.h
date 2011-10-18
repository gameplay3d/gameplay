/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 

#ifndef __DAE_URI_H__
#define __DAE_URI_H__

#include <string>
#include <dae/daeTypes.h>
#include <dae/daeElement.h>
#include <dae/daeUtils.h>
class DAE;

/**
 * The @c daeURI is a simple class designed to aid in the parsing and resolution
 * of URI references inside COLLADA elements.
 * A @c daeURI is created for every @c anyURL and @c IDREF in the COLLADA schema.
 * For example, the <instance> element has the url= attribute of type @c anyURL, and the 
 * <controller> element has the target= attribute of type @c IDREF.
 * The @c daeURI class contains a URI string; the @c set() method breaks the string into
 * its components including scheme, authority, path (directory), and fragment.
 * It also has the capability to attempt to resolve this reference
 * into a @c daeElement, through the method @c resolveElement().  
 * If a @c daeURI is stored within a @c daeElement, it fills
 * its container field to point to the containing element.
 *
 * The main API on the @c daeURI, @c resolveElement(), uses a @c daeURIResolver
 * to search for the @c daeElement inside a @c daeDatabase.
 *
 * URIs are resolved hierarchically, where each URI is resolved based on
 * the following criteria via itself and its element's base URI, which represents the
 * URI of the document that contains the element, retrieved by 
 * <tt>daeElement::getBaseURI().</tt>
 * If no base URI is provided, then the application URI
 * is used as a base.
 *
 * The URI resolution order for the COLLADA DOM is as follows:
 * - Absolute URI is specified (see definition below):
 *   The URI ignores its parent/base URI when validating. 
 * - Relative URI is specified:
 *   The URI uses the base URI to provide the scheme, authority, and base path.
 *    This URI's path is appended to the path given the the base URI.
 *    This URI's file and ID are used.
 * - Each level of URI is resolved in this way against the base URI of the
 *    containing file until the top level is reached.  Then the application URI
 *    is used as the default.
 *
 * <b>Definition of Absolute URI:</b>  
 * For the purposes of the COLLADA DOM, a URI is considered absolute
 * if it starts by specifying a scheme.
 * For example, 
 * - file:///c:/data/foo.dae#myScene is an absolute URI.
 * - foo.dae#myScene is relative.
 * - foo.dae is a top-level file reference and is relative.
 * If the URI does not include a pound sign (#), the <tt><i>fragment</i></tt> is empty.
 */
class DLLSPEC daeURI
{
private:
	daeElement* internalResolveElement() const;
	
public:
	/**
	 * An enum describing the status of the URI resolution process.
	 * This is pretty much entirely useless now. Just use the various accessors
	 * to query the state of the uri.
	 */
	enum ResolveState{
		/** No URI specified */
		uri_empty,
		/** URI specified but unresolved */
		uri_loaded,
		/** Resolution pending */
		uri_pending,
		/** Resolution successful */
		uri_success,
		/** Failure due to unsupported URI scheme */
		uri_failed_unsupported_protocol,
		/** Failure because the file was not found */
		uri_failed_file_not_found,
		/** Failure because the fragment was not found */
		uri_failed_id_not_found,
		/** Failure due to an invalid fragment */
		uri_failed_invalid_id,
		/** A flag specifying that the URI should be resolved locally to its own document */
		uri_resolve_local,
		/** A flag specifying that the URI should be resolved using this relative URI */
		uri_resolve_relative,
		/** A flag specifying that the URI should be resolved using this absolute URI */
		uri_resolve_absolute,
		/** Failure due to an invalid reference */
		uri_failed_invalid_reference,
		/** Failure due to an external error */
		uri_failed_externalization,
		/** Failure due to missing document */
		uri_failed_missing_container,
		/** Failure because automatic loading of a document is turned off */
		uri_failed_external_document
	};
	
private:
	// All daeURIs have a pointer to a master DAE that they use to access global information.
	mutable DAE* dae;
	
	/** Resolved version of the URI */
	std::string uriString;

	/** Original URI before resolution */
	std::string originalURIString;
	
	/** scheme component */
	std::string _scheme;
	/** authority component */
	std::string _authority;
	/** path component */
	std::string _path;
	/** query component */
	std::string _query;
	/** fragment component */
	std::string _fragment;
	/** Pointer to the element that owns this URI */
	daeElement* container;
	
public:
	/**
	 * Constructs a daeURI object that contains no URI reference.
	 * @param dae The DAE associated with this daeURI.
	 * current working directory.
	 */
	daeURI(DAE& dae);
	/**
	 * Destructor
	 */
	~daeURI();

	/**
	 * Constructs a daeURI object from a URI passed in as a string.
	 * @param dae The DAE associated with this daeURI.
	 * @param URIString Passed to set() automatically.
	 * @param nofrag If true, the fragment part of the URI is stripped off before construction.
	 */
	daeURI(DAE& dae, const std::string& URIString, daeBool nofrag = false);
	
	/**
	 * Constructs a daeURI object using a <tt><i>baseURI</i></tt> and a <tt><i>uriString.</i></tt> 
	 * Calls set(URIString), and @c validate(baseURI).
	 * @param baseURI Base URI to resolve against.
	 * @param URIString String designating this URI.
	 */
	daeURI(const daeURI& baseURI, const std::string& URIString);

	/**
	 * Constructs a daeURI object based on a simple copy from an existing @c daeURI. 
	 * @param constructFromURI  URI to copy into this one.
	 */
	daeURI(const daeURI& constructFromURI);

	/**
	 * Constructs a daeURI given a container element and a URI string.
	 * @param container The container element.
	 * @param uriString the URI string.
	 */
	daeURI(daeElement& container, const std::string& uriString = "");

	// This constructor is for internal DOM purposes only. For client code, use the constructor
	// that takes only a daeElement instead of this one.
	daeURI(DAE& dae, daeElement& container, const std::string& uriString = "");

	/**
	 * Gets the DAE objects associated with this daeURI.
	 * @return Returns a pointer to the associated DAE. This will never return null.
	 */
	DAE* getDAE() const;

	// Returns the fully resolved URI as a string
	const std::string& str() const;
	// Returns the URI as originally set (i.e. not resolved against the base URI)
	const std::string& originalStr() const;

	// Old C string versions of the previous functions
	daeString getURI() const; // Alias for str()
	daeString getOriginalURI() const; // Alias for originalStr();

	// Setter function for setting the full uri.
	void set(const std::string& uriStr, const daeURI* baseURI = NULL);
	// Setter function for setting the individual uri components.
	void set(const std::string& scheme,
	         const std::string& authority,
	         const std::string& path,
	         const std::string& query,
	         const std::string& fragment,
	         const daeURI* baseURI = NULL);

	// Old C string function. Alias for set().
	void setURI(daeString uriStr, const daeURI* baseURI = NULL);

	// std::string based component accessors.
	const std::string& scheme() const;
	const std::string& authority() const;
	const std::string& path() const;
	const std::string& query() const;
	const std::string& fragment() const;
	const std::string& id() const; // Alias for fragment()

	// Component setter functions. If you're going to be calling multiple setters, as in
	//   uri.path(path);
	//   uri.fragment(frag);
	// it'd be more efficient to call uri.set once instead.
	void scheme(const std::string& scheme);
	void authority(const std::string& authority);
	void path(const std::string& path);
	void query(const std::string& query);
	void fragment(const std::string& fragment);
	void id(const std::string& id); // Alias for uri.fragment(frag)
	
	// Retrieves the individual path components. For example, in a uri of the form
	// file:/folder/file.dae, dir = /folder/, baseName = file, ext = .dae
	void pathComponents(std::string& dir, std::string& baseName, std::string& ext) const;

	// Individual path component accessors. If you need access to multiple path
	// components, calling pathComponents() will be faster.
	std::string pathDir() const;      // daeURI("/folder/file.dae").pathDir() == "/folder/"
	std::string pathFileBase() const; // daeURI("/folder/file.dae").pathFileBase() == "file"
	std::string pathExt() const;      // daeURI("/folder/file.dae").pathExt() == ".dae"
	std::string pathFile() const;     // daeURI("/folder/file.dae").pathFile() == "file.dae"

	// Path component setter.
	void path(const std::string& dir, const std::string& baseName, const std::string& ext);

	// Individual path component setters. If you're going to be calling multiple setters,
	// it'd be more efficient to call set() instead.
	void pathDir(const std::string& dir);
	void pathFileBase(const std::string& baseName);
	void pathExt(const std::string& ext);
	void pathFile(const std::string& file);
	
	// The older C string accessors. Aliases for the std::string based component accessors.
	daeString getScheme() const;
	daeString getProtocol() const; // Alias for getScheme()
	daeString getAuthority() const;
	daeString getPath() const;
	daeString getQuery() const;
	daeString getFragment() const;
	daeString getID() const; // Alias for getFragment()
	// Same as getPath(), but puts the result in the destination buffer. This is only here
	// for backward compatibility. Use getPath() instead.
	daeBool getPath(daeChar* dest, daeInt size) const;

	/** 
	 * Gets the element that this URI resolves to in memory.
	 * @return Returns a ref to the element.
	 */
	daeElementRef getElement() const;

	// Returns the document that this URI references, or null if the document
	// hasn't been loaded yet.
	daeDocument* getReferencedDocument() const;
	
	/**
	 * Gets a pointer to the @c daeElement that contains this URI.
	 * @return Returns the pointer to the containing daeElmement.
	 */
	inline daeElement* getContainer() const {return(container);};

	/**
	 * Sets the pointer to the @c daeElement that contains this URI.
	 * @param cont Pointer to the containing @c daeElmement.
	 */
	void setContainer(daeElement* container);

	/**
	 * Gets if this URI resolves to an element that is not contained in the same document as the URI.
	 * @return Returns true if the URI references an external element. False otherwise.
	 */
	daeBool isExternalReference() const;
	 
	/**
	 * Copies the URI specified in <tt><i>from</i></tt> into @c this.
	 * Performs a simple copy without validating the URI.
	 * @param from URI to copy from.
	 */
	void copyFrom(const daeURI& from);

	/**
	 * Outputs all components of this URI to stderr.
	 * Useful for debugging URIs, this outputs each part of the URI separately.
	 */
	void print();
	
	/**
	 * Makes the "originalURI" in this URI relative to some other uri
	 * @param uri the URI to make "this" relative to.
	 * @note this is experimental and not fully tested, please don't use in critical code yet.
	 */
	int makeRelativeTo(const daeURI* uri);

	/**
	 * Comparison operator.
	 * @return Returns true if URI's are equal.
	 */
	inline bool operator==(const daeURI& other) const {
		return uriString == other.uriString;
	}

	daeURI& operator=(const daeURI& other);
	daeURI& operator=(const std::string& uri);

	// These methods are deprecated.
	void resolveElement(); // Call getElement directly.
	void validate(const daeURI* baseURI = NULL); // Shouldn't ever need to call this.
	ResolveState getState() const; 	// Call getElement to see if resolving succeeded.
	void setState(ResolveState newState); // Don't call this.

private:
	/**
	 * Resets this URI; frees all string references
	 * and returns <tt><i>state</i></tt> to @c empty.
	 */
	void reset();

	/**
	 * Provides a shared initialization for all constructors
	 */
	void initialize();
public:
	/**
	 * Performs RFC2396 path normalization.
	 * @param path Path to be normalized.
	 */
	static void normalizeURIPath(char* path);
};

class daeURIResolver;
typedef daeTArray<daeURIResolver*> daeURIResolverPtrArray;

/**
 * The @c daeURIResolver class is the plugin point for URI resolution.
 * This class is an abstract base class that defines an interface for
 * resolving URIs.
 * Every URI is passed through this list of @c daeURIResolvers for resolution.
 * The list is ordered on a first come, first serve basis, and resolution
 * terminates after any resolver instance resolves the URI.
 */
class DLLSPEC daeURIResolver
{
public:
	/**
	 * Constructor
	 * @param dae The associated dae object.
	 */
	daeURIResolver(DAE& dae);

	/**
	 * Destructor
	 */
	virtual ~daeURIResolver();
	
	/**
	 * Sets a flag that tells the URI resolver whether or not to load a separate document if a URI
	 * being resolved points to one.
	 * @param load Set to true if you want the URI Resolver to automatically load other documents to
	 * resolve URIs.
	 */
	static void setAutoLoadExternalDocuments( daeBool load );

	/**
	 * Gets a flag that tells if the URI resolver is set to load an external document if a URI
	 * being resolved points to one.
	 * @return Returns true if the resolver will automatically load documents to resolve a URI. 
	 * False otherwise.
	 */
	static daeBool getAutoLoadExternalDocuments();

	/**
	 * Provides an abstract interface for converting a @c daeURI into a @c daeElement
	 * @param uri @c daeURI to resolve.
	 * @return Returns the resolved element, or null if resolving failed.
	 * returns false otherwise.
	 */
	virtual daeElement* resolveElement(const daeURI& uri) = 0;

	/**
	 * Gets the name of this resolver.
	 * @return Returns the resolver name as a string.
	 */
	virtual daeString getName() = 0;

protected:
	static daeBool _loadExternalDocuments;
	DAE* dae;
};


// This is a container class for storing a modifiable list of daeURIResolver objects.
class DLLSPEC daeURIResolverList {
public:
	daeURIResolverList();
	~daeURIResolverList();

	daeTArray<daeURIResolver*>& list();
	daeElement* resolveElement(const daeURI& uri);

private:
	// Disabled copy constructor/assignment operator
	daeURIResolverList(const daeURIResolverList& resolverList) { };
	daeURIResolverList& operator=(const daeURIResolverList& resolverList) { return *this; };

	daeTArray<daeURIResolver*> resolvers;
};


// Helper functions for file path <--> URI conversion
namespace cdom {
	// Takes a uri reference and parses it into its components.
	DLLSPEC bool parseUriRef(const std::string& uriRef,
	                         std::string& scheme,
	                         std::string& authority,
	                         std::string& path,
	                         std::string& query,
	                         std::string& fragment);

	// Takes the uri components of a uri ref and combines them.
	//
	// The 'forceLibxmlCompatible' param is meant to work around bugs in the file
	// scheme uri handling of libxml. It causes the function to output a uri
	// that's fully compatible with libxml. It only modifies file scheme uris,
	// since uris with other schemes seem to work fine.
	//
	// The known libxml uri bugs are as follows:
	//   1) libxml won't write files when given file scheme URIs with an empty
	//      authority, as in "file:/home".
	//   2) libxml won't read or write Windows UNC paths represented with the
	//      machine name in the authority, as in "file://otherMachine/folder/file.dae"
	//   3) On Windows, libxml won't read or write paths that don't have a drive
	//      letter, as in "/folder/file.dae".
	DLLSPEC std::string assembleUri(const std::string& scheme,
	                                const std::string& authority,
	                                const std::string& path,
	                                const std::string& query,
	                                const std::string& fragment,
	                                bool forceLibxmlCompatible = false);

	// A wrapper function for calling assembleUri to create a URI that's compatible
	// with libxml.
	DLLSPEC std::string fixUriForLibxml(const std::string& uriRef);

	// This function takes a file path in the OS's native format and converts it to
	// a URI reference. If a relative path is given, a relative URI reference is
	// returned. If an absolute path is given, a relative URI reference containing 
	// a fully specified path is returned. Spaces are encoded as %20. The 'type'
	// parameter indicates the format of the nativePath.
	//
	// Examples - Windows
	//   nativePathToUri("C:\myFolder\myFile.dae") --> "/C:/myFolder/myFile.dae"
	//   nativePathToUri("\myFolder\myFile.dae") --> "/myFolder/myFile.dae"
	//   nativePathToUri("..\myFolder\myFile.dae") --> "../myFolder/myFile.dae"
	//   nativePathToUri("\\otherComputer\myFile.dae") --> "//otherComputer/myFile.dae"
	//
	// Examples - Linux/Mac
	//   nativePathToUri("/myFolder/myFile.dae") --> "/myFolder/myFile.dae"
	//   nativePathToUri("../myFolder/myFile.dae") --> "../myFolder/myFile.dae"
	//   nativePathToUri("/my folder/my file.dae") --> "/my%20folder/my%20file.dae"
	DLLSPEC std::string nativePathToUri(const std::string& nativePath,
	                                    systemType type = getSystemType());

	// This function takes a URI reference and converts it to an OS file path. Conversion
	// can fail if the URI reference is ill-formed, or if the URI contains a scheme other
	// than "file", in which case an empty string is returned. The 'type' parameter
	// indicates the format of the returned native path.
	//
	// Examples - Windows
	//   uriToNativePath("../folder/file.dae") --> "..\folder\file.dae"
	//   uriToNativePath("/folder/file.dae") --> "\folder\file.dae"
	//   uriToNativePath("file:/C:/folder/file.dae") --> "C:\folder\file.dae"
	//   uriToNativePath("file://otherComputer/file.dae") --> "\\otherComputer\file.dae"
	//   uriToNativePath("http://www.slashdot.org") --> "" (it's not a file scheme URI!)
	//
	// Examples - Linux/Mac
	//   uriToNativePath("../folder/file.dae") --> "../folder/file.dae"
	//   uriToNativePath("file:/folder/file.dae") --> "/folder/file.dae"
	//   uriToNativePath("http://www.slashdot.org") --> "" (it's not a file scheme URI!)
	DLLSPEC std::string uriToNativePath(const std::string& uriRef,
	                                    systemType type = getSystemType());

	DLLSPEC std::string filePathToUri(const std::string& filePath); // Alias for nativePathToUri
	DLLSPEC std::string uriToFilePath(const std::string& uriRef); // Alias for uriToNativePath
}

#endif //__DAE_URI_H__
