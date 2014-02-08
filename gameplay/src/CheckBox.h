#ifndef CHECKBOX_H_
#define CHECKBOX_H_

#include "Theme.h"
#include "Properties.h"
#include "Touch.h"
#include "Button.h"

namespace gameplay
{

/**
 * Defines a checkbox control.  
 *
 * This is a button that can be enabled or disabled.
 *
 * @see http://blackberry.github.io/GamePlay/docs/file-formats.html#wiki-UI_Forms
 */
class CheckBox : public Button
{
    friend class Container;
    friend class ControlFactory;

public:

    /**
     * Creates a new CheckBox.
     *
     * @param id The checkbox ID.
     * @param style The checkbox style (optional).
     *
     * @return The new checkbox.
     * @script{create}
     */
    static CheckBox* create(const char* id, Theme::Style* style = NULL);

    /**
     * Gets whether this checkbox is checked.
     *
     * @return Whether this checkbox is checked.
     */
    bool isChecked();

    /**
     * Sets whether the checkbox is checked.
     *
     * @param checked TRUE if the checkbox is checked; FALSE if the checkbox is not checked.
     */
    void setChecked(bool checked);

    /**
     * @see Control::getType
     */
    const char* getType() const;

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

protected:

    /**
     * Constructor.
     */
    CheckBox();

    /**
     * Destructor.
     */
    ~CheckBox();

    /**
     * Create a checkbox with a given style and properties.
     *
     * @param style The style to apply to this checkbox.
     * @param properties A properties object containing a definition of the checkbox (optional).
     *
     * @return The new checkbox.
     */
    static Control* create(Theme::Style* style, Properties* properties = NULL);

    /**
    * @see Control::initialize
    */
    void initialize(const char* typeName, Theme::Style* style, Properties* properties);

    /**
     * Keyboard callback on key events.
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
     * @see Control::updateBounds
     */
    void updateBounds();

    /**
     * @see Control::updateAbsoluteBounds
     */
    void updateAbsoluteBounds(const Vector2& offset);

    /**
     * @see Control::drawImages
     */
    unsigned int drawImages(Form* form, const Rectangle& clip);

    /**
     * Whether this checkbox is currently checked.
     */
    bool _checked;

    /**
     * The Theme::ThemeImage to display for the checkbox.
     */
    Theme::ThemeImage* _image;

private:

    /*
     * Constructor.
     */
    CheckBox(const CheckBox& copy);
};

}

#endif
