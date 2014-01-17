#ifndef TEXTBOX_H_
#define TEXTBOX_H_

#include "Base.h"
#include "Label.h"
#include "Theme.h"
#include "Keyboard.h"

namespace gameplay
{

/**
 * Defines a text control. 
 *
 * Listeners can listen for a TEXT_CHANGED event, and then query the text box
 * for the last keypress it received.
 * On mobile device you can tap or click within the text box to
 * bring up the virtual keyboard.
 *
 * @see http://blackberry.github.io/GamePlay/docs/file-formats.html#wiki-UI_Forms
 */
class TextBox : public Label
{
    friend class Container;
    friend class ControlFactory;

public:

    /**
     * Input modes. Default is Text.
     */
    enum InputMode
    {
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
     * Creates a new TextBox.
     *
     * @param id The textbox ID.
     * @param style The textbox style (optional).
     *
     * @return The new textbox.
     * @script{create}
     */
    static TextBox* create(const char* id, Theme::Style* style = NULL);

    /**
     * Returns the current location of the caret with the text of this TextBox.
     *
     * @return The current caret location.
     */
    unsigned int getCaretLocation() const;

    /**
     * Sets the location of the caret within this text box.
     *
     * @param index The new location of the caret within the text of this TextBox.
     */
    void setCaretLocation(unsigned int index);

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
     * @param properties A properties object containing a definition of the text box (optional).
     *
     * @return The new text box.
     */
    static Control* create(Theme::Style* style, Properties* properties = NULL);

    /**
     * @see Control::initialize
     */
    void initialize(const char* typeName, Theme::Style* style, Properties* properties);

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
     * @see Control#controlEvent
     */
    void controlEvent(Control::Listener::EventType evt);

    /**
     * @see Control::updateState
     */
    void updateState(State state);

    /**
     * @see Control::drawImages
     */
    unsigned int drawImages(Form* form, const Rectangle& clip);

    /**
     * @see Control::drawText
     */
    unsigned int drawText(Form* form, const Rectangle& clip);

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
     * The current location of the TextBox's caret.
     */
    unsigned int _caretLocation;

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

    void getCaretLocation(Vector2* p);
};

}

#endif
