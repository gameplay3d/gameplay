#include "Base.h"
#include "SerializerJson.h"
#include "Activator.h"
#include "Serializer.h"
#include "FileSystem.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix.h"

namespace gameplay
{

SerializerJson::SerializerJson(Type type,
                               const std::string& path,
                               Stream* stream,
                               uint32_t versionMajor,
                               uint32_t versionMinor,
                               JSONNODE* root) : 
    Serializer(type, path, stream, versionMajor, versionMinor), 
    _root(root)
{
    _nodes.push(root);
}

SerializerJson::~SerializerJson()
{
}
        
Serializer* SerializerJson::create(const std::string& path, Stream* stream)
{
    size_t length = stream->length();
    char* buffer = new char[length + 1];
    stream->read(buffer, sizeof(char), length);
    buffer[length] = '\0';
    JSONNODE* root = json_parse(buffer);
    if (root == nullptr)
        return nullptr;

    Serializer* serializer = nullptr;

    JSONNODE* versionNode = json_get(root, "version");
    int versionMajor = GP_ENGINE_VERSION_MAJOR;
    int versionMinor = GP_ENGINE_VERSION_MINOR;
    if (versionNode)
    {
        json_char* str = json_as_string(versionNode);
        std::string version = std::string(str);
        json_free(str);
        if (version.length() > 0)
        {
            std::string major = version.substr(0, 1);
            versionMajor = std::stoi(major);
        }
        if (version.length() > 2)
        {
            std::string minor = version.substr(2, 1);
            versionMinor = std::stoi(minor);
        }
        serializer = new SerializerJson(Type::eReader, path, stream, versionMajor, versionMinor, root);
    }
    GP_SAFE_DELETE_ARRAY(buffer);
    return serializer;
}

Serializer* SerializerJson::createWriter(const std::string& path)
{
    Stream* stream = FileSystem::open(path, FileSystem::AccessFlags::eWrite);
    if (stream == nullptr)
        return nullptr;

    JSONNODE* root = json_new(JSON_NODE);
    std::string version;
    version.append(std::to_string(GP_ENGINE_VERSION_MAJOR));
    version.append(".");
    version.append(std::to_string(GP_ENGINE_VERSION_MINOR));
    json_push_back(root, json_new_a("version", version.c_str()));

    Serializer* serializer = new SerializerJson(Type::eWriter, path, stream, GP_ENGINE_VERSION_MAJOR, GP_ENGINE_VERSION_MINOR, root);
  
    return serializer;
}

void SerializerJson::close()
{
    if (_stream)
    {
        if (_type == Type::eWriter)
        {
            json_char* buffer = json_write_formatted(_root);
            std::string str = buffer;
            _stream->write(str.c_str(), sizeof(char), str.length());
            json_free(buffer);
        }
        if (_root)
            json_delete(_root);
        _stream->close();
    }
}

Serializer::Format SerializerJson::getFormat() const
{
    return Format::eJson;
}

void SerializerJson::writeEnum(const char* propertyName, const char* enumName, int value, int defaultValue)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(enumName);
    
    if (value == defaultValue)
        return;
    
    std::string str = Activator::getActivator()->enumToString(enumName, value);
    writeString(propertyName, str.c_str(), "");
}
    
void SerializerJson::writeBool(const char* propertyName, bool value, bool defaultValue)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eWriter);
    
    if (value == defaultValue)
        return;
    
    JSONNODE* node = _nodes.top();
    json_push_back(node, json_new_b(propertyName, value));
}

void SerializerJson::writeInt(const char* propertyName, int value, int defaultValue)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eWriter);
    
    if (value == defaultValue)
        return;
    
    JSONNODE* node = _nodes.top();
    json_push_back(node, json_new_i(propertyName, value));
}

void SerializerJson::writeFloat(const char* propertyName, float value, float defaultValue)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eWriter);
    
    if (value == defaultValue)
        return;

    JSONNODE* node = _nodes.top();
    json_push_back(node, json_new_f(propertyName, value));
}

