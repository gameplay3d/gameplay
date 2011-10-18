/*
 * Properties.h
 */

#ifndef PROPERTIES_H_
#define PROPERTIES_H_

#include "Base.h"
#include "Matrix.h"
#include "Color.h"

using namespace std;

namespace gameplay
{

class Properties
{
public:
    static Properties* create(const char* filePath);

    ~Properties();

    /**
     * Get the map of key/value pairs that were parsed into this Properties object.
     */
    const map<string, string>* getMap() const;

    // Get all inner namespaces.
    const vector<Properties*>* getProperties() const;

    // Get a specific namespace by ID.
    Properties* getProperties(const char* id) const;

    const char* getNamespace() const;
    const char* getID() const;

    /**
     * Check if a property with the given name is specified in this Properties object.
     *
     * @param name The name of the property to query.
     */
    bool exists(const char* name) const;

    /**
     * Get the value of the given property as a string.  This can always be retrieved,
     * whatever the intended type of the property.
     *
     * @param name The name of the property to interpret.
     */
    const char* getString(const char* name) const;

    /**
     * Interpret the value of the given property as a boolean.
     *
     * @param name The name of the property to interpret.
     * @return true if the string value of the property is "true".  Otherwise, return false.
     */
    bool getBool(const char* name) const;

    /**
     * Interpret the value of the given property as an integer.
     *
     * @param name The name of the property to interpret.
     */
    int getInt(const char* name) const;

    /**
     * Interpret the value of the given property as a floating-point number.
     *
     * @param name The name of the property to interpret.
     */
    float getFloat(const char* name) const;

    /**
     * Interpret the value of the given property as a long integer.
     *
     * @param name The name of the property to interpret.
     */
    long getLong(const char* name) const;
    
    /**
     * Read the specified number of floats from the value of the specified property and copy
     * the floats into the given array.
     *
     * @param name The name of the property to interpret.
     */
    void getFloatArray(const char* name, float* out, unsigned int length) const;
    
    // Math classes.
    void getMatrix(const char* name, Matrix* out) const;
    void getVector2(const char* name, Vector2* out) const;
    void getVector3(const char* name, Vector3* out) const;
    void getVector4(const char* name, Vector4* out) const;
    void getColor(const char* name, Color* out) const;

private:
    // Constructor from top of file.
    Properties(FILE* file);
    // Constructor from beginning of namespace.
    Properties(FILE* file, const char* name, const char* id = NULL);

    void readProperties(FILE* file);
    void skipWhiteSpace(FILE* file);
    char* trimWhiteSpace(char* str);


    string _namespace;
    string _id;
    map<string, string> _map;
    vector<Properties*> _properties;
};

}

#endif