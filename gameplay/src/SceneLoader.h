#pragma once

#include "AssetLoader.h"

namespace gameplay
{

/**
 * Defines a scene loader.
 */
class SceneLoader : public AssetLoader
{
public:

    /**
     * Gets the scene loader.
     * 
     * @return The scene loader
     */
    static SceneLoader* getSceneLoader();

    /**
     * @see AssetLoader::Parameters
     */
    class Parameters : AssetLoader::Parameters
    {
    };

    /**
     * @see AssetLoader::getDependencies
     */
    void getDependencies(std::string url, std::vector<std::shared_ptr<Asset>> dependencies);

    /**
     * @see AssetLoader::load
     */
    std::shared_ptr<Asset> load(std::string url, std::shared_ptr<AssetLoader::Parameters> parameters);

private:

    SceneLoader();
    ~SceneLoader();
};

}
