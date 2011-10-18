/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 

#ifndef __DAE_IOPLUGIN__
#define __DAE_IOPLUGIN__

#include <string>
#include <vector>
#include <dae/daeTypes.h>
class daeDatabase;
class daeMetaElement;
class daeURI;
class daeDocument;

/**
* The @c daeIOPlugin class provides the input/output plugin interface, which is
* the interface between the COLLADA runtime and the backend storage. A native
* COLLADA XML plugin implementation is provided along with this interface.
*/
class DLLSPEC daeIOPlugin
{
public:	
	/**
	* Destructor
	*/
	virtual ~daeIOPlugin() {}	
	/** 
	* Sets the top meta object.
	* Called by @c dae::setIOPlugin() when the IO plugin changes. It passes to this function the
	* top meta object, which is the root of a 
    * hierarchy of @c daeMetaElement objects. This top meta object is capable of creating
	* any of the root objects in the DOM tree.
	* @param topMeta Top meta object to use to create objects to fill the database.
	* @return Returns DAE_OK if successful, otherwise returns a negative value defined in daeError.h.
	*/
	virtual daeInt setMeta(daeMetaElement *topMeta) = 0;

	/** @name Database setup	 */
	//@{
	/** 
	* Sets the database to use.
	* All @c daeIOPlugins use the same interface to the @c daeDatabase, 
	* @c setDatabase() tells the @c daeIOPlugin which @c daeDatabase object it should use
	* for storage and queries.
	* @param database Database to set.
	*/
	virtual void setDatabase(daeDatabase* database) = 0;
	//@}


	/** @name Operations	 */
	//@{
	/** 
	* Imports content into the database from an input.
	* The input can be a file, a database or another runtime.
	* @param uri the URI of the COLLADA document to load, not all plugins accept all types of URIs,
	* check the documentation for the IO plugin you are using.
	* @param docBuffer A string containing the text of the document to load. This is an optional attribute
	* and should only be used if the document has already been loaded into memory.
	* @return Returns DAE_OK if successfully loaded, otherwise returns a negative value defined in daeError.h.
	* @see @c DAE::load().
	*/
	virtual daeInt read(const daeURI& uri, daeString docBuffer) = 0;

	/** @name Operations	 */
	//@{
	/**
	* Writes a specific document to an output.
	* @param name URI to write the document to, not all IO plugins support all types of URIs
	* check the documentation for the IO plugin you are using.
	* @param document Pointer to the document that we're going to write out.
	* @param replace True if write should overwrite an existing file. False otherwise.
	* @return Returns DAE_OK if success, a negative value defined in daeError.h otherwise.
	* @see @c DAE::saveAs()
	*/
	virtual daeInt write(const daeURI& name, daeDocument *document, daeBool replace) = 0;
	//@}
	
	/**
	 * Returns a list of the URI protocols that this plugin supports.
	 * @return Returns a daeArray containing the supported protocols.
	 */
	virtual const std::vector<std::string>& getSupportedProtocols() {
		return supportedProtocols;
	}

	/**
	 * setOption allows you to set options for this IOPlugin. Which options a plugin supports is
	 * dependent on the plugin itself. There is currently no list of options that plugins are
	 * suggested to implement.
	 * @param option The option to set.
	 * @param value The value to set the option.
	 * @return Returns DAE_OK upon success.
	 */
	virtual daeInt setOption( daeString option, daeString value ) = 0;

	/**
	 * getOption retrieves the value of an option from this IOPlugin. Which options a plugin supports is
	 * dependent on the plugin itself.
	 * @param option The option to get.
	 * @return Returns the string value of the option or NULL if option is not valid.
	 */
	virtual daeString getOption( daeString option ) = 0;

protected:
	// This is an array of the URI protocols supported by this plugin, e.g. "http", "file",
	// etc. Each plugin should initialize this variable in the constructor.
	std::vector<std::string> supportedProtocols;
};


class DLLSPEC daeIOEmpty : public daeIOPlugin {
public:
	virtual daeInt setMeta(daeMetaElement *topMeta) { return DAE_ERROR; }
	virtual void setDatabase(daeDatabase* database) { }
	virtual daeInt read(const daeURI& uri, daeString docBuffer) { return DAE_ERROR; }
	virtual daeInt write(const daeURI& name, daeDocument *document, daeBool replace) { return DAE_ERROR; }
	virtual daeInt setOption( daeString option, daeString value ) { return DAE_ERROR; }
	virtual daeString getOption( daeString option ) { return ""; }
};


#endif // __DAE_IOPLUGIN__
