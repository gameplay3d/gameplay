#include "GPBDecoder.h"

namespace gameplay
{

GPBDecoder::GPBDecoder(void) : file(NULL), outFile(NULL)
{
}


GPBDecoder::~GPBDecoder(void)
{
}

void GPBDecoder::readBinary(const std::string& filepath)
{
    // open files
    file = fopen(filepath.c_str(), "rb");
    std::string outfilePath = filepath;
    outfilePath += ".xml";
    outFile = fopen(outfilePath.c_str(), "w");

    // read and write files
    assert(validateHeading());

    fprintf(outFile, "<root>\n");
    readRefs();
    fprintf(outFile, "</root>\n");


    // close files
    fclose(outFile);
    outFile = NULL;

    fclose(file);
    file = NULL;
}

bool GPBDecoder::validateHeading()
{
    const size_t HEADING_SIZE = 9;
    const char identifier[] = { '«', 'G', 'P', 'B', '»', '\r', '\n', '\x1A', '\n' };

    char heading[HEADING_SIZE];
    for (size_t i = 0; i < HEADING_SIZE; i++)
    {
        if (heading[i] != identifier[i])
        {
            return false;
        }
    }
    // read version
    unsigned char version[2];
    fread(version, sizeof(unsigned char), 2, file);
    // don't care about version

    return true;
}

void GPBDecoder::readRefs()
{
    fprintf(outFile, "<RefTable>\n");
    // read number of refs
    unsigned int refCount;
    assert(read(&refCount));
    for (size_t i = 0; i < refCount; i++)
    {
        readRef();
    }
    fprintf(outFile, "</RefTable>\n");
}

void GPBDecoder::readRef()
{   
    std::string xref = readString(file);
    unsigned int type, offset;
    assert(read(&type));
    assert(read(&offset));
    
    fprintf(outFile, "<Reference>\n");
    fprintfElement(outFile, "xref", xref);
    fprintfElement(outFile, "type", type);
    fprintfElement(outFile, "offset", offset);
    fprintf(outFile, "</Reference>\n");
}

bool GPBDecoder::read(unsigned int* ptr)
{
    return fread(ptr, sizeof(unsigned int), 1, file) == 1;
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

};