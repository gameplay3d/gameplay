#include "Base.h"
#include "TextureLoader.h"

namespace gameplay
{

TextureLoader::TextureLoader() 
{
}

TextureLoader::~TextureLoader()
{
}

TextureLoader* TextureLoader::getTextureLoader()
{
    static TextureLoader loader;
    return &loader;
}

void TextureLoader::getDependencies(std::string url, std::vector<std::shared_ptr<Asset>> dependencies)
{
}

std::shared_ptr<Asset> TextureLoader::load(std::string url, std::shared_ptr<AssetLoader::Parameters> parameters)
{
    return nullptr;
}

std::shared_ptr<Asset> TextureLoader::loadPNG(std::string url, std::shared_ptr<TextureLoader::Parameters> parameters)
{
    return nullptr;
}

std::shared_ptr<Asset> TextureLoader::loadDDS(std::string url, std::shared_ptr<TextureLoader::Parameters> parameters)
{
    return nullptr;
}

std::shared_ptr<Asset> TextureLoader::loadKTX(std::string url, std::shared_ptr<TextureLoader::Parameters> parameters)
{
    return nullptr;
}
}
