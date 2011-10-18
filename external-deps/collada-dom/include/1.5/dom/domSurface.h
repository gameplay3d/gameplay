#ifndef __domSurface_h__
#define __domSurface_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domPlane.h>
#include <dom/domSphere.h>
#include <dom/domTorus.h>
#include <dom/domSwept_surface.h>
#include <dom/domNurbs_surface.h>
#include <dom/domCone.h>
#include <dom/domExtra.h>
#include <dom/domOrient.h>
#include <dom/domOrigin.h>
class DAE;

/**
 * A surfaceType defines the attributes of a surface     element. With rotate
 * and translate the surface can be     positioned to its right location.
 */
class domSurface : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::SURFACE; }
	static daeInt ID() { return 359; }
	virtual daeInt typeID() const { return ID(); }
public:
	class domCylinder;

	typedef daeSmartRef<domCylinder> domCylinderRef;
	typedef daeTArray<domCylinderRef> domCylinder_Array;

	class domCylinder : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::CYLINDER; }
		static daeInt ID() { return 360; }
		virtual daeInt typeID() const { return ID(); }
	public:
		class domRadius;

		typedef daeSmartRef<domRadius> domRadiusRef;
		typedef daeTArray<domRadiusRef> domRadius_Array;

		class domRadius : public daeElement
		{
		public:
			virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::RADIUS; }
			static daeInt ID() { return 361; }
			virtual daeInt typeID() const { return ID(); }


		public:	//Accessors and Mutators
			/**
			 * Gets the value of this element.
			 * @return a domFloat of the value.
			 */
			domFloat& getValue() { return _value; }
			/**
			 * Sets the _value of this element.
			 * @param val The new value for this element.
			 */
			void setValue( const domFloat& val ) { _value = val; }

		protected:  // Value
			/**
			 * The domFloat value of the text data of this element. 
			 */
			domFloat _value;
		protected:
			/**
			 * Constructor
			 */
			domRadius(DAE& dae) : daeElement(dae), _value() {}
			/**
			 * Destructor
			 */
			virtual ~domRadius() {}
			/**
			 * Overloaded assignment operator
			 */
			virtual domRadius &operator=( const domRadius &cpy ) { (void)cpy; return *this; }

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



	protected:  // Elements
		domRadiusRef elemRadius;
		domExtra_Array elemExtra_array;

	public:	//Accessors and Mutators
		/**
		 * Gets the radius element.
		 * @return a daeSmartRef to the radius element.
		 */
		const domRadiusRef getRadius() const { return elemRadius; }
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
		domCylinder(DAE& dae) : daeElement(dae), elemRadius(), elemExtra_array() {}
		/**
		 * Destructor
		 */
		virtual ~domCylinder() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domCylinder &operator=( const domCylinder &cpy ) { (void)cpy; return *this; }

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
/**
 *       The id of the surface.     
 */
	domSid attrSid;
/**
 *       The name of the surface.     
 */
	xsToken attrName;

protected:  // Elements
	domPlaneRef elemPlane;
	domSphereRef elemSphere;
	domTorusRef elemTorus;
	domSwept_surfaceRef elemSwept_surface;
	domNurbs_surfaceRef elemNurbs_surface;
	domConeRef elemCone;
	domCylinderRef elemCylinder;
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
	 * Gets the plane element.
	 * @return a daeSmartRef to the plane element.
	 */
	const domPlaneRef getPlane() const { return elemPlane; }
	/**
	 * Gets the sphere element.
	 * @return a daeSmartRef to the sphere element.
	 */
	const domSphereRef getSphere() const { return elemSphere; }
	/**
	 * Gets the torus element.
	 * @return a daeSmartRef to the torus element.
	 */
	const domTorusRef getTorus() const { return elemTorus; }
	/**
	 * Gets the swept_surface element.
	 * @return a daeSmartRef to the swept_surface element.
	 */
	const domSwept_surfaceRef getSwept_surface() const { return elemSwept_surface; }
	/**
	 * Gets the nurbs_surface element.
	 * @return a daeSmartRef to the nurbs_surface element.
	 */
	const domNurbs_surfaceRef getNurbs_surface() const { return elemNurbs_surface; }
	/**
	 * Gets the cone element.
	 * @return a daeSmartRef to the cone element.
	 */
	const domConeRef getCone() const { return elemCone; }
	/**
	 * Gets the cylinder element.
	 * @return a daeSmartRef to the cylinder element.
	 */
	const domCylinderRef getCylinder() const { return elemCylinder; }
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
	domSurface(DAE& dae) : daeElement(dae), attrSid(), attrName(), elemPlane(), elemSphere(), elemTorus(), elemSwept_surface(), elemNurbs_surface(), elemCone(), elemCylinder(), elemOrient_array(), elemOrigin() {}
	/**
	 * Destructor
	 */
	virtual ~domSurface() { daeElement::deleteCMDataArray(_CMData); }
	/**
	 * Overloaded assignment operator
	 */
	virtual domSurface &operator=( const domSurface &cpy ) { (void)cpy; return *this; }

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
