#ifndef __domAsset_h__
#define __domAsset_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domExtra.h>
class DAE;

/**
 * The asset element defines asset management information regarding its parent
 * element.
 */
class domAsset : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::ASSET; }
	static daeInt ID() { return 38; }
	virtual daeInt typeID() const { return ID(); }
public:
	class domContributor;

	typedef daeSmartRef<domContributor> domContributorRef;
	typedef daeTArray<domContributorRef> domContributor_Array;

/**
 * The contributor element defines authoring information for asset management
 */
	class domContributor : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::CONTRIBUTOR; }
		static daeInt ID() { return 39; }
		virtual daeInt typeID() const { return ID(); }
	public:
		class domAuthor;

		typedef daeSmartRef<domAuthor> domAuthorRef;
		typedef daeTArray<domAuthorRef> domAuthor_Array;

/**
 * The author element contains a string with the author's name. There may
 * be only one author element.
 */
		class domAuthor : public daeElement
		{
		public:
			virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::AUTHOR; }
			static daeInt ID() { return 40; }
			virtual daeInt typeID() const { return ID(); }


		public:	//Accessors and Mutators
			/**
			 * Gets the value of this element.
			 * @return Returns a xsString of the value.
			 */
			xsString getValue() const { return _value; }
			/**
			 * Sets the _value of this element.
			 * @param val The new value for this element.
			 */
			void setValue( xsString val ) { *(daeStringRef*)&_value = val; }

		protected:  // Value
			/**
			 * The xsString value of the text data of this element. 
			 */
			xsString _value;
		protected:
			/**
			 * Constructor
			 */
			domAuthor(DAE& dae) : daeElement(dae), _value() {}
			/**
			 * Destructor
			 */
			virtual ~domAuthor() {}
			/**
			 * Overloaded assignment operator
			 */
			virtual domAuthor &operator=( const domAuthor &cpy ) { (void)cpy; return *this; }

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

		class domAuthor_email;

		typedef daeSmartRef<domAuthor_email> domAuthor_emailRef;
		typedef daeTArray<domAuthor_emailRef> domAuthor_email_Array;

/**
 * Contains a string with author's email address. String that compliant with
 * RFC 2822 section 3.4. This element has no attributes. There may be only
 * one author email address.
 */
		class domAuthor_email : public daeElement
		{
		public:
			virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::AUTHOR_EMAIL; }
			static daeInt ID() { return 41; }
			virtual daeInt typeID() const { return ID(); }


		public:	//Accessors and Mutators
			/**
			 * Gets the value of this element.
			 * @return Returns a xsString of the value.
			 */
			xsString getValue() const { return _value; }
			/**
			 * Sets the _value of this element.
			 * @param val The new value for this element.
			 */
			void setValue( xsString val ) { *(daeStringRef*)&_value = val; }

		protected:  // Value
			/**
			 * The xsString value of the text data of this element. 
			 */
			xsString _value;
		protected:
			/**
			 * Constructor
			 */
			domAuthor_email(DAE& dae) : daeElement(dae), _value() {}
			/**
			 * Destructor
			 */
			virtual ~domAuthor_email() {}
			/**
			 * Overloaded assignment operator
			 */
			virtual domAuthor_email &operator=( const domAuthor_email &cpy ) { (void)cpy; return *this; }

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

		class domAuthor_website;

		typedef daeSmartRef<domAuthor_website> domAuthor_websiteRef;
		typedef daeTArray<domAuthor_websiteRef> domAuthor_website_Array;

