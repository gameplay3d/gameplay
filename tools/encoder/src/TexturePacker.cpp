/*
 Texture packer for creating a texture atlas from multiple textures and 
 automatically updating UV coordinates for the meshes.
 
 Atlas description format goes like:
 
 #comments begin with hash sign
 #textureAtlas outputFile sizeX, sizeY
 textureAtlas diffuse.png, 1024, 1024
     texture item1.png #texture inputFile
     texture item2.png
     texture item3.png

 textureAtlas normal.png, 1024, 1024
     texture itemN1.png
     texture itemN2.png
     texture itemN3.png

 #node idInGPBFile
 node item1
     uvmap 0, diffuse.png, item1.png #uvmap texCoordIndex, outputFile, inputFile
     uvmap 1, normal.png, itemN1.png
 node item2
     uvmap 0, diffuse.png, item1.png
     uvmap 1, normal.png, itemN2.png
 */

#include "TexturePacker.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cctype>

#include "Base.h"
#include "Image.h"
#include "GPBFile.h"

using namespace std;

namespace gameplay {

class ImageFile {
public:
    static ImageFile* create(const string& fileName)
    {
        const Image *image = Image::create(fileName.c_str());
        if (image) {
            ImageFile *imageFile = new ImageFile(image, fileName);
            return imageFile;
        }

        return NULL;
    }

    ~ImageFile()
    {
        if (_image)
            delete _image;
    }

    const string& getFileName() const
    {
        return _fileName;
    }

    int getX() const { return _x; }
    int getY() const { return _y; }
    int getWidth() { return _width; }
    int getHeight() { return _height; }
    int getPerimeter() { return _width + _height; }
    void setX(int x) { _x = x; }
    void setY(int y) { _y = y; }
    const Image* getImage() const { return _image; };
private:
    ImageFile(const Image *image, const string& fileName)
    : _image(image), _fileName(fileName)
    {
        _width = _image->getWidth();
        _height = _image->getHeight();
    }

    const Image *_image;
    const string _fileName;
    int _x, _y, _width, _height;
};

class TextureAtlas
{
public:
    TextureAtlas(int width, int height)
    : TextureAtlas(0, 0, width, height)
    {
    }

    TextureAtlas(int x, int y, int w, int h)
    : _x(x), _y(y), _width(w), _height(h), _sub1(NULL), _sub2(NULL),
    _imageFile(NULL) {}

    ~TextureAtlas()
    {
        if (_sub1)
            delete _sub1;
        if (_sub2)
            delete _sub2;
        if (_imageFile)
            delete _imageFile;
    }

    bool pack(ImageFile *imageFile)
    {
        if (!_imageFile) {
            if ((imageFile->getWidth() > _width) ||
                (imageFile->getHeight() > _height)) {
                return false;
            }

            _imageFile = imageFile;
            _imageFile->setX(_x);
            _imageFile->setY(_y);

            _sub1 = new TextureAtlas(_x, _y + _imageFile->getHeight(),
                                   imageFile->getWidth(),
                                   _height - _imageFile->getHeight());
            _sub2 = new TextureAtlas(_x + imageFile->getWidth(), _y,
                                   _width - imageFile->getWidth(), _height);

            return true;
        }

        return _sub1->pack(imageFile) || _sub2->pack(imageFile);
    }

    void write(const string& fileName)
    {
        if (!_imageFile)
            return;
        Image *image = Image::create(_imageFile->getImage()->getFormat(),
                                     getWidth(), getHeight());
        pasteImages(image);
        image->save(fileName.c_str());
        delete image;
    }

    void pasteImages(Image *image)
    {
        const ImageFile *i = _imageFile;
        if (_imageFile)
            image->paste(i->getImage(), i->getX(), i->getY());
        if (_sub1)
            _sub1->pasteImages(image);
        if (_sub2)
            _sub2->pasteImages(image);
    }

    int getX() const { return _x; }
    int getY() const { return _y; }
    int getWidth() const { return _width; }
    int getHeight() const { return _height; }