void SerializerJson::writeVector(const char* propertyName, const Vector2& value, const Vector2& defaultValue)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eWriter);
    
    if (value == defaultValue)
        return;
    
    // "properyName" : [ x, y ]
    JSONNODE* node = _nodes.top();
    JSONNODE* array = json_new(JSON_ARRAY);
    json_set_name(array, propertyName);
    json_push_back(array, json_new_f(nullptr, value.x));
    json_push_back(array, json_new_f(nullptr, value.y));
    json_push_back(node, array);
}

void SerializerJson::writeVector(const char* propertyName, const Vector3& value, const Vector3& defaultValue)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eWriter);

    if (value == defaultValue)
        return;

    // "properyName" : [ x, y, z ]
    JSONNODE* node = _nodes.top();
    JSONNODE* array = json_new(JSON_ARRAY);
    json_set_name(array, propertyName);
    json_push_back(array, json_new_f(nullptr, value.x));
    json_push_back(array, json_new_f(nullptr, value.y));
    json_push_back(array, json_new_f(nullptr, value.z));
    json_push_back(node, array);
}

void SerializerJson::writeVector(const char* propertyName, const Vector4& value, const Vector4& defaultValue)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eWriter);
    
    if (value == defaultValue)
        return;
    
    // "properyName" : [ x, y, z, w ]
    JSONNODE* node = _nodes.top();
    JSONNODE* array = json_new(JSON_ARRAY);
    json_set_name(array, propertyName);
    json_push_back(array, json_new_f(nullptr, value.x));
    json_push_back(array, json_new_f(nullptr, value.y));
    json_push_back(array, json_new_f(nullptr, value.z));
    json_push_back(array, json_new_f(nullptr, value.w));
    json_push_back(node, array);
}

void SerializerJson::writeColor(const char* propertyName, const Vector3& value, const Vector3& defaultValue)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eWriter);
    
    if (value == defaultValue)
        return;
    
    // "property" : "#rrggbb"
   JSONNODE* node = _nodes.top();
   char buffer[9];
   sprintf(buffer, "%02x%02x%02x", (int)(value.x * 255.0f), (int)(value.y * 255.0f), (int)(value.z * 255.0f));
   std::ostringstream s;
   s << "#" << buffer;
    json_push_back(node, json_new_a(propertyName, s.str().c_str()));
}
    
void SerializerJson::writeColor(const char* propertyName, const Vector4& value, const Vector4& defaultValue)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eWriter);

    if (value == defaultValue)
        return;
    
    // "property" : "#rrggbbaa"
    JSONNODE* node = _nodes.top();
    std::ostringstream s;
    s << "#" << std::hex << value.toColor();    
    json_push_back(node, json_new_a(propertyName, s.str().c_str()));
}
    
void SerializerJson::writeMatrix(const char* propertyName, const Matrix& value, const Matrix& defaultValue)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eWriter);

    if (value == defaultValue)
        return;

    // "properyName" : [ m0, ... , m15 ]
    JSONNODE* node = _nodes.top();
    JSONNODE* array = json_new(JSON_ARRAY);
    json_set_name(array, propertyName);
    json_push_back(array, json_new_f(nullptr, value.m[0]));
    json_push_back(array, json_new_f(nullptr, value.m[1]));
    json_push_back(array, json_new_f(nullptr, value.m[2]));
    json_push_back(array, json_new_f(nullptr, value.m[3]));
    json_push_back(array, json_new_f(nullptr, value.m[4]));
    json_push_back(array, json_new_f(nullptr, value.m[5]));
    json_push_back(array, json_new_f(nullptr, value.m[6]));
    json_push_back(array, json_new_f(nullptr, value.m[7]));
    json_push_back(array, json_new_f(nullptr, value.m[8]));
    json_push_back(array, json_new_f(nullptr, value.m[9]));
    json_push_back(array, json_new_f(nullptr, value.m[10]));
    json_push_back(array, json_new_f(nullptr, value.m[11]));
    json_push_back(array, json_new_f(nullptr, value.m[12]));
    json_push_back(array, json_new_f(nullptr, value.m[13]));
    json_push_back(array, json_new_f(nullptr, value.m[14]));
    json_push_back(array, json_new_f(nullptr, value.m[15]));
    json_push_back(node, array);
}

