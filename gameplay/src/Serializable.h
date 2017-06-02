#pragma once

#include "Serializer.h"

namespace gameplay
{
    
/**
 * Defines a object that can be read(deserialized) and written(serailized) to file.
 *
 * For object encoding/compatibility,  properties must be serialized in the same
 * order there are deserialized.
 */
class Serializable
{
public:

    /**
     * Destructor
     */
    virtual ~Serializable() { }
    
    /**
     * Gets the class name string for the object.
     *
     * This is used by the Serializer when reading/writing objects.
     * The class name should be namespaced. Ex: gameplay::SceneObject
     */
    virtual std::string getClassName() { return "";  }

    /**
     * Event handled when an object is asked to serialize itself.
     * 
     * @param serializer The serializer to write properties to.
     */
    virtual void onSerialize(Serializer* serializer) { }

    /**
     * Event handled when an object properties are being deserialized.
     *
     * @param serializer The serializer to read properties from.
     */
    virtual void onDeserialize(Serializer* serializer) { }
};

}
