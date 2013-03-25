#include "Base.h"

namespace gameplay
{

void fillArray(float values[], float value, size_t length)
{
    for (size_t i = 0; i < length; ++i)
    {
        values[i] = value;
    }
}

std::string getBaseName(const std::string& filepath)
{
    size_t index1 = filepath.find_last_of('\\');
    size_t index2 = filepath.find_last_of('/');
    size_t index = (index1 != -1 && index1 > index2 ? index1 : index2);
    size_t length = filepath.length();
    std::string filename = filepath.substr(index + 1, length);
    length = filename.length();
    std::string output = filename.substr(0, (length-4));
    return output;
}

}