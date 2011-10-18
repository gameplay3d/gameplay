/*
* Copyright 2006 Sony Computer Entertainment Inc.
*
* Licensed under the MIT Open Source License, for details please see license.txt or the website
* http://www.opensource.org/licenses/mit-license.php
*
*/ 

#ifndef __domRigid_constraint_h__
#define __domRigid_constraint_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domTechnique.h>
#include <dom/domExtra.h>
#include <dom/domTranslate.h>
#include <dom/domRotate.h>
#include <dom/domTargetableFloat3.h>
#include <dom/domTargetableFloat.h>
class DAE;

/**
 * This element allows for connecting components, such as rigid_body into
 * complex physics models  with moveable parts.
 */
class domRigid_constraint : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::RIGID_CONSTRAINT; }
	static daeInt ID() { return 801; }
	virtual daeInt typeID() const { return ID(); }
public:
	class domRef_attachment;

	typedef daeSmartRef<domRef_attachment> domRef_attachmentRef;
	typedef daeTArray<domRef_attachmentRef> domRef_attachment_Array;

/**
 * Defines the attachment (to a rigid_body or a node) to be used as the reference-frame.
 */
	class domRef_attachment : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::REF_ATTACHMENT; }
		static daeInt ID() { return 802; }
		virtual daeInt typeID() const { return ID(); }
	protected:  // Attribute
/**
 *  The “rigid_body” attribute is a relative reference to a rigid-body
 * within the same  physics_model. 
 */
		xsAnyURI attrRigid_body;

	protected:  // Elements
/**
 *  Allows you to "position" the attachment point.  @see domTranslate
 */
		domTranslate_Array elemTranslate_array;
/**
 *  Allows you to "position" the attachment point.  @see domRotate
 */
		domRotate_Array elemRotate_array;
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
		 * Gets the rigid_body attribute.
		 * @return Returns a xsAnyURI reference of the rigid_body attribute.
		 */
		xsAnyURI &getRigid_body() { return attrRigid_body; }
		/**
		 * Gets the rigid_body attribute.
		 * @return Returns a constant xsAnyURI reference of the rigid_body attribute.
		 */
		const xsAnyURI &getRigid_body() const { return attrRigid_body; }
		/**
		 * Sets the rigid_body attribute.
		 * @param atRigid_body The new value for the rigid_body attribute.
		 */
		void setRigid_body( const xsAnyURI &atRigid_body ) { attrRigid_body = atRigid_body; _validAttributeArray[0] = true; }
		/**
		 * Sets the rigid_body attribute.
		 * @param atRigid_body The new value for the rigid_body attribute.
		 */
		void setRigid_body( xsString atRigid_body ) { attrRigid_body = atRigid_body; _validAttributeArray[0] = true; }

		/**
		 * Gets the translate element array.
		 * @return Returns a reference to the array of translate elements.
		 */
		domTranslate_Array &getTranslate_array() { return elemTranslate_array; }
		/**
		 * Gets the translate element array.
		 * @return Returns a constant reference to the array of translate elements.
		 */
		const domTranslate_Array &getTranslate_array() const { return elemTranslate_array; }
		/**
		 * Gets the rotate element array.
		 * @return Returns a reference to the array of rotate elements.
		 */
		domRotate_Array &getRotate_array() { return elemRotate_array; }
		/**
		 * Gets the rotate element array.
		 * @return Returns a constant reference to the array of rotate elements.
		 */
		const domRotate_Array &getRotate_array() const { return elemRotate_array; }
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
		domRef_attachment(DAE& dae) : daeElement(dae), attrRigid_body(dae, *this), elemTranslate_array(), elemRotate_array(), elemExtra_array() {}
		/**
		 * Destructor
		 */
		virtual ~domRef_attachment() { daeElement::deleteCMDataArray(_CMData); }
		/**
		 * Overloaded assignment operator
		 */
		virtual domRef_attachment &operator=( const domRef_attachment &cpy ) { (void)cpy; return *this; }

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

	class domAttachment;

	typedef daeSmartRef<domAttachment> domAttachmentRef;
	typedef daeTArray<domAttachmentRef> domAttachment_Array;

