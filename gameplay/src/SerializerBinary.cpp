#include "Base.h"
#include "SerializerBinary.h"
#include "Activator.h"
#include "Serializer.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix.h"

namespace gameplay
{

unsigned char SerializerBinary::kNull       = 0x00;
unsigned char SerializerBinary::kValue      = 0x01;
unsigned char SerializerBinary::kXref       = 0x02;
unsigned char SerializerBinary::kDefault    = 0x04;

SerializerBinary::SerializerBinary(Type type,
                                   const std::string& path,
                                   std::unique_ptr<std::fstream> stream,
                                   uint32_t versionMajor,
                                   uint32_t versionMinor) : 
    Serializer(type, path, std::move(stream), versionMajor, versionMinor)
{
}
    
SerializerBinary::~SerializerBinary()
{
}
    
Serializer* SerializerBinary::create(const std::string& path)
{
    // Read the binary file header info.
    std::unique_ptr<std::fstream> stream = std::make_unique<std::fstream>();
    stream->open(path, std::fstream::binary | std::fstream::in);
    if (!stream->is_open())
    {
        stream.release();
        return nullptr;
    }
    const char magicNumber[9] = GP_ENGINE_MAGIC_NUMBER;
    char header[9];
    stream->read(header, sizeof(char) * 9);
    if (stream->bad() || memcmp(header, magicNumber, 9) != 0)
    {
        stream->close();
        stream.release();
        return nullptr;
    }
    // Read the file version.
    unsigned char version[2];
    stream->read((char*)version, sizeof(unsigned char) * 2);
    if (stream->bad())
    {
        GP_WARN("Failed to read version from binary file: %s", path.c_str());
        stream->close();
        stream.release();
        return nullptr;
    }
    Serializer* serializer = new SerializerBinary(Type::eReader, path, std::move(stream), version[0], version[1]);
    return serializer;
}

Serializer* SerializerBinary::createWriter(const std::string& path)
{
    std::unique_ptr<std::fstream> stream = std::make_unique<std::fstream>();
    stream->open(path, std::fstream::binary | std::fstream::out);
    if (!stream->is_open())
    {
        stream.release();
        return nullptr;
    }

    const char magicNumber[9] = GP_ENGINE_MAGIC_NUMBER;
    const unsigned char version[2] = { GP_ENGINE_VERSION_MAJOR, GP_ENGINE_VERSION_MINOR};

    // Write out the file identifier and version
    stream->write(magicNumber, sizeof(char) * 9);
    if (stream->bad())
        GP_WARN("Unable to write binary file identifier.");

    stream->write((char*)version, sizeof(unsigned char) * 2);
    if (stream->bad())
        GP_WARN("Unable to write binary file version.");

    Serializer* serializer = new SerializerBinary(Type::eWriter, path, std::move(stream), version[0], version[1]);

    return serializer;
}
    
void SerializerBinary::close()
{
    if (_stream)
    {
        _stream->close();
    }
}
    
Serializer::Format SerializerBinary::getFormat() const
{
    return Format::eBinary;
}

void SerializerBinary::writeEnum(const char* propertyName, const char* enumName, int value, int defaultValue)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(enumName);
    
    writeInt(propertyName, value, defaultValue);
}
    
void SerializerBinary::writeBool(const char* propertyName, bool value, bool defaultValue)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eWriter);
    
    _stream->write((char*)&value, sizeof(bool));
}

void SerializerBinary::writeInt(const char* propertyName, int value, int defaultValue)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eWriter);
    
    if (value == defaultValue)
    {
        _stream->write((char*)&kDefault, sizeof(unsigned char));
    }
    else
    {
        _stream->write((char*)&kValue, sizeof(unsigned char));
        _stream->write((char*)&value, sizeof(int));
    }
}

void SerializerBinary::writeFloat(const char* propertyName, float value, float defaultValue)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eWriter);
    
    if (value == defaultValue)
    {
        _stream->write((char*)&kDefault, sizeof(unsigned char));
    }
    else
    {
        _stream->write((char*)&kValue, sizeof(unsigned char) );
        _stream->write((char*)&value, sizeof(float));
    }
}

void SerializerBinary::writeVector(const char* propertyName, const Vector2& value, const Vector2& defaultValue)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eWriter);
    
    if (value == defaultValue)
    {
        _stream->write((char*)&kDefault, sizeof(unsigned char));
    }
    else
    {
        _stream->write((char*)&kValue, sizeof(unsigned char));
        _stream->write((char*)&value.x, sizeof(float));
        _stream->write((char*)&value.y, sizeof(float));
    }
}

