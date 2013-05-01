#ifndef GAMEPAD_H_
#define GAMEPAD_H_

#include "Vector2.h"

namespace gameplay
{

class Button;
class Container;
class Form;
class Joystick;
class Platform;

/**
 * Defines an interface for handling gamepad input.
 */
class Gamepad
{
    friend class Platform;
    friend class Game;
    friend class Button;

public:

    /**
     *  Gamepad events.
     */
    enum GamepadEvent
    {
        CONNECTED_EVENT,
        DISCONNECTED_EVENT,
        BUTTON_EVENT,
        JOYSTICK_EVENT,
        TRIGGER_EVENT
    };

    /**
     * Gamepad buttons.
     */
    enum ButtonMapping
    {
        BUTTON_A,
        BUTTON_B,
        BUTTON_C,
        BUTTON_X,
        BUTTON_Y,
        BUTTON_Z,
        BUTTON_MENU1,
        BUTTON_MENU2,
        BUTTON_MENU3,
        BUTTON_MENU4,
        BUTTON_L1,
        BUTTON_L2,
        BUTTON_L3,
        BUTTON_R1,
        BUTTON_R2,
        BUTTON_R3,
        BUTTON_UP,
        BUTTON_DOWN,
        BUTTON_LEFT,
        BUTTON_RIGHT
    };

    /**
     * Gets the number of buttons on this gamepad.
     *
     * @return The number of buttons on this gamepad.
     */
    unsigned int getButtonCount() const;

    /** 
     * Gets whether the given button is currently pressed down.
     *
     * @param button The enum of the button on the gamepad to get the state for.
     * @return Whether the button is currently pressed or not.
     */
    bool isButtonDown(ButtonMapping button) const;
    
    /**
     * Gets the number of joysticks on the gamepad.
     *
     * @return the number of joysticks on the gamepad.
     */
    unsigned int getJoystickCount() const;

    /**
     * Returns the specified joystick's value as a Vector2.
     *
     * @param joystickId The index of the joystick to get the value for.
     * @param outValues The current x-axis and y-axis values of the joystick.
     */
    void getJoystickValues(unsigned int joystickId, Vector2* outValues) const;

    /**
     * Returns the number of analog triggers (as opposed to digital shoulder buttons)
     * on this gamepad.
     *
     * @return The number of analog triggers on this gamepad.
     */
    unsigned int getTriggerCount() const;

    /**
     * Returns the value of an analog trigger on this gamepad.  This value will be a
     * number between 0 and 1, where 0 means the trigger is in its resting (unpressed)
     * state and 1 means the trigger has been completely pressed down.
     *
     * @param triggerId The trigger to query.
     * @return The value of the given trigger.
     */
    float getTriggerValue(unsigned int triggerId) const;

   /**
     * Get this gamepad's vendor ID.
     *
     * @return This gamepad's vendor ID.
     */
    const unsigned int getVendorId() const;

    /**
     * Get this gamepad's product ID.
     *
     * @return This gamepad's product ID.
     */
    const unsigned int getProductId() const;

    /**
     * Get this gamepad's vendor name.
     *
     * @return This gamepad's vendor name.
     */
    const char* getVendorString() const;

    /**
     * Get this gamepad's product name.
     *
     * @return This gamepad's product name.
     */
    const char* getProductString() const;

    /**
     * Returns whether the gamepad is currently represented with a UI form or not.
     *
     * @return true if the gamepad is currently represented by a UI form; false if the gamepad is
     *         not represented by a UI form.
     */
    bool isVirtual() const;

    /**
     * Gets the Form used to represent this gamepad.
     *
     * @return the Form used to represent this gamepad. NULL if the gamepad is not represented with a Form.
     */
    Form* getForm() const;

    /**
     * Updates the gamepad's state.  For a virtual gamepad, this results in calling update()
     * on the gamepad's form.  For physical gamepads, this polls the gamepad's state
     * at the platform level.  Either way, this should be called once a frame before
     * getting and using a gamepad's current state.
     *
     * @param elapsedTime The elapsed game time.
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
     * @param formPath The path the the .form file.
     */ 
    Gamepad(const char* formPath);

    /**
     * Constructs a physical gamepad.
     *
     * @param handle The gamepad handle
     * @param buttonCount the number of buttons on the gamepad. 
     * @param joystickCount the number of joysticks on the gamepad.
     * @param triggerCount the number of triggers on the gamepad.
     * @param vendorId The vendor id
     * @param productId The product id
     * @param vendorString The vendor string/name.
     * @param productString The product string/name.
     */
    Gamepad(GamepadHandle handle, 
            unsigned int buttonCount, unsigned int joystickCount, unsigned int triggerCount,
            unsigned int vendorId, unsigned int productId, const char* vendorString, const char* productString);

    /**
     * Copy constructor.
     */
    Gamepad(const Gamepad& copy);

    /** 
     * Destructor.
     */
    virtual ~Gamepad();

    static void updateInternal(float elapsedTime);

    static Gamepad* add(GamepadHandle handle, 
                        unsigned int buttonCount, unsigned int joystickCount, unsigned int triggerCount,
                        unsigned int vendorId, unsigned int productId, const char* vendorString, const char* productString);

    static Gamepad* add(const char* formPath);

    static void remove(GamepadHandle handle);

    static void remove(Gamepad* gamepad);

    static unsigned int getGamepadCount();

    static Gamepad* getGamepad(unsigned int index, bool preferPhysical = true);

    static Gamepad* getGamepad(GamepadHandle handle);

    static ButtonMapping getButtonMappingFromString(const char* string);

    void setButtons(unsigned int buttons);

    void setJoystickValue(unsigned int index, float x, float y);

    void setTriggerValue(unsigned int index, float value);
    
    void bindGamepadControls(Container* container);

    GamepadHandle _handle;        // The handle of the Gamepad.
    unsigned int _buttonCount;    // Number of buttons.
    unsigned int _joystickCount;  // Number of joysticks.
    unsigned int _triggerCount;   // Number of triggers.
    unsigned int _vendorId;
    unsigned int _productId;
    std::string _vendorString;
    std::string _productString;
    Form* _form;
    Joystick* _uiJoysticks[2];
    Button* _uiButtons[20];
    unsigned int _buttons;
    Vector2 _joysticks[2];
    float _triggers[2];
};

}

#endif