/**
 * Contains a URI reference (xs:anyURI) to author's website.  This element
 * has no attributes.   There may be only one author website.
 */
		class domAuthor_website : public daeElement
		{
		public:
			virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::AUTHOR_WEBSITE; }
			static daeInt ID() { return 42; }
			virtual daeInt typeID() const { return ID(); }


		public:	//Accessors and Mutators
			/**
			 * Gets the value of this element.
			 * @return Returns a xsAnyURI of the value.
			 */
			xsAnyURI &getValue() { return _value; }
			/**
			 * Gets the value of this element.
			 * @return Returns a constant xsAnyURI of the value.
			 */
			const xsAnyURI &getValue() const { return _value; }
			/**
			 * Sets the _value of this element.
			 * @param val The new value for this element.
			 */
			void setValue( const xsAnyURI &val ) { _value = val; }
			/**
			 * Sets the _value of this element.
			 * @param val The new value for this element.
			 */
			void setValue( xsString val ) { _value = val; }

		protected:  // Value
			/**
			 * The xsAnyURI value of the text data of this element. 
			 */
			xsAnyURI _value;
		protected:
			/**
			 * Constructor
			 */
			domAuthor_website(DAE& dae) : daeElement(dae), _value(dae, *this) {}
			/**
			 * Destructor
			 */
			virtual ~domAuthor_website() {}
			/**
			 * Overloaded assignment operator
			 */
			virtual domAuthor_website &operator=( const domAuthor_website &cpy ) { (void)cpy; return *this; }

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

		class domAuthoring_tool;

		typedef daeSmartRef<domAuthoring_tool> domAuthoring_toolRef;
		typedef daeTArray<domAuthoring_toolRef> domAuthoring_tool_Array;

/**
 * The authoring_tool element contains a string with the authoring tool's
 * name. There may be only one authoring_tool element.
 */
		class domAuthoring_tool : public daeElement
		{
		public:
			virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::AUTHORING_TOOL; }
			static daeInt ID() { return 43; }
			virtual daeInt typeID() const { return ID(); }


		public:	//Accessors and Mutators
			/**
			 * Gets the value of this element.
			 * @return Returns a xsString of the value.
			 */
			xsString getValue() const { return _value; }
			/**
			 * Sets the _value of this element.
			 * @param val The new value for this element.
			 */
			void setValue( xsString val ) { *(daeStringRef*)&_value = val; }

		protected:  // Value
			/**
			 * The xsString value of the text data of this element. 
			 */
			xsString _value;
		protected:
			/**
			 * Constructor
			 */
			domAuthoring_tool(DAE& dae) : daeElement(dae), _value() {}
			/**
			 * Destructor
			 */
			virtual ~domAuthoring_tool() {}
			/**
			 * Overloaded assignment operator
			 */
			virtual domAuthoring_tool &operator=( const domAuthoring_tool &cpy ) { (void)cpy; return *this; }

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

		class domComments;

		typedef daeSmartRef<domComments> domCommentsRef;
		typedef daeTArray<domCommentsRef> domComments_Array;

/**
 * The comments element contains a string with comments from this contributor.
 * There may be only one comments element.
 */
		class domComments : public daeElement
		{
		public:
			virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::COMMENTS; }
			static daeInt ID() { return 44; }
			virtual daeInt typeID() const { return ID(); }


		public:	//Accessors and Mutators
			/**
			 * Gets the value of this element.
			 * @return Returns a xsString of the value.
			 */
			xsString getValue() const { return _value; }
			/**
			 * Sets the _value of this element.
			 * @param val The new value for this element.
			 */
			void setValue( xsString val ) { *(daeStringRef*)&_value = val; }

		protected:  // Value
			/**
			 * The xsString value of the text data of this element. 
			 */
			xsString _value;
		protected:
			/**
			 * Constructor
			 */
			domComments(DAE& dae) : daeElement(dae), _value() {}
			/**
			 * Destructor
			 */
			virtual ~domComments() {}
			/**
			 * Overloaded assignment operator
			 */
			virtual domComments &operator=( const domComments &cpy ) { (void)cpy; return *this; }

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

		class domCopyright;

		typedef daeSmartRef<domCopyright> domCopyrightRef;
		typedef daeTArray<domCopyrightRef> domCopyright_Array;