JSONNODE* SerializerJson::createNode(JSONNODE* parent, const char* propertyName, std::shared_ptr<Serializable> object, bool moreProperties)
{
    if (((json_type(parent) == JSON_NODE) && propertyName) || json_type(parent) == JSON_ARRAY)
    {
        JSONNODE* node = json_new(JSON_NODE);
        json_push_back(parent, node);
        
        if (json_type(parent) == JSON_NODE && propertyName)
            json_set_name(node, propertyName);
        
        if (moreProperties)
            _nodes.push(node);
        
        return node;
    }
    else
    {
        return parent;
    }
}

void SerializerJson::writeString(const char* propertyName, const char* value, const char* defaultValue)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eWriter);

    if ((value == defaultValue) || (value && defaultValue && strcmp (value, defaultValue) == 0))
        return;

    JSONNODE* node = _nodes.top();
    json_push_back(node, json_new_a(propertyName, value));
}

void SerializerJson::writeStringList(const char* propertyName, size_t count)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eWriter);
    if (count == 0)
        return;

    JSONNODE* node = _nodes.top();
    JSONNODE* list = json_new(JSON_ARRAY);
    json_push_back(node, list);
    json_set_name(list, propertyName);

    _nodes.push(list);
    _nodesListCounts.push(count);
}

void SerializerJson::writeObject(const char* propertyName, std::shared_ptr<Serializable> value)
{
    GP_ASSERT(_type == Type::eWriter);
    if (value == nullptr)
        return;

    JSONNODE* parentNode = _nodes.top();
    JSONNODE* writeNode = nullptr;
    JSONNODE* xrefNode = nullptr;
    if (value && value.use_count() > 1)
    {
        unsigned long xrefAddress = (unsigned long)value.get();
        std::map<unsigned long, JSONNODE*>::const_iterator itr = _xrefsWrite.find(xrefAddress);
        std::string url;
        if (itr == _xrefsWrite.end())
        {
            writeNode = createNode(parentNode, propertyName, value, true);
            json_push_back(writeNode, json_new_a("class", value->getClassName().c_str()));
            url = std::to_string(xrefAddress);
            _xrefsWrite[xrefAddress] = writeNode;
        }
        else
        {
            writeNode = createNode(parentNode, propertyName, value, false);
            json_push_back(writeNode, json_new_a("class", value->getClassName().c_str()));
            std::ostringstream o;
            o << "@" << std::to_string(xrefAddress);
            url = o.str();
            xrefNode = itr->second;
            json_push_back(writeNode, json_new_a("xref", url.c_str()));
        }
    }
    else
    {
        writeNode = createNode(parentNode, propertyName, value, true);
        json_push_back(writeNode, json_new_a("class", value->getClassName().c_str()));
    }
    
    if (xrefNode == nullptr)
        value->onSerialize(this);
    
    if (!xrefNode)
        _nodes.pop();
    
    if (json_type(parentNode) == JSON_ARRAY)
    {
        if (json_size(parentNode) == _nodesListCounts.top())
        {
            _nodes.pop();
            _nodesListCounts.pop();
        }
    }
}

void SerializerJson::writeObjectList(const char* propertyName, size_t count)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eWriter);
    if (count == 0)
        return;
    
    JSONNODE* node = _nodes.top();
    JSONNODE* list = json_new(JSON_ARRAY);
    json_push_back(node, list);
    json_set_name(list, propertyName);
    
    _nodes.push(list);
    _nodesListCounts.push(count);
}

