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
   
uint32_t Serializer::getVersionMajor() const
{
    return (uint32_t)_version[0];
}

uint32_t Serializer::getVersionMinor() const
{
    return (uint32_t)_version[1];
}

}
