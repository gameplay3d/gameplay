#include "Base.h"
#include "Serializer.h"
#include "SerializerBinary.h"
#include "SerializerJson.h"
#include "FileSystem.h"
#include "Game.h"
#include "Scene.h"
#include "SceneObject.h"
#include "Camera.h"
#include "Light.h"

namespace gameplay
{

static Serializer::Activator* __activator = nullptr;

Serializer::Activator::Activator()
{
}

Serializer::Activator::~Activator()
{
}
    
void Serializer::Activator::initializeTypes()
{
    // Register any Serializable types
    registerType("gameplay::Game::Config", Game::Config::createObject);
    registerType("gameplay::Scene", Scene::createObject);
    registerType("gameplay::SceneObject", SceneObject::createObject);
    registerType("gameplay::Camera", Camera::createObject);
    registerType("gameplay::Light", Light::createObject);
}

void Serializer::Activator::initializeEnums()
{
    // Register enums used within serialized objects
    registerEnum("gameplay::Camera::Mode", Camera::enumToString, Camera::enumParse);
    registerEnum("gameplay::Light::Type", Light::enumToString, Light::enumParse);
    registerEnum("gameplay::Light::Mode", Light::enumToString, Light::enumParse);
    registerEnum("gameplay::Light::Shadows", Light::enumToString, Light::enumParse);
}
    
std::shared_ptr<Serializable> Serializer::Activator::createObject(const std::string& className)
{
    std::shared_ptr<Serializable> object = nullptr;
    std::map<std::string,CreateObjectCallback>::const_iterator itr = __activator->_classes.find(className);
    if (itr == __activator->_classes.end())
        return nullptr;
    
    CreateObjectCallback createObject = itr->second;
    if (createObject)
        object = createObject();
    return object;
}
    
std::string Serializer::Activator::enumToString(const std::string& enumName, int value)
{
    std::map<std::string,std::pair<EnumToStringCallback, EnumParseCallback>>::const_iterator itr = __activator->_enums.find(enumName);
    if (itr != __activator->_enums.end())
    {
        EnumToStringCallback enumToString = itr->second.first;
        if (enumToString)
            return enumToString(enumName, value);
    }
    return nullptr;
}

int Serializer::Activator::enumParse(const std::string& enumName, const std::string& str)
{
    std::map<std::string,std::pair<EnumToStringCallback, EnumParseCallback>>::const_iterator itr = __activator->_enums.find(enumName);
    if (itr != __activator->_enums.end())
    {
        EnumParseCallback enumParse = itr->second.second;
        if (enumParse)
            return enumParse(enumName, str);
    }
    return 0;
}

void Serializer::Activator::registerType(const std::string&  className, CreateObjectCallback createObject)
{
    std::map<std::string,CreateObjectCallback>::const_iterator itr = __activator->_classes.find(className);
    if ( itr == __activator->_classes.end() )
    {
        __activator->_classes[className] = createObject;
    }
    else
    {
        GP_ERROR("className already registered:%s", className.c_str());
    }
}
    
void Serializer::Activator::registerEnum(const std::string& enumName, EnumToStringCallback toString, EnumParseCallback parse)
{
    std::map<std::string, std::pair<EnumToStringCallback, EnumParseCallback>>::const_iterator itr = __activator->_enums.find(enumName);
    if (itr == __activator->_enums.end())
    {
        __activator->_enums[enumName] = std::make_pair(toString, parse);
    }
    else
    {
        GP_ERROR("enumName already registered:%s", enumName.c_str());
    }
}

Serializer::Serializer(Type type, const std::string& path, Stream* stream, uint32_t versionMajor, uint32_t versionMinor) : 
    _type(type),
    _path(path),
    _stream(stream)
{
    _version[0] = versionMajor;
    _version[1] = versionMinor;
}

Serializer::~Serializer()
{
    _stream->close();
    GP_SAFE_DELETE(_stream);
}

Serializer* Serializer::createReader(const std::string& path)
{
    Stream* stream = FileSystem::open(path);
    if (!stream)
        return nullptr;

    Serializer* serializer = SerializerBinary::create(path, stream);
    if (!serializer)
    {
        stream->rewind();
        serializer = SerializerJson::create(path, stream);
    }
    return serializer;
}

std::string Serializer::getPath() const
{
    return _path;
}

Serializer::Activator* Serializer::getActivator()
{
    if (!__activator)
    {
        __activator = new Serializer::Activator();
        __activator->initializeTypes();
        __activator->initializeEnums();
    }
    return __activator;
}
    
uint32_t Serializer::getVersionMajor() const
{
    return (uint32_t)_version[0];
}

uint32_t Serializer::getVersionMinor() const
{
    return (uint32_t)_version[1];
}

}
