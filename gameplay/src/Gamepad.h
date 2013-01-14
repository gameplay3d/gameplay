#ifndef GAMEPAD_H_
#define GAMEPAD_H_

#include "Form.h"
#include "Joystick.h"

namespace gameplay
{

class Platform;
class GamepadButton;

/**
 * Defines an interface for handling gamepad input.
 */
class Gamepad
{
    friend class Platform;
    friend class Game;
    friend class GamepadButton;
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
     * Gamepad buttons.
     */
    enum ButtonMapping
    {
        BUTTON_A       = (1 << 0),
        BUTTON_B       = (1 << 1),
        BUTTON_C       = (1 << 2),
        BUTTON_X       = (1 << 3),
        BUTTON_Y       = (1 << 4),
        BUTTON_Z       = (1 << 5),
        BUTTON_MENU1   = (1 << 6),
        BUTTON_MENU2   = (1 << 7),
        BUTTON_MENU3   = (1 << 8),
        BUTTON_MENU4   = (1 << 9),
        BUTTON_L1      = (1 << 10),
        BUTTON_L2      = (1 << 11),
        BUTTON_L3      = (1 << 12),
        BUTTON_R1      = (1 << 13),
        BUTTON_R2      = (1 << 14),
        BUTTON_R3      = (1 << 15),
        BUTTON_UP      = (1 << 16),
        BUTTON_DOWN    = (1 << 17),
        BUTTON_LEFT    = (1 << 18),
        BUTTON_RIGHT   = (1 << 19)
    };

    /**
     * Get all connected gamepads.
     *
     * @return A vector of all connected gamepads.
     */
    static std::vector<Gamepad*>* getGamepads();

    /**
     * Get this gamepad's ID as a string.
     *
     * @return This gamepad's ID as a string.
     */
    const char* getId() const;

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
     * @param id The gamepad's id.
     * @param handle The gamepad handle
     * @param buttonCount the number of buttons on the gamepad. 
     * @param joystickCount the number of joysticks on the gamepad.
     * @param triggerCount the number of triggers on the gamepad.
     */
    Gamepad(const char* id, GamepadHandle handle, unsigned int buttonCount, unsigned int joystickCount, unsigned int triggerCount,
            unsigned int vendorId, unsigned int productId, char* vendorString, char* productString);

    /**
     * Copy constructor.
     */
    Gamepad(const Gamepad& copy);

    /**
     * Used by platforms to add gamepads to the set available to games.
     */
    static Gamepad* add(const char* id, GamepadHandle handle, unsigned int buttonCount, unsigned int joystickCount, unsigned int triggerCount,
                        unsigned int vendorId, unsigned int productId, char* vendorString, char* productString);

    /**
     * Used by Game::loadGamepads() to add virtual gamepads from a game's config file.
     */
    static Gamepad* add(const char* formPath);

    static void remove(GamepadHandle handle);

    static void remove(Gamepad* gamepad);

    static Gamepad* getGamepad(GamepadHandle handle);

    static ButtonMapping getButtonMappingFromString(const char* string);

    /** 
     * Destructor.
     */
    virtual ~Gamepad();
    
    /** 
     * Binds the Joystick and Button Control object's from the specified container.
     */
    void bindGamepadControls(Container* container);

    static unsigned int getIndexFromMapping(Gamepad::ButtonMapping mapping);

    std::string _id;
    GamepadHandle _handle;        // The handle of the Gamepad.
    unsigned int _vendorId;
    unsigned int _productId;
    std::string _vendorString;
    std::string _productString;

    unsigned int _buttonCount;    // Number of buttons.
    unsigned int _joystickCount;  // Number of joysticks.
    unsigned int _triggerCount;   // Number of triggers.
    
    // Data needed for virtual gamepads.
    Form* _form;
    Joystick* _uiJoysticks[2];
    GamepadButton* _uiButtons[20];

    // Current gamepad state.
    unsigned int _buttons;
    Vector2 _joysticks[2];
    float _triggers[2];
};

}

#endif
