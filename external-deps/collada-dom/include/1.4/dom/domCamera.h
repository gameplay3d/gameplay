/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 

#ifndef __domCamera_h__
#define __domCamera_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domAsset.h>
#include <dom/domExtra.h>
#include <dom/domTechnique.h>
#include <dom/domTargetableFloat.h>
class DAE;

/**
 * The camera element declares a view into the scene hierarchy or scene graph.
 * The camera contains  elements that describe the camera’s optics and imager.
 */
class domCamera : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::CAMERA; }
	static daeInt ID() { return 645; }
	virtual daeInt typeID() const { return ID(); }
public:
	class domOptics;

	typedef daeSmartRef<domOptics> domOpticsRef;
	typedef daeTArray<domOpticsRef> domOptics_Array;

/**
 * Optics represents the apparatus on a camera that projects the image onto
 * the image sensor.
 */
	class domOptics : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::OPTICS; }
		static daeInt ID() { return 646; }
		virtual daeInt typeID() const { return ID(); }
	public:
		class domTechnique_common;

		typedef daeSmartRef<domTechnique_common> domTechnique_commonRef;
		typedef daeTArray<domTechnique_commonRef> domTechnique_common_Array;

/**
 * The technique_common element specifies the optics information for the common
 * profile  which all COLLADA implementations need to support.
 */
		class domTechnique_common : public daeElement
		{
		public:
			virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::TECHNIQUE_COMMON; }
			static daeInt ID() { return 647; }
			virtual daeInt typeID() const { return ID(); }
		public:
			class domOrthographic;

			typedef daeSmartRef<domOrthographic> domOrthographicRef;
			typedef daeTArray<domOrthographicRef> domOrthographic_Array;

/**
 * The orthographic element describes the field of view of an orthographic
 * camera.
 */
			class domOrthographic : public daeElement
			{
			public:
				virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::ORTHOGRAPHIC; }
				static daeInt ID() { return 648; }
				virtual daeInt typeID() const { return ID(); }

			protected:  // Elements
/**
 * The xmag element contains a floating point number describing the horizontal
 * magnification of the view. @see domXmag
 */
				domTargetableFloatRef elemXmag;
/**
 * The ymag element contains a floating point number describing the vertical
 * magnification of the view.  It can also have a sid. @see domYmag
 */
				domTargetableFloatRef elemYmag;
/**
 * The aspect_ratio element contains a floating point number describing the
 * aspect ratio of  the field of view. If the aspect_ratio element is not
 * present the aspect ratio is to be  calculated from the xmag or ymag elements
 * and the current viewport. @see domAspect_ratio
 */
				domTargetableFloatRef elemAspect_ratio;
/**
 * The znear element contains a floating point number that describes the distance
 * to the near  clipping plane. The znear element must occur exactly once.
 * @see domZnear
 */
				domTargetableFloatRef elemZnear;
/**
 * The zfar element contains a floating point number that describes the distance
 * to the far  clipping plane. The zfar element must occur exactly once. @see
 * domZfar
 */
				domTargetableFloatRef elemZfar;
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
				 * Gets the xmag element.
				 * @return a daeSmartRef to the xmag element.
				 */
				const domTargetableFloatRef getXmag() const { return elemXmag; }
				/**
				 * Gets the ymag element.
				 * @return a daeSmartRef to the ymag element.
				 */
				const domTargetableFloatRef getYmag() const { return elemYmag; }
				/**
				 * Gets the aspect_ratio element.
				 * @return a daeSmartRef to the aspect_ratio element.
				 */
				const domTargetableFloatRef getAspect_ratio() const { return elemAspect_ratio; }
				/**
				 * Gets the znear element.
				 * @return a daeSmartRef to the znear element.
				 */
				const domTargetableFloatRef getZnear() const { return elemZnear; }
				/**
				 * Gets the zfar element.
				 * @return a daeSmartRef to the zfar element.
				 */
				const domTargetableFloatRef getZfar() const { return elemZfar; }
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
				domOrthographic(DAE& dae) : daeElement(dae), elemXmag(), elemYmag(), elemAspect_ratio(), elemZnear(), elemZfar() {}
				/**
				 * Destructor
				 */
				virtual ~domOrthographic() { daeElement::deleteCMDataArray(_CMData); }
				/**
				 * Overloaded assignment operator
				 */
				virtual domOrthographic &operator=( const domOrthographic &cpy ) { (void)cpy; return *this; }

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

			class domPerspective;

			typedef daeSmartRef<domPerspective> domPerspectiveRef;
			typedef daeTArray<domPerspectiveRef> domPerspective_Array;

