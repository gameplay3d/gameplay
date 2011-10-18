/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 

#ifndef __DAE_RAWRESOLVER_H__
#define __DAE_RAWRESOLVER_H__

#include <string>
#include <map>
#include <dae/daeURI.h>
class DAE;

/**
 * The @c daeRawResolver class derives from @c daeURIResolver and implements
 * the .raw backend resolver for raw binary data.
 */
class DLLSPEC daeRawResolver : public daeURIResolver
{
public:
	/**
	 * Constructor.
	 */
	daeRawResolver(DAE& dae);
	/**
	 * Destructor.
	 */
	~daeRawResolver();

public: // Abstract Interface
	virtual daeElement* resolveElement(const daeURI& uri);
	virtual daeString getName();
};

// A simple class to make speed up the process of resolving a .raw URI.
// The result of the resolve is cached for future use.
// This is meant for DOM internal use only.
class DLLSPEC daeRawRefCache {
public:
	daeRawRefCache() { lookupTable = new std::map<std::string, daeElement*>(); }
	~daeRawRefCache() { delete lookupTable; }

	daeElement* lookup(const daeURI& uri);
	void add(const daeURI& uri, daeElement* elt);
	void remove(const daeURI& uri);
	void clear();

private:
	std::map<std::string, daeElement*> * lookupTable;
};

#endif
