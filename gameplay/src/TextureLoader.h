#pragma once

#include "AssetLoader.h"

namespace gameplay
{

/**
 * Defines a loader of texures from image formats. (png, jpg, bmp, dds, ktx)
 */
class TextureLoader : public AssetLoader
{
public:

    /**
     * Constructor.
     */
    TextureLoader();

    /**
     * Destructor.
     */
    ~TextureLoader();

    /**
     * Defines the base asset loading parameters.
     */
    class Parameters : AssetLoader::Parameters
    {
    };

    /**
     * Gets the dependency asset required to be loaded first
     * before loading the specified asset.
     * 
     * @param url The asset url to retrieve the dependencies for.
     * @param dependencies The found dependencies for the url specified.
     */
    void getDependencies(std::string url, std::vector<std::shared_ptr<Asset>> dependencies);

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
    std::shared_ptr<Asset> load(std::string url, std::shared_ptr<AssetLoader::Parameters> parameters);
};

}
