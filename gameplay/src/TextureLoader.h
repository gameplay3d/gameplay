#pragma once

#include "AssetLoader.h"
#include "Vector2.h"

namespace gameplay
{

/**
 * Defines a loader of texures from supported image formats. (png, dds, ktx)
 */
class TextureLoader : public AssetLoader
{
    friend class AssetManager;

public:

    /**
     * Gets the texture loader.
     * 
     * @return The texture loader
     */
    static TextureLoader* getTextureLoader();

    /**
     * Texture loading parameters
     */
    class Parameters : AssetLoader::Parameters
    {
    public:

        static const int kDefaultMaxSize = 2048;

        /**
         *
         */
        enum class Type
        {
            eDefault,
            eSprite,
            eCursor,
            eNormalMap,
            eLightMap
        };

        enum class Mapping
        {
            eAuto,
            eCube,
            eCylinder,
            eSphere
        };

        enum class Compression
        {
            eNone,
            eLowQuality,
            eNormalQuality,
            eHighQuality
        };

        Type type = Type::eDefault;
        Mapping mapping = Mapping::eAuto;
        Compression compression = Compression::eNormalQuality;
        bool generateMipMaps = true;
        Vector2 maxSize = Vector2(kDefaultMaxSize, kDefaultMaxSize);
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

    TextureLoader();
    ~TextureLoader();
    std::shared_ptr<Asset> loadPNG(std::string url, std::shared_ptr<TextureLoader::Parameters> parameters);
    std::shared_ptr<Asset> loadDDS(std::string url, std::shared_ptr<TextureLoader::Parameters> parameters);
    std::shared_ptr<Asset> loadKTX(std::string url, std::shared_ptr<TextureLoader::Parameters> parameters);
};

}
