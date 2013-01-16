#include "GamepadTest.h"

#if defined(ADD_TEST)
    ADD_TEST("Input", "Gamepads", GamepadTest, 3);
#endif

GamepadTest::GamepadTest()
{
}

void GamepadTest::finalize()
{
    SAFE_RELEASE(_font);
}

void GamepadTest::initialize()
{
    setMultiTouch(true);

    for (int i = 0; i < 2; ++i)
    {
        _gamepads[i] = 0;
    }

    std::vector<Gamepad*>* gamepads = Gamepad::getGamepads();
    std::vector<Gamepad*>::iterator it;
    int i = 0;
    for (it = gamepads->begin(); it != gamepads->end(); it++)
    {
        Gamepad* gamepad = *it;
        if (gamepad)
        {
            _gamepads[i] = gamepad;
            i++;
        }

        if (i == 2)
        {
            break;
        }
    }

    _font = Font::create("res/common/arial18.gpb");
    _status = "Looking for gamepads...";
}

void GamepadTest::update(float elapsedTime)
{
    char s[128];
    _status = "";

    for (int i = 0; i < 2; ++i)
    {
        Gamepad* gamepad = _gamepads[i];
        if (gamepad)
        {
            gamepad->update(elapsedTime);

            sprintf(s, "Player: %d - VendorID:%d,%s, Product ID:%d,%s\nButtons: ", i,
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
    }
}

void GamepadTest::render(float elapsedTime)
{
    clear(CLEAR_COLOR_DEPTH, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0);

    for (int i = 0; i < 2; ++i)
    {
        if (_gamepads[i] && _gamepads[i]->isVirtual())
            _gamepads[i]->draw();
    }

    drawFrameRate(_font, Vector4(0, 0.5f, 1, 1), 5, 1, getFrameRate());

    _font->start();
    _font->drawText(_status.c_str(), 5, 25, Vector4::one());
    _font->finish();
}

void GamepadTest::gamepadEvent(Gamepad::GamepadEvent evt, Gamepad* gamepad)
{
    switch(evt)
    {
        case Gamepad::CONNECTED_EVENT:
        {
            bool physical = false;
            if (gamepad->isVirtual())
            {
                gamepad->getForm()->setConsumeInputEvents(false);
            }
            else
            {
                physical = true;
            }

            bool foundUnassigned = false;
            for (int i = 0; i < 2; ++i)
            {
                // If no gamepad is assigned to this player, assign this gamepad.
                if (!_gamepads[i])
                {
                    _gamepads[i] = gamepad;
                    foundUnassigned = true;
                    break;
                }
            }

            if (physical && !foundUnassigned)
            {
                // If a new physical gamepad was connected, replace the virtual gamepad with this new one.
                for (int i = 0; i < 2; ++i)
                {
                    if (_gamepads[i] && _gamepads[i]->isVirtual())
                    {
                        _gamepads[i] = gamepad;
                        break;
                    }
                }
            }
            break;
        }
        case Gamepad::DISCONNECTED_EVENT:
        {
            std::vector<Gamepad*>* gamepads = Gamepad::getGamepads();
            for (int i = 0; i < 2; ++i)
            {
                bool done = false;
                if (_gamepads[i] == gamepad)
                {
                    _gamepads[i] = NULL;

                    // Replace disconnected gamepad with first available gamepad not currently in use,
                    // if one is available.
                    std::vector<Gamepad*>::iterator it;
                    for (it = gamepads->begin(); it != gamepads->end(); it++)
                    {
                        Gamepad* g = *it;
                        if (g == gamepad)
                        {
                            continue;
                        }

                        bool inUse = false;
                        for (int j = 0; j < 2; ++j)
                        {
                            if (g == _gamepads[j])
                            {
                                inUse = true;
                            }
                        }

                        if (!inUse)
                        {
                            _gamepads[i] = g;
                            done = true;
                            break;
                        }
                    }
                }

                if (done)
                {
                    break;
                }
            }
            break;
        }
    }
}