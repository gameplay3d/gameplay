/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 

#ifndef __domP_h__
#define __domP_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

class DAE;

/**
 * The p element represents primitive data for the primitive types (lines,
 * linestrips, polygons,  polylist, triangles, trifans, tristrips). The p
 * element contains indices that reference into  the parent's source elements
 * referenced by the input elements.
 */
class domP : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::P; }
	static daeInt ID() { return 617; }
	virtual daeInt typeID() const { return ID(); }

protected:  // Value
	/**
	 * The domListOfUInts value of the text data of this element. 
	 */
	domListOfUInts _value;

public:	//Accessors and Mutators
	/**
	 * Gets the _value array.
	 * @return Returns a domListOfUInts reference of the _value array.
	 */
	domListOfUInts &getValue() { return _value; }
	/**
	 * Gets the _value array.
	 * @return Returns a constant domListOfUInts reference of the _value array.
	 */
	const domListOfUInts &getValue() const { return _value; }
	/**
	 * Sets the _value array.
	 * @param val The new value for the _value array.
	 */
	void setValue( const domListOfUInts &val ) { _value = val; }

protected:
	/**
	 * Constructor
	 */
	domP(DAE& dae) : daeElement(dae), _value() {}
	/**
	 * Destructor
	 */
	virtual ~domP() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domP &operator=( const domP &cpy ) { (void)cpy; return *this; }

public: // STATIC METHODS
	/**
	 * Creates an instance of this class and returns a daeElementRef referencing it.
	 * @return a daeElementRef referencing an instance of this object.
	 */
	static DLLSPEC daeElementRef create(DAE& dae);
	/**
	 * Creates a daeMetaElement object that describes this element in the meta object reflection framework.
	 * If a daeMetaElement already exists it will return that instead of creating a new one. 
	 * @return A daeMetaElement describing this COLLADA element.
	 */
	static DLLSPEC daeMetaElement* registerElement(DAE& dae);
};


#endif
