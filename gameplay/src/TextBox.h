#ifndef TEXTBOX_H_
#define TEXTBOX_H_

#include "Base.h"
#include "Label.h"
#include "Theme.h"
#include "Keyboard.h"

namespace gameplay
{

/**
 * An editable text label.  Tap or click within the text box to bring up the
 * virtual keyboard.
 *
 * Listeners can listen for a TEXT_CHANGED event, and then query the text box
 * for the last keypress it received.
 *
 * The following properties are available for text boxes:

 @verbatim
    textBox <labelID>
    {
         style       = <styleID>
         alignment   = <Control::Alignment constant> // Note: 'position' will be ignored.
         position    = <x, y>
         autoWidth   = <bool>
         autoHeight  = <bool>
         size        = <width, height>
         width       = <width>   // Can be used in place of 'size', e.g. with 'autoHeight = true'
         height      = <height>  // Can be used in place of 'size', e.g. with 'autoWidth = true'
         text        = <string>
         inputMode   = <TextBox::InputMode constant>
    }
 @endverbatim
 */
class TextBox : public Label
{
    friend class Container;
	friend class ControlFactory;
	
public:

    /**
     * Input modes. Default is Text.
     */
    enum InputMode {
        /**
         * Text: Text is displayed directly.
         */
        TEXT = 0x01,

        /**
         * Password: Text is replaced by _passwordChar, which is '*' by default.
         */
        PASSWORD = 0x02
    };

    /**
     * Create a new text box control.
     *
     * @param id The control's ID.
     * @param style The control's style.
     *
     * @return The new text box.
     * @script{create}
     */
    static TextBox* create(const char* id, Theme::Style* style);

    /**
     * Initialize this textbox.
     */
    virtual void initialize(Theme::Style* style, Properties* properties);

    /**
     * Add a listener to be notified of specific events affecting
     * this control.  Event types can be OR'ed together.
     * E.g. To listen to touch-press and touch-release events,
     * pass <code>Control::Listener::TOUCH | Control::Listener::RELEASE</code>
     * as the second parameter.
     *
     * @param listener The listener to add.
     * @param eventFlags The events to listen for.
     */
    virtual void addListener(Control::Listener* listener, int eventFlags);

    /**
     * Get the last key pressed within this text box.
     *
     * @return The last key pressed within this text box.
     */
    int getLastKeypress();

    /**
     * @see Control::getType
     */
    const char* getType() const;

    /**
     * Set the character displayed in password mode.
     *
     * @param character Character to display in password mode.
     */
    void setPasswordChar(char character);

    /**
     * Get the character displayed in password mode.
     *
     * @return The character displayed in password mode.
     */
    char getPasswordChar() const;

    /**
     * Set the input mode.
     *
     * @param inputMode Input mode to set.
     */
    void setInputMode(InputMode inputMode);

    /**
     * Get the input mode.
     *
     * @return The input mode.
     */
    InputMode getInputMode() const;

protected:

    /**
     * Constructor.
     */
    TextBox();

    /**
     * Destructor.
     */
    ~TextBox();

    /**
     * Create a text box with a given style and properties.
     *
     * @param style The style to apply to this text box.
     * @param properties The properties to set on this text box.
     * @param theme The theme to set on this control if needed
	 *
     * @return The new text box.
     */
    static Control* create(Theme::Style* style, Properties* properties, Theme *theme = NULL);

    /**
     * Touch callback on touch events.  Controls return true if they consume the touch event.
     *
     * @param evt The touch event that occurred.
     * @param x The x position of the touch in pixels. Left edge is zero.
     * @param y The y position of the touch in pixels. Top edge is zero.
     * @param contactIndex The order of occurrence for multiple touch contacts starting at zero.
     *
     * @return Whether the touch event was consumed by the control.
     *
     * @see Touch::TouchEvent
     */
    bool touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

    /**
     * Keyboard callback on key events.
     *
     * @param evt The key event that occurred.
     * @param key If evt is KEY_PRESS or KEY_RELEASE then key is the key code from Keyboard::Key.
     *            If evt is KEY_CHAR then key is the unicode value of the character.
     * 
     * @see Keyboard::KeyEvent
     * @see Keyboard::Key
     */
    bool keyEvent(Keyboard::KeyEvent evt, int key);

    /**
     * Called when a control's properties change.  Updates this control's internal rendering
     * properties, such as its text viewport.
     *
     * @param container This control's parent container.
     * @param offset Positioning offset to add to the control's position.
     */
    void update(const Control* container, const Vector2& offset);

    /**
     * Draw the images associated with this control.
     *
     * @param spriteBatch The sprite batch containing this control's icons.
     * @param clip The clipping rectangle of this control's parent container.
     */
    void drawImages(SpriteBatch* spriteBatch, const Rectangle& clip);

    /**
     * Draw this textbox's text.
     *
     * @param clip The clipping rectangle of this textbox's
     * parent container.
     */
    virtual void drawText(const Rectangle& clip);

    /**
     * Gets an InputMode by string.
     *
     * @param inputMode The string representation of the InputMode type.
     * @return The InputMode enum value corresponding to the given string.
     */
    static InputMode getInputMode(const char* inputMode);

    /**
     * Get the text which should be displayed, depending on
     * _inputMode.
     *
     * @return The text to be displayed.
     */
    std::string getDisplayedText() const;

    /**
     * The current position of the TextBox's caret.
     */
    Vector2 _caretLocation;

    /**
     * The previous position of the TextBox's caret.
     */
    Vector2 _prevCaretLocation;
    
    /**
     * The last character that was entered into the TextBox.
     */
    int _lastKeypress;

    /**
     * The font size to be used in the TextBox.
     */
    unsigned int _fontSize;
    
    /**
     * The Theme::Image for the TextBox's caret.
     */
    Theme::ThemeImage* _caretImage;

    /**
     * The character displayed in password mode.
     */
    char _passwordChar;

    /**
     * The mode used to display the typed text.
     */
    InputMode _inputMode;

    /**
     * Indicate if the CTRL key is currently pressed.
     */
    bool _ctrlPressed;

private:

    /**
     * Constructor.
     */
    TextBox(const TextBox& copy);

    void setCaretLocation(int x, int y);
};

}

#endif