void SerializerJson::writeIntArray(const char* propertyName, const int* data, size_t count)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eWriter);
    if (!data || count == 0)
        return;

    // "properyName" : [ 0, ... , count - 1 ]
    JSONNODE* node = _nodes.top();
    JSONNODE* array = json_new(JSON_ARRAY);
    json_set_name(array, propertyName);
    for (size_t i = 0; i < count; i++)
    {
        json_push_back(array, json_new_f(nullptr, data[i]));
    }
    json_push_back(node, array);
}

void SerializerJson::writeFloatArray(const char* propertyName, const float* data, size_t count)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eWriter);
    if (!data || count == 0)
        return;
    
    // "properyName" : [ 0.0, ... , count - 1 ]
    JSONNODE* node = _nodes.top();
    JSONNODE* array = json_new(JSON_ARRAY);
    json_set_name(array, propertyName);
    for (size_t i = 0; i < count; i++)
    {
        json_push_back(array, json_new_f(nullptr, data[i]));
    }
    json_push_back(node, array);
}

void SerializerJson::writeByteArray(const char* propertyName, const unsigned char* data, size_t count)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eWriter);
    if (!data || count == 0)
        return;
    
    // "properyName" : "base64_encode(data)"
    JSONNODE* node = _nodes.top();
    json_push_back(node, json_new_a(propertyName, json_encode64((const void*)data, sizeof(unsigned char) * (uint32_t)count)));
}

int SerializerJson::readEnum(const char* propertyName, const char* enumName, int defaultValue)
{
    GP_ASSERT(enumName);
    
    std::string str;
    readString(propertyName, str, "");
    
    return Activator::getActivator()->enumParse(enumName, str.c_str());
}

bool SerializerJson::readBool(const char* propertyName, bool defaultValue)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eReader);
    
    JSONNODE* node = _nodes.top();
    JSONNODE* property = json_get(node, propertyName);
    if (property)
    {
        if (json_type(property) != JSON_BOOL)
            GP_ERROR("Invalid json bool for propertyName:%s", propertyName);
        return json_as_bool(property);
    }
    return defaultValue;
}

int SerializerJson::readInt(const char* propertyName, int defaultValue)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eReader);

    JSONNODE* node = _nodes.top();
    JSONNODE* property = json_get(node, propertyName);
    if (property)
    {
        if (json_type(property) != JSON_NUMBER)
            GP_ERROR("Invalid json number for propertyName:%s", propertyName);
        
        return json_as_int(property);
    }
    return defaultValue;
}

float SerializerJson::readFloat(const char* propertyName, float defaultValue)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eReader);

    JSONNODE* node = _nodes.top();
    JSONNODE* property = json_get(node, propertyName);
    if (property)
    {
        if (json_type(property) != JSON_NUMBER)
            GP_ERROR("Invalid json number for propertyName:%s", propertyName);
        
        return (float)json_as_float(property);
    }
    return defaultValue;
}

Vector2 SerializerJson::readVector(const char* propertyName, const Vector2& defaultValue)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eReader);
    
    JSONNODE* node = _nodes.top();
    JSONNODE* property = json_get(node, propertyName);
    if (property)
    {
        if (json_type(property) != JSON_ARRAY || json_size(property) < 2)
            GP_ERROR("Invalid json array from Vector2 for propertyName:%s", propertyName);
        
        Vector2 value;
        JSONNODE* x = json_at(property, 0);
        value.x  = json_as_float(x);
        JSONNODE* y = json_at(property, 1);
        value.y  = json_as_float(y);
        return value;
    }
    return defaultValue;
}