/**
 * The copyright element contains a string with copyright information. There
 * may be only one copyright element.
 */
		class domCopyright : public daeElement
		{
		public:
			virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::COPYRIGHT; }
			static daeInt ID() { return 45; }
			virtual daeInt typeID() const { return ID(); }


		public:	//Accessors and Mutators
			/**
			 * Gets the value of this element.
			 * @return Returns a xsString of the value.
			 */
			xsString getValue() const { return _value; }
			/**
			 * Sets the _value of this element.
			 * @param val The new value for this element.
			 */
			void setValue( xsString val ) { *(daeStringRef*)&_value = val; }

		protected:  // Value
			/**
			 * The xsString value of the text data of this element. 
			 */
			xsString _value;
		protected:
			/**
			 * Constructor
			 */
			domCopyright(DAE& dae) : daeElement(dae), _value() {}
			/**
			 * Destructor
			 */
			virtual ~domCopyright() {}
			/**
			 * Overloaded assignment operator
			 */
			virtual domCopyright &operator=( const domCopyright &cpy ) { (void)cpy; return *this; }

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

		class domSource_data;

		typedef daeSmartRef<domSource_data> domSource_dataRef;
		typedef daeTArray<domSource_dataRef> domSource_data_Array;

/**
 * The source_data element contains a URI reference to the source data used
 * for this asset. There may be only one source_data element.
 */
		class domSource_data : public daeElement
		{
		public:
			virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::SOURCE_DATA; }
			static daeInt ID() { return 46; }
			virtual daeInt typeID() const { return ID(); }


		public:	//Accessors and Mutators
			/**
			 * Gets the value of this element.
			 * @return Returns a xsAnyURI of the value.
			 */
			xsAnyURI &getValue() { return _value; }
			/**
			 * Gets the value of this element.
			 * @return Returns a constant xsAnyURI of the value.
			 */
			const xsAnyURI &getValue() const { return _value; }
			/**
			 * Sets the _value of this element.
			 * @param val The new value for this element.
			 */
			void setValue( const xsAnyURI &val ) { _value = val; }
			/**
			 * Sets the _value of this element.
			 * @param val The new value for this element.
			 */
			void setValue( xsString val ) { _value = val; }

		protected:  // Value
			/**
			 * The xsAnyURI value of the text data of this element. 
			 */
			xsAnyURI _value;
		protected:
			/**
			 * Constructor
			 */
			domSource_data(DAE& dae) : daeElement(dae), _value(dae, *this) {}
			/**
			 * Destructor
			 */
			virtual ~domSource_data() {}
			/**
			 * Overloaded assignment operator
			 */
			virtual domSource_data &operator=( const domSource_data &cpy ) { (void)cpy; return *this; }

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
 * The author element contains a string with the author's name. There may
 * be only one author element. @see domAuthor
 */
		domAuthorRef elemAuthor;
/**
 * Contains a string with author's email address. String that compliant with
 * RFC 2822 section 3.4. This element has no attributes. There may be only
 * one author email address. @see domAuthor_email
 */
		domAuthor_emailRef elemAuthor_email;
/**
 * Contains a URI reference (xs:anyURI) to author's website.  This element
 * has no attributes.   There may be only one author website. @see domAuthor_website
 */
		domAuthor_websiteRef elemAuthor_website;
/**
 * The authoring_tool element contains a string with the authoring tool's
 * name. There may be only one authoring_tool element. @see domAuthoring_tool
 */
		domAuthoring_toolRef elemAuthoring_tool;
/**
 * The comments element contains a string with comments from this contributor.
 * There may be only one comments element. @see domComments
 */
		domCommentsRef elemComments;
/**
 * The copyright element contains a string with copyright information. There
 * may be only one copyright element. @see domCopyright
 */
		domCopyrightRef elemCopyright;
