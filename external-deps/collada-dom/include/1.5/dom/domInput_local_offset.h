#ifndef __domInput_local_offset_h__
#define __domInput_local_offset_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

class DAE;

/**
 * The input_local_offset_type element is used to represent indexed inputs
 * that can only reference resources declared in the same document.
 */
class domInput_local_offset : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::INPUT_LOCAL_OFFSET; }
	static daeInt ID() { return 2; }
	virtual daeInt typeID() const { return ID(); }
protected:  // Attributes
/**
 *  The offset attribute represents the offset into the list of indices. 
 * If two input elements share  the same offset, they will be indexed the
 * same.  This works as a simple form of compression for the  list of indices
 * as well as defining the order the inputs should be used in.  Required attribute.
 */
	domUint attrOffset;
/**
 *  The semantic attribute is the user-defined meaning of the input connection.
 * Required attribute. 
 */
	xsNMTOKEN attrSemantic;
/**
 *  The source attribute indicates the location of the data source. Required
 * attribute. 
 */
	domUrifragment attrSource;
/**
 *  The set attribute indicates which inputs should be grouped together as
 * a single set. This is helpful  when multiple inputs share the same semantics.
 */
	domUint attrSet;


public:	//Accessors and Mutators
	/**
	 * Gets the offset attribute.
	 * @return Returns a domUint of the offset attribute.
	 */
	domUint getOffset() const { return attrOffset; }
	/**
	 * Sets the offset attribute.
	 * @param atOffset The new value for the offset attribute.
	 */
	void setOffset( domUint atOffset ) { attrOffset = atOffset; }

	/**
	 * Gets the semantic attribute.
	 * @return Returns a xsNMTOKEN of the semantic attribute.
	 */
	xsNMTOKEN getSemantic() const { return attrSemantic; }
	/**
	 * Sets the semantic attribute.
	 * @param atSemantic The new value for the semantic attribute.
	 */
	void setSemantic( xsNMTOKEN atSemantic ) { *(daeStringRef*)&attrSemantic = atSemantic;}

	/**
	 * Gets the source attribute.
	 * @return Returns a domUrifragment of the source attribute.
	 */
	domUrifragment getSource() const { return attrSource; }
	/**
	 * Sets the source attribute.
	 * @param atSource The new value for the source attribute.
	 */
	void setSource( domUrifragment atSource ) { attrSource = atSource; }

	/**
	 * Gets the set attribute.
	 * @return Returns a domUint of the set attribute.
	 */
	domUint getSet() const { return attrSet; }
	/**
	 * Sets the set attribute.
	 * @param atSet The new value for the set attribute.
	 */
	void setSet( domUint atSet ) { attrSet = atSet; }

protected:
	/**
	 * Constructor
	 */
	domInput_local_offset(DAE& dae) : daeElement(dae), attrOffset(), attrSemantic(), attrSource(dae, *this), attrSet() {}
	/**
	 * Destructor
	 */
	virtual ~domInput_local_offset() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domInput_local_offset &operator=( const domInput_local_offset &cpy ) { (void)cpy; return *this; }

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
