#ifndef GPBDECODER_H_
#define GPBDECODER_H_

#include "FileIO.h"

namespace gameplay
{
/**
 * This class is used for decoding a GPB file for the purpose of debugging.
 */
class GPBDecoder
{
public:

    /**
     * Constructor.
     */
    GPBDecoder(void);

    /**
     * Destructor.
     */
    ~GPBDecoder(void);

    void readBinary(const std::string& filepath);
    bool validateHeading();

    void readRefs();
    void readRef();

    bool read(unsigned int* ptr);
    std::string readString(FILE* fp);

private:

    FILE* _file;
    FILE* _outFile;
};

}

#endif
