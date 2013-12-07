#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#include "Control.h"

namespace gameplay
{

/**
 * Defines a control representing a joystick (axis).
 *
  @verbatim
    slider
    {
        style       = <styleID>                 // A Style from the Theme.
        position    = <x, y>                    // Position of the Control on-screen, measured in pixels.
        alignment   = <Control::Alignment constant> // Note: 'position' will be ignored.
        size        = <width, height>           // Size of the Control, measured in pixels.
        radius      = <float>                   // The value of the left- / bottom-most point on the slider.
        consumeEvents = <bool>                  // Whether the slider propagates input events to the Game's input event handler. Default is true.
    }
 @endverbatim
 */
class Joystick : public Control
{
    friend class Container;
    friend class Gamepad;
	friend class ControlFactory;

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
     * Sets the image size of the inner region of the joystick. Does not do anything if there is no
     * inner image region defined.
     * 
     * @param size The size of the inner region of the joystick. (x, y) == (width, height)
     */
    inline void setInnerRegionSize(const Vector2& size);

    /**
     * Gets the image size of the inner region of the joystick. Returns (0,0) if there is no inner image
     * region defined.
     * 
     * @return The image size of the inner region of the joystick. (x, y) == (width, height)
     */
    inline const Vector2& getInnerRegionSize() const;

    /**
     * Sets the image size of the outer region of the joystick. Does not do anything if there is no
     * outer image region defined.
     * 
     * @param size The size of the outer region of the joystick. (x, y) == (width, height)
     */
    inline void setOuterRegionSize(const Vector2& size);

    /**
     * Gets the image size of the outer region of the joystick. Returns (0,0) if there is no outer image
     * region defined.
     * 
     * @return The image size of the outer region of the joystick. (x, y) == (width, height)
     */
    inline const Vector2& getOuterRegionSize() const;

    /**
     * Sets whether relative positioning is enabled or not.
     * 
     * Note: The default behavior is absolute positioning, and not relative.
     *
     * @param relative Whether relative positioning should be enabled or not.
     */
    inline void setRelative(bool relative);

    /**
     * Retrieves whether absolute positioning is enabled or not.
     * 
     * Note: The default behavior is absolute positioning, and not relative.
     *
     * @return <code>true</code> if relative positioning is enabled; <code>false</code> otherwise.
     */
    inline bool isRelative() const;

    /**
     * @see Control::getType
     */
    const char* getType() const;

    /**
     * Gets the index of this joystick across all joysticks on a form.
     *
     * @return The index of this joystick on a form.
     */
    inline const unsigned int getIndex() const;

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
    static Control* create(Theme::Style* style, Properties* properties);

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
    unsigned int _index;
};

}

#include "Joystick.inl"

#endif