/**
 * The perspective element describes the optics of a perspective camera.
 */
			class domPerspective : public daeElement
			{
			public:
				virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::PERSPECTIVE; }
				static daeInt ID() { return 649; }
				virtual daeInt typeID() const { return ID(); }

			protected:  // Elements
/**
 * The xfov element contains a floating point number describing the horizontal
 * field of view in degrees. @see domXfov
 */
				domTargetableFloatRef elemXfov;
/**
 * The yfov element contains a floating point number describing the verticle
 * field of view in degrees. @see domYfov
 */
				domTargetableFloatRef elemYfov;
/**
 * The aspect_ratio element contains a floating point number describing the
 * aspect ratio of the field  of view. If the aspect_ratio element is not
 * present the aspect ratio is to be calculated from the  xfov or yfov elements
 * and the current viewport. @see domAspect_ratio
 */
				domTargetableFloatRef elemAspect_ratio;
/**
 * The znear element contains a floating point number that describes the distance
 * to the near  clipping plane. The znear element must occur exactly once.
 * @see domZnear
 */
				domTargetableFloatRef elemZnear;
/**
 * The zfar element contains a floating point number that describes the distance
 * to the far  clipping plane. The zfar element must occur exactly once. @see
 * domZfar
 */
				domTargetableFloatRef elemZfar;
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
				 * Gets the xfov element.
				 * @return a daeSmartRef to the xfov element.
				 */
				const domTargetableFloatRef getXfov() const { return elemXfov; }
				/**
				 * Gets the yfov element.
				 * @return a daeSmartRef to the yfov element.
				 */
				const domTargetableFloatRef getYfov() const { return elemYfov; }
				/**
				 * Gets the aspect_ratio element.
				 * @return a daeSmartRef to the aspect_ratio element.
				 */
				const domTargetableFloatRef getAspect_ratio() const { return elemAspect_ratio; }
				/**
				 * Gets the znear element.
				 * @return a daeSmartRef to the znear element.
				 */
				const domTargetableFloatRef getZnear() const { return elemZnear; }
				/**
				 * Gets the zfar element.
				 * @return a daeSmartRef to the zfar element.
				 */
				const domTargetableFloatRef getZfar() const { return elemZfar; }
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
				domPerspective(DAE& dae) : daeElement(dae), elemXfov(), elemYfov(), elemAspect_ratio(), elemZnear(), elemZfar() {}
				/**
				 * Destructor
				 */
				virtual ~domPerspective() { daeElement::deleteCMDataArray(_CMData); }
				/**
				 * Overloaded assignment operator
				 */
				virtual domPerspective &operator=( const domPerspective &cpy ) { (void)cpy; return *this; }

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
/**
 * The orthographic element describes the field of view of an orthographic
 * camera. @see domOrthographic
 */
			domOrthographicRef elemOrthographic;
/**
 * The perspective element describes the optics of a perspective camera. @see
 * domPerspective
 */
			domPerspectiveRef elemPerspective;
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
			 * Gets the orthographic element.
			 * @return a daeSmartRef to the orthographic element.
			 */
			const domOrthographicRef getOrthographic() const { return elemOrthographic; }
			/**
			 * Gets the perspective element.
			 * @return a daeSmartRef to the perspective element.
			 */
			const domPerspectiveRef getPerspective() const { return elemPerspective; }
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
			domTechnique_common(DAE& dae) : daeElement(dae), elemOrthographic(), elemPerspective() {}
			/**
			 * Destructor
			 */
			virtual ~domTechnique_common() { daeElement::deleteCMDataArray(_CMData); }
			/**
			 * Overloaded assignment operator
			 */
			virtual domTechnique_common &operator=( const domTechnique_common &cpy ) { (void)cpy; return *this; }

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
/**
 * The technique_common element specifies the optics information for the common
 * profile  which all COLLADA implementations need to support. @see domTechnique_common
 */
		domTechnique_commonRef elemTechnique_common;
