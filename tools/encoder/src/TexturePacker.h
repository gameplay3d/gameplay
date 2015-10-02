#ifndef __TEXTURE_PACKER_H
#define __TEXTURE_PACKER_H

#include <string>
#include <map>
#include <vector>

namespace gameplay
{

class TextureAtlas;
class GPBFile;

class TexturePacker {
public:
    TexturePacker(GPBFile *gpbFile) : _gpbFile(gpbFile) {}
    ~TexturePacker();
    bool parse(std::string fileName);
    bool write();
private:
    enum class LineType {
        TextureAtlas,
        Texture,
        Node,
        UVMap,
    };

    struct Line {
        LineType type;
        std::string sparams[3];
        int paramsCount;
    };

    struct UVMapInfo {
        std::string nodeId;
        int texCoordIndex;
        std::string textureAtlasName;
        std::string textureName;
    };

    bool parseLine(std::string& str, Line& line);
    typedef std::map<std::string, TextureAtlas*> AtlasMap;

    void updateUV(UVMapInfo uvmapinfo);
    AtlasMap _atlases;
    std::vector<UVMapInfo> _uvmaps;
    GPBFile *_gpbFile;

    const std::string meshSuffix = "_Mesh";
};

}

#endif /* __TEXTURE_PACKER_H */
