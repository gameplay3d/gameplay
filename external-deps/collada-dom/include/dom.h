/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 
#ifndef __DOM__
#define __DOM__

class DAE;
class daeMetaElement;

extern daeString COLLADA_VERSION;
extern daeString COLLADA_NAMESPACE;

// Register all types
void registerDomTypes(DAE& dae);

// Register all elements
daeMetaElement* registerDomElements(DAE& dae);


#endif // __DOM_INTERFACE__
