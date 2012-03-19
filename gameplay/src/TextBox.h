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
 *
 * label <Label ID>
 * {
 *      style       = <Style ID>
 *      position    = <x, y>
 *      size        = <width, height>
 *      text        = <string>
 * }
 */
class TextBox : public Label
{
    friend class Container;

public:
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

    int getLastKeypress();

protected:
    TextBox();
    ~TextBox();

    static TextBox* create(Theme::Style* style, Properties* properties);

    void setCursorLocation(int x, int y);

    bool touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

    void keyEvent(Keyboard::KeyEvent evt, int key);

    void update(const Rectangle& clip);

    // Draw the cursor.
    void drawSprites(SpriteBatch* spriteBatch, const Rectangle& clip);

    Vector2 _cursorLocation;
    unsigned int textIndex;
    int _lastKeypress;

private:
    TextBox(const TextBox& copy);
};

}

#endif