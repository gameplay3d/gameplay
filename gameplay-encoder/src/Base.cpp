#include "Base.h"

namespace gameplay
{

void fillArray(float values[], float value, size_t length)
{
    for (size_t i = 0; i < length; i++)
    {
        values[i] = value;
    }
}

void setIdentityMatrix(float values[])
{
    values[0] = 1.0f;
    values[1] = 0.0f;
    values[2] = 0.0f;
    values[3] = 0.0f;
    values[4] = 0.0f;
    values[5] = 1.0f;
    values[6] = 0.0f;
    values[7] = 0.0f;
    values[8] = 0.0f;
    values[9] = 0.0f;
    values[10] = 1.0f;
    values[11] = 0.0f;
    values[12] = 0.0f;
    values[13] = 0.0f;
    values[14] = 0.0f;
    values[15] = 1.0f;
}

}