/**
 * Defines an attachment to a rigid-body or a node.
 */
	class domAttachment : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::ATTACHMENT; }
		static daeInt ID() { return 803; }
		virtual daeInt typeID() const { return ID(); }
	protected:  // Attribute
/**
 *  The “rigid_body” attribute is a relative reference to a rigid-body
 * within the same physics_model. 
 */
		xsAnyURI attrRigid_body;

	protected:  // Elements
/**
 *  Allows you to "position" the attachment point.  @see domTranslate
 */
		domTranslate_Array elemTranslate_array;
/**
 *  Allows you to "position" the attachment point.  @see domRotate
 */
		domRotate_Array elemRotate_array;
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
		 * Gets the rigid_body attribute.
		 * @return Returns a xsAnyURI reference of the rigid_body attribute.
		 */
		xsAnyURI &getRigid_body() { return attrRigid_body; }
		/**
		 * Gets the rigid_body attribute.
		 * @return Returns a constant xsAnyURI reference of the rigid_body attribute.
		 */
		const xsAnyURI &getRigid_body() const { return attrRigid_body; }
		/**
		 * Sets the rigid_body attribute.
		 * @param atRigid_body The new value for the rigid_body attribute.
		 */
		void setRigid_body( const xsAnyURI &atRigid_body ) { attrRigid_body = atRigid_body; _validAttributeArray[0] = true; }
		/**
		 * Sets the rigid_body attribute.
		 * @param atRigid_body The new value for the rigid_body attribute.
		 */
		void setRigid_body( xsString atRigid_body ) { attrRigid_body = atRigid_body; _validAttributeArray[0] = true; }

		/**
		 * Gets the translate element array.
		 * @return Returns a reference to the array of translate elements.
		 */
		domTranslate_Array &getTranslate_array() { return elemTranslate_array; }
		/**
		 * Gets the translate element array.
		 * @return Returns a constant reference to the array of translate elements.
		 */
		const domTranslate_Array &getTranslate_array() const { return elemTranslate_array; }
		/**
		 * Gets the rotate element array.
		 * @return Returns a reference to the array of rotate elements.
		 */
		domRotate_Array &getRotate_array() { return elemRotate_array; }
		/**
		 * Gets the rotate element array.
		 * @return Returns a constant reference to the array of rotate elements.
		 */
		const domRotate_Array &getRotate_array() const { return elemRotate_array; }
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
		domAttachment(DAE& dae) : daeElement(dae), attrRigid_body(dae, *this), elemTranslate_array(), elemRotate_array(), elemExtra_array() {}
		/**
		 * Destructor
		 */
		virtual ~domAttachment() { daeElement::deleteCMDataArray(_CMData); }
		/**
		 * Overloaded assignment operator
		 */
		virtual domAttachment &operator=( const domAttachment &cpy ) { (void)cpy; return *this; }

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

	class domTechnique_common;

	typedef daeSmartRef<domTechnique_common> domTechnique_commonRef;
	typedef daeTArray<domTechnique_commonRef> domTechnique_common_Array;

/**
 * The technique_common element specifies the rigid_constraint information
 * for the common profile  which all COLLADA implementations need to support.
 */
	class domTechnique_common : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::TECHNIQUE_COMMON; }
		static daeInt ID() { return 804; }
		virtual daeInt typeID() const { return ID(); }
	public:
		class domEnabled;

		typedef daeSmartRef<domEnabled> domEnabledRef;
		typedef daeTArray<domEnabledRef> domEnabled_Array;

/**
 * If false, the constraint doesn’t exert any force or influence on the
 * rigid bodies.
 */
		class domEnabled : public daeElement
		{
		public:
			virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::ENABLED; }
			static daeInt ID() { return 805; }
			virtual daeInt typeID() const { return ID(); }
		protected:  // Attribute
