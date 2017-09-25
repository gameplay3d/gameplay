#include "Base.h"
#include "Platform.h"

namespace gameplay
{

void Platform::onKeyEvent(KeyEvent evt, Input::Key key)
{
    // TODO: update the Input class key state.
}

void Platform::onMouseEvent(MouseEvent evt, int x, int y, int wheelDelta)
{
    // TODO: update the Input class button state.
}

void Platform::onGamepadEvent(GamepadEvent evt, size_t index)
{
    // TODO: update the Input class gamepads connected state.
}

}