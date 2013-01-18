#include "GamepadTest.h"

#if defined(ADD_TEST)
    //ADD_TEST("Input", "Gamepads", GamepadTest, 3);
#endif

GamepadTest::GamepadTest() : _player1(NULL), _player2(NULL)
{
}

void GamepadTest::finalize()
{
    SAFE_RELEASE(_font);
    if (_player1->isVirtual())
    {
        _player1->getForm()->setEnabled(false);
    }
}

void GamepadTest::initialize()
{
    setMultiTouch(true);

    unsigned int gamepadCount = getGamepadCount();
    if (gamepadCount == 1)
        _player1 = getGamepad(0);
    if (gamepadCount == 2)
        _player2 = getGamepad(1);

    _font = Font::create("res/common/arial18.gpb");
    _status = "Looking for gamepads...";
}

void GamepadTest::updateGamepad(float elapsedTime, Gamepad* gamepad, unsigned int player)
{
    char s[128];
    gamepad->update(elapsedTime);

    sprintf(s, "Player: %d - VendorID:%d,%s, Product ID:%d,%s\nButtons: ", 
                player, 
                gamepad->getVendorId(), gamepad->getVendorString(),
                gamepad->getProductId(), gamepad->getProductString());
    _status += s;
    for (int j = 0; j < 20; ++j)
    {
        if (gamepad->isButtonDown((Gamepad::ButtonMapping)j))
        {
            sprintf(s, "%d ", j);
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

void GamepadTest::update(float elapsedTime)
{
    _status = "";
    if (_player1)
        updateGamepad(elapsedTime, _player1, 1);
    if (_player2)
        updateGamepad(elapsedTime, _player2, 2);
}

void GamepadTest::render(float elapsedTime)
{
    clear(CLEAR_COLOR_DEPTH, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0);

    drawFrameRate(_font, Vector4(0, 0.5f, 1, 1), 5, 1, getFrameRate());

    _font->start();
    _font->drawText(_status.c_str(), 5, 25, Vector4::one());
    _font->finish();

    if (_player1 && _player1->isVirtual())
        _player1->draw();
}

void GamepadTest::gamepadEvent(Gamepad::GamepadEvent evt, Gamepad* gamepad)
{
    switch(evt)
    {
        case Gamepad::CONNECTED_EVENT:
        {   
            // Virtual Gamepad event come right when the game starts
            if (gamepad->isVirtual())
            {
                gamepad->getForm()->setConsumeInputEvents(false);
                _player1 = gamepad;
            }
            else
            {
                // First physical gamepad will go in player 2
                if (_player2 == NULL)
                {
                    _player2 = gamepad;
                }
                else
                {
                    // Second physical gamepad will bump virtual gamepad out of player one
                    _player1 = gamepad;
                }
            }
            break;
        }
        case Gamepad::DISCONNECTED_EVENT:
        {
            if (gamepad == _player2)
                _player2 = NULL;
            else if (gamepad == _player1)
                _player1 = NULL;
            break;
        }
    }
}