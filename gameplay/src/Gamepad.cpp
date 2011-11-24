/*
 * Gamepad.cpp
 */

#include "Base.h"
#include "Gamepad.h"
#include "Texture.h"

namespace gameplay
{

static Gamepad* __gamepad = NULL;

Gamepad::Gamepad()
    : _buttonCount(0), _joystickCount(0), _texture(NULL), _spriteBatch(NULL)
{
}

Gamepad::Gamepad(const Gamepad* g)
{
}

Gamepad::~Gamepad()
{
}

Gamepad::Button::Button()
    : _pressed(BUTTON_RELEASED), _key(Input::KEY_NONE), _position(Vector2::zero()),
    _defaultTexCoord(Vector4::zero()), _defaultTexture(false), 
    _focusTexCoord(Vector4::zero()), _focusTexture(false)
{

}

Gamepad::Button::Button(const Button& button)
{
}

Gamepad::Button::~Button()
{
}

Gamepad::Joystick::Joystick()
    : _direction(Vector2::zero()), _deltaX(0.0f), _deltaY(0.0f),
    _north(Input::KEY_NONE), _south(Input::KEY_NONE), _east(Input::KEY_NONE), _west(Input::KEY_NONE), _radius(0.0f), _enabledMovement(false),
    _boundsInner(Vector4::zero()),
    _defaultTextureInner(false), _defaultTexCoordInner(Vector4::zero()),
    _focusTextureInner(false), _focusTexCoordInner(Vector4::zero()),
    _boundsOuter(Vector4::zero()),
    _defaultTextureOuter(false), _defaultTexCoordOuter(Vector4::zero()), 
    _focusTextureOuter(false), _focusTexCoordOuter(Vector4::zero())
{

}

Gamepad::Joystick::Joystick(const Joystick& joystick)
{
}

Gamepad::Joystick::~Joystick()
{
}

Gamepad::ButtonState Gamepad::getButtonState(unsigned int index) const
{
    assert(index < MAX_BUTTONS);

    return _buttons[index]._pressed;
}

Input::Key Gamepad::getButtonKeyModifier(unsigned int index) const
{
    assert(index < MAX_BUTTONS);

    return _buttons[index]._key;
}

void Gamepad::setButtonKeyModifier(unsigned int index, Input::Key key)
{
    assert(index < MAX_BUTTONS);

    _buttons[index]._key = key;
}

const Vector2& Gamepad::getJoystickState(unsigned int index) const
{
    assert(index < MAX_JOYSTICKS);

    return _joysticks[index]._direction;
}

Input::Key Gamepad::getJoystickKeyModifier(unsigned int index, JoystickOrientation orientation) const
{
    assert(index < MAX_JOYSTICKS);
    
    Input::Key key;
    
    switch(orientation)
    {
    case NORTH:
        key = _joysticks[index]._north;
        break;
    case SOUTH:
        key = _joysticks[index]._south;
        break;
    case EAST:
        key = _joysticks[index]._east;
        break;
    case WEST:
        key = _joysticks[index]._west;
        break;
    }
    
    return key;
}

void Gamepad::setJoystickKeyModifier(unsigned int index, JoystickOrientation orientation, Input::Key key)
{
    assert(index < MAX_JOYSTICKS);

    switch(orientation)
    {
    case NORTH:
        _joysticks[index]._north = key;
        break;
    case SOUTH:
        _joysticks[index]._south  = key;
        break;
    case EAST:
        _joysticks[index]._east = key;
        break;
    case WEST:
        _joysticks[index]._west = key;
        break;
    }
}

bool Gamepad::loadButtons(Gamepad* gamepad, Properties* properties)
{
    properties->rewind();

    Properties* buttons = NULL;
    while ((buttons = properties->getNextNamespace()))
    {
        buttons->rewind();

        // Create and load a button.
        if (strcmp(buttons->getNamespace(), "button") == 0)
        {
            Button* button = new Button();
            
            const char* name = NULL;
            while (name = buttons->getNextProperty())
            {
                float u1 = 0.0f, v1 = 0.0f , u2 = 0.0f, v2 = 0.0f;

                if (strcmp(name, "x") == 0)
                {
                    button->_position.x = buttons->getFloat();
                }
                else if (strcmp(name, "y") == 0)
                {
                    button->_position.y = buttons->getFloat();
                }
                if (strcmp(name, "width") == 0)
                {
                    button->_width = buttons->getFloat();
                }
                else if (strcmp(name, "height") == 0)
                {
                    button->_height = buttons->getFloat();
                }
                else if (strcmp(name, "default") == 0)
                {
                    Vector2 out = Vector2(0.0, 0.0);
                    buttons->getVector2(NULL, &out);
                    button->_defaultTexCoord.x = out.x; // u1
                    button->_defaultTexCoord.y = out.y; // v1
                    button->_defaultTexture = true;
                }
                else if (strcmp(name, "focus") == 0)
                {
                    Vector2 out = Vector2(0.0, 0.0);
                    buttons->getVector2(NULL, &out); 
                    button->_focusTexCoord.x = out.x; // u1
                    button->_focusTexCoord.y = out.y; // v1
                    button->_focusTexture = true;
                }
                else if (strcmp(name, "key-modifier") == 0)
                {
                    button->_key = (Input::Key)buttons->getInt();
                }
            }

            // if (u1, v1) of default and focus button texture is specified in the gamepad file then
            // calculate the respective (u2, v2).
            if (button->_defaultTexture)
            {
                button->_defaultTexCoord.x = (button->_defaultTexCoord.x / gamepad->_texture->getWidth() );
                button->_defaultTexCoord.y = 1.0f - (button->_defaultTexCoord.y / gamepad->_texture->getHeight());
                button->_defaultTexCoord.z = button->_defaultTexCoord.x + (button->_width / gamepad->_texture->getWidth()); // u2
                button->_defaultTexCoord.w = button->_defaultTexCoord.y - (button->_width / gamepad->_texture->getHeight()); // v2
            }

            if (button->_focusTexture)
            {
                button->_focusTexCoord.x = (button->_focusTexCoord.x / gamepad->_texture->getWidth() );
                button->_focusTexCoord.y = 1.0f - (button->_focusTexCoord.y / gamepad->_texture->getHeight());
                button->_focusTexCoord.z = button->_focusTexCoord.x + (button->_width / gamepad->_texture->getWidth()); // u2
                button->_focusTexCoord.w = button->_focusTexCoord.y - (button->_width / gamepad->_texture->getWidth()); // v2
            }

            gamepad->addButton(button);
        }
    }

    return true;
}

bool Gamepad::loadJoysticks(Gamepad* gamepad, Properties* properties)
{
    properties->rewind();

    Properties* joysticks = NULL;
    while ((joysticks = properties->getNextNamespace()))
    {
        joysticks->rewind();
        Vector4 defaultTexCoordInner = Vector4::zero();
        Vector4 defaultTexCoordOuter = Vector4::zero();

        // Create and load a button.
        if (strcmp(joysticks->getNamespace(), "joystick") == 0)
        {
            Joystick* joystick = new Joystick();
            
            const char* name = NULL;
            while (name = joysticks->getNextProperty())
            {
                float u1 = 0.0f, v1 = 0.0f , u2 = 0.0f, v2 = 0.0f;

                if (strcmp(name, "bounds-outer") == 0)
                {
                    joysticks->getVector4(NULL, &joystick->_boundsOuter);
                }
                else if (strcmp(name, "bounds-inner") == 0)
                {
                    joysticks->getVector4(NULL, &joystick->_boundsInner);
                }
                else if (strcmp(name, "default-inner") == 0)
                {
                    joysticks->getVector4(NULL, &defaultTexCoordInner);
                    joystick->_defaultTextureInner = true;
                }
                else if (strcmp(name, "focus-inner") == 0)
                {
                    // TODO.
                }
                else if (strcmp(name, "default-outer") == 0)
                {
                    joysticks->getVector4(NULL, &defaultTexCoordOuter);
                    joystick->_defaultTextureOuter = true;
                }
                else if (strcmp(name, "focus-outer") == 0)
                {
                    // TODO.
                }
                else if (strcmp(name, "key-modifier") == 0)
                {
                    // TODO.
                }
                else if (strcmp(name, "radius") == 0)
                {
                    joystick->_radius = joysticks->getFloat(NULL);
                }
            }

            // if (u1, v1) of default and focus button texture is specified in the gamepad file then
            // calculate the respective (u2, v2).
            if (joystick->_defaultTextureInner)
            {
                joystick->_defaultTexCoordInner.x = (defaultTexCoordInner.x / gamepad->_texture->getWidth());
                joystick->_defaultTexCoordInner.y = 1.0f - (defaultTexCoordInner.y / gamepad->_texture->getHeight());
                joystick->_defaultTexCoordInner.z = joystick->_defaultTexCoordInner.x + (defaultTexCoordInner.z / gamepad->_texture->getWidth()); // u2
                joystick->_defaultTexCoordInner.w = joystick->_defaultTexCoordInner.y - (defaultTexCoordInner.w / gamepad->_texture->getHeight()); // v2
            }

            if (joystick->_defaultTextureOuter)
            {
                joystick->_defaultTexCoordOuter.x = (defaultTexCoordOuter.x / gamepad->_texture->getWidth());
                joystick->_defaultTexCoordOuter.y = 1.0f - (defaultTexCoordOuter.y / gamepad->_texture->getHeight());
                joystick->_defaultTexCoordOuter.z = joystick->_defaultTexCoordOuter.x + (defaultTexCoordOuter.z / gamepad->_texture->getWidth()); // u2
                joystick->_defaultTexCoordOuter.w = joystick->_defaultTexCoordOuter.y - (defaultTexCoordOuter.w / gamepad->_texture->getHeight()); // v2
            }

            gamepad->addJoystick(joystick);
        }
    }

    return true;
}

void Gamepad::setSpriteBatch(SpriteBatch* spriteBatch)
{
    _spriteBatch = spriteBatch;
}

Gamepad* Gamepad::create(const char* gamepadPath)
{
    assert (gamepadPath);

    // Load the gamepad properties from file
    Properties* properties = Properties::create(gamepadPath);
    assert(properties);
    if (properties == NULL)
    {
        return NULL;
    }

    Properties* gamepadProperties = properties->getNextNamespace();
    assert (gamepadProperties);
    if (!gamepadProperties || !(strcmp(gamepadProperties->getNamespace(), "gamepad") == 0))
    {
        SAFE_DELETE(properties);
        return NULL;
    }

    // Load the gamepad texture.
    const char* texturePath = gamepadProperties->getString("texture");
    if (strlen(texturePath) == 0)
    {
        LOG_ERROR_VARG("Error loading Gamepad: No valid texture path specified, in %s", gamepadPath);
        return NULL;
    }

    Gamepad* gamepad = new Gamepad();

    gamepad->_texture = Texture::create(texturePath, true);
    assert(gamepad->_texture);

    // Load gamepad components.
    Properties* componentProperties = NULL;
    while ((componentProperties = gamepadProperties->getNextNamespace()))
    {
        if (strcmp(componentProperties->getNamespace(), "buttons") == 0)
        {
            if (!loadButtons(gamepad, componentProperties))
            {
                SAFE_RELEASE(gamepad);
                SAFE_DELETE(properties);
                return NULL;
            }
            
        }

        else if (strcmp(componentProperties->getNamespace(), "joysticks") == 0)
        {
            if(!loadJoysticks(gamepad, componentProperties))
            {
                SAFE_RELEASE(gamepad);
                SAFE_DELETE(properties);
                return NULL;
            }
        }
    }
    
    // Create a sprite batch.
    // TODO: determine the "actual" number of sprites required when parsing the .gamepad file.
    SpriteBatch* spriteBatch = SpriteBatch::create(gamepad->_texture, 0, MAX_BUTTONS + MAX_JOYSTICKS);
    gamepad->_spriteBatch = spriteBatch;

    return gamepad;
}

void Gamepad::update(long elapsedTime)
{


}

void Gamepad::draw()
{
    _spriteBatch->begin();
    
    // Draw buttons.
    for (int i = 0; i < _buttonCount; ++i)
    {
        if ((_buttons[i]._pressed == BUTTON_PRESSED) && (_buttons[i]._focusTexture))
        {
            _spriteBatch->draw(_buttons[i]._position.x, _buttons[i]._position.y,  _buttons[i]._width,  _buttons[i]._height, 
                           _buttons[i]._focusTexCoord.x, _buttons[i]._focusTexCoord.y, _buttons[i]._focusTexCoord.z, _buttons[i]._focusTexCoord.w, 
                           Vector4::one());
            continue;
        }
        else
        {
            _spriteBatch->draw(_buttons[i]._position.x, _buttons[i]._position.y,  _buttons[i]._width,  _buttons[i]._height, 
                           _buttons[i]._defaultTexCoord.x, _buttons[i]._defaultTexCoord.y, _buttons[i]._defaultTexCoord.z, _buttons[i]._defaultTexCoord.w, 
                           Vector4::one());
        }
    }

    // Draw joysticks.
    for (int i = 0; i < _joystickCount; ++i)
    {
        // Draw Outer joggle.
        float x = _joysticks[i]._boundsOuter.x;
        float y = _joysticks[i]._boundsOuter.y;
        float width = _joysticks[i]._boundsOuter.z;
        float height = _joysticks[i]._boundsOuter.w;
        float radius = _joysticks[i]._radius;
        float outerJoggleRadius = _joysticks[i]._boundsOuter.z / 2.0f;

        x = x - (width / 2.0f);
        y = y - (height / 2.0f);
        
        float u1 = _joysticks[i]._defaultTexCoordOuter.x;
        float v1 = _joysticks[i]._defaultTexCoordOuter.y;
        float u2 = _joysticks[i]._defaultTexCoordOuter.z;
        float v2 = _joysticks[i]._defaultTexCoordOuter.w;

        _spriteBatch->draw(x, y, width,  height, u1, v1, u2, v2, Vector4::one());

        // Draw Inner joggle.
        x = _joysticks[i]._boundsInner.x;
        y = _joysticks[i]._boundsInner.y;
        width = _joysticks[i]._boundsInner.z;
        height = _joysticks[i]._boundsInner.w;

        // Move position to reflect displacement.
        if (((_joysticks[i]._deltaX * _joysticks[i]._deltaX) + 
             (_joysticks[i]._deltaY * _joysticks[i]._deltaY)) <= 
            (outerJoggleRadius * outerJoggleRadius))
        {
            x += _joysticks[i]._deltaX;
            y += _joysticks[i]._deltaY;
        }
        else
        {
            Vector2 dir = Vector2(_joysticks[i]._deltaX, _joysticks[i]._deltaY);
            dir.normalize();
            dir.scale(outerJoggleRadius);
            x += dir.x;
            y += dir.y;
        }
        
        // Adjust inner joggle (x, y) position to point to top right.
        x = x - (width / 2.0f);
        y = y - (height / 2.0f);

        u1 = _joysticks[i]._defaultTexCoordInner.x;
        v1 = _joysticks[i]._defaultTexCoordInner.y;
        u2 = _joysticks[i]._defaultTexCoordInner.z;
        v2 = _joysticks[i]._defaultTexCoordInner.w;
        
        _spriteBatch->draw(x, y, width, height, u1, v1, u2, v2, Vector4::one());
    }

    _spriteBatch->end();
}

void Gamepad::keyPress(int key, int keyEvent)
{

}

void Gamepad::touch(int x, int y, int touchEvent)
{
    for (int i = 0; i < _buttonCount; ++i)
    {
        if ((x >= _buttons[i]._position.x) && (x <= (_buttons[i]._position.x + _buttons[i]._width)) &&
            (y >= _buttons[i]._position.y) && (y <= (_buttons[i]._position.y + _buttons[i]._height)))
        {
            switch(touchEvent)
            {
            case Input::TOUCHEVENT_PRESS:
            {
                _buttons[i]._pressed = BUTTON_PRESSED;
            }
            break;
            case Input::TOUCHEVENT_RELEASE:
            {
                _buttons[i]._pressed = BUTTON_RELEASED;
            }
            break;
            }
        }
    }

    for (int i = 0; i < _joystickCount; ++i)
    {
        switch(touchEvent)
        {
        case Input::TOUCHEVENT_PRESS:
        {
            float bx = _joysticks[i]._boundsOuter.x;
            float by = _joysticks[i]._boundsOuter.y;
            float bwidth = _joysticks[i]._boundsOuter.z;
            float bheight = _joysticks[i]._boundsOuter.w;

            if ((x >= (bx - (bwidth / 2.0f))) && (x <= (bx + (bwidth / 2.0f))) &&
                (y >= (by - (bheight / 2.0f))) && (y <= (by + (bheight / 2.0f))))
            {
                _joysticks[i]._enabledMovement = true;
                _joysticks[i]._deltaX = 0.0f;
                _joysticks[i]._deltaY = 0.0f;
                _joysticks[i]._direction.zero();
            }
        }
        case Input::TOUCHEVENT_MOVE:
        {
            if (_joysticks[i]._enabledMovement)
            {
                _joysticks[i]._deltaX = x - _joysticks[i]._boundsOuter.x;
                _joysticks[i]._deltaY = y - _joysticks[i]._boundsOuter.y;
                
                if (((_joysticks[i]._deltaX * _joysticks[i]._deltaX) +
                     (_joysticks[i]._deltaY * _joysticks[i]._deltaY)) <=
                    ((_joysticks[i]._radius * _joysticks[i]._radius)))
                {
                    _joysticks[i]._direction.x = _joysticks[i]._deltaX;
                    _joysticks[i]._direction.y = _joysticks[i]._deltaY;
                    //_joysticks[i]._direction.normalize();
                    _joysticks[i]._direction.scale(1.0f /  _joysticks[i]._radius);
                }
                else
                {
                    if (_joysticks[i]._deltaX > _joysticks[i]._radius)
                    {
                        _joysticks[i]._direction.x = _joysticks[i]._radius;
                    }
                    if (_joysticks[i]._deltaX < -_joysticks[i]._radius)
                    {
                        _joysticks[i]._direction.x = -_joysticks[i]._radius;
                    }
                    if (_joysticks[i]._deltaY > _joysticks[i]._radius)
                    {
                        _joysticks[i]._direction.y = _joysticks[i]._radius;
                    }
                    if (_joysticks[i]._deltaY < -_joysticks[i]._radius)
                    {
                        _joysticks[i]._direction.y = -_joysticks[i]._radius;
                    }

                    _joysticks[i]._direction.scale(1.0f /  _joysticks[i]._radius);
                    //_joysticks[i]._direction.scale(_joysticks[i]._radius);
                    //_joysticks[i]._direction.normalize();
                }
                
                // _joysticks[i]._direction.normalize();
                // _joysticks[i]._direction.scale(1.0f / _joysticks[i]._radius);
            }
        }
        break;
        case Input::TOUCHEVENT_RELEASE:
        {
            if (_joysticks[i]._enabledMovement)
            {
                _joysticks[i]._deltaX = 0.0f;
                _joysticks[i]._deltaY = 0.0f;
                _joysticks[i]._direction.x = 0.0f;
                _joysticks[i]._direction.y = 0.0f;
                _joysticks[i]._enabledMovement = false;
            }
        }
        break;
        }
    }
}

void Gamepad::setCurrentGamepad(Gamepad* gamepad)
{
    __gamepad = gamepad;
}

Gamepad* Gamepad::getCurrentGamepad()
{
    return __gamepad;
}

void Gamepad::addButton(Button* button)
{
    if (_buttonCount == MAX_BUTTONS)
    {
        return;
    }
    
    _buttons[_buttonCount] = *button;
    _buttonCount++;
}

void Gamepad::addJoystick(Joystick* joystick)
{
    if (_joystickCount == MAX_JOYSTICKS)
    {
        return;    
    }

    _joysticks[_joystickCount] = *joystick;
    _joystickCount++;
}

}