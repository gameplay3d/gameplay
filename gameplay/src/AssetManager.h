#pragma once

#include "AssetLoader.h"

namespace gameplay
{

/**
 * Defines a manager of loaded assets.
 */
class AssetManager
{
public:

    /**
     * Constructor.
     */
    AssetManager();

    /**
     * Destructor.
     */
    ~AssetManager();

    /**
     * Sets an asset loader for a specified file extension.
     *
     * @param fileExt The file extension Ex. "png", "fbx", "ttf"
     * @param loader The asset loader to use for the fileExt.
     */
    void setLoader(std::string fileExt, std::shared_ptr<AssetLoader> loader);

    /**
     * Loads an asset asynchronously.
     *
     * The asset can be checked for load completion
     * using Asset::isLoaded()
     *
     * @param url The url of the asset to load.
     * @see Asset::isLoaded
     */
    std::shared_ptr<Asset> load(const std::string& url);

    /**
     * Determines if an asset is loaded.
     *
     * @param url The url of the asset to check loading completion.
     * @return true if an asset is loaded, false if not loaded.
     */
    bool isLoaded(const std::string& url) const;

    /**
     * Gets the object associated with the asset being loaded.
     *
     * @return The object associated with the asset being loaded.
     */
    void* getObject() const;

    /**
     * Cancels loading an asset and unloads any portion(s)
     * of the resources already loaded.
     *
     * @param url The url of the asset to to cancel loading.
     */
    void cancelLoad(const std::string& url);

    /**
     * Unloads a previously loaded asset.
     *
     * @param url The url of asset to unloaded.
     */
    void unload(const std::string& url);

    /**
     * Begins loading several assets.
     */
    void beginLoading();

    /**
     * Cancels loading all the resource up to the last
     * call to beginLoading().
     */
    void cancelLoading();

    /**
     * Ends loading and waits/blocks until all resources
     * loaded are loaded into the manager.
     */
    void endLoading();

    /**
     * Clears all the loaded asset from the manager.
     */
    void clear();

private:

    void process();

    std::map<std::string, AssetLoader*> _assetLoaders;
    std::queue<std::shared_ptr<Asset>> _assets;
    std::thread* _thread;
    std::mutex _mutex;
    std::condition_variable _assetsPopulated;
};

}