/**
 *  This element may contain any number of non-common profile techniques.
 * @see domTechnique
 */
		domTechnique_Array elemTechnique_array;
/**
 *  The extra element may appear any number of times.  @see domExtra
 */
		domExtra_Array elemExtra_array;

	public:	//Accessors and Mutators
		/**
		 * Gets the technique_common element.
		 * @return a daeSmartRef to the technique_common element.
		 */
		const domTechnique_commonRef getTechnique_common() const { return elemTechnique_common; }
		/**
		 * Gets the technique element array.
		 * @return Returns a reference to the array of technique elements.
		 */
		domTechnique_Array &getTechnique_array() { return elemTechnique_array; }
		/**
		 * Gets the technique element array.
		 * @return Returns a constant reference to the array of technique elements.
		 */
		const domTechnique_Array &getTechnique_array() const { return elemTechnique_array; }
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
		domOptics(DAE& dae) : daeElement(dae), elemTechnique_common(), elemTechnique_array(), elemExtra_array() {}
		/**
		 * Destructor
		 */
		virtual ~domOptics() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domOptics &operator=( const domOptics &cpy ) { (void)cpy; return *this; }

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

	class domImager;

	typedef daeSmartRef<domImager> domImagerRef;
	typedef daeTArray<domImagerRef> domImager_Array;

/**
 * Imagers represent the image sensor of a camera (for example film or CCD).
 */
	class domImager : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::IMAGER; }
		static daeInt ID() { return 650; }
		virtual daeInt typeID() const { return ID(); }

	protected:  // Elements
/**
 *  This element may contain any number of non-common profile techniques.
 * There is no common technique for imager.  @see domTechnique
 */
		domTechnique_Array elemTechnique_array;
/**
 *  The extra element may appear any number of times.  @see domExtra
 */
		domExtra_Array elemExtra_array;

	public:	//Accessors and Mutators
		/**
		 * Gets the technique element array.
		 * @return Returns a reference to the array of technique elements.
		 */
		domTechnique_Array &getTechnique_array() { return elemTechnique_array; }
		/**
		 * Gets the technique element array.
		 * @return Returns a constant reference to the array of technique elements.
		 */
		const domTechnique_Array &getTechnique_array() const { return elemTechnique_array; }
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
		domImager(DAE& dae) : daeElement(dae), elemTechnique_array(), elemExtra_array() {}
		/**
		 * Destructor
		 */
		virtual ~domImager() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domImager &operator=( const domImager &cpy ) { (void)cpy; return *this; }

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
 *  The id attribute is a text string containing the unique identifier of
 * this element. This value  must be unique within the instance document.
 * Optional attribute. 
 */
	xsID attrId;
/**
 *  The name attribute is the text string name of this element. Optional attribute.
 */
	xsNCName attrName;

protected:  // Elements
/**
 *  The camera element may contain an asset element.  @see domAsset
 */
	domAssetRef elemAsset;
/**
 * Optics represents the apparatus on a camera that projects the image onto
 * the image sensor. @see domOptics
 */
	domOpticsRef elemOptics;
/**
 * Imagers represent the image sensor of a camera (for example film or CCD).
 * @see domImager
 */
	domImagerRef elemImager;
/**
 *  The extra element may appear any number of times.  @see domExtra
 */
	domExtra_Array elemExtra_array;

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
	 * Gets the optics element.
	 * @return a daeSmartRef to the optics element.
	 */
	const domOpticsRef getOptics() const { return elemOptics; }
	/**
	 * Gets the imager element.
	 * @return a daeSmartRef to the imager element.
	 */
	const domImagerRef getImager() const { return elemImager; }
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
	domCamera(DAE& dae) : daeElement(dae), attrId(), attrName(), elemAsset(), elemOptics(), elemImager(), elemExtra_array() {}
	/**
	 * Destructor
	 */
	virtual ~domCamera() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domCamera &operator=( const domCamera &cpy ) { (void)cpy; return *this; }

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
