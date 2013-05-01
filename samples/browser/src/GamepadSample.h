#ifndef GAMEPADSAMPLE_H_
#define GAMEPADSAMPLE_H_

#include "SamplesGame.h"
#include "Gamepad.h"

using namespace gameplay;

class GamepadSample : public Sample
{
public:

    GamepadSample();

    void gamepadEvent(Gamepad::GamepadEvent evt, Gamepad* gamepad);

protected:

    void initialize();

    void finalize();

    void update(float elapsedTime);

    void render(float elapsedTime);

private:

    void updateGamepad(float elapsedTime, Gamepad* gamepad, unsigned int player);

    const char* getStringFromButtonMapping(Gamepad::ButtonMapping mapping);
    
    Gamepad* _gamepad;
    
    Font* _font;
    std::string _status;
};

#endif