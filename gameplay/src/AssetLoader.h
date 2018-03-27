#pragma once

#include "Asset.h"

namespace gameplay
{

/**
 * Defines a loader of specialized type of resource.
 */
class AssetLoader
{
public:

    /**
     * Constructor.
     */
    AssetLoader();

    /**
     * Destructor.
     */
    ~AssetLoader();

    /**
     * Defines the base asset loading parameters.
     */
    class Parameters
    {
    public:
        size_t priority = 0;
    };

    /**
     * Gets the dependency asset required to be loaded first
     * before loading the specified asset.
     * 
     * @param url The asset url to retrieve the dependencies for.
     * @param dependencies The found dependencies for the url specified.
     */
    virtual void getDependencies(std::string url, std::vector<std::shared_ptr<Asset> > dependencies) = 0;

    /**
     * Loads the assets.
     *
     * The host coherent (cpu) portion of the asset is loaded asyncrounously.
     * The gpu portion of the asset of queud to be loaded and flushed sychronously.
     * 
     * Use Asset::isLoaded() to check if a asset is completly loaded.
     *
     * @param url The url of the asset to be loaded.
     * @param parameters The load parameters for the asset.
     * @return The asset that has begun to be loaded.
     * @see Asset::isLoaded()
     */
    virtual std::shared_ptr<Asset> load(std::string url, std::shared_ptr<AssetLoader::Parameters> parameters) = 0;
};

}