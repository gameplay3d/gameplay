#ifndef GAMEPAD_H_
#define GAMEPAD_H_

#include "Form.h"
#include "Button.h"

namespace gameplay
{

/**
 * Defines an interface for handling gamepad input.
 */
class Gamepad : public Control::Listener
{
    friend class Game;
    friend class Control;

public:

    /**
     *  Gamepad events.
     */
    enum GamepadEvent
    {
        CONNECTED_EVENT,
        DISCONNECTED_EVENT,
        JOYSTICK_EVENT,
        BUTTON_EVENT
    };

    /**
     * Gamepad button states.
     */
    enum ButtonState
    {
        BUTTON_PRESSED = gameplay::Button::Listener::PRESS, 
        BUTTON_RELEASED = gameplay::Button::Listener::RELEASE
    };
    
    /** 
     * Gets the current state of the specified button.
     *
     * @param buttonId The index of the button on the gamepad to get the state for.
     * @return whether the button is currently pressed or not.
     */
    ButtonState getButtonState(unsigned int buttonId) const;

    /**
     * Gets whether the specified joystick's state is active or not.
     * 
     * @param joystickId The index of the joystick on the gamepad to get state for.
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
     * Returns whether the gamepad is currently represented with a UI form or not.
     *
     * @return true if the gamepad is currently represented by a UI form; false if the gamepad is
     *      not represented by a UI form
     */
    bool isVirtual() const;

    /**
     * Gets the Form used to represent this gamepad.
     *
     * Note: What if the user decides to add gamepad controls (joysticks, buttons) to the gamepad form? How do we handle new/deleted controls?
     *
     * @return the Form used to represent this gamepad. NULL if the gamepad is not reprented with a Form.
     */
    Form* getForm() const;

protected:

    /**
     * @see Control::Listener::controlEvent
     */
    void controlEvent(Control* control, Control::Listener::EventType evt);

private:
    
    struct GamepadData
    {
        enum {JOYSTICK, BUTTON};

        GamepadData(unsigned int controlType, unsigned int controlIndex)
            : _controlType(controlType), _controlIndex(controlIndex) {}

        ~GamepadData();

        unsigned int _controlType;
        unsigned int _controlIndex;
    };

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
