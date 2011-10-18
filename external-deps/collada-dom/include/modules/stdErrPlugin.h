/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 

#ifndef _STDERR_PLUGIN_
#define _STDERR_PLUGIN_

#include <dae/daeTypes.h>
#include <dae/daeErrorHandler.h>

/**
 * The @c stdErrPlugin class is the default implementation of daeErrorHandler. It routes the Error
 * and Warning messaged to stdout.
 */
class DLLSPEC stdErrPlugin : public daeErrorHandler {
public:
	stdErrPlugin();
	virtual ~stdErrPlugin();

public:
	void handleError( daeString msg );
	void handleWarning( daeString msg );
};

/**
 * The @c quietErrorHandler class is an alternative implementation of daeErrorHandler. It suppresses
 * error and warning messages. The easiest way to use it is like this:
 *   daeErrorHandler::setErrorHandler(&quietErrorHandler::getInstance());
 */
class DLLSPEC quietErrorHandler : public daeErrorHandler {
public:
	quietErrorHandler() { }
	void handleError(daeString msg) { }
	void handleWarning(daeString msg) { }

	static quietErrorHandler& getInstance() { return theInstance; }

private:
	static quietErrorHandler theInstance;
};

#endif
