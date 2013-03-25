#ifndef HEIGHTMAP_H_
#define HEIGHTMAP_H_

#include "Node.h"

namespace gameplay
{

class Heightmap
{
public:

    /**
     * Generates heightmap data and saves the result to the specified filename (PNG file).
     *
     * @param nodeIds List of node ids to include in the heightmap generation.
     * @param width Width of the produced heightmap image.
     * @param height Height of the produced  heightmap image.
     * @param filename Output PNG file to write the heightmap image to.
     * @param highP Use packed 24-bit (RGB) instead of standard 8-bit grayscale.
     */
    static void generate(const std::vector<std::string>& nodeIds, int width, int height, const char* filename, bool highP = false);

};

}

#endif
