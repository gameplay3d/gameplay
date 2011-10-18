/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 

#ifndef __DAE__ERROR__
#define __DAE__ERROR__

#include <dae/daePlatform.h>

/** Success */
#define DAE_OK 0 
/** Fatal Error, should never be returned unless there is a bug in the library. */
#define DAE_ERR_FATAL -1
/** Call invalid, the combination of parameters given is invalid. */
#define DAE_ERR_INVALID_CALL -2
/** Generic error */
#define DAE_ERROR -3
/** IO error, the file hasn't been found or there is a problem with the IO plugin. */
#define DAE_ERR_BACKEND_IO -100
/** The IOPlugin backend wasn't able to successfully validate the data. */
#define DAE_ERR_BACKEND_VALIDATION -101
/** The IOPlugin tried to write to a file that already exists and the "replace" parameter was set to false */
#define DAE_ERR_BACKEND_FILE_EXISTS -102
/** Error in the syntax of the query. */
#define DAE_ERR_QUERY_SYNTAX -200
/** No match to the search criteria. */
#define DAE_ERR_QUERY_NO_MATCH -201
/** A document with that name already exists. */
#define DAE_ERR_COLLECTION_ALREADY_EXISTS -202
/** A document with that name does not exist. */
#define DAE_ERR_COLLECTION_DOES_NOT_EXIST -203
/** Function is not implemented. */
#define DAE_ERR_NOT_IMPLEMENTED -1000
		

/** Gets the ASCII error string.  
* @param errorCode Error code returned by a function of the API.
* @return Returns an English string describing the error.
*/
DLLSPEC const char *daeErrorString(int errorCode);

#endif //__DAE__ERROR__