    const ImageFile* getImageFile() const { return _imageFile; }
    const ImageFile* findImageFile(const string& name) const
    {
        if (!_imageFile)
            return NULL;

        if (_imageFile->getFileName() == name)
            return _imageFile;

        const ImageFile *i1 = _sub1->findImageFile(name),
        *i2 = _sub2->findImageFile(name);
        if (i1)
            return i1;
        return i2;
    }
private:
    TextureAtlas *_sub1, *_sub2;
    ImageFile *_imageFile;
    const int _x, _y, _width, _height;
};

// trim from start
static inline string &ltrim(string &s) {
    s.erase(s.begin(), find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace))));
    return s;
}

// trim from end
static inline string &rtrim(string &s) {
    s.erase(find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(), s.end());
    return s;
}

// trim comments
static inline string &ctrim(string &s) {
    size_t hashpos = s.find_first_of("#");
    if (hashpos != string::npos)
        s.erase(s.begin() + hashpos, s.end());
    return s;
}

// trim from both ends and after comments
static inline string &trim(string &s) {
    return ltrim(rtrim(ctrim(s)));
}

TexturePacker::~TexturePacker()
{
    for (pair<string, TextureAtlas*> p: _atlases) {
        delete p.second;
    }
}

bool TexturePacker::parseLine(string& str, Line& line)
{
    string::iterator cursor = find_if(str.begin(), str.end(),
                                      ptr_fun<int, int>(isspace));
    string typestr = str.substr(0, cursor - str.begin());
    string params = str.substr(cursor - str.begin(), string::npos);

    if (typestr == "textureAtlas")
        line.type = LineType::TextureAtlas;
    else if (typestr == "texture")
        line.type = LineType::Texture;
    else if (typestr == "node")
        line.type = LineType::Node;
    else if (typestr == "uvmap")
        line.type = LineType::UVMap;
    else
        return false;

    stringstream p(params);
    string t;
    int i = 0;
    while (getline(p, t, ',')) {
        t = trim(t);
        line.sparams[i++] = t;
    }
    line.paramsCount = i;

    return true;
}

