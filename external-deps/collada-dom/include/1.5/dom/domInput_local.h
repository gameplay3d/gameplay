#ifndef __domInput_local_h__
#define __domInput_local_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

class DAE;

/**
 * The input_local_type element is used to represent inputs that can only
 * reference resources declared in the same document.
 */
class domInput_local : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::INPUT_LOCAL; }
	static daeInt ID() { return 1; }
	virtual daeInt typeID() const { return ID(); }
protected:  // Attributes
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


public:	//Accessors and Mutators
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

protected:
	/**
	 * Constructor
	 */
	domInput_local(DAE& dae) : daeElement(dae), attrSemantic(), attrSource(dae, *this) {}
	/**
	 * Destructor
	 */
	virtual ~domInput_local() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domInput_local &operator=( const domInput_local &cpy ) { (void)cpy; return *this; }

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