void SerializerBinary::writeVector(const char* propertyName, const Vector3& value, const Vector3& defaultValue)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eWriter);
    
    if (value == defaultValue)
    {
        _stream->write((char*)&kDefault, sizeof(unsigned char));
    }
    else
    {
        _stream->write((char*)&kValue, sizeof(unsigned char));
        _stream->write((char*)&value.x, sizeof(float));
        _stream->write((char*)&value.y, sizeof(float));
        _stream->write((char*)&value.z, sizeof(float));
    }
}

void SerializerBinary::writeVector(const char* propertyName, const Vector4& value, const Vector4& defaultValue)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eWriter);
    
    if (value == defaultValue)
    {
        _stream->write((char*)&kDefault, sizeof(unsigned char));
    }
    else
    {
        _stream->write((char*)&kValue, sizeof(unsigned char));
        _stream->write((char*)&value.x, sizeof(float));
        _stream->write((char*)&value.y, sizeof(float));
        _stream->write((char*)&value.z, sizeof(float));
        _stream->write((char*)&value.w, sizeof(float));
    }
}

void SerializerBinary::writeColor(const char* propertyName, const Vector3& value, const Vector3& defaultValue)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eWriter);
    
    if (value == defaultValue)
    {
        _stream->write((char*)&kDefault, sizeof(unsigned char));
    }
    else
    {
        _stream->write((char*)&kValue, sizeof(unsigned char));
        uint32_t color = value.toColor();
        _stream->write((char*)&color, sizeof(uint32_t));
    }
}

void SerializerBinary::writeColor(const char* propertyName, const Vector4& value, const Vector4& defaultValue)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eWriter);
    
    if (value == defaultValue)
    {
        _stream->write((char*)&kDefault, sizeof(unsigned char));
    }
    else
    {
        _stream->write((char*)&kValue, sizeof(unsigned char));
        uint32_t color = value.toColor();
        _stream->write((char*)&color, sizeof(uint32_t));
    }
}

void SerializerBinary::writeMatrix(const char* propertyName, const Matrix& value, const Matrix& defaultValue)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eWriter);
    
    if (value == defaultValue)
    {
        _stream->write((char*)&kDefault, sizeof(unsigned char));
    }
    else
    {
        _stream->write((char*)&kValue, sizeof(unsigned char));
        _stream->write((char*)value.m, sizeof(float) * 16);
    }
}

void SerializerBinary::writeString(const char* propertyName, const char* value, const char* defaultValue)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eWriter);

    if ((value == defaultValue) || (value && defaultValue && strcmp(value, defaultValue) == 0))
    {
        _stream->write((char*)&kDefault, sizeof(unsigned char));
    }
    else
    {
        _stream->write((char*)&kValue, sizeof(unsigned char));
        writeLengthPrefixedString(value);
    }
}

void SerializerBinary::writeStringList(const char* propertyName, size_t count)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eWriter);

    _stream->write((char*)&count, sizeof(size_t));
}
    
void SerializerBinary::writeObject(const char* propertyName, std::shared_ptr<Serializable> value)
{
    GP_ASSERT(_type == Type::eWriter);
    
    if (value == nullptr)
    {
        _stream->write((char*)&kNull, sizeof(unsigned char));
        return;
    }
    
    bool writeValue = true;
    if (value && value.use_count() > 1)
    {
        _stream->write((char*)&kXref, sizeof(unsigned char));
        unsigned long xrefAddress = reinterpret_cast<unsigned long>(value.get());
        _stream->write((char*)&xrefAddress, sizeof(unsigned long));
        
        // Check if already serialized from xref table
        std::map<unsigned long, std::shared_ptr<Serializable>>::const_iterator itr = _xrefs.find(xrefAddress);
        if (itr == _xrefs.end())
        {
            writeValue = true;
            _xrefs[xrefAddress] = value;
        }
        else
        {
            writeValue = false;
        }
    }
    else
    {
        // Write out this is a object value
        _stream->write((char*)&kValue, sizeof(unsigned char));
    }
    
    if (writeValue)
    {
        // Write out the objects class
        writeLengthPrefixedString(value->getClassName().c_str());
        
        // Serialize the object properties
        value->onSerialize(this);
    }
}

