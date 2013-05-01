#include "GamepadSample.h"

#if defined(ADD_SAMPLE)
    ADD_SAMPLE("Input", "Gamepads", GamepadSample, 3);
#endif

GamepadSample::GamepadSample() : _gamepad(NULL)
{
}

void GamepadSample::finalize()
{
    SAFE_RELEASE(_font);
}

void GamepadSample::initialize()
{
    setMultiTouch(true);

    _gamepad = getGamepad(0);
    // This is needed because the virtual gamepad is shared between all samples.
    // SamplesGame always ensures the virtual gamepad is disabled when a sample is exited.
    if (_gamepad && _gamepad->isVirtual())
        _gamepad->getForm()->setEnabled(true);

    _font = Font::create("res/common/arial18.gpb");
    _status = "Looking for gamepads...";
}

void GamepadSample::updateGamepad(float elapsedTime, Gamepad* gamepad, unsigned int player)
{
    char s[128];
    sprintf(s, "Player: %d - VendorID: %d, %s, Product ID: %d, %s\nButtons: ", 
                player, 
                gamepad->getVendorId(), gamepad->getVendorString(),
                gamepad->getProductId(), gamepad->getProductString());
    _status += s;
    for (int j = 0; j < 20; ++j)
    {
        if (gamepad->isButtonDown((Gamepad::ButtonMapping)j))
        {
            sprintf(s, "%s ", getStringFromButtonMapping((Gamepad::ButtonMapping)j));
            _status += s;
        }
    }
    _status += "\n";
    for (unsigned int j = 0; j < gamepad->getJoystickCount(); ++j)
    {
        Vector2 joystick;
        gamepad->getJoystickValues(j, &joystick);
        sprintf(s, "Joystick %d: (%f, %f)\n", j, joystick.x, joystick.y);
        _status += s;
    }
    for (unsigned int j = 0; j < gamepad->getTriggerCount(); ++j)
    {
        sprintf(s, "Trigger %d: %f\n", j, gamepad->getTriggerValue(j));
        _status += s;
    }
    _status += "\n";
}

void GamepadSample::update(float elapsedTime)
{
    _status = "";
    if (_gamepad)
        updateGamepad(elapsedTime, _gamepad, 1);
}

void GamepadSample::render(float elapsedTime)
{
    clear(CLEAR_COLOR_DEPTH, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0);

    drawFrameRate(_font, Vector4(0, 0.5f, 1, 1), 5, 1, getFrameRate());

    _font->start();
    _font->drawText(_status.c_str(), 5, 25, Vector4::one());
    _font->finish();


    _gamepad->draw();
}

void GamepadSample::gamepadEvent(Gamepad::GamepadEvent evt, Gamepad* gamepad)
{
    switch(evt)
    {
        case Gamepad::CONNECTED_EVENT:
        case Gamepad::DISCONNECTED_EVENT:
            _gamepad = getGamepad(0);
            // This is needed because the virtual gamepad is shared between all samples.
            // SamplesGame always ensures the virtual gamepad is disabled when a sample is exited.
            if (_gamepad && _gamepad->isVirtual())
                _gamepad->getForm()->setEnabled(true);
            break;
    }
}

const char* GamepadSample::getStringFromButtonMapping(Gamepad::ButtonMapping mapping)
{
    switch (mapping)
    {
        case Gamepad::BUTTON_A:
            return "A";
        case Gamepad::BUTTON_B:
            return "B";
        case Gamepad::BUTTON_C:
            return "C";
        case Gamepad::BUTTON_X:
            return "X";
        case Gamepad::BUTTON_Y:
            return "Y";
        case Gamepad::BUTTON_Z:
            return "Z";
        case Gamepad::BUTTON_MENU1:
            return "MENU1";
        case Gamepad::BUTTON_MENU2:
            return "MENU2";
        case Gamepad::BUTTON_MENU3:
            return "MENU3";
        case Gamepad::BUTTON_MENU4:
            return "MENU4";
        case Gamepad::BUTTON_L1:
            return "L1";
        case Gamepad::BUTTON_L2:
            return "L2";
        case Gamepad::BUTTON_L3:
            return "L3";
        case Gamepad::BUTTON_R1:
            return "R1";
        case Gamepad::BUTTON_R2:
            return "R2";
        case Gamepad::BUTTON_R3:
            return "R3";
        case Gamepad::BUTTON_UP:
            return "UP";
        case Gamepad::BUTTON_DOWN:
            return "DOWN";
        case Gamepad::BUTTON_LEFT:
            return "LEFT";
        case Gamepad::BUTTON_RIGHT:
            return "RIGHT";
        default:
            return "";
    }
}