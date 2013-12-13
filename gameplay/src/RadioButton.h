#ifndef RADIOBUTTON_H_
#define RADIOBUTTON_H_

#include "Button.h"
#include "Theme.h"
#include "Properties.h"

namespace gameplay
{

/**
 * Defines a radio button control.
 *
 * Radio buttons can be toggled between two states.
 * A radio button can belong to a group, and only one radio button
 * from a group can be selected at one time.
 *
 * @see http://blackberry.github.io/GamePlay/docs/file-formats.html#wiki-UI_Forms
 */
class RadioButton : public Button
{
    friend class Container;
    friend class ControlFactory;

public:

    /**
     * Creates a new RadioButton.
     *
     * @param id The radio button ID.
     * @param id The radio button style (optional).
     *
     * @return The new radio button.
     * @script{create}
     */
    static RadioButton* create(const char* id, Theme::Style* style = NULL);

    /**
     * Get whether this radio button is currently selected.
     *
     * @return Whether this radio button is currently selected.
     */
    bool isSelected() const;

    /**
     * Sets whether this radio button is currently selected.
     */
    void setSelected(bool selected);

    /**
     * Set the size to draw the radio button icon.
     *
     * @param width The width to draw the radio button icon.
     * @param height The height to draw the radio button icon.
     */
    void setImageSize(float width, float height);

    /**
     * Get the size at which the radio button icon will be drawn.
     *
     * @return The size of the radio button icon.
     */
    const Vector2& getImageSize() const;

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

    /**
     *
     * @param groupId
     */
    void setGroupId(const char* groupId);

    /**
     * Gets the RadioButton's group ID.
     *
     * @return the RadioButton's group ID.
     */
    const char* getGroupId() const;

protected:

    /**
     * Constructor.
     */
    RadioButton();

    /**
     * Destructor.
     */
    virtual ~RadioButton();

    /**
     * Create a radio button with a given style and properties.
     *
     * @param style The style to apply to this radio button.
     * @param properties A properties object containing a definition of the radio button (optional).
     *
     * @return The new radio button.
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
     * Called when a control's properties change.  Updates this control's internal rendering
     * properties, such as its text viewport.
     *
     * @param container This control's parent container.
     * @param offset Positioning offset to add to the control's position.
     */
    void update(const Control* container, const Vector2& offset);

    /**
     * @see Control::drawImages
     */
    unsigned int drawImages(Form* form, const Rectangle& clip);

    /**
     * Clear the _selected flag of all radio buttons in the given group.
     *
     * @param groupId The group to clear.
     */
    static void clearSelected(const std::string& groupId);

    /**
     * The RadioButton's group ID.
     */
    std::string _groupId;
    
    /**
     * Whether the RadioButton is currently selected.
     */
    bool _selected;
    
    /**
     * The size at which the RadioButton's icon will be drawn.
     */
    Vector2 _imageSize;

    /**
     * The ThemeImage to use for the RadioButton.
     */ 
    Theme::ThemeImage* _image;

private:

    RadioButton(const RadioButton& copy);
};

}

#endif
