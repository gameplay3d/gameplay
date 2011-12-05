#include "Base.h"
#include "Input.h"
#include "Platform.h"
#include "Game.h"

namespace gameplay
{

Input::Input()
{
}

bool Input::isAccelerometerSupported()
{
    return Platform::isAccelerometerSupported();
}

void Input::getAccelerometerPitchAndRoll(float* pitch, float* roll)
{
    Platform::getAccelerometerPitchAndRoll(pitch, roll);
}

}