/**
 * The source_data element contains a URI reference to the source data used
 * for this asset. There may be only one source_data element. @see domSource_data
 */
		domSource_dataRef elemSource_data;

	public:	//Accessors and Mutators
		/**
		 * Gets the author element.
		 * @return a daeSmartRef to the author element.
		 */
		const domAuthorRef getAuthor() const { return elemAuthor; }
		/**
		 * Gets the author_email element.
		 * @return a daeSmartRef to the author_email element.
		 */
		const domAuthor_emailRef getAuthor_email() const { return elemAuthor_email; }
		/**
		 * Gets the author_website element.
		 * @return a daeSmartRef to the author_website element.
		 */
		const domAuthor_websiteRef getAuthor_website() const { return elemAuthor_website; }
		/**
		 * Gets the authoring_tool element.
		 * @return a daeSmartRef to the authoring_tool element.
		 */
		const domAuthoring_toolRef getAuthoring_tool() const { return elemAuthoring_tool; }
		/**
		 * Gets the comments element.
		 * @return a daeSmartRef to the comments element.
		 */
		const domCommentsRef getComments() const { return elemComments; }
		/**
		 * Gets the copyright element.
		 * @return a daeSmartRef to the copyright element.
		 */
		const domCopyrightRef getCopyright() const { return elemCopyright; }
		/**
		 * Gets the source_data element.
		 * @return a daeSmartRef to the source_data element.
		 */
		const domSource_dataRef getSource_data() const { return elemSource_data; }
	protected:
		/**
		 * Constructor
		 */
		domContributor(DAE& dae) : daeElement(dae), elemAuthor(), elemAuthor_email(), elemAuthor_website(), elemAuthoring_tool(), elemComments(), elemCopyright(), elemSource_data() {}
		/**
		 * Destructor
		 */
		virtual ~domContributor() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domContributor &operator=( const domContributor &cpy ) { (void)cpy; return *this; }

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

	class domCoverage;

	typedef daeSmartRef<domCoverage> domCoverageRef;
	typedef daeTArray<domCoverageRef> domCoverage_Array;

	class domCoverage : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::COVERAGE; }
		static daeInt ID() { return 47; }
		virtual daeInt typeID() const { return ID(); }
	public:
		class domGeographic_location;

		typedef daeSmartRef<domGeographic_location> domGeographic_locationRef;
		typedef daeTArray<domGeographic_locationRef> domGeographic_location_Array;

/**
 * Specifies the location of the asset using the WGS84 coordinate system.
 */
		class domGeographic_location : public daeElement
		{
		public:
			virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::GEOGRAPHIC_LOCATION; }
			static daeInt ID() { return 48; }
			virtual daeInt typeID() const { return ID(); }
		public:
			class domLongitude;

			typedef daeSmartRef<domLongitude> domLongitudeRef;
			typedef daeTArray<domLongitudeRef> domLongitude_Array;

			class domLongitude : public daeElement
			{
			public:
				virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::LONGITUDE; }
				static daeInt ID() { return 49; }
				virtual daeInt typeID() const { return ID(); }


			public:	//Accessors and Mutators
				/**
				 * Gets the value of this element.
				 * @return a xsFloat of the value.
				 */
				xsFloat& getValue() { return _value; }
				/**
				 * Sets the _value of this element.
				 * @param val The new value for this element.
				 */
				void setValue( const xsFloat& val ) { _value = val; }

			protected:  // Value
				/**
				 * The xsFloat value of the text data of this element. 
				 */
				xsFloat _value;
			protected:
				/**
				 * Constructor
				 */
				domLongitude(DAE& dae) : daeElement(dae), _value() {}
				/**
				 * Destructor
				 */
				virtual ~domLongitude() {}
				/**
				 * Overloaded assignment operator
				 */
				virtual domLongitude &operator=( const domLongitude &cpy ) { (void)cpy; return *this; }

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

			class domLatitude;

			typedef daeSmartRef<domLatitude> domLatitudeRef;
			typedef daeTArray<domLatitudeRef> domLatitude_Array;

			class domLatitude : public daeElement
			{
			public:
				virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::LATITUDE; }
				static daeInt ID() { return 50; }
				virtual daeInt typeID() const { return ID(); }


			public:	//Accessors and Mutators
				/**
				 * Gets the value of this element.
				 * @return a xsFloat of the value.
				 */
				xsFloat& getValue() { return _value; }
				/**
				 * Sets the _value of this element.
				 * @param val The new value for this element.
				 */
				void setValue( const xsFloat& val ) { _value = val; }

			protected:  // Value
				/**
				 * The xsFloat value of the text data of this element. 
				 */
				xsFloat _value;
			protected:
				/**
				 * Constructor
				 */
				domLatitude(DAE& dae) : daeElement(dae), _value() {}
				/**
				 * Destructor
				 */
				virtual ~domLatitude() {}
				/**
				 * Overloaded assignment operator
				 */
				virtual domLatitude &operator=( const domLatitude &cpy ) { (void)cpy; return *this; }

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

			class domAltitude;

			typedef daeSmartRef<domAltitude> domAltitudeRef;
			typedef daeTArray<domAltitudeRef> domAltitude_Array;

