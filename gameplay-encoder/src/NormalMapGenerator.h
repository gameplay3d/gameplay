#ifndef NORMALMAPGENERATOR_H_
#define NORMALMAPGENERATOR_H_

#include "Vector3.h"

namespace gameplay
{

class NormalMapGenerator
{

public:

    NormalMapGenerator(const char* inputFile, const char* outputFile, int resolutionX, int resolutionY, const Vector3& worldSize);
    ~NormalMapGenerator();

    void generate();

private:

    // Hidden copy/assignment
    NormalMapGenerator(const NormalMapGenerator&);
    NormalMapGenerator& operator=(const NormalMapGenerator&);

    std::string _inputFile;
    std::string _outputFile;
    int _resolutionX;
    int _resolutionY;
    Vector3 _worldSize;

};

}

#endif
