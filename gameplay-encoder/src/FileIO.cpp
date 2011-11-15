#include "FileIO.h"

namespace gameplay
{


// Writing out a binary file //

void write(unsigned char value, FILE* file)
{
    size_t r = fwrite(&value, sizeof(unsigned char), 1, file);
    assert(r == 1);
}

void write(char value, FILE* file)
{
    size_t r = fwrite(&value, sizeof(char), 1, file);
    assert(r == 1);
}

void write(const char* str, FILE* file)
{
    size_t length = strlen(str);
    size_t r = fwrite(str, 1, length, file);
    assert(r == length);
}

void write(unsigned int value, FILE* file)
{
    size_t r = fwrite(&value, sizeof(unsigned int), 1, file);
    assert(r == 1);
}

void write(unsigned long value, FILE* file)
{
    size_t r = fwrite(&value, sizeof(unsigned long), 1, file);
    assert(r == 1);
}

void write(unsigned short value, FILE* file)
{
    size_t r = fwrite(&value, sizeof(unsigned short), 1, file);
    assert(r == 1);
}

void write(bool value, FILE* file)
{
    // write booleans as a unsigned char
    unsigned char b = value;
    write(b, file);
}
void write(float value, FILE* file)
{
    fwrite(&value, sizeof(float), 1, file);
}
void write(const float* values, int length, FILE* file)
{
    for (int i = 0; i < length; i++)
    {
        write(values[i], file);
    }
}
void write(const std::string& str, FILE* file)
{
    // Write the length of the string
    write(str.size(), file);
    // Write the array of characters of the string
    write(str.c_str(), file);
}

void writeZero(FILE* file)
{
    write((unsigned int)0, file);
}

// Writing to a text file //

void fprintfElement(FILE* file, const char* elementName, const float values[], int length)
{
    fprintf(file, "<%s>", elementName);
    for (int i = 0; i < length; i++)
    {
        fprintf(file, "%f ", values[i]);
    }
    fprintf(file, "</%s>\n", elementName);
}
void fprintfElement(FILE* file, const char* elementName, const char* value)
{
    fprintf(file, "<%s>%s</%s>\n", elementName, value, elementName);
}
void fprintfElement(FILE* file, const char* elementName, const std::string& value)
{
    fprintf(file, "<%s>%s</%s>\n", elementName, value.c_str(), elementName);
}
void fprintfElement(FILE* file, const char* elementName, float value)
{
    fprintf(file, "<%s>%f</%s>\n", elementName, value, elementName);
}
void fprintfElement(FILE* file, const char* elementName, unsigned int value)
{
    fprintf(file, "<%s>%u</%s>\n", elementName, value, elementName);
}
void fprintfElement(FILE* file, const char* elementName, unsigned char value)
{
    fprintf(file, "<%s>%u</%s>\n", elementName, value, elementName);
}

void fprintfMatrix4f(FILE* file, const float* m)
{
    for (size_t i = 0; i < 16; i ++)
    {
        float v = m[i];
        if (v == 1.0f)
        {
            fprintf(file, "1.0 ");
        }
        else if (v == 0.0)
        {
            fprintf(file, "0.0 ");
        }
        else
        {
            fprintf(file, "%f ",v);
        }
    }
}
void skipString(FILE* file)
{
    // get the size of the char array
    unsigned int length = 0;
    fread(&length, sizeof(unsigned int), 1, file);
    // skip over the unsigned int length
    fseek(file, sizeof(unsigned int), SEEK_CUR);
    if (length > 0)
    {
        // Skip over the array of chars
        long seek = (long)(length * sizeof(char));
        fseek(file, seek, SEEK_CUR);
    }
}

void skipUint(FILE* file)
{
    fseek(file, sizeof(unsigned int), SEEK_CUR);
}

}