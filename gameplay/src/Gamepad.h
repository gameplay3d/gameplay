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
    friend class Platform;
    friend class Game;
    friend class Control;

public:

    /**
     *  Gamepad events.
     */
    enum GamepadEvent
    {
        CONNECTED_EVENT,
        DISCONNECTED_EVENT
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
     * @param joystickId The index of the joystick to get the value for.
     * @param outValue The current x-axis and y-asix value displacement of the joystick.
     */
    void getJoystickAxisValues(unsigned int joystickId, Vector2* outValues) const;

    /**
     * Returns the specified joystick's x-axis value.
     *
     * @param joystickId The index of the joystick to get the x-axis value for.
     * @return The current value of the joystick's x-axis value.
     */
    float getJoystickAxisX(unsigned int joystickId) const;
    
    /**
     * Returns the specified joystick's y-axis value.
     * 
     * @param joystickId The index of the joystick to get the y-axis value for.
     * @return The current value of the joystick's y-axis value.
     */
    float getJoystickAxisY(unsigned int joystickId) const;

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
     * Constructs a gamepad from the specified .form file.
     *
     * @param id The gamepad's id.
     * @param formPath The path the the .form file.
     */ 
    Gamepad(unsigned int handle, const char* formPath);

    /**
     * Constructs a physical gamepad.
     *
     * @param id The gamepad's id.
     * @param buttonCount the number of buttons on the gamepad. 
     * @param joystickCount the number of joysticks on the gamepad.
     * @param triggerCount the number of triggers on the gamepad.
     */
    Gamepad(const char* id, unsigned int handle, unsigned int buttonCount, unsigned int joystickCount, unsigned int triggerCount);

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

    /**
     * Gets whether the Gamepad is currently connected to the Platform.
     */
    bool isConnected() const;
        
    std::string _id;              // ID of the Gamepad
    unsigned int _handle;         // The handle of the Gamepad.
    unsigned int _buttonCount;    // Number of buttons.
    unsigned int _joystickCount;  // Number of joysticks.
    unsigned int _triggerCount;   // Number of triggers.
    
    // Data needed for virtual gamepads.
    Form* _gamepadForm;
    std::vector<Joystick*>* _uiJoysticks;
    std::vector<Button*>* _uiButtons;
};

}

#endif
