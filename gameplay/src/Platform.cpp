// Implementation of base platform-agnostic platform functionality.
#include "Base.h"
#include "Platform.h"
#include "Game.h"
#include "ScriptController.h"
#include "Form.h"

namespace gameplay
{

void Platform::touchEventInternal(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex, bool actuallyMouse)
{
    if (actuallyMouse || !Form::touchEventInternal(evt, x, y, contactIndex))
    {
        Game::getInstance()->touchEvent(evt, x, y, contactIndex);
        Game::getInstance()->getScriptController()->touchEvent(evt, x, y, contactIndex);
    }
}

void Platform::keyEventInternal(Keyboard::KeyEvent evt, int key)
{
    if (!Form::keyEventInternal(evt, key))
    {
        Game::getInstance()->keyEvent(evt, key);
        Game::getInstance()->getScriptController()->keyEvent(evt, key);
    }
}

bool Platform::mouseEventInternal(Mouse::MouseEvent evt, int x, int y, int wheelDelta)
{
    if (Form::mouseEventInternal(evt, x, y, wheelDelta))
    {
        return true;
    }
    else if (Game::getInstance()->mouseEvent(evt, x, y, wheelDelta))
    {
        return true;
    }
    else
    {
        return Game::getInstance()->getScriptController()->mouseEvent(evt, x, y, wheelDelta);
    }
}

void Platform::gestureSwipeEventInternal(int x, int y, int direction)
{
    // TODO: Add support to Form for gestures
    Game::getInstance()->gestureSwipeEvent(x, y, direction);
    Game::getInstance()->getScriptController()->gestureSwipeEvent(x, y, direction);
}

void Platform::gesturePinchEventInternal(int x, int y, float scale)
{
    // TODO: Add support to Form for gestures
    Game::getInstance()->gesturePinchEvent(x, y, scale);
    Game::getInstance()->getScriptController()->gesturePinchEvent(x, y, scale);
}

void Platform::gestureTapEventInternal(int x, int y)
{
    // TODO: Add support to Form for gestures
    Game::getInstance()->gestureTapEvent(x, y);
    Game::getInstance()->getScriptController()->gestureTapEvent(x, y);
}

void Platform::resizeEventInternal(unsigned int width, unsigned int height)
{
    // Update the width and height of the game
    Game* game = Game::getInstance();
    if (game->_width != width || game->_height != height)
    {
        game->_width = width;
        game->_height = height;
        game->resizeEvent(width, height);
        game->getScriptController()->resizeEvent(width, height);
    }
}

void Platform::gamepadEventInternal(Gamepad::GamepadEvent evt, Gamepad* gamepad, unsigned int analogIndex)
{
	switch(evt)
	{
	case Gamepad::CONNECTED_EVENT:
	case Gamepad::DISCONNECTED_EVENT:
		Game::getInstance()->gamepadEvent(evt, gamepad);
        Game::getInstance()->getScriptController()->gamepadEvent(evt, gamepad);
		break;
	case Gamepad::BUTTON_EVENT:
	case Gamepad::JOYSTICK_EVENT:
	case Gamepad::TRIGGER_EVENT:
		Form::gamepadEventInternal(evt, gamepad, analogIndex);
		break;
	}
}

void Platform::gamepadEventConnectedInternal(GamepadHandle handle,  unsigned int buttonCount, unsigned int joystickCount, unsigned int triggerCount,
                                             unsigned int vendorId, unsigned int productId, const char* vendorString, const char* productString)
{
    Gamepad::add(handle, buttonCount, joystickCount, triggerCount, vendorId, productId, vendorString, productString);
}

void Platform::gamepadEventDisconnectedInternal(GamepadHandle handle)
{
    Gamepad::remove(handle);
}

Platform::TextureCompressionType Platform::getTextureCompressionType()
{
    TextureCompressionType type = TEXTURE_COMPRESSION_NONE;

    char *extensions = (char*) glGetString(GL_EXTENSIONS);

    if (!extensions)
        return type;

    if (strstr(extensions, "GL_AMD_compressed_ATC_texture"))
        type = TEXTURE_COMPRESSION_ATITC;
    if (strstr(extensions, "GL_ATI_texture_compression_atitc"))
        type = TEXTURE_COMPRESSION_ATITC;

    if (strstr(extensions, "GL_IMG_texture_compression_pvrtc"))
        type = TEXTURE_COMPRESSION_PVRTC;

    if (strstr(extensions, "GL_OES_texture_compression_S3TC"))
        type = TEXTURE_COMPRESSION_DXTC;
    if (strstr(extensions, "GL_EXT_texture_compression_s3tc"))
        type = TEXTURE_COMPRESSION_DXTC;
    if (strstr(extensions, "GL_EXT_texture_compression_dxt1"))
        type = TEXTURE_COMPRESSION_DXTC;
    if (strstr(extensions, "GL_EXT_texture_compression_dxt3"))
        type = TEXTURE_COMPRESSION_DXTC;
    if (strstr(extensions, "GL_EXT_texture_compression_dxt5"))
        type = TEXTURE_COMPRESSION_DXTC;

    return type;
}

}
