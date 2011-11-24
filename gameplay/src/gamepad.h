/*
 * Gamepad.h
 */

#ifndef GAMEPAD_H_
#define GAMEPAD_H_

#include "Vector3.h"
#include "Input.h"
#include "Properties.h"
#include "Ref.h"
#include "SpriteBatch.h"

namespace gameplay
{

/**
 * Defines a class for accessing gamepad controls: Buttons and Joysticks.
 */
class Gamepad: public Ref
{
public:

    enum ButtonState
    {
        BUTTON_PRESSED,
        BUTTON_RELEASED
    };

    enum JoystickOrientation
    {
        NORTH,
        SOUTH,
        EAST,
        WEST
    };

    /**
     * Represents the maximum number of buttons supported on a gamepad.
     */
    static const int MAX_BUTTONS = 32;

    /**
     * Represents the maximum number of joysticks supported on a gamepad.
     */
    static const int MAX_JOYSTICKS = 2;

    /**
     * Returns a button state.
     *
     * @param index Index of the button in the list of buttons specified in the gamepad file.
     * @return BUTTON_PRESSED if the button is pressed; BUTTON_RELEASED otherwise.
     */
    ButtonState getButtonState(unsigned int index) const;

    /**
     * Returns an input key mapped to this button.
     *
     * @param index Index of a button in the list of buttons specified in the gamepad file.
     * @return input key mapped to this button.
     */
    Input::Key getButtonKeyModifier(unsigned int index) const;

    /**
     * Sets an input key mapping for this button.
     *
     * @param index Index of a button in the list of buttons specified in the gamepad file.
     * @return input key mapped to this button.
     */
    void setButtonKeyModifier(unsigned int index, Input::Key key);

    /**
     * Returns a joystick state in the specified direction vector.
     *
     * @param index index of the joystick in the list of joysticks specified in the gamepad file.
     * @return direction of the joystick movement as Vector3.
     */
    const Vector2& getJoystickState(unsigned int index) const;

    /**
     * Returns the key mapped to a joystick orientation.
     *
     * @param index Index of the joystick in the list of joysticks specified in the gamepad file.
     * @param orientation One of the 4 supported Joystick orientations.
     * @return Key associated with a specified joystick orientation.
     */
    Input::Key getJoystickKeyModifier(unsigned int index, JoystickOrientation orientation) const;

    /**
     * Sets a key mapping for a joystick orientation.
     *
     * @param index Index of the joystick in the list of joysticks specified in the gamepad file.
     * @param orientation One of the 4 supported Joystick orientations.
     * @param key associated with a specified joystick orientation.
     */
    void setJoystickKeyModifier(unsigned int index, JoystickOrientation orientation, Input::Key key);

    /**
     * Creates an gamepad from an input gamepad file. 
     * 
     * @param gamepadPath Path of the gamepad file.
     * @return instance of a Gamepad.
     */
    static Gamepad* create(const char* gamepadPath);

    /**
     * Sets a gamepad current.
     *
     * @param gamepad Instance of a Gamepad.
     */
    static void setCurrentGamepad(Gamepad* gamepad);

    /**
     * Returns the current Gamepad.
     *
     * @return currently set gamepad.
     */
    static Gamepad* getCurrentGamepad();
    
    /**
     * Updates the gamepad.
     *
     * @param elapsedTime The amount of time that has passed since the last call to update(), in milliseconds.
     */
    void update(long elapsedTime);

    /**
     * Draws the gamepad.
     */
    void draw();

    /**
     * Input callback on keyPress events.
     *
     * @param key The key code pressed.
     * @param keyEvent The key event that occured.
     * 
     * @see Input::Key
     * @see Input::KeyEvent
     */
    void keyPress(int key, int keyEvent);

    /**
     * Input callback on touch events.
     *
     * @param x The x position of the touch.
     * @param y The y position of the touch.
     * @param touchEvent The touch event that occurred.
     * 
     * @see Input::TouchEvent
     */
    void touch(int x, int y, int touchEvent);


private:

    /**
     * Constructor.
     */
    Gamepad();

    /**
     * Hidden copy constructor.
     */
    Gamepad(const Gamepad* gamepad);

    /**
     * Destructor.
     */
    ~Gamepad();

    /**
     * Sets a spritebatch to this gamepad.
     */
    void setSpriteBatch(SpriteBatch* spriteBatch);

    /**
     * Load the gamepad buttons specified in the properties.
     */
    static bool loadButtons(Gamepad* gamepad, Properties* properties);

    /**
     * Load the gamepad joysticks specified in the properties.
     */
    static bool loadJoysticks(Gamepad* gamepad, Properties* properties);

    /** 
     * Defines a gamepad button.
     */
    class Button
    {
    public:
       /**
        * Constructor.
        */
        Button();

       /**
        * Copy constructor.
        */
        Button(const Button& button);

        /**
         * Destructor.
         */
        ~Button();
        
        Gamepad::ButtonState _pressed;
        Input::Key _key;
        Vector2 _position;
        Vector4 _defaultTexCoord;
        bool _defaultTexture;
        Vector4 _focusTexCoord;
        bool _focusTexture;
        float _height;
        float _width;
    };

    /**
     * Defines a gamepad joystick.
     */
    class Joystick
    {
    public:
       /**
        * Constructor.
        */
        Joystick();

       /**
        * Copy constructor.
        */
        Joystick(const Joystick& joystick);

        /**
         * Destructor.
         */
        ~Joystick();
        
        bool _enabledMovement;
        float _deltaX;
        float _deltaY;
        Vector2 _direction;
        float _radius;
        
        Vector4 _boundsInner;
        bool _defaultTextureInner;
        bool _focusTextureInner;
        Vector4 _defaultTexCoordInner;
        Vector4 _focusTexCoordInner;

        Vector4 _boundsOuter;
        bool _defaultTextureOuter;
        bool _focusTextureOuter;
        Vector4 _defaultTexCoordOuter;
        Vector4 _focusTexCoordOuter;
        
        Input::Key _north;
        Input::Key _south;
        Input::Key _east;
        Input::Key _west;
    };

    /**
     * Adds a button to the Gamepad.
     */
    void addButton(Button* button);

    /**
     * Adds a joystick to the Gamepad.
     */
    void addJoystick(Joystick* joystick);

    Button _buttons[MAX_BUTTONS];
    int _buttonCount;
    Joystick _joysticks[MAX_JOYSTICKS];
    int _joystickCount;
    Texture* _texture;
    SpriteBatch* _spriteBatch;
};

}

#endif