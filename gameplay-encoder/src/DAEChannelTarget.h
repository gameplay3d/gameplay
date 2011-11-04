#ifndef CHANNELTARGET_H_
#define CHANNELTARGET_H_

#include <dae.h>
#include <dae/daeSIDResolver.h>
#include <dae/domAny.h>
#include <dom/domCOLLADA.h>
#include <dom/domConstants.h>
#include <dom/domElements.h>
#include <dom/domProfile_COMMON.h>

#include <vector>

#include "Base.h"

namespace gameplay
{

/**
 * DAEChannelTarget represents the target attribute of the COLLADA dom element "channel".
 * The syntax for the channel target is in the COLLADA spec under "COLLADA Target Addressing".
 */
class DAEChannelTarget
{
public:

    /**
     * Constructs the DAEChannelTarget from the given channel element.
     */
    DAEChannelTarget(const domChannel* channelRef);

    /**
     * Destructor.
     */
    virtual ~DAEChannelTarget(void);

    /**
     * Returns a pointer to the dom element that this targeted.
     * 
     * @return Pointer to the dom element or NULL if not found.
     */
    daeElement* getTargetElement();

    /**
     * Returns the target ID string.
     */
    const std::string& getTargetId() const;

    /**
     * Returns the number of target attributes for this channel target.
     */
    size_t getTargetAttributeCount() const;

    /**
     * Returns the attribute element at the given index.
     */
    daeElement* getTargetAttribute(size_t index);

    /**
     * Returns property name of the attribute at the given index.
     * The property name is copied to str.
     * If the attribute is not found or it doesn't have a property, str is cleared.
     * 
     * @param index Index of the attribute.
     * @param str Destination string to copy the property name to.
     */
    void getPropertyName(size_t index, std::string* str);

private:
    /**
     * Pointer to the <channel> element.
     */
    const domChannel* _channel;

    domElement* _targetElement;

    /**
     * The first part is the id attribute of an element in the instance document
     * or a dot segment (".") indicating that this is a relative address.
     */
    std::string _targetId;

    /**
     * A channel target can have zero or more target attributes.
     * Each target attribute my have a property.
     * Example: "cube/Translate.X/Translate.Y"
     * Result: attributeIds will contain 2 elements. "Translate.X" and "Translate.Y"
     * Refer to the COLLADA spec "COLLADA Target Addressing".
     */
    std::vector<std::string> _attributeIds;
};

}

#endif