Vector3 SerializerJson::readVector(const char* propertyName, const Vector3& defaultValue)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eReader);

    JSONNODE* node = _nodes.top();
    JSONNODE* property = json_get(node, propertyName);
    if (property)
    {
        if (json_type(property) != JSON_ARRAY || json_size(property) < 3)
            GP_ERROR("Invalid json array from Vector3 for propertyName:%s", propertyName);
        
        Vector3 value;
        JSONNODE* x = json_at(property, 0);
        value.x  = json_as_float(x);
        JSONNODE* y = json_at(property, 1);
        value.y  = json_as_float(y);
        JSONNODE* z = json_at(property, 2);
        value.z  = json_as_float(z);
        return value;
    }
    return defaultValue;
}

Vector4 SerializerJson::readVector(const char* propertyName, const Vector4& defaultValue)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eReader);

    JSONNODE* node = _nodes.top();
    JSONNODE* property = json_get(node, propertyName);
    if (property)
    {
        if (json_type(property) != JSON_ARRAY || json_size(property) < 4)
            GP_ERROR("Invalid json array from Vector4 for propertyName:%s", propertyName);
        
        Vector4 value;
        JSONNODE* x = json_at(property, 0);
        value.x  = json_as_float(x);
        JSONNODE* y = json_at(property, 1);
        value.y  = json_as_float(y);
        JSONNODE* z = json_at(property, 2);
        value.z  = json_as_float(z);
        JSONNODE* w = json_at(property, 3);
        value.w  = json_as_float(w);
        return value;
    }
    return defaultValue;
}

Vector3 SerializerJson::readColor(const char* propertyName, const Vector3& defaultValue)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eReader);
    
    JSONNODE* node = _nodes.top();
    JSONNODE* property = json_get(node, propertyName);
    if (property)
    {
        if (json_type(property) != JSON_STRING)
            GP_ERROR("Invalid json string from color for propertyName:%s", propertyName);
        json_char* str = json_as_string(property);
        Vector3 value = Vector3::fromColorString(str);
        json_free(str);
        return value;
    }
    return defaultValue;
}

Vector4 SerializerJson::readColor(const char* propertyName, const Vector4& defaultValue)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eReader);
    
    JSONNODE* node = _nodes.top();
    JSONNODE* property = json_get(node, propertyName);
    if (property)
    {
        if (json_type(property) != JSON_STRING)
            GP_ERROR("Invalid json string from color for propertyName:%s", propertyName);
        json_char* str = json_as_string(property);
        Vector4 value = Vector4::fromColorString(str);
        json_free(str);
        return value;
    }
    return defaultValue;
}

Matrix SerializerJson::readMatrix(const char* propertyName, const Matrix& defaultValue)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eReader);

    JSONNODE* node = _nodes.top();
    JSONNODE* property = json_get(node, propertyName);
    if (property)
    {
        if (json_type(property) != JSON_ARRAY || json_size(property) < 16)
            GP_ERROR("Invalid json array from Matrix for propertyName:%s", propertyName);
        
        Matrix value;
        JSONNODE* m0 = json_at(property, 0);
        value.m[0]  = json_as_float(m0);
        JSONNODE* m1 = json_at(property, 1);
        value.m[1]  = json_as_float(m1);
        JSONNODE* m2 = json_at(property, 2);
        value.m[2]  = json_as_float(m2);
        JSONNODE* m3 = json_at(property, 3);
        value.m[3]  = json_as_float(m3);
        JSONNODE* m4 = json_at(property, 4);
        value.m[4]  = json_as_float(m4);
        JSONNODE* m5 = json_at(property, 5);
        value.m[5]  = json_as_float(m5);
        JSONNODE* m6 = json_at(property, 6);
        value.m[6]  = json_as_float(m6);
        JSONNODE* m7 = json_at(property, 7);
        value.m[7]  = json_as_float(m7);
        JSONNODE* m8 = json_at(property, 8);
        value.m[8]  = json_as_float(m8);
        JSONNODE* m9 = json_at(property, 9);
        value.m[9]  = json_as_float(m9);
        JSONNODE* m10 = json_at(property, 10);
        value.m[10]  = json_as_float(m10);
        JSONNODE* m11 = json_at(property, 11);
        value.m[11]  = json_as_float(m11);
        JSONNODE* m12 = json_at(property, 12);
        value.m[12]  = json_as_float(m12);
        JSONNODE* m13 = json_at(property, 13);
        value.m[13]  = json_as_float(m13);
        JSONNODE* m14 = json_at(property, 14);
        value.m[14]  = json_as_float(m14);
        JSONNODE* m15 = json_at(property, 15);
        value.m[15]  = json_as_float(m15);
        
        return value;
    }
    return defaultValue;
}

