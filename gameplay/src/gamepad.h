/*
 * Gamepad.h
 */

#ifndef GAMEPAD_H_
#define GAMEPAD_H_

#include "Vector3.h"

namespace gameplay
{

/*
 * Defines a class for accessing gamepad controls.
 */
class Gamepad
{
public:

    /*
     * Represents the maximum number of buttons supported on a gamepad.
     */
    static const int MAX_BUTTONS = 32;

    /*
     * Represents the maximum number of joysticks supported on a gamepad.
     */
    static const int MAX_JOYSTICKS = 2;

    /*
     * Represents the maximum number of dpads supported on a gamepad.
     */
    static const int MAX_DPADS = 2;

private:

    /*
     * Creates an instance of the gamepad with the input .gamepad file.
     * 
     * @param filepath Path of the .gamepad file
     * @return instance of Gamepad.
     */

    Gamepad* createGamepad(std::string filepath);
    
    /* 
     * Defines a gamepad button.
     */
    class Button
    {
        bool pressed;
    };

    /*
     * Defines a gamepad joystick.
     */
    class Joystick
    {
        Vector3 direction;
    };

    /*
     * Defines a gamepad dpad.
     */
    class Dpad
    {
        // TODO.
    };

    Button _buttons[MAX_BUTTONS];
    Joystick _joysticks[MAX_JOYSTICKS];
    Dpad _dpads[MAX_DPADS];

};

}

#endif
