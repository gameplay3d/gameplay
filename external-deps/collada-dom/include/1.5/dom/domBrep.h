#ifndef __domBrep_h__
#define __domBrep_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domCurves.h>
#include <dom/domSurface_curves.h>
#include <dom/domSurfaces.h>
#include <dom/domSource.h>
#include <dom/domVertices.h>
#include <dom/domEdges.h>
#include <dom/domWires.h>
#include <dom/domFaces.h>
#include <dom/domPcurves.h>
#include <dom/domShells.h>
#include <dom/domSolids.h>
#include <dom/domExtra.h>
class DAE;

/**
 * The brep element contains the complete topological     description of a
 * static structure. There are also the     corresponding geometrical descriptions
 * of the vertices,     edges and faces.
 */
class domBrep : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::BREP; }
	static daeInt ID() { return 363; }
	virtual daeInt typeID() const { return ID(); }

protected:  // Elements
/**
 * The curves element holds all the curves that are needed for the brep structure.
 * Here are the curves that describes the kind of an edge, but here are also
 * the curves that are needed to create a extrusion for a surface. This element
 * is needed, if the edges element is present. @see domCurves
 */
	domCurvesRef elemCurves;
	domSurface_curvesRef elemSurface_curves;
/**
 * The surfaces element holds all the surfaces that are needed for the brep
 * structure. Here are the surfaces that describes the kind of a face. This
 * element is needed, if the faces element is present. @see domSurfaces
 */
	domSurfacesRef elemSurfaces;
/**
 * The source elements define the access of the elements vertices, edges and
 * faces to their geometric entities. At least one source element is needed
 * for the vertices. If there are edges a second source element is needed
 * for accessing the curves in the curve element by an IDREF_array. If there
 * are faces the third source element is needed for accessing the surfaces
 * in the surface element by an IDREF_array. @see domSource
 */
	domSource_Array elemSource_array;
/**
 * This element defines all the vertices of an brep structure. Vertices are
 * the base topological entity for all brep structures, so this element is
 * ever needed. @see domVertices
 */
	domVerticesRef elemVertices;
/**
 * This element defines all the edges of the brep structure. @see domEdges
 */
	domEdgesRef elemEdges;
/**
 * This element defines all the wires of the brep structure. @see domWires
 */
	domWiresRef elemWires;
/**
 * This element defines all the faces of the brep structure. @see domFaces
 */
	domFacesRef elemFaces;
	domPcurvesRef elemPcurves;
/**
 * This element defines all the shells of the brep structure. @see domShells
 */
	domShellsRef elemShells;
/**
 * This element defines all the solids of the brep structure. @see domSolids
 */
	domSolidsRef elemSolids;
	domExtra_Array elemExtra_array;

public:	//Accessors and Mutators
	/**
	 * Gets the curves element.
	 * @return a daeSmartRef to the curves element.
	 */
	const domCurvesRef getCurves() const { return elemCurves; }
	/**
	 * Gets the surface_curves element.
	 * @return a daeSmartRef to the surface_curves element.
	 */
	const domSurface_curvesRef getSurface_curves() const { return elemSurface_curves; }
	/**
	 * Gets the surfaces element.
	 * @return a daeSmartRef to the surfaces element.
	 */
	const domSurfacesRef getSurfaces() const { return elemSurfaces; }
	/**
	 * Gets the source element array.
	 * @return Returns a reference to the array of source elements.
	 */
	domSource_Array &getSource_array() { return elemSource_array; }
	/**
	 * Gets the source element array.
	 * @return Returns a constant reference to the array of source elements.
	 */
	const domSource_Array &getSource_array() const { return elemSource_array; }
	/**
	 * Gets the vertices element.
	 * @return a daeSmartRef to the vertices element.
	 */
	const domVerticesRef getVertices() const { return elemVertices; }
	/**
	 * Gets the edges element.
	 * @return a daeSmartRef to the edges element.
	 */
	const domEdgesRef getEdges() const { return elemEdges; }
	/**
	 * Gets the wires element.
	 * @return a daeSmartRef to the wires element.
	 */
	const domWiresRef getWires() const { return elemWires; }
	/**
	 * Gets the faces element.
	 * @return a daeSmartRef to the faces element.
	 */
	const domFacesRef getFaces() const { return elemFaces; }
	/**
	 * Gets the pcurves element.
	 * @return a daeSmartRef to the pcurves element.
	 */
	const domPcurvesRef getPcurves() const { return elemPcurves; }
	/**
	 * Gets the shells element.
	 * @return a daeSmartRef to the shells element.
	 */
	const domShellsRef getShells() const { return elemShells; }
	/**
	 * Gets the solids element.
	 * @return a daeSmartRef to the solids element.
	 */
	const domSolidsRef getSolids() const { return elemSolids; }
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
protected:
	/**
	 * Constructor
	 */
	domBrep(DAE& dae) : daeElement(dae), elemCurves(), elemSurface_curves(), elemSurfaces(), elemSource_array(), elemVertices(), elemEdges(), elemWires(), elemFaces(), elemPcurves(), elemShells(), elemSolids(), elemExtra_array() {}
	/**
	 * Destructor
	 */
	virtual ~domBrep() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domBrep &operator=( const domBrep &cpy ) { (void)cpy; return *this; }

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
