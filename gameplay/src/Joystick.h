#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#include "Control.h"

namespace gameplay
{

class Joystick : public Control
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
    void addListener(Control::Listener* listener, int eventFlags);

    /**
     * Retrieves the value (2-dimensional direction) of the joystick.
     * 
     * @return The value of the joystick.
     */
    inline const Vector2& getValue() const;

    /**
     * Sets the region within which the joystick will be spontaneously created on a user's touch.
     * 
     * Note: This does not actually enable spontaneous joystick creation on touch input.
     * To enable (or disable) absolute position explicitly, use #setAbsolute.
     * 
     * @param region The region to use.
     */
    inline void setRegion(const Rectangle& region);

    /**
     * Gets the region within which the joystick will be spontaneously created on a user's touch.
     * 
     * Note: just because the returned region is not empty does not mean that it is necessarily
     * being used. If absolute positioning is not enabled, then it will be used (to check if
     * absolute positioning is enabled, call #isAbsolute).
     * 
     * @return The region within which the joystick will be spontaneously created on a user's touch.
     */
    inline const Rectangle& getRegion() const;

    /**
     * Sets whether absolute positioning is enabled or not.
     * 
     * @param absolute Whether absolute positioning should be enabled or not.
     */
    inline void setAbsolute(bool absolute);

    /**
     * Retrieves whether absolute positioning is enabled or not.
     * 
     * @return <code>true</code> if absolute positioning is enabled; <code>false</code> otherwise.
     */
    inline bool isAbsolute() const;

protected:
    
    /**
     * Constructor.
     */
    Joystick();

    /**
     * Destructor.
     */
    virtual ~Joystick();

    /**
     * Create a joystick with a given style and properties.
     *
     * @param style The style to apply to this joystick.
     * @param properties The properties to set on this joystick.
     *
     * @return The new joystick.
     */
    static Joystick* create(Theme::Style* style, Properties* properties);

    /**
     * Initialize this joystick.
     */
    virtual void initialize(Theme::Style* style, Properties* properties);

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
     * @param offset Layout-computed positioning offset to add to the control's position.
     */
    void drawImages(SpriteBatch* spriteBatch, const Rectangle& clip);

private:

    /**
     * Copy constructor.
     */
    Joystick(const Joystick& copy);

    float _radius;
    unsigned int _contactIndex;
    bool _absolute;
    Vector2 _displacement;
    Vector2 _value;
    Rectangle _region;
};

}

#include "Joystick.inl"

#endif