/**
 * Altitude is always given in meters.  If the mode is "absolute", then the
 * value is interpreted as meters from mean sea level.  If the mode is "relativeToGround"
 * then the value is interpreted as meters above the actual ground elevation
 * at that particular location.
 */
			class domAltitude : public daeElement
			{
			public:
				virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::ALTITUDE; }
				static daeInt ID() { return 51; }
				virtual daeInt typeID() const { return ID(); }
			protected:  // Attribute
				domAltitude_mode attrMode;


			public:	//Accessors and Mutators
				/**
				 * Gets the mode attribute.
				 * @return Returns a domAltitude_mode of the mode attribute.
				 */
				domAltitude_mode getMode() const { return attrMode; }
				/**
				 * Sets the mode attribute.
				 * @param atMode The new value for the mode attribute.
				 */
				void setMode( domAltitude_mode atMode ) { attrMode = atMode; _validAttributeArray[0] = true; }

				/**
				 * Gets the value of this element.
				 * @return a xsFloat of the value.
				 */
				xsFloat& getValue() { return _value; }
				/**
				 * Sets the _value of this element.
				 * @param val The new value for this element.
				 */
				void setValue( const xsFloat& val ) { _value = val; }

			protected:  // Value
				/**
				 * The xsFloat value of the text data of this element. 
				 */
				xsFloat _value;
			protected:
				/**
				 * Constructor
				 */
				domAltitude(DAE& dae) : daeElement(dae), attrMode(), _value() {}
				/**
				 * Destructor
				 */
				virtual ~domAltitude() {}
				/**
				 * Overloaded assignment operator
				 */
				virtual domAltitude &operator=( const domAltitude &cpy ) { (void)cpy; return *this; }

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
			domLongitudeRef elemLongitude;
			domLatitudeRef elemLatitude;
/**
 * Altitude is always given in meters.  If the mode is "absolute", then the
 * value is interpreted as meters from mean sea level.  If the mode is "relativeToGround"
 * then the value is interpreted as meters above the actual ground elevation
 * at that particular location. @see domAltitude
 */
			domAltitudeRef elemAltitude;

		public:	//Accessors and Mutators
			/**
			 * Gets the longitude element.
			 * @return a daeSmartRef to the longitude element.
			 */
			const domLongitudeRef getLongitude() const { return elemLongitude; }
			/**
			 * Gets the latitude element.
			 * @return a daeSmartRef to the latitude element.
			 */
			const domLatitudeRef getLatitude() const { return elemLatitude; }
			/**
			 * Gets the altitude element.
			 * @return a daeSmartRef to the altitude element.
			 */
			const domAltitudeRef getAltitude() const { return elemAltitude; }
		protected:
			/**
			 * Constructor
			 */
			domGeographic_location(DAE& dae) : daeElement(dae), elemLongitude(), elemLatitude(), elemAltitude() {}
			/**
			 * Destructor
			 */
			virtual ~domGeographic_location() {}
			/**
			 * Overloaded assignment operator
			 */
			virtual domGeographic_location &operator=( const domGeographic_location &cpy ) { (void)cpy; return *this; }

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



	protected:  // Element
/**
 * Specifies the location of the asset using the WGS84 coordinate system.
 * @see domGeographic_location
 */
		domGeographic_locationRef elemGeographic_location;

	public:	//Accessors and Mutators
		/**
		 * Gets the geographic_location element.
		 * @return a daeSmartRef to the geographic_location element.
		 */
		const domGeographic_locationRef getGeographic_location() const { return elemGeographic_location; }
	protected:
		/**
		 * Constructor
		 */
		domCoverage(DAE& dae) : daeElement(dae), elemGeographic_location() {}
		/**
		 * Destructor
		 */
		virtual ~domCoverage() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domCoverage &operator=( const domCoverage &cpy ) { (void)cpy; return *this; }

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

	class domCreated;

	typedef daeSmartRef<domCreated> domCreatedRef;
	typedef daeTArray<domCreatedRef> domCreated_Array;

