#ifndef GAMEPAD_H_
#define GAMEPAD_H_

#include "Form.h"
#include "Button.h"

namespace gameplay
{

/**
 * Defines an interface for handling gamepad support.
 */
class Gamepad : public Control::Listener
{
    friend class Game;

public:

    enum GamepadEvent
    {
        JOYSTICK_EVENT,
        BUTTON_EVENT
    };

    enum ButtonState
    {
        BUTTON_PRESSED = gameplay::Button::Listener::PRESS, 
        BUTTON_RELEASED = gameplay::Button::Listener::RELEASE
    };
    
    /** 
     * Gets the current state of the specified button.
     */
    ButtonState getButtonState(unsigned int buttonId) const;

    /**
     * Gets whether the specified joystick's state is active or not.
     * 
     * @param joystickId The unique integer ID of the joystick to set.
     * @return Whether the given joystick is active or not.
     */
    bool isJoystickActive(unsigned int joystickId) const;

    /**
     * Returns the specified joystick's value as a Vector2.
     *
     * @param joystickId The unique integer ID of the joystick to set.
     * @return A Vector2 of the joystick displacement for the specified joystick.
     */
    const Vector2& getJoystickValue(unsigned int joystickId) const;

    /** 
     * Listener for Control events on the gamepads Joystick or Buttons.
     */
    void controlEvent(Control* control, Control::Listener::EventType evt);

    /**
     * Returns whether the gamepad is represented with a UI form or not.
     */
    bool isVirtual() const;

private:

    /**
     * Constructor.
     */
    Gamepad();
    
    /**
     * Constructor.
     * Create a gamepad from the specified formPath.
     *
     * @param formPath
     */ 
    Gamepad(const char* formPath);

    /**
     * Copy constructor.
     */
    Gamepad(const Gamepad& copy);

    /** 
     * Destructor.
     */
    virtual ~Gamepad();
    
    /** 
     * Gets the Joystick and Button Control object's from the specified form.
     */
    void getGamepadControls(Form* form);

    /**
     * Updates the gamepad.
     */
    void update();

    /**
     * Renders the gamepad if it is based on a form and if the form is enabled.
     */
    void render();

    int _playerIndex;
    Vector2** _joystickValues;
    unsigned int _joystickCount;
    ButtonState** _buttonStates;
    unsigned int _buttonCount;

    Form* _gamepadForm;
};

}

#endif
