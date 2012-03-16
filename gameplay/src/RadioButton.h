#ifndef RADIOBUTTON_H_
#define RADIOBUTTON_H_

#include "Button.h"
#include "Theme.h"
#include "Properties.h"

namespace gameplay
{

/**
 * Similar to a checkbox, a radio button can be toggled between two states.
 *
 * However, a radio button can belong to a group, and only one radio button
 * from a group can be selected at one time.
 *
 * The following properties are available for radio buttons:
 *
 * radioButton <RadioButton ID>
 * {
 *      style       = <Style ID>
 *      position    = <x, y>
 *      size        = <width, height>
 *      text        = <string>
 *      group       = <string>
 *      iconSize    = <width, height>   // The size to draw the radio button icon, if different from its size in the texture.
 * }
 */
class RadioButton : public Button
{
    friend class Container;

public:
    /**
     * Get whether this radio button is currently selected.
     *
     * @return Whether this radio button is currently selected.
     */
    bool isSelected();

    /**
     * Set the size to draw the radio button icon.
     *
     * @param width The width to draw the radio button icon.
     * @param height The height to draw the radio button icon.
     */
    void setIconSize(float width, float height);

    /**
     * Get the size at which the radio button icon will be drawn.
     *
     * @return The size of the radio button icon.
     */
    const Vector2& getIconSize() const;

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
    RadioButton();
    virtual ~RadioButton();

    /**
     * Create a radio button with a given style and properties.
     *
     * @param style The style to apply to this radio button.
     * @param properties The properties to set on this radio button.
     *
     * @return The new radio button.
     */
    static RadioButton* create(Theme::Style* style, Properties* properties);

    bool touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

    void update(const Rectangle& clip);

    void drawSprites(SpriteBatch* spriteBatch, const Rectangle& clip);

    // Clear the _selected flag of all radio buttons in the given group.
    static void clearSelected(const std::string& groupId);

    std::string _groupId;
    bool _selected;
    Vector2 _iconSize;

private:
    RadioButton(const RadioButton& copy);
};

}

#endif