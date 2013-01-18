#ifndef GAMEPADTEST_H_
#define GAMEPADTEST_H_

#include "TestsGame.h"
#include "Gamepad.h"

using namespace gameplay;

class GamepadTest : public Test
{
public:

    GamepadTest();

    /**
     * @see Game::gamepadEvent
     */
    void gamepadEvent(Gamepad::GamepadEvent evt, Gamepad* gamepad);

protected:

    void initialize();

    void finalize();

    void update(float elapsedTime);

    void render(float elapsedTime);

private:

    void updateGamepad(float elapsedTime, Gamepad* gamepad, unsigned int player);

    Gamepad* _player1;
    Gamepad* _player2;
    
    Font* _font;
    std::string _status;
};

#endif