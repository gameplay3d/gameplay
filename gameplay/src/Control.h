#ifndef CONTROL_H_
#define CONTROL_H_

#include "Ref.h"
#include "Rectangle.h"
#include "Vector2.h"
#include "SpriteBatch.h"
#include "Theme.h"
#include "Touch.h"
#include "Keyboard.h"

namespace gameplay
{

/**
 * Base class for UI controls.
 */
class Control : public Ref
{
    friend class Form;
    friend class Container;
    friend class Layout;
    friend class AbsoluteLayout;
    friend class VerticalLayout;

public:
    /**
     * The possible states a control can be in.
     */
    enum State
    {
        NORMAL,
        FOCUS,
        ACTIVE,
        DISABLED
    };

    class Listener
    {
    public:
        enum EventType
        {
            PRESS           = 0x01,
            RELEASE         = 0x02,
            CLICK           = 0x04,
            VALUE_CHANGED   = 0x08,
            TEXT_CHANGED    = 0x10,
            //MOUSE           = PRESS | RELEASE | CLICK,
            //ALL             = MOUSE | VALUE_CHANGED | TEXT_CHANGED 
        };

        virtual void controlEvent(Control* control, EventType evt) = 0;
    };

    /**
     * Get this control's ID string.
     *
     * @return This control's ID.
     */
    const char* getID() const;

    /**
     * Set the position of this control relative to its parent container.
     *
     * @param x The x coordinate.
     * @param y The y coordinate.
     */
    void setPosition(float x, float y);

    /**
     * Get the position of this control relative to its parent container.
     *
     * @return The position of this control relative to its parent container.
     */
    const Vector2& getPosition() const;

    /**
     * Set the desired size of this control, including its border and padding, before clipping.
     *
     * @param width The width.
     * @param height The height.
     */
    void setSize(float width, float height);

    /**
     * Get the desired size of this control, including its border and padding, before clipping.
     *
     * @return The size of this control.
     */
    const Vector2& getSize() const;

    /**
     * Get the bounds of this control, relative to its parent container, after clipping.
     *
     * @return The bounds of this control.
     */
    const Rectangle& getBounds() const;

    /**
     * Get the content area of this control, in screen coordinates, after clipping.
     *
     * @return The clipping area of this control.
     */
    const Rectangle& getClip() const;

    /**
     * Set width and/or height to auto-size to size a Control to tightly fit
     * its text and themed visual elements (CheckBox / RadioButton toggle etc.).
     *
     * Similarly set this on the width and/or height of a Container to tightly fit
     * the Container around all its children.
     *
     * @param width Whether to automatically determine this Control's width.
     * @param height Whether to automatically determine this Control's height.
     */
    void setAutoSize(bool width, bool height);

    /**
     * Change this control's state.
     *
     * @param state The state to switch this control to.
     */
    void setState(State state);

    /**
     * Get this control's current state.
     *
     * @return This control's current state.
     */
    State getState();

    /**
     * Disable this control.
     */
    void disable();

    /**
     * Enable this control.
     */
    void enable();

    /**
     * Get whether this control is currently enabled.
     *
     * @return Whether this control is currently enabled.
     */
    bool isEnabled();

    /**
     * Set whether this control consumes touch events,
     * preventing them from being passed to the game.
     *
     * @param consume Whether this control consumes touch events.
     */
    void setConsumeTouchEvents(bool consume);

    /**
     * Get whether this control consumes touch events.
     *
     * @return Whether this control consumes touch events.
     */
    bool getConsumeTouchEvents();

    /**
     * Set the style this control will use when rendering.
     *
     * @param style The style this control will use when rendering.
     */
    void setStyle(Theme::Style* style);

    /**
     * Get this control's style.
     *
     * @return This control's style.
     */
    Theme::Style* getStyle() const;

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
    Control();
    virtual ~Control();

    /**
     * Get the overlay type corresponding to this control's current state.
     *
     * @return The overlay type corresponding to this control's current state.
     */
    Theme::Style::OverlayType getOverlayType() const;

    /**
     * Touch callback on touch events.  Controls return true if they consume the touch event.
     *
     * @param evt The touch event that occurred.
     * @param x The x position of the touch in pixels. Left edge is zero.
     * @param y The y position of the touch in pixels. Top edge is zero.
     * @param contactIndex The order of occurrence for multiple touch contacts starting at zero.
     *
     * @return Whether the touch event was consumed by this control.
     *
     * @see Touch::TouchEvent
     */
    virtual bool touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

    /**
     * Keyboard callback on key events.
     *
     * @param evt The key event that occured.
     * @param key If evt is KEY_PRESS or KEY_RELEASE then key is the key code from Keyboard::Key.
     *            If evt is KEY_CHAR then key is the unicode value of the character.
     * 
     * @see Keyboard::KeyEvent
     * @see Keyboard::Key
     */
    virtual void keyEvent(Keyboard::KeyEvent evt, int key);

    /**
     * Called when a control's properties change.  Updates this control's internal rendering
     * properties, such as its text viewport.
     *
     * @param clip The clipping rectangle of this control's parent container.
     */
    virtual void update(const Rectangle& clip);

    /**
     * Draws the themed border and background of a control.
     *
     * @param spriteBatch The sprite batch containing this control's border images.
     * @param clip The clipping rectangle of this control's parent container.
     */
    virtual void drawBorder(SpriteBatch* spriteBatch, const Rectangle& clip);

    /**
     * Draw the icons associated with this control.
     *
     * @param spriteBatch The sprite batch containing this control's icons.
     * @param clip The clipping rectangle of this control's parent container.
     */
    virtual void drawSprites(SpriteBatch* spriteBatch, const Rectangle& clip);

    /**
     * Draw this control's text.
     *
     * @param clip The clipping rectangle of this control's parent container.
     */
    virtual void drawText(const Rectangle& clip);

    /**
     * Initialize properties common to all Controls.
     */
    virtual void init(Theme::Style* style, Properties* properties);

    /**
     * Container and classes that extend it should implement this and return true.
     */
    virtual bool isContainer();

    /**
     * Returns whether this control has been modified and requires an update.
     */
    virtual bool isDirty();

    /**
     * Get a Control::State enum from a matching string.
     */
    static State getStateFromString(const char* state);

    /**
     * Alert all listeners of a specific event.
     */
    void alertListeners(Listener::EventType eventType);

    void addSpecificListener(Control::Listener* listener, Listener::EventType eventType);

    std::string _id;
    State _state;           // Determines overlay used during draw().
    Vector2 _position;      // Position, relative to parent container's clipping window.
    Vector2 _size;          // Desired size.  Will be clipped.
    Rectangle _bounds;      // The position and size of this control, relative to parent container's bounds, including border and padding, after clipping.
    Rectangle _textBounds;  // The position and size of this control's content, before clipping.  Used for text alignment.
    Rectangle _clip;        // Clipping window of this control's content, after clipping.
    bool _autoWidth;
    bool _autoHeight;
    bool _dirty;
    bool _consumeTouchEvents;
    Theme::Style* _style;
    typedef std::map<Listener::EventType, std::list<Listener*>*> ListenerMap;
    ListenerMap* _listeners;

private:
    Control(const Control& copy);
};

}

#endif