/**
 *  The sid attribute is a text string value containing the sub-identifier
 * of this element.  This value must be unique within the scope of the parent
 * element. Optional attribute. 
 */
			xsNCName attrSid;

		protected:  // Value
			/**
			 * The domBool value of the text data of this element. 
			 */
			domBool _value;

		public:	//Accessors and Mutators
			/**
			 * Gets the sid attribute.
			 * @return Returns a xsNCName of the sid attribute.
			 */
			xsNCName getSid() const { return attrSid; }
			/**
			 * Sets the sid attribute.
			 * @param atSid The new value for the sid attribute.
			 */
			void setSid( xsNCName atSid ) { *(daeStringRef*)&attrSid = atSid; _validAttributeArray[0] = true; }

			/**
			 * Gets the value of this element.
			 * @return a domBool of the value.
			 */
			domBool getValue() const { return _value; }
			/**
			 * Sets the _value of this element.
			 * @param val The new value for this element.
			 */
			void setValue( domBool val ) { _value = val; }

		protected:
			/**
			 * Constructor
			 */
			domEnabled(DAE& dae) : daeElement(dae), attrSid(), _value() {}
			/**
			 * Destructor
			 */
			virtual ~domEnabled() {}
			/**
			 * Overloaded assignment operator
			 */
			virtual domEnabled &operator=( const domEnabled &cpy ) { (void)cpy; return *this; }

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

		class domInterpenetrate;

		typedef daeSmartRef<domInterpenetrate> domInterpenetrateRef;
		typedef daeTArray<domInterpenetrateRef> domInterpenetrate_Array;

/**
 * Indicates whether the attached rigid bodies may inter-penetrate.
 */
		class domInterpenetrate : public daeElement
		{
		public:
			virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::INTERPENETRATE; }
			static daeInt ID() { return 806; }
			virtual daeInt typeID() const { return ID(); }
		protected:  // Attribute
/**
 *  The sid attribute is a text string value containing the sub-identifier
 * of this element.  This value must be unique within the scope of the parent
 * element. Optional attribute. 
 */
			xsNCName attrSid;

		protected:  // Value
			/**
			 * The domBool value of the text data of this element. 
			 */
			domBool _value;

		public:	//Accessors and Mutators
			/**
			 * Gets the sid attribute.
			 * @return Returns a xsNCName of the sid attribute.
			 */
			xsNCName getSid() const { return attrSid; }
			/**
			 * Sets the sid attribute.
			 * @param atSid The new value for the sid attribute.
			 */
			void setSid( xsNCName atSid ) { *(daeStringRef*)&attrSid = atSid; _validAttributeArray[0] = true; }

			/**
			 * Gets the value of this element.
			 * @return a domBool of the value.
			 */
			domBool getValue() const { return _value; }
			/**
			 * Sets the _value of this element.
			 * @param val The new value for this element.
			 */
			void setValue( domBool val ) { _value = val; }

		protected:
			/**
			 * Constructor
			 */
			domInterpenetrate(DAE& dae) : daeElement(dae), attrSid(), _value() {}
			/**
			 * Destructor
			 */
			virtual ~domInterpenetrate() {}
			/**
			 * Overloaded assignment operator
			 */
			virtual domInterpenetrate &operator=( const domInterpenetrate &cpy ) { (void)cpy; return *this; }

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

		class domLimits;

		typedef daeSmartRef<domLimits> domLimitsRef;
		typedef daeTArray<domLimitsRef> domLimits_Array;

/**
 * The limits element provides a flexible way to specify the constraint limits
 * (degrees of freedom  and ranges).
 */
		class domLimits : public daeElement
		{
		public:
			virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::LIMITS; }
			static daeInt ID() { return 807; }
			virtual daeInt typeID() const { return ID(); }
		public:
			class domSwing_cone_and_twist;

			typedef daeSmartRef<domSwing_cone_and_twist> domSwing_cone_and_twistRef;
			typedef daeTArray<domSwing_cone_and_twistRef> domSwing_cone_and_twist_Array;

/**
 * The swing_cone_and_twist element describes the angular limits along each
 * rotation axis in degrees. The the X and Y limits describe a “swing cone”
 * and the Z limits describe the “twist angle” range
 */
			class domSwing_cone_and_twist : public daeElement
			{
			public:
				virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::SWING_CONE_AND_TWIST; }
				static daeInt ID() { return 808; }
				virtual daeInt typeID() const { return ID(); }

			protected:  // Elements
