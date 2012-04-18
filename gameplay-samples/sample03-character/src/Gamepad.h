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

    /**
     * The button state's.
     */
    enum ButtonState
    {
        BUTTON_RELEASED,
        BUTTON_PRESSED
    };

    /**
     * Constructor.
     *
     * @param texturePath The path to the gamepad's texture atlas.
     * @param joysticks The number of joysticks the game pad will have.
     * @param buttons The number of buttons the game pad will have.
     */
    Gamepad(const char* texturePath, unsigned int joystickCount, unsigned int buttonCount);

    /**
     * Destructor.
     */
    ~Gamepad();

    /**
     * Sets the screen region and textures to use for the specified button on the gamepad.
     *
     * @param buttonId The unique integer ID of the button to set.
     * @param screenRegion The region on the screen to draw the button.
     * @param releasedTextureRegion The region on the gamepad's texture atlas to draw as the BUTTON_RELEASED state. 
     * @param pressedTextureRegion The region on the gamepad's texture atlas to draw as the BUTTON_PRESSED state.
     */
    void setButton(unsigned int buttonId, const Rectangle& screenRegion, const Rectangle& releasedTextureRegion, const Rectangle& pressedTextureRegion);

    /**
     * Returns the current state of the specified button.
     *
     * @param buttonId The ID of the button on the gamepad.
     * @return The state of the specified button. BUTTON_PRESSED if the button is pressed; BUTTON_RELEASED otherwise.
     */
    ButtonState getButtonState(unsigned int buttonId) const;

    /**
     * Sets the screen region and textures to use for the specified joystick on the gamepad.
     *
     * @param joystickId The unique integer ID of the joystick to set.
     * @param thumbScreenRegion The region on the screen to draw the thumb stick of the joystick.
     * @param thumbTextureRegion The region on the gamepad's texture atlas to draw as the thumb stick of the joystick.
     * @param dockScreenRegion The region on the screen to draw the dock that the thumb stick sits in.
     * @param dockTextureRegion The region on the gamepad's texture atlas to draw as the dock for the thumb stick of the joystick.
     * @param radius The maximum radius that the thumb stick will revolve around the dock region.
     */ 
    void setJoystick(unsigned int joystickId, const Rectangle& thumbScreenRegion, const Rectangle& thumbTextureRegion, const Rectangle& dockScreenRegion, const Rectangle& dockTextureRegion, float radius);

    /**
     * Gets whether the specified joystick is active or not.
     * 
     * @param joystickId The unique integer ID of the joystick to set.
     * @return Whether the given joystick is active or not.
     */
    bool isJoystickActive(unsigned int joystickId) const;

    /**
     * Returns the specified joystick's state as a Vector2.
     *
     * @param joystickId The unique integer ID of the joystick to set.
     * @return A Vector2 of the joystick displacement for the specified joystick.
     */
    const Vector2& getJoystickState(unsigned int joystickId) const;

    /**
     * Returns the current touch contact index for the specified joystick.
     *
     * @return The contact index for the specified joystick.
     */
    unsigned int getJoystickContactIndex(unsigned int joystickId) const;

    /**
     * Draws the gamepad.
     *
     * @param color 
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
    void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

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
     * Structure for holding texture coordinates for a rectangle.
     */
    struct TexCoords
    {
        float u1;
        float u2;
        float v1;
        float v2;
    };

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
        Rectangle _region;
        bool _defaultTextureEnabled;
        TexCoords _defaultTexCoord;
        bool _focusTextureEnabled;
        TexCoords _focusTexCoord;
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
        Rectangle _regionInner;
        bool _defaultTextureInnerEnabled;
        TexCoords _defaultTexCoordInner;
        Rectangle _regionOuter;
        bool _defaultTextureOuterEnabled;
        TexCoords _defaultTexCoordOuter;
        unsigned int _contactIndex;
    };

    unsigned int _joystickCount;
    unsigned int _buttonCount;
    Button** _buttons;
    Joystick** _joysticks;
    Texture* _texture;
    SpriteBatch* _spriteBatch;
};

#endif
