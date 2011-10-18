/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 

#ifndef __DAE_META_GROUP_H__
#define __DAE_META_GROUP_H__

#include <dae/daeMetaCMPolicy.h>

class daeMetaElementAttribute;

/**
 * The daeMetaGroup class defines the behavior of an xs:group ref content model from the COLLADA Schema.
 */
class daeMetaGroup : public daeMetaCMPolicy
{
public:
	/**
	 * Constructor.
	 * @param econ The daeMetaElementAttribute that represents the group element in the parent.
	 * @param container The daeMetaElement that this policy object belongs to.
	 * @param parent The daeMetaCMPolicy parent of this policy object.
	 * @param odinal The ordinal value offset of this specific policy object. Used for maintaining the 
	 * correct order of child elements.
	 * @param minO The minimum number of times this CMPolicy object must appear. This value comes from the COLLADA schema.
	 * @param maxO The maximum number of times this CMPolicy object may appear. This value comes from the COLLADA schema.
	 */
	daeMetaGroup( daeMetaElementAttribute *econ, daeMetaElement *container, daeMetaCMPolicy *parent = NULL, 
					daeUInt ordinal = 0, daeInt minO = 1, daeInt maxO = 1 );
	
	/**
	 * Destructor.
	 */
	~daeMetaGroup();

	daeElement *placeElement( daeElement *parent, daeElement *child, daeUInt &ordinal, daeInt offset = 0, daeElement* before = NULL, daeElement *after = NULL );
	daeBool removeElement(daeElement* parent, daeElement* child);
	daeMetaElement *findChild( daeString elementName );
	void getChildren( daeElement* parent, daeElementRefArray &array );

protected:
	daeMetaElementAttribute *_elementContainer;
};

#endif

