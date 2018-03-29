#pragma once

namespace gameplay
{

/**
 * Defines a manager of loaded assets.
 */
class Asset
{
    friend class AssetManager;

public:
    /**
     * Constructor.
     */
    Asset();

    /**
     * Destructor.
     */
    ~Asset();

    /**
     * Gets the url this asset was loaded from.
     *
     * @return The url this asset was loaded from.
     */
    std::string getUrl();

    /**
     * Gets the dependent asset that should be loaded before this asset is loaded.
     *
     * 
     */
    void getDependencies(std::string url, std::vector<std::shared_ptr<Asset>> dependencies);

    /**
     * Gets the object that has been loaded.
     */
    void* getObject();

private:

    std::string _url;
    bool _loaded;
    void* _object;
};

}