/**
 * The minimum values for the limit. @see domMin
 */
				domTargetableFloat3Ref elemMin;
/**
 * The maximum values for the limit. @see domMax
 */
				domTargetableFloat3Ref elemMax;

			public:	//Accessors and Mutators
				/**
				 * Gets the min element.
				 * @return a daeSmartRef to the min element.
				 */
				const domTargetableFloat3Ref getMin() const { return elemMin; }
				/**
				 * Gets the max element.
				 * @return a daeSmartRef to the max element.
				 */
				const domTargetableFloat3Ref getMax() const { return elemMax; }
			protected:
				/**
				 * Constructor
				 */
				domSwing_cone_and_twist(DAE& dae) : daeElement(dae), elemMin(), elemMax() {}
				/**
				 * Destructor
				 */
				virtual ~domSwing_cone_and_twist() {}
				/**
				 * Overloaded assignment operator
				 */
				virtual domSwing_cone_and_twist &operator=( const domSwing_cone_and_twist &cpy ) { (void)cpy; return *this; }

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

			class domLinear;

			typedef daeSmartRef<domLinear> domLinearRef;
			typedef daeTArray<domLinearRef> domLinear_Array;

/**
 * The linear element describes linear (translational) limits along each axis.
 */
			class domLinear : public daeElement
			{
			public:
				virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::LINEAR; }
				static daeInt ID() { return 809; }
				virtual daeInt typeID() const { return ID(); }

			protected:  // Elements
/**
 * The minimum values for the limit. @see domMin
 */
				domTargetableFloat3Ref elemMin;
/**
 * The maximum values for the limit. @see domMax
 */
				domTargetableFloat3Ref elemMax;

			public:	//Accessors and Mutators
				/**
				 * Gets the min element.
				 * @return a daeSmartRef to the min element.
				 */
				const domTargetableFloat3Ref getMin() const { return elemMin; }
				/**
				 * Gets the max element.
				 * @return a daeSmartRef to the max element.
				 */
				const domTargetableFloat3Ref getMax() const { return elemMax; }
			protected:
				/**
				 * Constructor
				 */
				domLinear(DAE& dae) : daeElement(dae), elemMin(), elemMax() {}
				/**
				 * Destructor
				 */
				virtual ~domLinear() {}
				/**
				 * Overloaded assignment operator
				 */
				virtual domLinear &operator=( const domLinear &cpy ) { (void)cpy; return *this; }

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
 * The swing_cone_and_twist element describes the angular limits along each
 * rotation axis in degrees. The the X and Y limits describe a “swing cone”
 * and the Z limits describe the “twist angle” range @see domSwing_cone_and_twist
 */
			domSwing_cone_and_twistRef elemSwing_cone_and_twist;
/**
 * The linear element describes linear (translational) limits along each axis.
 * @see domLinear
 */
			domLinearRef elemLinear;

		public:	//Accessors and Mutators
			/**
			 * Gets the swing_cone_and_twist element.
			 * @return a daeSmartRef to the swing_cone_and_twist element.
			 */
			const domSwing_cone_and_twistRef getSwing_cone_and_twist() const { return elemSwing_cone_and_twist; }
			/**
			 * Gets the linear element.
			 * @return a daeSmartRef to the linear element.
			 */
			const domLinearRef getLinear() const { return elemLinear; }
		protected:
			/**
			 * Constructor
			 */
			domLimits(DAE& dae) : daeElement(dae), elemSwing_cone_and_twist(), elemLinear() {}
			/**
			 * Destructor
			 */
			virtual ~domLimits() {}
			/**
			 * Overloaded assignment operator
			 */
			virtual domLimits &operator=( const domLimits &cpy ) { (void)cpy; return *this; }

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

		class domSpring;

		typedef daeSmartRef<domSpring> domSpringRef;
		typedef daeTArray<domSpringRef> domSpring_Array;

