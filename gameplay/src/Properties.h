/*
 * Properties.h
 */

#ifndef PROPERTIES_H_
#define PROPERTIES_H_

#include "Base.h"
#include "Matrix.h"
#include "Color.h"

namespace gameplay
{

/**
 * Defines a utility for loading text files in the GamePlay "properties" files
 * and reading primitive types and GamePlay math classes out of them.
 *
 * This class is used by ParticleEmitter, Animation and Materials to create objects
 * of these types from text files.
 *
 * A properties file has very simple syntax and can contain only namespaces and
 * name/value pairs (the properties of a namespace).  
 * The file can have any file extension a user specifies.
 *
 * Here's an example of a simple
 * file that uses all the available features of the markup language:
 *
 * --- File Start: example.properties ---
 *
 * // This is a comment.
 *
 * // This property is in the default namespace:
 * integerProperty = 5
 *
 * // This line defines a namespace of type "mynamespace" without an ID:
 * mynamespace
 * {
 *      // This namespace can be retrieved by searching for its ID, "spriteTexture":
 *      texture spriteTexture 
 *      {
 *          fileName = sprite.png
 *          width = 64
 *          height = 64
 *      }
 *
 *      // This property is in the "space" namespace:
 *      booleanProperty = true
 *
 *      // It's legal to have a name without a value if you leave out the '=' character:
 *      foo
 *
 *      // In fact, the '=' character is optional if you'd rather write:
 *      bar 23
 *
 *      // But don't write this or you'll get an error:
 *      // illegalProperty =
 *
 *      // Or this:
 *      // = 15
 *
 *      // Properties objects let you retrieve values as various types.
 *      floatProperty = 3.333
 *      stringProperty = This is a string.
 *      vector3Property = 1.0, 5.0, 3.55
 *      colorProperty = 1.0, 0.4, 0.0, 1.0
 * }
 * --- File End ---
 *
 * Retrieving information out of a file like this could be done in two ways.  If the
 * available namespaces and name/value pairs are known in advance they can be queried by ID or name.
 * For example, if the namespace "spriteTexture" and its properties are required then they can
 * be retrieved with a call to getNamespace() followed by calls to getString() and getInt().
 * A namespace is stored and retrieved as a Properties object.
 * Reading the spriteTexture properties out of the file above in this way could be done with the following code:
 *
 *      // Create the top-level Properties object.
 *      Properties* properties = Properties::create("example.properties");
 *      // Retrieve the "spriteTexture" namespace.
 *      Properties* spriteTexture = properties->getNamespace("spriteTexture");
 *
 *      // Get the values of known texture properties out of the namespace.
 *      const char* fileName = spriteTexture->getString("fileName");
 *      int width = spriteTexture->getInt("width");
 *      int height = spriteTexture->getInt("height");
 *
 *      // Deleting the top-level Properties object will clean up all nested namespaces.
 *      SAFE_DELETE(properties);
 *
 * On the other hand, if the structure of the file is not known in advance its 
 * namespaces and name/value pairs can be retrieved one by one using the getNextNamespace()
 * and getNextProperty() methods.  The following method prints the contents of any properties file
 * to the console:
 *
 * void printProperties(Properties* properties)
 * {
 *     // Print the name and ID of the current namespace.
 *     const char* spacename = properties->getNamespace();
 *     const char* id = properties->getID();
 *     WARN_VARG("Namespace: %s  ID: %s\n{", spacename, id);
 *
 *     // Print all properties in this namespace.
 *     const char* name = properties->getNextProperty();
 *     const char* value = NULL;
 *     while (name != NULL)
 *     {
 *         value = properties->getString(name);
 *         WARN_VARG("%s = %s", name, value);
 *         name = properties->getNextProperty();
 *     }
 *     WARN("}\n");
 *
 *     // Print the properties of every namespace within this one.
 *     Properties* space = properties->getNextNamespace();
 *     while (space != NULL)
 *     {
 *         printProperties(space);
 *         space = properties->getNextNamespace();
 *     }
 *  }
 *
 * Note that this method does not keep track of the namespace hierarchy, but could be
 * modified to do so.  Also note that nothing in a properties file indicates the type
 * of a property. If the type is unknown, its string can be retrieved and interpreted
 * as necessary.
 */
class Properties
{
public:

    /**
     * Creates a Properties runtime settings from a specified file path.
     *
     * @param filePath The file to create the properties from.
     */
    static Properties* create(const char* filePath);

    /**
     * Destructor.
     */
    ~Properties();

    /**
     * Get the name of the next property.
     */
    const char* getNextProperty();

    /**
     * Get the next namespace.
     */
    Properties* getNextNamespace();

    /** 
     * Rewind the getNextProperty() and getNextNamespace() iterators
     * to the beginning of the file.
     */
    void rewind();

    /**
     * Get a specific namespace by ID.  This method will perform a depth-first search
     * on all namespaces and inner namespaces within this Property.
     *
     * @param The ID of a specific namespace.
     * 
     * @return A properties object with the given ID.
     */
    Properties* getNamespace(const char* id) const;

    /**
     * Get the name of this Property's namespace.
     *
     * @return The name of this Property's namespace.
     */
    const char* getNamespace() const;

    /**
     * Get the ID of this Property's namespace. The ID should be a unique identifier,
     * but its uniqueness is not enforced.
     *
     * @return The ID of this Property's namespace.
     */
    const char* getID() const;