/**
 * The created element contains the date and time that the parent element
 * was created and is  represented in an ISO 8601 format.  The created element
 * may appear zero or one time.
 */
	class domCreated : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::CREATED; }
		static daeInt ID() { return 52; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return Returns a xsDateTime of the value.
		 */
		xsDateTime getValue() const { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( xsDateTime val ) { *(daeStringRef*)&_value = val; }

	protected:  // Value
		/**
		 * The xsDateTime value of the text data of this element. 
		 */
		xsDateTime _value;
	protected:
		/**
		 * Constructor
		 */
		domCreated(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domCreated() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domCreated &operator=( const domCreated &cpy ) { (void)cpy; return *this; }

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

	class domKeywords;

	typedef daeSmartRef<domKeywords> domKeywordsRef;
	typedef daeTArray<domKeywordsRef> domKeywords_Array;

/**
 * The keywords element contains a list of words used as search criteria for
 * the parent element.  There may be only one keywords element.
 */
	class domKeywords : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::KEYWORDS; }
		static daeInt ID() { return 53; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return Returns a xsToken of the value.
		 */
		xsToken getValue() const { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( xsToken val ) { *(daeStringRef*)&_value = val; }

	protected:  // Value
		/**
		 * The xsToken value of the text data of this element. 
		 */
		xsToken _value;
	protected:
		/**
		 * Constructor
		 */
		domKeywords(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domKeywords() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domKeywords &operator=( const domKeywords &cpy ) { (void)cpy; return *this; }

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

	class domModified;

	typedef daeSmartRef<domModified> domModifiedRef;
	typedef daeTArray<domModifiedRef> domModified_Array;

/**
 * The modified element contains the date and time that the parent element
 * was last modified and  represented in an ISO 8601 format. The modified
 * element may appear zero or one time.
 */
	class domModified : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::MODIFIED; }
		static daeInt ID() { return 54; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return Returns a xsDateTime of the value.
		 */
		xsDateTime getValue() const { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( xsDateTime val ) { *(daeStringRef*)&_value = val; }

	protected:  // Value
		/**
		 * The xsDateTime value of the text data of this element. 
		 */
		xsDateTime _value;
	protected:
		/**
		 * Constructor
		 */
		domModified(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domModified() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domModified &operator=( const domModified &cpy ) { (void)cpy; return *this; }

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

	class domRevision;

	typedef daeSmartRef<domRevision> domRevisionRef;
	typedef daeTArray<domRevisionRef> domRevision_Array;

/**
 * The revision element contains the revision information for the parent element.
 * The revision  element may appear zero or one time.
 */
	class domRevision : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::REVISION; }
		static daeInt ID() { return 55; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return Returns a xsString of the value.
		 */
		xsString getValue() const { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( xsString val ) { *(daeStringRef*)&_value = val; }

	protected:  // Value
		/**
		 * The xsString value of the text data of this element. 
		 */
		xsString _value;
	protected:
		/**
		 * Constructor
		 */
		domRevision(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domRevision() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domRevision &operator=( const domRevision &cpy ) { (void)cpy; return *this; }

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

	class domSubject;

	typedef daeSmartRef<domSubject> domSubjectRef;
	typedef daeTArray<domSubjectRef> domSubject_Array;

/**
 * The subject element contains a description of the topical subject of the
 * parent element. The  subject element may appear zero or one time.
 */
	class domSubject : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::SUBJECT; }
		static daeInt ID() { return 56; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return Returns a xsString of the value.
		 */
		xsString getValue() const { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( xsString val ) { *(daeStringRef*)&_value = val; }

	protected:  // Value
		/**
		 * The xsString value of the text data of this element. 
		 */
		xsString _value;
	protected:
		/**
		 * Constructor
		 */
		domSubject(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domSubject() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domSubject &operator=( const domSubject &cpy ) { (void)cpy; return *this; }

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

	class domTitle;

	typedef daeSmartRef<domTitle> domTitleRef;
	typedef daeTArray<domTitleRef> domTitle_Array;

/**
 * The title element contains the title information for the parent element.
 * The title element may  appear zero or one time.
 */
	class domTitle : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::TITLE; }
		static daeInt ID() { return 57; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return Returns a xsString of the value.
		 */
		xsString getValue() const { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( xsString val ) { *(daeStringRef*)&_value = val; }

	protected:  // Value
		/**
		 * The xsString value of the text data of this element. 
		 */
		xsString _value;
	protected:
		/**
		 * Constructor
		 */
		domTitle(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domTitle() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domTitle &operator=( const domTitle &cpy ) { (void)cpy; return *this; }

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

	class domUnit;

	typedef daeSmartRef<domUnit> domUnitRef;
	typedef daeTArray<domUnitRef> domUnit_Array;

/**
 * The unit element contains descriptive information about unit of measure.
 * It has attributes for  the name of the unit and the measurement with respect
 * to the meter. The unit element may appear  zero or one time.
 */
	class domUnit : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::UNIT; }
		static daeInt ID() { return 58; }
		virtual daeInt typeID() const { return ID(); }
	protected:  // Attributes
/**
 *  The meter attribute specifies the measurement with respect to the meter.
 * The default  value for the meter attribute is "1.0". 
 */
		domFloat attrMeter;
/**
 *  The name attribute specifies the name of the unit. The default value for
 * the name  attribute is "meter". 
 */
		xsNMTOKEN attrName;


	public:	//Accessors and Mutators
		/**
		 * Gets the meter attribute.
		 * @return Returns a domFloat of the meter attribute.
		 */
		domFloat getMeter() const { return attrMeter; }
		/**
		 * Sets the meter attribute.
		 * @param atMeter The new value for the meter attribute.
		 */
		void setMeter( domFloat atMeter ) { attrMeter = atMeter; _validAttributeArray[0] = true; }

		/**
		 * Gets the name attribute.
		 * @return Returns a xsNMTOKEN of the name attribute.
		 */
		xsNMTOKEN getName() const { return attrName; }
		/**
		 * Sets the name attribute.
		 * @param atName The new value for the name attribute.
		 */
		void setName( xsNMTOKEN atName ) { *(daeStringRef*)&attrName = atName; _validAttributeArray[1] = true; }

	protected:
		/**
		 * Constructor
		 */
		domUnit(DAE& dae) : daeElement(dae), attrMeter(), attrName() {}
		/**
		 * Destructor
		 */
		virtual ~domUnit() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domUnit &operator=( const domUnit &cpy ) { (void)cpy; return *this; }

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

	class domUp_axis;

	typedef daeSmartRef<domUp_axis> domUp_axisRef;
	typedef daeTArray<domUp_axisRef> domUp_axis_Array;

/**
 * The up_axis element contains descriptive information about coordinate system
 * of the geometric  data. All coordinates are right-handed by definition.
 * This element specifies which axis is  considered up. The default is the
 * Y-axis. The up_axis element may appear zero or one time.
 */
	class domUp_axis : public daeElement
	{
	public:
		virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::UP_AXIS; }
		static daeInt ID() { return 59; }
		virtual daeInt typeID() const { return ID(); }


	public:	//Accessors and Mutators
		/**
		 * Gets the value of this element.
		 * @return a ::domUp_axis of the value.
		 */
		::domUp_axis& getValue() { return _value; }
		/**
		 * Sets the _value of this element.
		 * @param val The new value for this element.
		 */
		void setValue( const ::domUp_axis& val ) { _value = val; }

	protected:  // Value
		/**
		 * The ::domUp_axis value of the text data of this element. 
		 */
		::domUp_axis _value;
	protected:
		/**
		 * Constructor
		 */
		domUp_axis(DAE& dae) : daeElement(dae), _value() {}
		/**
		 * Destructor
		 */
		virtual ~domUp_axis() {}
		/**
		 * Overloaded assignment operator
		 */
		virtual domUp_axis &operator=( const domUp_axis &cpy ) { (void)cpy; return *this; }

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
 * The contributor element defines authoring information for asset management
 * @see domContributor
 */
	domContributor_Array elemContributor_array;
	domCoverageRef elemCoverage;
/**
 * The created element contains the date and time that the parent element
 * was created and is  represented in an ISO 8601 format.  The created element
 * may appear zero or one time. @see domCreated
 */
	domCreatedRef elemCreated;
/**
 * The keywords element contains a list of words used as search criteria for
 * the parent element.  There may be only one keywords element. @see domKeywords
 */
	domKeywordsRef elemKeywords;
/**
 * The modified element contains the date and time that the parent element
 * was last modified and  represented in an ISO 8601 format. The modified
 * element may appear zero or one time. @see domModified
 */
	domModifiedRef elemModified;
/**
 * The revision element contains the revision information for the parent element.
 * The revision  element may appear zero or one time. @see domRevision
 */
	domRevisionRef elemRevision;
/**
 * The subject element contains a description of the topical subject of the
 * parent element. The  subject element may appear zero or one time. @see
 * domSubject
 */
	domSubjectRef elemSubject;
/**
 * The title element contains the title information for the parent element.
 * The title element may  appear zero or one time. @see domTitle
 */
	domTitleRef elemTitle;
/**
 * The unit element contains descriptive information about unit of measure.
 * It has attributes for  the name of the unit and the measurement with respect
 * to the meter. The unit element may appear  zero or one time. @see domUnit
 */
	domUnitRef elemUnit;
/**
 * The up_axis element contains descriptive information about coordinate system
 * of the geometric  data. All coordinates are right-handed by definition.
 * This element specifies which axis is  considered up. The default is the
 * Y-axis. The up_axis element may appear zero or one time. @see domUp_axis
 */
	domUp_axisRef elemUp_axis;
	domExtra_Array elemExtra_array;

public:	//Accessors and Mutators
	/**
	 * Gets the contributor element array.
	 * @return Returns a reference to the array of contributor elements.
	 */
	domContributor_Array &getContributor_array() { return elemContributor_array; }
	/**
	 * Gets the contributor element array.
	 * @return Returns a constant reference to the array of contributor elements.
	 */
	const domContributor_Array &getContributor_array() const { return elemContributor_array; }
	/**
	 * Gets the coverage element.
	 * @return a daeSmartRef to the coverage element.
	 */
	const domCoverageRef getCoverage() const { return elemCoverage; }
	/**
	 * Gets the created element.
	 * @return a daeSmartRef to the created element.
	 */
	const domCreatedRef getCreated() const { return elemCreated; }
	/**
	 * Gets the keywords element.
	 * @return a daeSmartRef to the keywords element.
	 */
	const domKeywordsRef getKeywords() const { return elemKeywords; }
	/**
	 * Gets the modified element.
	 * @return a daeSmartRef to the modified element.
	 */
	const domModifiedRef getModified() const { return elemModified; }
	/**
	 * Gets the revision element.
	 * @return a daeSmartRef to the revision element.
	 */
	const domRevisionRef getRevision() const { return elemRevision; }
	/**
	 * Gets the subject element.
	 * @return a daeSmartRef to the subject element.
	 */
	const domSubjectRef getSubject() const { return elemSubject; }
	/**
	 * Gets the title element.
	 * @return a daeSmartRef to the title element.
	 */
	const domTitleRef getTitle() const { return elemTitle; }
	/**
	 * Gets the unit element.
	 * @return a daeSmartRef to the unit element.
	 */
	const domUnitRef getUnit() const { return elemUnit; }
	/**
	 * Gets the up_axis element.
	 * @return a daeSmartRef to the up_axis element.
	 */
	const domUp_axisRef getUp_axis() const { return elemUp_axis; }
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
	domAsset(DAE& dae) : daeElement(dae), elemContributor_array(), elemCoverage(), elemCreated(), elemKeywords(), elemModified(), elemRevision(), elemSubject(), elemTitle(), elemUnit(), elemUp_axis(), elemExtra_array() {}
	/**
	 * Destructor
	 */
	virtual ~domAsset() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domAsset &operator=( const domAsset &cpy ) { (void)cpy; return *this; }

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