/**
 * Spring, based on distance (“LINEAR”) or angle (“ANGULAR”).
 */
		class domSpring : public daeElement
		{
		public:
			virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::SPRING; }
			static daeInt ID() { return 810; }
			virtual daeInt typeID() const { return ID(); }
		public:
			class domAngular;

			typedef daeSmartRef<domAngular> domAngularRef;
			typedef daeTArray<domAngularRef> domAngular_Array;

/**
 * The angular spring properties.
 */
			class domAngular : public daeElement
			{
			public:
				virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::ANGULAR; }
				static daeInt ID() { return 811; }
				virtual daeInt typeID() const { return ID(); }

			protected:  // Elements
/**
 * The stiffness (also called spring coefficient) has units of force/angle
 * in degrees. @see domStiffness
 */
				domTargetableFloatRef elemStiffness;
/**
 * The spring damping coefficient. @see domDamping
 */
				domTargetableFloatRef elemDamping;
/**
 * The spring's target or resting distance. @see domTarget_value
 */
				domTargetableFloatRef elemTarget_value;

			public:	//Accessors and Mutators
				/**
				 * Gets the stiffness element.
				 * @return a daeSmartRef to the stiffness element.
				 */
				const domTargetableFloatRef getStiffness() const { return elemStiffness; }
				/**
				 * Gets the damping element.
				 * @return a daeSmartRef to the damping element.
				 */
				const domTargetableFloatRef getDamping() const { return elemDamping; }
				/**
				 * Gets the target_value element.
				 * @return a daeSmartRef to the target_value element.
				 */
				const domTargetableFloatRef getTarget_value() const { return elemTarget_value; }
			protected:
				/**
				 * Constructor
				 */
				domAngular(DAE& dae) : daeElement(dae), elemStiffness(), elemDamping(), elemTarget_value() {}
				/**
				 * Destructor
				 */
				virtual ~domAngular() {}
				/**
				 * Overloaded assignment operator
				 */
				virtual domAngular &operator=( const domAngular &cpy ) { (void)cpy; return *this; }

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

			class domLinear;

			typedef daeSmartRef<domLinear> domLinearRef;
			typedef daeTArray<domLinearRef> domLinear_Array;

/**
 * The linear spring properties.
 */
			class domLinear : public daeElement
			{
			public:
				virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::LINEAR; }
				static daeInt ID() { return 812; }
				virtual daeInt typeID() const { return ID(); }

			protected:  // Elements
/**
 * The stiffness (also called spring coefficient) has units of force/distance.
 * @see domStiffness
 */
				domTargetableFloatRef elemStiffness;
/**
 * The spring damping coefficient. @see domDamping
 */
				domTargetableFloatRef elemDamping;
/**
 * The spring's target or resting distance. @see domTarget_value
 */
				domTargetableFloatRef elemTarget_value;

			public:	//Accessors and Mutators
				/**
				 * Gets the stiffness element.
				 * @return a daeSmartRef to the stiffness element.
				 */
				const domTargetableFloatRef getStiffness() const { return elemStiffness; }
				/**
				 * Gets the damping element.
				 * @return a daeSmartRef to the damping element.
				 */
				const domTargetableFloatRef getDamping() const { return elemDamping; }
				/**
				 * Gets the target_value element.
				 * @return a daeSmartRef to the target_value element.
				 */
				const domTargetableFloatRef getTarget_value() const { return elemTarget_value; }
			protected:
				/**
				 * Constructor
				 */
				domLinear(DAE& dae) : daeElement(dae), elemStiffness(), elemDamping(), elemTarget_value() {}
				/**
				 * Destructor
				 */
				virtual ~domLinear() {}
				/**
				 * Overloaded assignment operator
				 */
				virtual domLinear &operator=( const domLinear &cpy ) { (void)cpy; return *this; }

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
 * The angular spring properties. @see domAngular
 */
			domAngularRef elemAngular;