    /**
     * Check if a property with the given name is specified in this Properties object.
     *
     * @param name The name of the property to query.
     * 
     * @return True if the property exists, false otherwise.
     */
    bool exists(const char* name) const;

    /**
     * Get the value of the given property as a string. This can always be retrieved,
     * whatever the intended type of the property.
     *
     * @param name The name of the property to interpret.
     * 
     * @return The value of the given property as a string, or the empty string if no property with that name exists.
     */
    const char* getString(const char* name) const;

    /**
     * Interpret the value of the given property as a boolean.
     *
     * @param name The name of the property to interpret.
     * 
     * @return true if the property exists and its value is "true", otherwise false.
     */
    bool getBool(const char* name) const;

    /**
     * Interpret the value of the given property as an integer.
     * If the property does not exist, zero will be returned.
     * If the property exists but could not be scanned, an error will be logged and zero will be returned.
     *
     * @param name The name of the property to interpret.
     * 
     * @return The value of the given property interpreted as an integer.
     *   Zero if the property does not exist or could not be scanned.
     */
    int getInt(const char* name) const;

    /**
     * Interpret the value of the given property as a floating-point number.
     * If the property does not exist, zero will be returned.
     * If the property exists but could not be scanned, an error will be logged and zero will be returned.
     *
     * @param name The name of the property to interpret.
     * 
     * @return The value of the given property interpreted as a float.
     *   Zero if the property does not exist or could not be scanned.
     */
    float getFloat(const char* name) const;

    /**
     * Interpret the value of the given property as a long integer.
     * If the property does not exist, zero will be returned.
     * If the property exists but could not be scanned, an error will be logged and zero will be returned.
     *
     * @param name The name of the property to interpret.
     * 
     * @return The value of the given property interpreted as a long.
     *   Zero if the property does not exist or could not be scanned.
     */
    long getLong(const char* name) const;
    
    /**
     * Read the specified number of floats from the value of the specified property and copy
     * the floats into the given array.
     *
     * @param name The name of the property to interpret.
     * @param out The array to set to this property's interpreted value.
     * @param count The number of float elements to read.
     * 
     * @return true if the property was found and all elements in the array parsed successfully, false if not.
     */
    bool getFloatArray(const char* name, float* out, unsigned int count) const;
    
    /**
     * Interpret the value of the given property as a Matrix.
     * If the property does not exist, out will be set to the identity matrix.
     * If the property exists but could not be scanned, an error will be logged and out will be set
     * to the identity matrix.
     *
     * @param name The name of the property to interpret.
     * @param out The matrix to set to this property's interpreted value.
     * 
     * @return True on success, false if the property does not exist or could not be scanned.
     */
    bool getMatrix(const char* name, Matrix* out) const;

    /**
     * Interpret the value of the given property as a Vector2.
     * If the property does not exist, out will be set to Vector2(0.0f, 0.0f).
     * If the property exists but could not be scanned, an error will be logged and out will be set
     * to Vector2(0.0f, 0.0f).
     *
     * @param name The name of the property to interpret.
     * @param out The matrix to set to this property's interpreted value.
     * 
     * @return True on success, false if the property does not exist or could not be scanned.
     */
    bool getVector2(const char* name, Vector2* out) const;

    /**
     * Interpret the value of the given property as a Vector3.
     * If the property does not exist, out will be set to Vector3(0.0f, 0.0f, 0.0f).
     * If the property exists but could not be scanned, an error will be logged and out will be set
     * to Vector3(0.0f, 0.0f, 0.0f).
     *
     * @param name The name of the property to interpret.
     * @param out The matrix to set to this property's interpreted value.
     * 
     * @return True on success, false if the property does not exist or could not be scanned.
     */
    bool getVector3(const char* name, Vector3* out) const;

    /**
     * Interpret the value of the given property as a Vector4.
     * If the property does not exist, out will be set to Vector4(0.0f, 0.0f, 0.0f, 0.0f).
     * If the property exists but could not be scanned, an error will be logged and out will be set
     * to Vector4(0.0f, 0.0f, 0.0f, 0.0f).
     *
     * @param name The name of the property to interpret.
     * @param out The matrix to set to this property's interpreted value.
     * 
     * @return True on success, false if the property does not exist or could not be scanned.
     */
    bool getVector4(const char* name, Vector4* out) const;

    /**
     * Interpret the value of the given property as a Color.
     * If the property does not exist, out will be set to Color(0.0f, 0.0f, 0.0f, 0.0f).
     * If the property exists but could not be scanned, an error will be logged and out will be set
     * to Color(0.0f, 0.0f, 0.0f, 0.0f).
     *
     * @param name The name of the property to interpret.
     * @param out The matrix to set to this property's interpreted value.
     * 
     * @return True on success, false if the property does not exist or could not be scanned.
     */
    bool getColor(const char* name, Color* out) const;

private:
    
    /**
     * Constructor.
     */
    Properties(FILE* file);

    /**
     * Constructor. Read from the beginning of namespace specified
     */
    Properties(FILE* file, const char* name, const char* id = NULL);

    void readProperties(FILE* file);

    void skipWhiteSpace(FILE* file);

    char* trimWhiteSpace(char* str);

    std::string _namespace;
    std::string _id;
    std::map<std::string, std::string> _map;
    std::map<std::string, std::string>::const_iterator _mapIt;
    std::vector<Properties*> _properties;
    std::vector<Properties*>::const_iterator _propertiesIt;
};

}

#endif