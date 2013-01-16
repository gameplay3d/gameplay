#include "NormalMapGenerator.h"
#include "Image.h"
#include "Base.h"

namespace gameplay
{

NormalMapGenerator::NormalMapGenerator(const char* inputFile, const char* outputFile, int resolutionX, int resolutionY, const Vector3& worldSize)
    : _inputFile(inputFile), _outputFile(outputFile), _resolutionX(resolutionX), _resolutionY(resolutionY), _worldSize(worldSize)
{
}

NormalMapGenerator::~NormalMapGenerator()
{
}

bool equalsIgnoreCase(const std::string& s1, const std::string& s2)
{
    size_t l1 = s1.size();
    size_t l2 = s2.size();
    if (l1 != l2)
        return false;

    for (size_t i = 0; i < l1; ++i)
    {
        if (tolower(s1[i]) != tolower(s2[i]))
            return false;
    }

    return true;
}

float getHeight(float* heights, int width, int height, int x, int y)
{
    if (x < 0)
        x = 0;
    else if (x >= width)
        x = width-1;
    if (y < 0)
        y = 0;
    else if (y >= height)
        y = height-1;
    return heights[y*width+x];
}

void calculateNormal(
    float x1, float y1, float z1,
    float x2, float y2, float z2,
    float x3, float y3, float z3,
    Vector3* normal)
{
    Vector3 E(x1, y1, z1);
    Vector3 F(x2, y2, z2);
    Vector3 G(x3, y3, z3);

    Vector3 P, Q;
    Vector3::subtract(F, E, &P);
    Vector3::subtract(G, E, &Q);

    Vector3::cross(Q, P, normal);
}

void NormalMapGenerator::generate()
{
    // Load the input heightmap
    float* heights = NULL;
    size_t pos = _inputFile.find_last_of('.');
    std::string ext = pos == std::string::npos ? "" : _inputFile.substr(pos, _inputFile.size()-pos);
    if (equalsIgnoreCase(ext, ".png"))
    {
        // Load heights from PNG image
        Image* image = Image::create(_inputFile.c_str());
        if (image == NULL)
        {
            LOG(1, "Failed to load input heightmap PNG: %s.\n", _inputFile.c_str());
            return;
        }

        // TODO: Add command-line argument for high precision heightmaps

        _resolutionX = image->getWidth();
        _resolutionY = image->getHeight();
        int size = _resolutionX * _resolutionY;
        heights = new float[size];
        unsigned char* data = (unsigned char*)image->getData();
        for (int i = 0; i < size; ++i)
        {
            switch (image->getFormat())
            {
            case Image::LUMINANCE:
                heights[i] = data[i];
                break;
            case Image::RGB:
            case Image::RGBA:
                {
                    int pos = i * image->getBpp();
                    heights[i] = (data[pos] + data[pos+1] + data[pos+2]) / 3.0f;
                }
                break;
            default:
                heights[i] = 0.0f;
                break;
            }
            heights[i] = (heights[i] / 255.0f) * _worldSize.y;
        }
        SAFE_DELETE(image);
    }
    else if (equalsIgnoreCase(ext, ".raw"))
    {
        // Load heights from RAW 8 or 16-bit file
        if (_resolutionX <= 0 || _resolutionY <= 0)
        {
            LOG(1, "Missing resolution argument - must be explicitly specified for RAW heightmap files: %s\n.", _inputFile.c_str());
            return;
        }

        // TODO
        heights = new float[_resolutionX * _resolutionY];
        // TODO

        LOG(1, "RAW files not yet implemented...");
        return;
    }
    else
    {
        LOG(1, "Unsupported input heightmap file (must be a valid PNG or RAW file: %s.\n", _inputFile.c_str());
        return;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    //
    // NOTE: This method assumes the heightmap geometry is generated as follows.
    //
    //   -----------
    //  | / | / | / |
    //  |-----------|
    //  | / | / | / |
    //  |-----------|
    //  | / | / | / |
    //   -----------
    //
    ///////////////////////////////////////////////////////////////////////////////////////////////

    struct NormalPixel
    {
        unsigned char r, g, b;
    };
    NormalPixel* normalPixels = new NormalPixel[_resolutionX * _resolutionY];

    struct Face
    {
        Vector3 normal1;
        Vector3 normal2;
    };

    int progressMax = (_resolutionX-1) * (_resolutionY-1) + _resolutionX * _resolutionY;
    int progress = 0;

    Vector2 scale(_worldSize.x / (_resolutionX-1), _worldSize.z / (_resolutionY-1));

    // First calculate all face normals for the heightmap
    LOG(1, "Calculating normals... 0%%");
    Face* faceNormals = new Face[(_resolutionX - 1) * (_resolutionY - 1)];
    Vector3 v1, v2;
    for (int z = 0; z < _resolutionY-1; z++)
    {
        for (int x = 0; x < _resolutionX-1; x++)
        {
            float topLeftHeight = getHeight(heights, _resolutionX, _resolutionY, x, z);
            float bottomLeftHeight = getHeight(heights, _resolutionX, _resolutionY, x, z + 1);
            float bottomRightHeight = getHeight(heights, _resolutionX, _resolutionY, x + 1, z + 1);
            float topRightHeight = getHeight(heights, _resolutionX, _resolutionY, x + 1, z);

            // Triangle 1
            calculateNormal(
                (float)x*scale.x, bottomLeftHeight, (float)(z + 1)*scale.y,
                (float)x*scale.x, topLeftHeight, (float)z*scale.y,
                (float)(x + 1)*scale.x, topRightHeight, (float)z*scale.y,
                &faceNormals[z*(_resolutionX-1)+x].normal1);

            // Triangle 2
            calculateNormal(
                (float)x*scale.x, bottomLeftHeight, (float)(z + 1)*scale.y,
                (float)(x + 1)*scale.x, topRightHeight, (float)z*scale.y,
                (float)(x + 1)*scale.x, bottomRightHeight, (float)(z + 1)*scale.y,
                &faceNormals[z*(_resolutionX-1)+x].normal2);

            ++progress;
            LOG(1, "\rCalculating normals... %d%%", (int)(((float)progress / progressMax) * 100));
        }
    }

    // Free height array
    delete[] heights;
    heights = NULL;

    // Smooth normals by taking an average for each vertex
    Vector3 normal;
    for (int z = 0; z < _resolutionY; z++)
    {
        for (int x = 0; x < _resolutionX; x++)
        {
            // Reset normal sum
            normal.set(0, 0, 0);

            if (x > 0)
            {
                if (z > 0)
                {
                    // Top left
                    normal.add(faceNormals[(z-1)*(_resolutionX-1) + (x-1)].normal2);
                }

                if (z < (_resolutionY - 1))
                {
                    // Bottom left
                    normal.add(faceNormals[z*(_resolutionX-1) + (x - 1)].normal1);
                    normal.add(faceNormals[z*(_resolutionX-1) + (x - 1)].normal2);
                }
            }

            if (x < (_resolutionX - 1))
            {
                if (z > 0)
                {
                    // Top right
                    normal.add(faceNormals[(z-1)*(_resolutionX-1) + x].normal1);
                    normal.add(faceNormals[(z-1)*(_resolutionX-1) + x].normal2);
                }

                if (z < (_resolutionY - 1))
                {
                    // Bottom right
                    normal.add(faceNormals[z*(_resolutionX-1) + x].normal1);
                }
            }

            // We don't have to worry about weighting the normals by
            // the surface area of the triangles since a heightmap 
            // guarantees that all triangles have the same surface area.
            normal.normalize();

            // Store this vertex normal
            NormalPixel& pixel = normalPixels[z*_resolutionX + x];
            pixel.r = (unsigned char)((normal.x + 1.0f) * 0.5f * 255.0f);
            pixel.g = (unsigned char)((normal.y + 1.0f) * 0.5f * 255.0f);
            pixel.b = (unsigned char)((normal.z + 1.0f) * 0.5f * 255.0f);

            ++progress;
            LOG(1, "\rCalculating normals... %d%%", (int)(((float)progress / progressMax) * 100));
        }
    }

    LOG(1, "\rCalculating normals... Done.\n");

    // Create and save an image for the normal map
    Image* normalMap = Image::create(Image::RGB, _resolutionX, _resolutionY);
    normalMap->setData(normalPixels);
    normalMap->save(_outputFile.c_str());

    LOG(1, "Normal map saved to '%s'.\n", _outputFile.c_str());

    // Free temp data
    delete[] normalPixels;
    normalPixels = NULL;
}

}
