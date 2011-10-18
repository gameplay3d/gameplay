/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 

#ifndef __DAE_TINYXMLPLUGIN__
#define __DAE_TINYXMLPLUGIN__

#include <vector>
#include <list>
#include <dae/daeElement.h>
#include <dae/daeURI.h>
#include <dae/daeIOPluginCommon.h>

class TiXmlDocument;
class TiXmlElement;

class daeTinyXMLPlugin : public daeIOPluginCommon
{
public:
	// Constructor / destructor
	/**
	 * Constructor.
	 */
	DLLSPEC daeTinyXMLPlugin();
	/**
	 * Destructor.
	 */
	virtual DLLSPEC ~daeTinyXMLPlugin();

	// Operations
	virtual DLLSPEC daeInt write(const daeURI& name, daeDocument *document, daeBool replace);

	/**
	 * setOption allows you to set options for this IOPlugin. Which options a plugin supports is
	 * dependent on the plugin itself. There is currently no list of options that plugins are
	 * suggested to implement. daeLibXML2Plugin supports only one option, "saveRawBinary". Set to 
	 * "true" to save float_array data as a .raw binary file. The daeRawResolver will convert the 
	 * data back into COLLADA domFloat_array elements upon load.
	 * @param option The option to set.
	 * @param value The value to set the option.
	 * @return Returns DAE_OK upon success.
	 */
	virtual DLLSPEC daeInt setOption( daeString option, daeString value );

	/**
	 * getOption retrieves the value of an option from this IOPlugin. Which options a plugin supports is
	 * dependent on the plugin itself.
	 * @param option The option to get.
	 * @return Returns the string value of the option or NULL if option is not valid.
	 */
	virtual DLLSPEC daeString getOption( daeString option );
	
private:
	TiXmlDocument*  m_doc;
	std::list<TiXmlElement*>  m_elements;

	virtual daeElementRef readFromFile(const daeURI& uri);
	virtual daeElementRef readFromMemory(daeString buffer, const daeURI& baseUri);
	daeElementRef readElement(TiXmlElement* tinyXmlElement, daeElement* parentElement);

	void writeElement( daeElement* element ); 
	void writeAttribute( daeMetaAttribute* attr, daeElement* element );
	void writeValue( daeElement* element );
};

#endif //__DAE_TINYXMLPLUGIN__
