#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#include "Control.h"

namespace gameplay
{

/**
 * Defines a control representing a joystick (axis).
 */
class Joystick : public Control
{
    friend class Container;
    friend class Gamepad;

public:

    /**
     * Create a new joystick control.
     *
     * @param id The control's ID.
     * @param style The control's style.
     *
     * @return The new joystick.
     * @script{create}
     */
    static Joystick* create(const char* id, Theme::Style* style);
    
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
     * Sets the image size of the inner region of the joystick.
     * 
     * @param region The size of the inner region of the joystick. (x, y) == (width, height)
     */
    inline void setInnerSize(const Vector2& size);

    /**
     * Gets the image size of the inner region of the joystick.
     * 
     * @return The image size of the inner region of the joystick. (x, y) == (width, height)
     */
    inline const Vector2& getInnerSize() const;

    /**
     * Sets the image size of the outer region of the joystick.
     * 
     * @param region The size of the outer region of the joystick. (x, y) == (width, height)
     */
    inline void setOuterSize(const Vector2& size);

    /**
     * Gets the image size of the outer region of the joystick.
     * 
     * @return The image size of the outer region of the joystick. (x, y) == (width, height)
     */
    inline const Vector2& getOuterSize() const;

    /**
     * Sets whether absolute positioning is enabled or not.
     * 
     * @param absolute Whether absolute positioning should be enabled or not.
     */
    inline void setRelative(bool relative);

    /**
     * Retrieves whether absolute positioning is enabled or not.
     * 
     * @return <code>true</code> if absolute positioning is enabled; <code>false</code> otherwise.
     */
    inline bool isRelative() const;

    /**
     * @see Control::getType
     */
    const char* getType() const;

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
     * Draw the images associated with this control.
     *
     * @param spriteBatch The sprite batch containing this control's icons.
     * @param clip The clipping rectangle of this control's parent container.
     */
    void drawImages(SpriteBatch* spriteBatch, const Rectangle& clip);

private:

    /**
     * Copy constructor.
     */
    Joystick(const Joystick& copy);

    float _radius; 
    bool _relative;
    Rectangle _screenRegion;
    Vector2 _value;
    Vector2 _displacement;
    Vector2* _innerSize;
    Vector2* _outerSize;
};

}

#include "Joystick.inl"

#endif