void SerializerJson::readString(const char* propertyName, std::string& value, const char* defaultValue)
{
    GP_ASSERT(_type == Type::eReader);

    JSONNODE* node = _nodes.top();
    JSONNODE* property = nullptr;

    if (json_type(node) == JSON_ARRAY)
    {
        size_t  arraySize = json_size(node);
        property = json_at(node, (uint32_t)arraySize - (uint32_t)_nodesListCounts.top());
    }
    else
    {
        GP_ASSERT(propertyName);
        property = json_get(node, propertyName);
    }
   
    if (property)
    {
        if (json_type(property) != JSON_STRING)
            GP_ERROR("Invalid json string for propertyName:%s", propertyName);

        json_char* str = json_as_string(property);
        value.clear();
        value.resize(strlen(str));
        std::strcpy(&value[0], str);
        json_free(str);
    }
    else
    {
        value.clear();
        value.resize(strlen(defaultValue));
        std::strcpy(&value[0], defaultValue);
    }

    if (json_type(node) == JSON_ARRAY)
    {
        _nodesListCounts.top() -= 1;
        if (_nodesListCounts.top() == 0)
        {
            _nodes.pop();
            _nodesListCounts.pop();
        }
    }
  
}

std::shared_ptr<Serializable> SerializerJson::readObject(const char* propertyName)
{
    GP_ASSERT(_type == Type::eReader);
    
    JSONNODE* parentNode = _nodes.top();
    JSONNODE* readNode = nullptr;
    
    if (json_type(parentNode) == JSON_ARRAY)
    {
        size_t  arraySize = json_size(parentNode);
        readNode = json_at(parentNode, (uint32_t)arraySize - (uint32_t)_nodesListCounts.top());
        _nodes.push(readNode);
    }
    else if (json_type(parentNode) == JSON_NODE && propertyName)
    {
        JSONNODE* propertyNode = json_get(parentNode, propertyName);
        if (propertyNode == nullptr)
            return nullptr;
        if (json_type(propertyNode) != JSON_NODE)
        {
            GP_WARN("Invalid json object for propertyName:%s", propertyName);
            return nullptr;
        }
        _nodes.push(propertyNode);
        readNode = propertyNode;
    }
    else
    {
        readNode = parentNode;
    }
    
    JSONNODE* classProperty = json_get(readNode, "class");
    char* className = json_as_string(classProperty);
    
    // Look for xref's
    unsigned long xrefAddress = 0L;
    JSONNODE* xrefProperty = json_get(readNode, "xref");
    if (xrefProperty)
    {
        json_char* str = json_as_string(xrefProperty);
        std::string url = str;
        json_free(str);
        std::string at = "@";
        if (url.compare(0, at.length(), at) != 0)
        {
            // no @ sign. This is xref'ed by others
            xrefAddress = std::strtol(url.c_str(), nullptr, 10);
        }
        else
        {
            // This needs to lookup the node from the xref address. So save it for lookup
            std::string addressStr = url.substr(1, url.length());
            xrefAddress = std::strtol(addressStr.c_str(), nullptr, 10);
         
            std::map<unsigned long, std::shared_ptr<Serializable>>::const_iterator itr = _xrefsRead.find(xrefAddress);
            if (itr != _xrefsRead.end())
            {
                std::shared_ptr<Serializable> ref = itr->second;
                finishNode(parentNode);
                return ref;
            }
            else
            {
                GP_WARN("Unresolved xref:%u for class:%s", xrefAddress, className);
                json_free(className);
                return nullptr;
            }
        }
    }
    
    std::shared_ptr<Serializable> value = std::dynamic_pointer_cast<Serializable>(Activator::getActivator()->createObject(className));
    if (value == nullptr)
    {
        GP_WARN("Failed to deserialize json object:%s for class:", className);
        json_free(className);
        return nullptr;
    }
    json_free(className);

    value->onDeserialize(this);
    
    if (xrefAddress)
        _xrefsRead[xrefAddress] = value;

    finishNode(parentNode);
    
    return value;
}

