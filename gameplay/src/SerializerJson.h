#pragma once

#include "Serializer.h"
#define JSON_LIBRARY
#include "json/libjson.h"

namespace gameplay
{

/**
 * Defines a json serializer.
 *
 * @see Serializer
 */
class SerializerJson : public Serializer
{
    friend class Serializer;
    
public:

    /**
     * Creates a json serializer.
     *
     * @param path The path to open create and open the serializer from.
     * @return The new json serializer.
     */
    static Serializer* createWriter(const std::string& path);

    /**
     * @see Serializer::close
     */
    void close();
    
    /**
     * @see Serializer::getFormat
     */
    Format getFormat() const;

    /**
     * @see Serializer::writeEnum
     */
    void writeEnum(const char* propertyName, const char* enumName, int value, int defaultValue);

    /**
     * @see Serializer::writeBool
     */
    void writeBool(const char* propertyName, bool value, bool defaultValue);

    /**
     * @see Serializer::writeInt
     */
    void writeInt(const char* propertyName, int value, int defaultValue);

    /**
     * @see Serializer::writeFloat
     */
    void writeFloat(const char* propertyName, float value, float defaultValue);

    /**
     * @see Serializer::writeVector
     */
    void writeVector(const char* propertyName, const Vector2& value, const Vector2& defaultValue);
    
    /**
     * @see Serializer::writeVector
     */
    void writeVector(const char* propertyName, const Vector3& value, const Vector3& defaultValue);
    
    /**
     * @see Serializer::writeVector
     */
    void writeVector(const char* propertyName, const Vector4& value, const Vector4& defaultValue);

    /**
     * @see Serializer::writeColor
     */
    void writeColor(const char* propertyName, const Vector3& value, const Vector3& defaultValue);

    /**
     * @see Serializer::writeColor
     */
    void writeColor(const char* propertyName, const Vector4& value, const Vector4& defaultValue);

    /**
     * @see Serializer::writeMatrix
     */
    void writeMatrix(const char* propertyName, const Matrix& value, const Matrix& defaultValue);

    /**
     * @see Serializer::writeString
     */
    void writeString(const char* propertyName, const char* value, const char* defaultValue);

    /**
     * @see Serializer::writeStringList
     */
    void writeStringList(const char* propertyName, size_t count);

    /**
     * @see Serializer::writeObject
     */
    void writeObject(const char* propertyName, std::shared_ptr<Serializable> value);

    /**
     * @see Serializer::writeObjectList
     */
    void writeObjectList(const char* propertyName, size_t count);

    /**
     * @see Serializer::writeIntArray
     */
    void writeIntArray(const char* propertyName, const int* data, size_t count);

    /**
     * @see Serializer::writeFloatArray
     */
    void writeFloatArray(const char* propertyName, const float* data, size_t count);
    
    /**
     * @see Serializer::writeByteArray
     */
    void writeByteArray(const char* propertyName, const unsigned char* data, size_t count);
    
    /**
     * @see Serializer::readEnum
     */
    int readEnum(const char* propertyName, const char* enumName, int defaultValue);

    /**
     * @see Serializer::readBool
     */
    bool readBool(const char* propertyName, bool defaultValue);

    /**
     * @see Serializer::readInt
     */
    int readInt(const char* propertyName, int defaultValue);

    /**
     * @see Serializer::readFloat
     */
    float readFloat(const char* propertyName, float defaultValue);

    /**
     * @see Serializer::readVector
     */
    Vector2 readVector(const char* propertyName, const Vector2& defaultValue);
    
    /**
     * @see Serializer::readVector
     */
    Vector3 readVector(const char* propertyName, const Vector3& defaultValue);
    
    /**
     * @see Serializer::readVector
     */
    Vector4 readVector(const char* propertyName, const Vector4& defaultValue);
    
    /**
     * @see Serializer::readColor
     */
    Vector3 readColor(const char* propertyName, const Vector3& defaultValue);
    
    /**
     * @see Serializer::readColor
     */
    Vector4 readColor(const char* propertyName, const Vector4& defaultValue);

    /**
     * @see Serializer::readMatrix
     */
    Matrix readMatrix(const char* propertyName, const Matrix& defaultValue);

    /**
     * @see Serializer::readString
     */
    void readString(const char* propertyName, std::string& value, const char* defaultValue);

    /**
     * @see Serializer::readStringList
     */
    size_t readStringList(const char* propertyName);

    /**
     * @see Serializer::readObject
     */
    std::shared_ptr<Serializable> readObject(const char* propertyName);

    /**
     * @see Serializer::readObjectList
     */
    size_t readObjectList(const char* propertyName);

    /**
     * @see Serializer::readIntArray
     */
    size_t readIntArray(const char* propertyName, int** data);

    /**
     * @see Serializer::readFloatArray
     */
    size_t readFloatArray(const char* propertyName, float** data);
    
    /**
     * @see Serializer::readByteArray
     */
    size_t readByteArray(const char* propertyName, unsigned char** data);

    /**
     * Destructor
     */
    ~SerializerJson();
    
protected:
    
    SerializerJson(Type type, const std::string& path, Stream* stream, uint32_t versionMajor, uint32_t versionMinor, JSONNODE* root);
    static Serializer* create(const std::string& path, Stream* stream);
    
private:
    
    JSONNODE* createNode(JSONNODE* parent, const char* propertyName, std::shared_ptr<Serializable> object, bool moreProperties);
    void finishNode(JSONNODE* parent);
    
    JSONNODE* _root;
    std::stack<JSONNODE*> _nodes;
    std::stack<size_t> _nodesListCounts;
    std::map<unsigned long, JSONNODE*> _xrefsWrite;
    std::map<unsigned long, std::shared_ptr<Serializable>> _xrefsRead;
};

}
