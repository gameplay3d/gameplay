#include "Base.h"
#include "Serializer.h"
#include "SerializerBinary.h"
#include "SerializerJson.h"
#include "Game.h"
#include "Scene.h"
#include "SceneObject.h"
#include "Camera.h"
#include "Light.h"

namespace gameplay
{

Serializer::Serializer(Type type, const std::string& path, std::unique_ptr<std::fstream> stream, uint32_t versionMajor, uint32_t versionMinor) : 
    _type(type),
    _path(path)
{
    _stream = std::move(stream);
    _version[0] = versionMajor;
    _version[1] = versionMinor;
}

Serializer::~Serializer()
{
    if (_stream)
    {
        _stream->close();
    }
    _stream.release();
}

Serializer* Serializer::createReader(const std::string& path)
{
    if (!fs::exists(path))
        return nullptr;

    Serializer* serializer = SerializerBinary::create(path);
    if (!serializer)
    {
        serializer = SerializerJson::create(path);
    }
    return serializer;
}

std::string Serializer::getPath() const
{
    return _path;
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
