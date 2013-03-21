#include "Base.h"
#include "GPBDecoder.h"

namespace gameplay
{

GPBDecoder::GPBDecoder(void) : _file(NULL), _outFile(NULL)
{
}


GPBDecoder::~GPBDecoder(void)
{
}

void GPBDecoder::readBinary(const std::string& filepath)
{
    // open files
    _file = fopen(filepath.c_str(), "rb");
    std::string outfilePath = filepath;
    outfilePath += ".xml";
    _outFile = fopen(outfilePath.c_str(), "w");

    // read and write files
    assert(validateHeading());

    fprintf(_outFile, "<root>\n");
    readRefs();
    fprintf(_outFile, "</root>\n");


    // close files
    fclose(_outFile);
    _outFile = NULL;

    fclose(_file);
    _file = NULL;
}

bool GPBDecoder::validateHeading()
{
    const size_t HEADING_SIZE = 9;
    const char identifier[] = "\xABGPB\xBB\r\n\x1A\n";

    char heading[HEADING_SIZE];
    fread(heading, sizeof(unsigned char), HEADING_SIZE, _file);
    for (size_t i = 0; i < HEADING_SIZE; ++i)
    {
        if (heading[i] != identifier[i])
        {
            return false;
        }
    }
    // read version
    unsigned char version[2];
    fread(version, sizeof(unsigned char), 2, _file);
    // don't care about version

    return true;
}

void GPBDecoder::readRefs()
{
    fprintf(_outFile, "<RefTable>\n");
    // read number of refs
    unsigned int refCount = 0;
    assert(read(&refCount));
    for (size_t i = 0; i < refCount; ++i)
    {
        readRef();
    }
    fprintf(_outFile, "</RefTable>\n");
}

void GPBDecoder::readRef()
{
    std::string xref = readString(_file);
    unsigned int type = 0, offset = 0;
    assert(read(&type));
    assert(read(&offset));
    
    fprintf(_outFile, "<Reference>\n");
    fprintfElement(_outFile, "xref", xref);
    fprintfElement(_outFile, "type", type);
    fprintfElement(_outFile, "offset", offset);
    fprintf(_outFile, "</Reference>\n");
}

bool GPBDecoder::read(unsigned int* ptr)
{
    return fread(ptr, sizeof(unsigned int), 1, _file) == 1;
}

std::string GPBDecoder::readString(FILE* fp)
{
    unsigned int length;
    if (fread(&length, 4, 1, fp) != 1)
    {
        return std::string();
    }

    char* str = new char[length + 1];
    if (length > 0)
    {
        if (fread(str, 1, length, fp) != length)
        {
            delete[] str;
            return std::string();
        }
    }

    str[length] = '\0';
    std::string result(str);
    delete[] str;
    return result;
}

}
