#ifndef __domNurbs_surface_h__
#define __domNurbs_surface_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domSource.h>
#include <dom/domInput_local.h>
#include <dom/domExtra.h>
class DAE;

class domNurbs_surface : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::NURBS_SURFACE; }
	static daeInt ID() { return 339; }
	virtual daeInt typeID() const { return ID(); }
public:
	class domControl_vertices;

	typedef daeSmartRef<domControl_vertices> domControl_verticesRef;
	typedef daeTArray<domControl_verticesRef> domControl_vertices_Array;

/**
 * The control vertices element must occur exactly one time. It is used to
 * describe the CVs of the spline.
 */
	class domControl_vertices : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::CONTROL_VERTICES; }
		static daeInt ID() { return 340; }
		virtual daeInt typeID() const { return ID(); }

	protected:  // Elements
/**
 * The input element must occur at least one time. These inputs are local
 * inputs. @see domInput
 */
		domInput_local_Array elemInput_array;
/**
 * The extra element may appear any number of times. @see domExtra
 */
		domExtra_Array elemExtra_array;

	public:	//Accessors and Mutators
		/**
		 * Gets the input element array.
		 * @return Returns a reference to the array of input elements.
		 */
		domInput_local_Array &getInput_array() { return elemInput_array; }
		/**
		 * Gets the input element array.
		 * @return Returns a constant reference to the array of input elements.
		 */
		const domInput_local_Array &getInput_array() const { return elemInput_array; }
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
		domControl_vertices(DAE& dae) : daeElement(dae), elemInput_array(), elemExtra_array() {}
		/**
		 * Destructor
		 */
		virtual ~domControl_vertices() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domControl_vertices &operator=( const domControl_vertices &cpy ) { (void)cpy; return *this; }

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


protected:  // Attributes
	domUint attrDegree_u;
	xsBoolean attrClosed_u;
	domUint attrDegree_v;
	xsBoolean attrClosed_v;

protected:  // Elements
	domSource_Array elemSource_array;
/**
 * The control vertices element must occur exactly one time. It is used to
 * describe the CVs of the spline. @see domControl_vertices
 */
	domControl_verticesRef elemControl_vertices;
	domExtra_Array elemExtra_array;

public:	//Accessors and Mutators
	/**
	 * Gets the degree_u attribute.
	 * @return Returns a domUint of the degree_u attribute.
	 */
	domUint getDegree_u() const { return attrDegree_u; }
	/**
	 * Sets the degree_u attribute.
	 * @param atDegree_u The new value for the degree_u attribute.
	 */
	void setDegree_u( domUint atDegree_u ) { attrDegree_u = atDegree_u; }

	/**
	 * Gets the closed_u attribute.
	 * @return Returns a xsBoolean of the closed_u attribute.
	 */
	xsBoolean getClosed_u() const { return attrClosed_u; }
	/**
	 * Sets the closed_u attribute.
	 * @param atClosed_u The new value for the closed_u attribute.
	 */
	void setClosed_u( xsBoolean atClosed_u ) { attrClosed_u = atClosed_u; }

	/**
	 * Gets the degree_v attribute.
	 * @return Returns a domUint of the degree_v attribute.
	 */
	domUint getDegree_v() const { return attrDegree_v; }
	/**
	 * Sets the degree_v attribute.
	 * @param atDegree_v The new value for the degree_v attribute.
	 */
	void setDegree_v( domUint atDegree_v ) { attrDegree_v = atDegree_v; }

	/**
	 * Gets the closed_v attribute.
	 * @return Returns a xsBoolean of the closed_v attribute.
	 */
	xsBoolean getClosed_v() const { return attrClosed_v; }
	/**
	 * Sets the closed_v attribute.
	 * @param atClosed_v The new value for the closed_v attribute.
	 */
	void setClosed_v( xsBoolean atClosed_v ) { attrClosed_v = atClosed_v; }

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
	 * Gets the control_vertices element.
	 * @return a daeSmartRef to the control_vertices element.
	 */
	const domControl_verticesRef getControl_vertices() const { return elemControl_vertices; }
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
	domNurbs_surface(DAE& dae) : daeElement(dae), attrDegree_u(), attrClosed_u(), attrDegree_v(), attrClosed_v(), elemSource_array(), elemControl_vertices(), elemExtra_array() {}
	/**
	 * Destructor
	 */
	virtual ~domNurbs_surface() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domNurbs_surface &operator=( const domNurbs_surface &cpy ) { (void)cpy; return *this; }

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
