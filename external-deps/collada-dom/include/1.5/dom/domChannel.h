#ifndef __domChannel_h__
#define __domChannel_h__

#include <dae/daeDocument.h>
#include <dom/domTypes.h>
#include <dom/domElements.h>

class DAE;

/**
 * The channel element declares an output channel of an animation.
 */
class domChannel : public daeElement
{
public:
	virtual COLLADA_TYPE::TypeEnum getElementType() const { return COLLADA_TYPE::CHANNEL; }
	static daeInt ID() { return 23; }
	virtual daeInt typeID() const { return ID(); }
protected:  // Attributes
/**
 *  The source attribute indicates the location of the sampler using a URL
 * expression.  The sampler must be declared within the same document. Required
 * attribute. 
 */
	domUrifragment attrSource;
/**
 *  The target attribute indicates the location of the element bound to the
 * output of the sampler.  This text string is a path-name following a simple
 * syntax described in Address Syntax.  Required attribute. 
 */
	xsToken attrTarget;


public:	//Accessors and Mutators
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
	 * Gets the target attribute.
	 * @return Returns a xsToken of the target attribute.
	 */
	xsToken getTarget() const { return attrTarget; }
	/**
	 * Sets the target attribute.
	 * @param atTarget The new value for the target attribute.
	 */
	void setTarget( xsToken atTarget ) { *(daeStringRef*)&attrTarget = atTarget;}

protected:
	/**
	 * Constructor
	 */
	domChannel(DAE& dae) : daeElement(dae), attrSource(dae, *this), attrTarget() {}
	/**
	 * Destructor
	 */
	virtual ~domChannel() {}
	/**
	 * Overloaded assignment operator
	 */
	virtual domChannel &operator=( const domChannel &cpy ) { (void)cpy; return *this; }

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
