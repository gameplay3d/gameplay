#pragma once

#include "Serializable.h"

namespace gameplay
{

/**
 * Defines a class for dynamic object activation.
 */
class Activator
{
    friend class Game;

public:

    /**
     * Destructor.
     */
    ~Activator();
    
    /**
     * Gets the activator.
     *
     * @return The activator.
     */
    static Activator* getActivator();

    /**
     * Callback used to create objects on registered serializable classes.
     */
    typedef std::shared_ptr<Serializable> (*CreateObjectCallback)();
    
    /**
     * Callback used to convert enum values to string on registered serializable enums.
     *
     * @param enumName The enum name to convert.
     * @param value The enum value to be converted to string.
     * @return The string converted or nullptr if not converted successfully.
     */
    typedef std::string (*EnumToStringCallback)(const std::string& enumName, int value);
    
    /**
     * Callback used to parse the enum string to value on registered serializable enums.
     *
     * @param enumName The enum name to parse the string for.
     * @param str The string to be parsed
     * @return The enum value or -1 is not parsed successfully.
     */
    typedef int (*EnumParseCallback)(const std::string& enumName, const std::string& str);
    
    /**
     * Creates a new shared object for the specified class name.
     *
     * @param className The serialized class name of the instance to be created.
     * @return The new object instance.
     */
    std::shared_ptr<Serializable> createObject(const std::string& className);
    
    /**
     * Converts the enum value to a string representation.
     *
     * Enum string values are returned uppercase.
     *
     * @param enumName The enum name to convert.
     * @param value The enum value to be converted to string.
     * @return The string converted or default enum string if not matching.
     */
    std::string enumToString(const std::string& enumName, int value);
    
    /**
     * Parses the string representation of the enumeration.
     *
     * @param enumName The enum name to parse the string for.
     * @param str The string to be parsed
     * @return The enum value or -1 is not parsed successfully.
     */
    int enumParse(const std::string& enumName, const std::string& str);
    
    /**
     * Registers a callback for functor for instantiating new object for the specified type name.
     *
     * The className should be namespaced. Ex. gameplay::SceneObject
     *
     * @param className The type name to be registered to create a custom object.
     * @param create The create instance callback used to dynamically create a serializable object.
     */
    void registerType(const std::string& className, CreateObjectCallback create);
    
    /**
     * Registers an enum for resolving enumerated values to/from strings. 
     *
     * The enumName should be namespaced. Ex gameplay::Camera::Type
     *
     * If the enumeration support bitwise operations, it must return the string representation.
     * 
     * @param enumName The enum name to be register for to/from string resolvers.
     * @param enumToString The enumToString callback function.
     * @param enumParse The enumParse callback function.
     */
    void registerEnum(const std::string& enumName, EnumToStringCallback enumToString, EnumParseCallback enumParse);
    
private:
    
    Activator();
    void registerSystemTypes();
    std::map<std::string, CreateObjectCallback> _classes;
    std::map<std::string, std::pair<EnumToStringCallback, EnumParseCallback>> _enums;
};

}
