#ifndef __domCurve_h__
#define __domCurve_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domLine.h>
#include <dom/domCircle.h>
#include <dom/domEllipse.h>
#include <dom/domParabola.h>
#include <dom/domHyperbola.h>
#include <dom/domNurbs.h>
#include <dom/domOrient.h>
#include <dom/domOrigin.h>
class DAE;

/**
 * A curveType defines the attributes of a curve element.     With rotate
 * and translate the surface can be positioned     to its right location.
 */
class domCurve : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::CURVE; }
	static daeInt ID() { return 349; }
	virtual daeInt typeID() const { return ID(); }
protected:  // Attributes
/**
 * The id of a curve.
 */
	domSid attrSid;
/**
 *       The name of a curve.     
 */
	xsToken attrName;

protected:  // Elements
	domLineRef elemLine;
	domCircleRef elemCircle;
	domEllipseRef elemEllipse;
	domParabolaRef elemParabola;
	domHyperbolaRef elemHyperbola;
	domNurbsRef elemNurbs;
	domOrient_Array elemOrient_array;
	domOriginRef elemOrigin;
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
	 * Gets the sid attribute.
	 * @return Returns a domSid of the sid attribute.
	 */
	domSid getSid() const { return attrSid; }
	/**
	 * Sets the sid attribute.
	 * @param atSid The new value for the sid attribute.
	 */
	void setSid( domSid atSid ) { *(daeStringRef*)&attrSid = atSid;}

	/**
	 * Gets the name attribute.
	 * @return Returns a xsToken of the name attribute.
	 */
	xsToken getName() const { return attrName; }
	/**
	 * Sets the name attribute.
	 * @param atName The new value for the name attribute.
	 */
	void setName( xsToken atName ) { *(daeStringRef*)&attrName = atName;}

	/**
	 * Gets the line element.
	 * @return a daeSmartRef to the line element.
	 */
	const domLineRef getLine() const { return elemLine; }
	/**
	 * Gets the circle element.
	 * @return a daeSmartRef to the circle element.
	 */
	const domCircleRef getCircle() const { return elemCircle; }
	/**
	 * Gets the ellipse element.
	 * @return a daeSmartRef to the ellipse element.
	 */
	const domEllipseRef getEllipse() const { return elemEllipse; }
	/**
	 * Gets the parabola element.
	 * @return a daeSmartRef to the parabola element.
	 */
	const domParabolaRef getParabola() const { return elemParabola; }
	/**
	 * Gets the hyperbola element.
	 * @return a daeSmartRef to the hyperbola element.
	 */
	const domHyperbolaRef getHyperbola() const { return elemHyperbola; }
	/**
	 * Gets the nurbs element.
	 * @return a daeSmartRef to the nurbs element.
	 */
	const domNurbsRef getNurbs() const { return elemNurbs; }
	/**
	 * Gets the orient element array.
	 * @return Returns a reference to the array of orient elements.
	 */
	domOrient_Array &getOrient_array() { return elemOrient_array; }
	/**
	 * Gets the orient element array.
	 * @return Returns a constant reference to the array of orient elements.
	 */
	const domOrient_Array &getOrient_array() const { return elemOrient_array; }
	/**
	 * Gets the origin element.
	 * @return a daeSmartRef to the origin element.
	 */
	const domOriginRef getOrigin() const { return elemOrigin; }
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
	domCurve(DAE& dae) : daeElement(dae), attrSid(), attrName(), elemLine(), elemCircle(), elemEllipse(), elemParabola(), elemHyperbola(), elemNurbs(), elemOrient_array(), elemOrigin() {}
	/**
	 * Destructor
	 */
	virtual ~domCurve() { daeElement::deleteCMDataArray(_CMData); }
	/**
	 * Overloaded assignment operator
	 */
	virtual domCurve &operator=( const domCurve &cpy ) { (void)cpy; return *this; }

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
