#ifndef __domLibrary_kinematics_scenes_h__
#define __domLibrary_kinematics_scenes_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domAsset.h>
#include <dom/domKinematics_scene.h>
#include <dom/domExtra.h>
class DAE;

class domLibrary_kinematics_scenes : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::LIBRARY_KINEMATICS_SCENES; }
	static daeInt ID() { return 452; }
	virtual daeInt typeID() const { return ID(); }
protected:  // Attributes
	xsID attrId;
	xsToken attrName;

protected:  // Elements
	domAssetRef elemAsset;
	domKinematics_scene_Array elemKinematics_scene_array;
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
	void setId( xsID atId ) { *(daeStringRef*)&attrId = atId;
		if( _document != NULL ) _document->changeElementID( this, attrId );
	}

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
	 * Gets the asset element.
	 * @return a daeSmartRef to the asset element.
	 */
	const domAssetRef getAsset() const { return elemAsset; }
	/**
	 * Gets the kinematics_scene element array.
	 * @return Returns a reference to the array of kinematics_scene elements.
	 */
	domKinematics_scene_Array &getKinematics_scene_array() { return elemKinematics_scene_array; }
	/**
	 * Gets the kinematics_scene element array.
	 * @return Returns a constant reference to the array of kinematics_scene elements.
	 */
	const domKinematics_scene_Array &getKinematics_scene_array() const { return elemKinematics_scene_array; }
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
	domLibrary_kinematics_scenes(DAE& dae) : daeElement(dae), attrId(), attrName(), elemAsset(), elemKinematics_scene_array(), elemExtra_array() {}
	/**
	 * Destructor
	 */
	virtual ~domLibrary_kinematics_scenes() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domLibrary_kinematics_scenes &operator=( const domLibrary_kinematics_scenes &cpy ) { (void)cpy; return *this; }

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
