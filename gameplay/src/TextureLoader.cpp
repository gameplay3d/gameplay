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

void TextureLoader::getDependencies(std::string url, std::vector<std::shared_ptr<Asset>> dependencies)
{
}

std::shared_ptr<Asset> TextureLoader::load(std::string url, std::shared_ptr<AssetLoader::Parameters> parameters)
{
    return nullptr;
}

}
