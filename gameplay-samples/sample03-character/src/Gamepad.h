#ifndef GAMEPAD_H_
#define GAMEPAD_H_

#include "Keyboard.h"
#include "Properties.h"
#include "SpriteBatch.h"
#include "Touch.h"
#include "Vector2.h"

using namespace gameplay;

/**
 * Defines a class for accessing gamepad controls: Buttons and Joysticks.
 */
class Gamepad
{
public:

    enum ButtonState
    {
        BUTTON_PRESSED,
        BUTTON_RELEASED
    };

    typedef struct texCoord
    {
        float u1;
        float v1;
        float u2;
        float v2;
    } TexCoord;

    typedef struct rect
    {
        float x;
        float y;
        float width;
        float height;
    } Rect;

    /**
     * Represents the maximum number of buttons supported on a gamepad.
     */
    static const unsigned int MAX_BUTTONS = 16;

    /**
     * Represents the maximum number of joysticks supported on a gamepad.
     */
    static const unsigned int MAX_JOYSTICKS = 2;

    /**
     * Represents the maximum number of touch inputs the gamepad supports.
     */
    static const unsigned int MAX_TOUCH_INPUTS = 4;

    /**
     * Constructor.
     *
     * @param texturePath The path to the gamepad's texture atlas.
     * @param joysticks The number of joysticks the game pad will have.
     * @param buttons The number of buttons the game pad will have.
     */
    Gamepad(const char* texturePath, unsigned int joysticks, unsigned int buttons);

    /**
     * Destructor.
     */
    ~Gamepad();

    /**
     * Creates and sets a button to the gamepad with the specified information.
     *
     * @param buttonId unique integer button id between (0 - MAX_BUTTONS).
     * @param region screen region where the button must be drawn.
     * @param defaultTextureRegion region in the gamepad's texture atlas that represents default mode.
     * @param focusTextureRegion region in the gamepad's texture atlas that represents focus mode.
     */
    void setButton(unsigned int buttonId, Rect* region, Rect* defaultTextureRegion, Rect* focusTextureRegion);

    /**
     * Returns a button state.
     *
     * @param buttonId Id of the button in the gamepad.
     * @return BUTTON_PRESSED if the button is pressed; BUTTON_RELEASED otherwise.
     */
    ButtonState getButtonState(unsigned int buttonId) const;

    /**
     * Creates and sets a joystick to the gamepad with the specified information.
     * @param joystickId unique integer joystick id between (0 - MAX_JOYSTICKS).
     * @param regionInner region where the inner joggle must be drawn.
     * @param textureRegionInner region in the gamepad's texture atlas that represents inner joggle.
     * @param regionOuter region where the outer joggle must be drawn.
     * @param textureRegionOuter region in the gamepad's texture atlas that represents outer joggle.
     */ 
    void setJoystick(unsigned int joystickId, Rect* regionInner, Rect* textureRegionInner, Rect* regionOuter, Rect* textureRegionOuter, float radius);

    /**
     * Gets whether the given joystick is active or not.
     * 
     * @param joystickId Id of the joystick.
     * @return Whether the given joystick is active or not.
     */
    bool isJoystickActive(unsigned int joystickId) const;

    /**
     * Returns a joystick state in the specified direction vector.
     *
     * @param joystickId Id of the joystick.
     * @return direction of the joystick movement as Vector3.
     */
    const Vector2& getJoystickState(unsigned int joystickId) const;

    /**
     * Draws the gamepad.
     */
    void draw(const Vector4& color = Vector4::one());

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
    void touch(int x, int y, int touchEvent, unsigned int contactIndex);

private:

    /**
     * Hidden copy constructor.
     */
    Gamepad(const Gamepad* gamepad);
    
    /**
     * Sets a spritebatch to this gamepad.
     */
    void setSpriteBatch(SpriteBatch* spriteBatch);

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
        Rect _region;
        bool _defaultTextureEnabled;
        TexCoord _defaultTexCoord;
        bool _focusTextureEnabled;
        TexCoord _focusTexCoord;
        unsigned int _contactIndex;
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
        
        Vector2 _displacement;
        Vector2 _direction;
        float _radius;
        
        Rect _regionInner;
        bool _defaultTextureInnerEnabled;
        TexCoord _defaultTexCoordInner;
        
        Rect _regionOuter;
        bool _defaultTextureOuterEnabled;
        TexCoord _defaultTexCoordOuter;

        unsigned int _contactIndex;
    };

    unsigned int _joystickCount;
    unsigned int _buttonCount;
    Button** _buttons;
    Joystick** _joysticks;
    Texture* _texture;
    SpriteBatch* _spriteBatch;
    int _touches[MAX_TOUCH_INPUTS];
};

#endif
