/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 

#ifndef __DAE_STANDARD_URI_RESOLVER__
#define __DAE_STANDARD_URI_RESOVLER__

#include <string>
#include "dae/daeURI.h"
class DAE;

/**
 * The @c daeStandardURIResolver class derives from @c daeURIResolver and implements
 * the default XML backend resolver.
 */
class daeStandardURIResolver : public daeURIResolver
{
public:
	/**
	 * Constructor.
	 * @param database The @c daeDatabase used.
	 * @param plugin The @c daeIOPlugin used.
	 */
	DLLSPEC daeStandardURIResolver(DAE& dae);
	/**
	 * Destructor.
	 */
	DLLSPEC ~daeStandardURIResolver();

public: // Abstract Interface
	virtual DLLSPEC daeElement* resolveElement(const daeURI& uri);
	virtual DLLSPEC daeString getName();
};

#endif //__DAE_STANDARD_URI_RESOLVER__
