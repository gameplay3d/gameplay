#include "Base.h"
#include "AssetManager.h"
#include "FileSystem.h"
#include "TextureLoader.h"
#include "SceneLoader.h"

namespace gameplay
{

AssetManager::AssetManager() 
{
    setLoader("png", TextureLoader::getTextureLoader());
    setLoader("dds", TextureLoader::getTextureLoader());
    setLoader("ktx", TextureLoader::getTextureLoader());
    setLoader("fbx", SceneLoader::getSceneLoader());
    setLoader("dae", SceneLoader::getSceneLoader());
    setLoader("gltf", SceneLoader::getSceneLoader());
    setLoader("glb", SceneLoader::getSceneLoader());
    setLoader("obj", SceneLoader::getSceneLoader());

    _thread = new std::thread(&AssetManager::process, this);
}

AssetManager::~AssetManager()
{
}

void AssetManager::setLoader(std::string fileExt, AssetLoader* loader)
{
    GP_ASSERT(loader);

    _assetLoaders[fileExt] = loader;
}

std::shared_ptr<Asset> AssetManager::load(const std::string & url)
{
    if (!FileSystem::fileExists(url))
    {
        GP_WARN("Invalid asset url: %s", url.c_str());
        return nullptr;
    }
    std::lock_guard<std::mutex> lock(_mutex);
    std::shared_ptr<Asset> asset = std::make_shared<Asset>();
    asset->_url = url;
    _assets.push(asset);
    _assetsPopulated.notify_one();

    return asset;
}

void AssetManager::cancelLoad(const std::string& url)
{
}

void AssetManager::unload(const std::string& url)
{
}

bool AssetManager::isLoaded(const std::string& url) const
{
    return false;
}

void* AssetManager::getObject() const
{
    return nullptr;
}

void AssetManager::beginLoading()
{
}

void AssetManager::cancelLoading()
{
}

void AssetManager::endLoading()
{
}

void AssetManager::clear()
{
}

void AssetManager::process()
{
    while (1)
    {
        std::unique_lock<std::mutex> assetLock(_mutex);
        while (_assets.empty())
        {
            _assetsPopulated.wait(assetLock);
        }
        if (_assets.empty())
            continue;

        std::shared_ptr<Asset> asset = _assets.front();
        _assets.pop();

        // todo
    }
}

}