void SerializerJson::finishNode(JSONNODE* parent)
{
    _nodes.pop();
    if (json_type(parent) == JSON_ARRAY)
    {
        _nodesListCounts.top() -= 1;
        if (_nodesListCounts.top() == 0)
        {
            _nodes.pop();
            _nodesListCounts.pop();
        }
    }
}

size_t SerializerJson::readStringList(const char* propertyName)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eReader);

    JSONNODE* node = _nodes.top();
    JSONNODE* list = json_get(node, propertyName);
    size_t count = json_size(list);
    if (count > 0)
    {
        _nodes.push(list);
        _nodesListCounts.push(count);
    }
    return count;
}

size_t SerializerJson::readObjectList(const char* propertyName)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eReader);
    
    JSONNODE* node = _nodes.top();
    JSONNODE* list = json_get(node, propertyName);
    size_t count = json_size(list);
    if (count > 0)
    {
        _nodes.push(list);
        _nodesListCounts.push(count);
    }
    return count;
}

size_t SerializerJson::readIntArray(const char* propertyName, int** data)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eReader);
    
    JSONNODE* node = _nodes.top();
    size_t count = 0;
    JSONNODE* property = json_get(node, propertyName);
    if (property)
    {
        if (json_type(property) != JSON_ARRAY )
            GP_ERROR("Invalid json array for propertyName:%s", propertyName);
        
        count = json_size(property);
        int* buffer = nullptr;
        if (*data == nullptr)
        {
            buffer = new int[count];
        }
        else
        {
            buffer = *data;
        }
        
        for (size_t i = 0; i < count; i++)
        {
            JSONNODE* item = json_at(property, (uint32_t)i);
            buffer[i] = (uint32_t)json_as_int(item);
        }
        *data = buffer;
    }
    return count;
}

size_t SerializerJson::readFloatArray(const char* propertyName, float** data)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eReader);
    
    JSONNODE* node = _nodes.top();
    size_t count = 0;
    JSONNODE* property = json_get(node, propertyName);
    if (property)
    {
        if (json_type(property) != JSON_ARRAY )
            GP_ERROR("Invalid json array for propertyName:%s", propertyName);
        
        count = json_size(property);
        float* buffer = nullptr;
        if (*data == nullptr)
        {
            buffer = new float[count];
        }
        else
        {
            buffer = *data;
        }
        
        for (size_t i = 0; i < count; i++)
        {
            JSONNODE* item = json_at(property, (uint32_t)i);
            buffer[i] = json_as_float(item);
        }
        *data = buffer;
    }
    return count;
}
    
size_t SerializerJson::readByteArray(const char* propertyName, unsigned char** data)
{
    GP_ASSERT(propertyName);
    GP_ASSERT(_type == Type::eReader);
    
    unsigned long size = 0L;
    JSONNODE* node = _nodes.top();
    JSONNODE* property = json_get(node, propertyName);
    if (property)
    {
        if (json_type(property) != JSON_STRING)
            GP_ERROR("Invalid json base64 string for propertyName:%s", propertyName);
        
        json_char* str = json_as_string(property);
        void* decoded = json_decode64(str, &size);
        
        if (*data == nullptr)
        {
            *data = (unsigned char*)decoded;
        }
        else
        {
            memcpy(*data, decoded, size);
            json_free(decoded);
        }
        json_free(str);
    }
    return (size_t)size;
}

}
