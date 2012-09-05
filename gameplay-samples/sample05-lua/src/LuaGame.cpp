#include "LuaGame.h"

// Declare our game instance
LuaGame game;

LuaGame::LuaGame()
{
}

void LuaGame::initialize()
{
}

void LuaGame::finalize()
{
}

void LuaGame::update(float elapsedTime)
{
}

void LuaGame::render(float elapsedTime)
{
}


void LuaGame::gestureSwipeEvent(int x, int y, int direction)
{
    printf("Regonized Swipe Event: %d, %d, %d\n", x, y, direction);
}

void LuaGame::gesturePinchEvent(int x, int y, float scale)
{
    printf("Regonized Pinch Event: %d, %d, %f\n", x, y, scale);
}

void LuaGame::gestureRotateEvent(int x, int y, float angle)
{
    printf("Regonized Rotate Event: %d, %d, %f\n", x, y, angle);
}

void LuaGame::gestureTapEvent(int x, int y)
{
    printf("Regonized Rotate Event: %d, %d\n", x, y);
}

void LuaGame::gestureTapDoubleEvent(int x, int y)
{
    printf("Regonized Double Tap Event: %d, %d\n", x, y);
}