void SerializerBinary::writeObjectList(const char* propertyName, size_t count)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eWriter);
    
    _stream->write((char*)&count, sizeof(size_t));
}

void SerializerBinary::writeIntArray(const char* propertyName, const int* data, size_t count)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eWriter);
    
    _stream->write((char*)&count, sizeof(size_t));
    if (count > 0 && data )
    {
        _stream->write((char*)data, sizeof(int) * count);
    }
}

void SerializerBinary::writeFloatArray(const char* propertyName, const float* data, size_t count)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eWriter);
    
    _stream->write((char*)&count, sizeof(size_t));
    if (count > 0 && data )
    {
        _stream->write((char*)data, sizeof(float) * count);
    }
}

void SerializerBinary::writeByteArray(const char* propertyName, const unsigned char* data, size_t count)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eWriter);
    
    _stream->write((char*)&count, sizeof(size_t));
    if (count > 0 && data )
    {
        _stream->write((char*)data, sizeof(unsigned char) * count);
    }
}

int SerializerBinary::readEnum(const char* propertyName, const char* enumName, int defaultValue)
{
    GP_ASSERT(enumName);
    
    return readInt(propertyName, defaultValue);
}

bool SerializerBinary::readBool(const char* propertyName, bool defaultValue)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eReader);
    
    bool value;
    _stream->read((char*)&value, sizeof(bool));
    return value;
}

int SerializerBinary::readInt(const char* propertyName, int defaultValue)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eReader);
    
    unsigned char bit;
    _stream->read((char*)&bit, sizeof(unsigned char));
    if (bit == kDefault)
    {
        return defaultValue;
    }
    else
    {
        int value;
        _stream->read((char*)&value, sizeof(int));
        return value;
    }
}

float SerializerBinary::readFloat(const char* propertyName, float defaultValue)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eReader);
    
    unsigned char bit;
    _stream->read((char*)&bit, sizeof(unsigned char));
    if (bit == kDefault)
    {
        return defaultValue;
    }
    else
    {
        float value;
        _stream->read((char*)&value, sizeof(float));
        return value;
    }
}
    
Vector2 SerializerBinary::readVector(const char* propertyName, const Vector2& defaultValue)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eReader);
    
    unsigned char bit;
    _stream->read((char*)&bit, sizeof(unsigned char));
    if (bit == kDefault)
    {
        return defaultValue;
    }
    else
    {
        Vector2 value;
        _stream->read((char*)&value.x, sizeof(float));
        _stream->read((char*)&value.y, sizeof(float));
        return value;
    }
}

Vector3 SerializerBinary::readVector(const char* propertyName, const Vector3& defaultValue)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eReader);
    
    unsigned char bit;
    _stream->read((char*)&bit, sizeof(unsigned char));
    if (bit == kDefault)
    {
        return defaultValue;
    }
    else
    {
        Vector3 value;
        _stream->read((char*)&value.x, sizeof(float));
        _stream->read((char*)&value.y, sizeof(float));
        _stream->read((char*)&value.z, sizeof(float));
        return value;
    }
}

Vector4 SerializerBinary::readVector(const char* propertyName, const Vector4& defaultValue)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eReader);
    
    unsigned char bit;
    _stream->read((char*)&bit, sizeof(unsigned char));
    if (bit == kDefault)
    {
        return defaultValue;
    }
    else
    {
        Vector4 value;
        _stream->read((char*)&value.x, sizeof(float));
        _stream->read((char*)&value.y, sizeof(float));
        _stream->read((char*)&value.z, sizeof(float));
        _stream->read((char*)&value.w, sizeof(float));
        return value;
    }
}

Vector3 SerializerBinary::readColor(const char* propertyName, const Vector3& defaultValue)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eReader);
    
    unsigned char bit;
    _stream->read((char*)&bit, sizeof(unsigned char));
    if (bit == kDefault)
    {
        return defaultValue;
    }
    else
    {
        uint32_t color;
        _stream->read((char*)&color, sizeof(uint32_t));
        return Vector3::fromColor(color);
    }
}
    
Vector4 SerializerBinary::readColor(const char* propertyName, const Vector4& defaultValue)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eReader);
    
    unsigned char bit;
    _stream->read((char*)&bit, sizeof(unsigned char));
    if (bit == kDefault)
    {
        return defaultValue;
    }
    else
    {
        uint32_t color;
        _stream->read((char*)&color, sizeof(uint32_t));
        return Vector4::fromColor(color);
    }
}

