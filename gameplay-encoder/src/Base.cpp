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

}