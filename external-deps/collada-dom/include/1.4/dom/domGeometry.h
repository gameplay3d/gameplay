/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 

#ifndef __domGeometry_h__
#define __domGeometry_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domAsset.h>
#include <dom/domConvex_mesh.h>
#include <dom/domMesh.h>
#include <dom/domSpline.h>
#include <dom/domExtra.h>
class DAE;

/**
 * Geometry describes the visual shape and appearance of an object in the
 * scene. The geometry element categorizes the declaration of geometric information.
 * Geometry is a  branch of mathematics that deals with the measurement, properties,
 * and relationships of  points, lines, angles, surfaces, and solids.
 */
class domGeometry : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::GEOMETRY; }
	static daeInt ID() { return 613; }
	virtual daeInt typeID() const { return ID(); }
protected:  // Attributes
/**
 *  The id attribute is a text string containing the unique identifier of
 * this element.  This value must be unique within the instance document.
 * Optional attribute. 
 */
	xsID attrId;
/**
 *  The name attribute is the text string name of this element. Optional attribute.
 */
	xsNCName attrName;

protected:  // Elements
/**
 *  The geometry element may contain an asset element.  @see domAsset
 */
	domAssetRef elemAsset;
/**
 *  The geometry element may contain only one mesh or convex_mesh.  @see domConvex_mesh
 */
	domConvex_meshRef elemConvex_mesh;
/**
 *  The geometry element may contain only one mesh or convex_mesh.  @see domMesh
 */
	domMeshRef elemMesh;
	domSplineRef elemSpline;
/**
 *  The extra element may appear any number of times.  @see domExtra
 */
	domExtra_Array elemExtra_array;
	/**
	 * Used to preserve order in elements that do not specify strict sequencing of sub-elements.
	 */
	daeElementRefArray _contents;
	/**
	 * Used to preserve order in elements that have a complex content model.
	 */
	daeUIntArray       _contentsOrder;

	/**
	 * Used to store information needed for some content model objects.
	 */
	daeTArray< daeCharArray * > _CMData;


public:	//Accessors and Mutators
	/**
	 * Gets the id attribute.
	 * @return Returns a xsID of the id attribute.
	 */
	xsID getId() const { return attrId; }
	/**
	 * Sets the id attribute.
	 * @param atId The new value for the id attribute.
	 */
	void setId( xsID atId ) { *(daeStringRef*)&attrId = atId; _validAttributeArray[0] = true; 
		if( _document != NULL ) _document->changeElementID( this, attrId );
	}

	/**
	 * Gets the name attribute.
	 * @return Returns a xsNCName of the name attribute.
	 */
	xsNCName getName() const { return attrName; }
	/**
	 * Sets the name attribute.
	 * @param atName The new value for the name attribute.
	 */
	void setName( xsNCName atName ) { *(daeStringRef*)&attrName = atName; _validAttributeArray[1] = true; }

	/**
	 * Gets the asset element.
	 * @return a daeSmartRef to the asset element.
	 */
	const domAssetRef getAsset() const { return elemAsset; }
	/**
	 * Gets the convex_mesh element.
	 * @return a daeSmartRef to the convex_mesh element.
	 */
	const domConvex_meshRef getConvex_mesh() const { return elemConvex_mesh; }
	/**
	 * Gets the mesh element.
	 * @return a daeSmartRef to the mesh element.
	 */
	const domMeshRef getMesh() const { return elemMesh; }
	/**
	 * Gets the spline element.
	 * @return a daeSmartRef to the spline element.
	 */
	const domSplineRef getSpline() const { return elemSpline; }
	/**
	 * Gets the extra element array.
	 * @return Returns a reference to the array of extra elements.
	 */
	domExtra_Array &getExtra_array() { return elemExtra_array; }
	/**
	 * Gets the extra element array.
	 * @return Returns a constant reference to the array of extra elements.
	 */
	const domExtra_Array &getExtra_array() const { return elemExtra_array; }
	/**
	 * Gets the _contents array.
	 * @return Returns a reference to the _contents element array.
	 */
	daeElementRefArray &getContents() { return _contents; }
	/**
	 * Gets the _contents array.
	 * @return Returns a constant reference to the _contents element array.
	 */
	const daeElementRefArray &getContents() const { return _contents; }

protected:
	/**
	 * Constructor
	 */
	domGeometry(DAE& dae) : daeElement(dae), attrId(), attrName(), elemAsset(), elemConvex_mesh(), elemMesh(), elemSpline(), elemExtra_array() {}
	/**
	 * Destructor
	 */
	virtual ~domGeometry() { daeElement::deleteCMDataArray(_CMData); }
	/**
	 * Overloaded assignment operator
	 */
	virtual domGeometry &operator=( const domGeometry &cpy ) { (void)cpy; return *this; }

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
