/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 

#ifndef _DAE_ERROR_HANDLER_
#define _DAE_ERROR_HANDLER_

#include <memory>
#include <dae/daeTypes.h>

/**
 * The @c daeErrorHandler class is a plugin that allows the use to overwrite how error and warning
 * messages get handled in the client application. An example of this would be a class that reports
 * the message to a gui front end instead of just printing on stdout.
 */
class DLLSPEC daeErrorHandler {
public:
	/**
	 * Constructor.
	 */
	daeErrorHandler();
	/**
	 * Destructor.
	 */
	virtual ~daeErrorHandler();

	/**
	 * This function is called when there is an error and a string needs to be sent to the user.
	 * You must overwrite this function in your plugin.
	 * @param msg Error message.
	 */
	virtual void handleError( daeString msg ) = 0;
	/**
	 * This function is called when there is a warning and a string needs to be sent to the user.
	 * You must overwrite this function in your plugin.
	 * @param msg Warning message.
	 */
	virtual void handleWarning( daeString msg ) = 0;

	/**
	 * Sets the daeErrorHandler to the one specified.
	 * @param eh The new daeErrorHandler to use. Passing in NULL results in the default plugin being used.
	 */
	static void setErrorHandler( daeErrorHandler *eh );
	/**
	 * Returns the current daeErrorHandlerPlugin. A program has one globally-accessible
	 * daeErrorHandler active at a time.
	 * @return The current daeErrorHandler.
	 */
	static daeErrorHandler *get();

private:
	static daeErrorHandler *_instance;
	static std::auto_ptr<daeErrorHandler> _defaultInstance;
};

#endif
