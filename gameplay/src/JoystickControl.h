#ifndef JOYSTICKCONTROL_H_
#define JOYSTICKCONTROL_H_

#include "Control.h"

namespace gameplay
{

/**
 * Defines a control representing a joystick (axis).
 *
 * This is used in virtual Gamepad instances.
 *
 * @see http://blackberry.github.io/GamePlay/docs/file-formats.html#wiki-UI_Forms
 */
class JoystickControl : public Control
{
    friend class Container;
    friend class Gamepad;
	friend class ControlFactory;

public:

    /**
     * Creates a new Joystick control.
     *
     * @param id The joystick ID.
     * @param style The joystick style.
     *
     * @return The new joystick.
     * @script{create}
     */
    static JoystickControl* create(const char* id, Theme::Style* style = NULL);

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
     * Gets the value (2-dimensional direction) of the joystick.
     * 
     * @return The value of the joystick.
     */
    const Vector2& getValue() const;

    /**
     * Sets the image size of the inner region of the joystick. Does not do anything if there is no
     * inner image region defined.
     * 
     * @param size The size of the inner region of the joystick. (x, y) == (width, height)
     */
    void setInnerRegionSize(const Vector2& size);

    /**
     * Gets the image size of the inner region of the joystick. Returns (0,0) if there is no inner image
     * region defined.
     * 
     * @return The image size of the inner region of the joystick. (x, y) == (width, height)
     */
    const Vector2& getInnerRegionSize() const;

    /**
     * Sets the image size of the outer region of the joystick. Does not do anything if there is no
     * outer image region defined.
     * 
     * @param size The size of the outer region of the joystick. (x, y) == (width, height)
     */
    void setOuterRegionSize(const Vector2& size);

    /**
     * Gets the image size of the outer region of the joystick. Returns (0,0) if there is no outer image
     * region defined.
     * 
     * @return The image size of the outer region of the joystick. (x, y) == (width, height)
     */
    const Vector2& getOuterRegionSize() const;

    /**
     * Sets whether relative positioning is enabled or not.
     * 
     * Note: The default behavior is absolute positioning, and not relative.
     *
     * @param relative Whether relative positioning should be enabled or not.
     */
    void setRelative(bool relative);

    /**
     * Gets whether absolute positioning is enabled or not.
     * 
     * Note: The default behavior is absolute positioning, and not relative.
     *
     * @return <code>true</code> if relative positioning is enabled; <code>false</code> otherwise.
     */
    bool isRelative() const;

    /**
     * Gets the index of this joystick across all joysticks on a form.
     *
     * @return The index of this joystick on a form.
     */
    unsigned int getIndex() const;
    
    /**
     * @see Control::getType
     */
    const char* getType() const;

protected:
    
    /**
     * Constructor.
     */
    JoystickControl();

    /**
     * Destructor.
     */
    virtual ~JoystickControl();

    /**
     * Create a joystick control with a given style and properties.
     *
     * @param style The style to apply to this joystick.
     * @param properties A properties object containing a definition of the joystick (optional).
	 *
     * @return The new joystick.
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
     * @see Control::drawImages
     */
    unsigned int drawImages(Form* form, const Rectangle& clip);

private:

    /**
     * Copy constructor.
     */
    JoystickControl(const JoystickControl& copy);

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

#endif
