#ifndef __domSampler_h__
#define __domSampler_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

#include <dom/domInput_local.h>
class DAE;

/**
 * The sampler element declares an N-dimensional function used for animation.
 * Animation function curves  are represented by 1-D sampler elements in COLLADA.
 * The sampler defines sampling points and how to  interpolate between them.
 */
class domSampler : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::SAMPLER; }
	static daeInt ID() { return 24; }
	virtual daeInt typeID() const { return ID(); }
protected:  // Attributes
/**
 *  The id attribute is a text string containing the unique identifier of
 * this element. This value  must be unique within the instance document.
 * Optional attribute. 
 */
	xsID attrId;
/**
 *  Indicates what the sampled value should be before the first key. 
 */
	domSampler_behavior attrPre_behavior;
/**
 *  Indicates what the sampled value should be after the last key. 
 */
	domSampler_behavior attrPost_behavior;

protected:  // Element
/**
 * The input element must occur at least one time. These inputs are local
 * inputs. @see domInput
 */
	domInput_local_Array elemInput_array;

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
	 * Gets the pre_behavior attribute.
	 * @return Returns a domSampler_behavior of the pre_behavior attribute.
	 */
	domSampler_behavior getPre_behavior() const { return attrPre_behavior; }
	/**
	 * Sets the pre_behavior attribute.
	 * @param atPre_behavior The new value for the pre_behavior attribute.
	 */
	void setPre_behavior( domSampler_behavior atPre_behavior ) { attrPre_behavior = atPre_behavior; }

	/**
	 * Gets the post_behavior attribute.
	 * @return Returns a domSampler_behavior of the post_behavior attribute.
	 */
	domSampler_behavior getPost_behavior() const { return attrPost_behavior; }
	/**
	 * Sets the post_behavior attribute.
	 * @param atPost_behavior The new value for the post_behavior attribute.
	 */
	void setPost_behavior( domSampler_behavior atPost_behavior ) { attrPost_behavior = atPost_behavior; }

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
protected:
	/**
	 * Constructor
	 */
	domSampler(DAE& dae) : daeElement(dae), attrId(), attrPre_behavior(), attrPost_behavior(), elemInput_array() {}
	/**
	 * Destructor
	 */
	virtual ~domSampler() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domSampler &operator=( const domSampler &cpy ) { (void)cpy; return *this; }

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
