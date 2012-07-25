#ifndef GAMEPAD_H_
#define GAMEPAD_H_

#include "Form.h"
#include "Button.h"
#include "Joystick.h"

namespace gameplay
{

/**
 * Defines an interface for handling gamepad input.
 */
class Gamepad
{
    friend class Game;
    friend class Control;

public:

    /**
     *  Gamepad events.
     */
    enum GamepadEvent
    {
        ATTACHED_EVENT,
        DETACHED_EVENT,
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
     * Gets the Gamepad's ID.
     *
     * @return the Gamepad's ID.
     */
    const char* getId() const;

    /**
     * Gets the number of button on the gamepad.
     *
     * @return the number of buttons on the gamepad.
     */
    unsigned int getButtonCount() const;

    /** 
     * Gets the current state of the specified button.
     *
     * @param buttonId The index of the button on the gamepad to get the state for.
     * @return whether the button is currently pressed or not.
     */
    ButtonState getButtonState(unsigned int buttonId) const;
    
    /**
     * Gets the number of joysticks on the gamepad.
     *
     * @return the number of joysticks on the gamepad.
     */
    unsigned int getJoystickCount() const;

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

    /**
     * Updates the gamepad.
     */
    void update(float elapsedTime);

    /**
     * Draws the gamepad if it is based on a form and if the form is enabled.
     */
    void draw();

private:

    /**
     * Constructor.
     * 
     * @param id The gamepad's id.
     */
    Gamepad(const char* id);
    
    /**
     * Constructs a gamepad from the specified .form file.
     *
     * @param id The gamepad's id.
     * @param formPath The path the the .form file.
     */ 
    Gamepad(const char* id, const char* formPath);

    /**
     * Copy constructor.
     */
    Gamepad(const Gamepad& copy);

    /** 
     * Destructor.
     */
    virtual ~Gamepad();

    /** 
     * Binds the Joystick and Button Control object's from the specified container.
     */
    void bindGamepadControls(Container* container);

    std::string _id;
    std::vector<Joystick*> _joysticks;
    std::vector<Button*> _buttons;
    Form* _gamepadForm;
};

}

#endif