bool TexturePacker::parse(string fileName)
{
    ifstream infile(fileName);
    if (!infile.is_open())
    {
        LOG(1, "Cannot open file: %s\n", fileName.c_str());
        return false;
    }

    struct AtlasItem {
        TextureAtlas *atlas;
        ImageFile *ifile;
    };

    vector<AtlasItem> atlasItems;

    TextureAtlas *currentAtlas = NULL;
    string currentNode = "";
    string originalLine;
    for (; getline(infile, originalLine);) {
        string line = trim(originalLine);
        Line l;

        if (!(line.length() > 0))
            continue;

        if (!parseLine(line, l))
            return false;

        switch (l.type) {
            case LineType::TextureAtlas:
            {
                currentNode = "";
                if (l.paramsCount < 3)
                {
                    LOG(1, "textureAtlas too few parameters, %d instead of 3: ",
                        l.paramsCount);
                    LOG(1, "\"%s\"\n", originalLine.c_str());
                    return false;
                }
                stringstream wi(l.sparams[1]), hi(l.sparams[2]);
                int w, h;
                wi >> w, hi >> h;

                if (w <= 0 || h <= 0)
                {
                    LOG(1, "invalid textureAtlas size.\n");
                    return false;
                }

                TextureAtlas *ta = new TextureAtlas(w, h);
                _atlases[l.sparams[0]] = ta;
                currentAtlas = ta;

                break;
            }
            case LineType::Texture:
            {
                if (!currentAtlas)
                {
                    LOG(1, "Invalid texture item...\n");
                    return false;
                }
                if (l.paramsCount < 1)
                {
                    LOG(1, "texture requires 1 parameter.\n");
                    return false;
                }
                ImageFile *ifile = ImageFile::create(l.sparams[0]);
                if (!ifile)
                {
                    LOG(1, "Cannot open image file - %s\n",
                        l.sparams[0].c_str());
                    return false;
                }
                struct AtlasItem ai = { .atlas = currentAtlas, .ifile = ifile };
                atlasItems.push_back(ai);
                break;
            }
            case LineType::Node:
            {
                currentAtlas = NULL;
                if (l.paramsCount < 1)
                {
                    LOG(1, "node requires 1 parameter.\n");
                    return false;
                }

                Mesh *mesh = dynamic_cast<Mesh*>(
                    _gpbFile->getFromRefTable(l.sparams[0] + meshSuffix));
                if (!mesh)
                {
                    LOG(1, "Node %s doesn't exit or doesn't have a mesh.\n",
                        l.sparams[0].c_str());
                    return false;
                }

                currentNode = l.sparams[0];
                break;
            }
            case LineType::UVMap:
            {
                if (currentNode.length() == 0)
                {
                    LOG(1, "Invalid uvmap item.\n");
                    return false;
                }
                if (l.paramsCount < 3)
                {
                    LOG(1, "uvmap requires 3 parameters.\n");
                    return false;
                }
                stringstream ii(l.sparams[0]);
                int index;
                ii >> index;
                const string& textureAtlas = l.sparams[1];
                const string& textureFile = l.sparams[2];

                AtlasMap::iterator atlasIter = _atlases.find(textureAtlas);
                if (atlasIter == _atlases.end())
                {
                    LOG(1, "TextureAtlas %s isn't defined.\n",
                        textureAtlas.c_str());
                    return false;
                }

                struct UVMapInfo uvmapinfo = {
                    .nodeId = currentNode,
                    .texCoordIndex = index,
                    .textureAtlasName = textureAtlas,
                    .textureName = textureFile
                };
                _uvmaps.push_back(uvmapinfo);

                break;
            }
        }
    }

    //Sort from biggest to smallest
    sort(atlasItems.begin(), atlasItems.end(),
         [](const AtlasItem a1, const AtlasItem a2){
             return a1.ifile->getPerimeter() > a2.ifile->getPerimeter();
         });

    bool failed = false;
    vector<AtlasItem>::const_iterator iter;
    for (iter = atlasItems.cbegin(); iter < atlasItems.cend(); ++iter)
    {
        const AtlasItem& ai = (*iter);

        if (failed) // In case of failure only clean the remaining ImageFiles
        {           // Rest will be cleaned when the TextureAtlas is destroyed.
            delete ai.ifile;
            continue;
        }

        if (ai.atlas->pack(ai.ifile))
        {
            LOG(1, "\tPacked %s!\n", ai.ifile->getFileName().c_str());
        } else {
            LOG(1, "Failed to pack image: %s\n",
                ai.ifile->getFileName().c_str());
            delete ai.ifile;
            failed = true;
        }
    }

    return !failed;
}

bool TexturePacker::write()
{
    for (pair<string, TextureAtlas*> p: _atlases)
    {
        p.second->write(p.first);
    }

    for (UVMapInfo uvmapinfo: _uvmaps) {
        updateUV(uvmapinfo);
    }
    return true;
}

void TexturePacker::updateUV(gameplay::TexturePacker::UVMapInfo uvmapinfo)
{
    Mesh *mesh = dynamic_cast<Mesh*>(_gpbFile->getFromRefTable(uvmapinfo.nodeId + meshSuffix));
    const TextureAtlas *atlas = (*_atlases.find(uvmapinfo.textureAtlasName)).second;
    const ImageFile *ifile = atlas->findImageFile(uvmapinfo.textureName);
    const int atlasWidth = atlas->getWidth(),
              atlasHeight = atlas->getHeight(),
              imageWidth = ifile->getImage()->getWidth(),
              imageHeight = ifile->getImage()->getHeight(),
              index = uvmapinfo.texCoordIndex;

    const float rX = (float) imageWidth / (float) atlasWidth,
                rY = (float) imageHeight / (float) atlasHeight,
                oX = (float) ifile->getX() / (float) atlasWidth,
                oY = 1.0f - ((float) (ifile->getY() + imageHeight) / (float) atlasHeight);

    for (Vertex& v: mesh->vertices)
    {
        float x = v.texCoord[index].x;
        x = oX + x*rX;
        v.texCoord[index].x = x;

        float y = v.texCoord[index].y;
        y = oY + y*rY;
        v.texCoord[index].y = y;
    }
}

}