Matrix SerializerBinary::readMatrix(const char* propertyName, const Matrix& defaultValue)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eReader);
    
    unsigned char bit;
    _stream->read((char*)&bit, sizeof(unsigned char));
    if (bit == kDefault)
    {
        return defaultValue;
    }
    else
    {
        Matrix value;
        _stream->read((char*)value.m, sizeof(float) * 16);
        return value;
    }
}

void SerializerBinary::readString(const char* propertyName, std::string& value,  const char* defaultValue)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eReader);

    unsigned char bit;
    _stream->read((char*)&bit, sizeof(unsigned char));
    if (bit == kDefault)
    {
        std::strcpy(&value[0], defaultValue);
    }
    else
    {
        readLengthPrefixedString(value);
    }
}

size_t SerializerBinary::readStringList(const char* propertyName)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eReader);

    size_t count;
    _stream->read((char*)&count, sizeof(size_t));
    return count;
}

std::shared_ptr<Serializable> SerializerBinary::readObject(const char* propertyName)
{
    GP_ASSERT(_type == Type::eReader);
    
    unsigned char bit;
    _stream->read((char*)&bit, sizeof(unsigned char));
    
    unsigned long xrefAddress = 0L;
    if (bit == kNull)
    {
        return nullptr;
    }
    else if (bit == kXref)
    {
        _stream->read((char*)&xrefAddress, sizeof(unsigned long));
        std::map<unsigned long, std::shared_ptr<Serializable>>::const_iterator itr = _xrefs.find(xrefAddress);
        if (itr != _xrefs.end())
        {
            return itr->second;
        }
    }
    
    // The class name for the object being read
    std::string className;
    readLengthPrefixedString(className);

    std::shared_ptr<Serializable> value = std::dynamic_pointer_cast<Serializable>(Activator::getActivator()->createObject(className));
    if (value == nullptr)
    {
        GP_ERROR("Failed to deserialize binary class: %s for propertyName:%s", className.c_str(), propertyName);
        return value;
    }
    
    // Deserialize the properties
    value->onDeserialize(this);
    
    if (xrefAddress != 0)
    {
        _xrefs[xrefAddress] = value;
    }
    
    return value;
}
    
size_t SerializerBinary::readObjectList(const char* propertyName)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eReader);
    
    size_t count;
    _stream->read((char*)&count, sizeof(size_t));
    return count;
}

size_t SerializerBinary::readIntArray(const char* propertyName, int** data)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eReader);

    size_t count = 0;
    _stream->read((char*)&count, sizeof(size_t));
    int* buffer = nullptr;
    if (count > 0)
    {
        if (*data == nullptr)
        {
            buffer = new int[count];
        }
        else
        {
            buffer = *data;
        }
        _stream->read((char*)buffer, sizeof(int) * count);
    }
    *data = buffer;
    
    return count;
}

size_t SerializerBinary::readFloatArray(const char* propertyName, float** data)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eReader);
    
    size_t count = 0;
    _stream->read((char*)&count, sizeof(size_t));
    float* buffer = nullptr;
    if (count > 0)
    {
        if (*data == nullptr)
        {
            buffer = new float[count];
        }
        else
        {
            buffer = *data;
        }
        _stream->read((char*)buffer, sizeof(float) * count);
    }
    *data = buffer;
    
    return count;
}

size_t SerializerBinary::readByteArray(const char* propertyName, unsigned char** data)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eReader);
    
    size_t count = 0;
    _stream->read((char*)&count, sizeof(size_t));
    unsigned char* buffer = nullptr;
    if (count > 0)
    {
        if (*data == nullptr)
        {
            buffer = new unsigned char[count];
        }
        else
        {
            buffer = *data;
        }
        _stream->read((char*)buffer, sizeof(unsigned char) * count);
    }
    *data = buffer;
    
    return count;
}

void SerializerBinary::writeLengthPrefixedString(const char* str)
{
    size_t length = strlen(str);
    _stream->write((char*)&length, sizeof(size_t));
    if (length > 0)
    {
        _stream->write(str, sizeof(char) * length);
    }
}
    
void SerializerBinary::readLengthPrefixedString(std::string& str)
{
    size_t length;
    _stream->read((char*)&length, sizeof(size_t));
    if (length > 0)
    {
        str.resize(length);
        _stream->read((char*)&str[0], sizeof(char) * length);
    }
    else
    {
        str.clear();
    }
}

}