/**
 * The linear spring properties. @see domLinear
 */
			domLinearRef elemLinear;

		public:	//Accessors and Mutators
			/**
			 * Gets the angular element.
			 * @return a daeSmartRef to the angular element.
			 */
			const domAngularRef getAngular() const { return elemAngular; }
			/**
			 * Gets the linear element.
			 * @return a daeSmartRef to the linear element.
			 */
			const domLinearRef getLinear() const { return elemLinear; }
		protected:
			/**
			 * Constructor
			 */
			domSpring(DAE& dae) : daeElement(dae), elemAngular(), elemLinear() {}
			/**
			 * Destructor
			 */
			virtual ~domSpring() {}
			/**
			 * Overloaded assignment operator
			 */
			virtual domSpring &operator=( const domSpring &cpy ) { (void)cpy; return *this; }

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
 * If false, the constraint doesn’t exert any force or influence on the
 * rigid bodies. @see domEnabled
 */
		domEnabledRef elemEnabled;
/**
 * Indicates whether the attached rigid bodies may inter-penetrate. @see domInterpenetrate
 */
		domInterpenetrateRef elemInterpenetrate;
/**
 * The limits element provides a flexible way to specify the constraint limits
 * (degrees of freedom  and ranges). @see domLimits
 */
		domLimitsRef elemLimits;
/**
 * Spring, based on distance (“LINEAR”) or angle (“ANGULAR”). @see
 * domSpring
 */
		domSpringRef elemSpring;

	public:	//Accessors and Mutators
		/**
		 * Gets the enabled element.
		 * @return a daeSmartRef to the enabled element.
		 */
		const domEnabledRef getEnabled() const { return elemEnabled; }
		/**
		 * Gets the interpenetrate element.
		 * @return a daeSmartRef to the interpenetrate element.
		 */
		const domInterpenetrateRef getInterpenetrate() const { return elemInterpenetrate; }
		/**
		 * Gets the limits element.
		 * @return a daeSmartRef to the limits element.
		 */
		const domLimitsRef getLimits() const { return elemLimits; }
		/**
		 * Gets the spring element.
		 * @return a daeSmartRef to the spring element.
		 */
		const domSpringRef getSpring() const { return elemSpring; }
	protected:
		/**
		 * Constructor
		 */
		domTechnique_common(DAE& dae) : daeElement(dae), elemEnabled(), elemInterpenetrate(), elemLimits(), elemSpring() {}
		/**
		 * Destructor
		 */
		virtual ~domTechnique_common() {}
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


protected:  // Attributes
/**
 *  The sid attribute is a text string value containing the sub-identifier
 * of this element.  This value must be unique within the scope of the parent
 * element. Optional attribute. 
 */
	xsNCName attrSid;
/**
 *  The name attribute is the text string name of this element. Optional attribute.
 */
	xsNCName attrName;

protected:  // Elements
/**
 * Defines the attachment (to a rigid_body or a node) to be used as the reference-frame.
 * @see domRef_attachment
 */
	domRef_attachmentRef elemRef_attachment;
/**
 * Defines an attachment to a rigid-body or a node. @see domAttachment
 */
	domAttachmentRef elemAttachment;
/**
 * The technique_common element specifies the rigid_constraint information
 * for the common profile  which all COLLADA implementations need to support.
 * @see domTechnique_common
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
	 * Gets the sid attribute.
	 * @return Returns a xsNCName of the sid attribute.
	 */
	xsNCName getSid() const { return attrSid; }
	/**
	 * Sets the sid attribute.
	 * @param atSid The new value for the sid attribute.
	 */
	void setSid( xsNCName atSid ) { *(daeStringRef*)&attrSid = atSid; _validAttributeArray[0] = true; }

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
	 * Gets the ref_attachment element.
	 * @return a daeSmartRef to the ref_attachment element.
	 */
	const domRef_attachmentRef getRef_attachment() const { return elemRef_attachment; }
	/**
	 * Gets the attachment element.
	 * @return a daeSmartRef to the attachment element.
	 */
	const domAttachmentRef getAttachment() const { return elemAttachment; }
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
	domRigid_constraint(DAE& dae) : daeElement(dae), attrSid(), attrName(), elemRef_attachment(), elemAttachment(), elemTechnique_common(), elemTechnique_array(), elemExtra_array() {}
	/**
	 * Destructor
	 */
	virtual ~domRigid_constraint() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domRigid_constraint &operator=( const domRigid_constraint &cpy ) { (void)cpy; return *this; }

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
