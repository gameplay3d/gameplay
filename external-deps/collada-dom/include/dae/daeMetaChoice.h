/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 

#ifndef __DAE_META_CHOICE_H__
#define __DAE_META_CHOICE_H__

#include <dae/daeMetaCMPolicy.h>

/**
 * The daeMetaChoice class defines the behavior of an xs:choice content model in the COLLADA Schema.
 */
class daeMetaChoice : public daeMetaCMPolicy
{
public:
	/**
	 * Constructor.
	 * @param container The daeMetaElement that this policy object belongs to.
	 * @param parent The daeMetaCMPolicy parent of this policy object.
	 * @param choiceNum An unsigned integer that represents which index in an element's CMData array coresponds to this choice's data.
	 * @param odinal The ordinal value offset of this specific policy object. Used for maintaining the 
	 * correct order of child elements.
	 * @param minO The minimum number of times this CMPolicy object must appear. This value comes from the COLLADA schema.
	 * @param maxO The maximum number of times this CMPolicy object may appear. This value comes from the COLLADA schema.
	 */
	daeMetaChoice( daeMetaElement *container, daeMetaCMPolicy *parent = NULL, daeUInt choiceNum = 0, daeUInt ordinal = 0, daeInt minO = 1, daeInt maxO = 1 );
	~daeMetaChoice();

	daeElement *placeElement( daeElement *parent, daeElement *child, daeUInt &ordinal, daeInt offset = 0, daeElement* before = NULL, daeElement *after = NULL );
	daeBool removeElement(daeElement* parent, daeElement* child);
	daeMetaElement *findChild( daeString elementName );
	void getChildren( daeElement* parent, daeElementRefArray &array );

private:
	daeUInt _choiceNum;
};

#endif

