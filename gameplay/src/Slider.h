#ifndef SLIDER_H_
#define SLIDER_H_

#include "Base.h"
#include "Theme.h"
#include "Properties.h"
#include "Button.h"
#include "Touch.h"

namespace gameplay
{

/**
 * A slider consists of a marker that can slide along a track between two end-caps.
 * The following properties are available for sliders:

 @verbatim
    slider
    {
        style       = <styleID>                 // A Style from the Theme.
        position    = <x, y>                    // Position of the Control on-screen, measured in pixels.
        size        = <width, height>           // Size of the Control, measured in pixels.
        min         = <float>                   // The value of the left- / bottom-most point on the slider.
        max         = <float>                   // The value of the right- / top-most point on the slider.
        value       = <float>                   // The default position of the marker.
        step        = <float>                   // If greater than 0, force the marker to snap to discrete multiples of 'step'.
        text        = <string>                  // Text to display above, below or alongside the slider (depending on the style).
        consumeEvents = <bool>                  // Whether the slider propagates input events to the Game's input event handler. Default is true.
        // TODO: orientation = <HORIZONTAL or VERTICAL>  // Determines whether a slider is stretched along its width or its height
    }
 @endverbatim
 */
class Slider : public Label
{
    friend class Container;

public:

    /**
     * Create a new slider control.
     *
     * @param id The control's ID.
     * @param style The control's style.
     *
     * @return The new slider.
     * @script{create}
     */
    static Slider* create(const char* id, Theme::Style* style);

    /**
     * Set the minimum value that can be set on this slider.
     *
     * @param min The new minimum.
     */
    void setMin(float min);

    /**
     * Get the minimum value that can be set on this slider.
     *
     * @return The minimum value that can be set on this slider.
     */
    float getMin() const;

    /**
     * Set the maximum value that can be set on this slider.
     *
     * @param max The new maximum.
     */
    void setMax(float max);

    /**
     * Get the maximum value that can be set on this slider.
     *
     * @return The maximum value that can be set on this slider.
     */
    float getMax() const;

    /**
     * Set this slider's step size.  If this is greater than zero, the marker
     * will snap to discrete multiples of the step size.
     *
     * @param step The new step size.
     */
    void setStep(float step);

    /**
     * Get this slider's step size.
     *
     * @return This slider's step size.
     */
    float getStep() const;

    /**
     * Set this slider's value.  The new value will be clamped to fit within
     * the slider's minimum and maximum values.
     *
     * @param value The new value.
     */
    void setValue(float value);

    /**
     * Get this slider's current value.
     *
     * @return This slider's current value.
     */
    float getValue() const;

    /**
     * @see Control::getType
     */
    const char* getType() const;

    /**
     * Sets if the slider value text is rendered below the control.
     * 
     * @param visible If the slider value text is rendered below the control. 
     */
    void setValueTextVisible(bool visible);

    /**
     * Gets if the slider value text is rendered below the control.
     *
     * @return true if the slider value text is rendered below the control, false if otherwise.
     */
    bool isValueTextVisible() const;

    /**
     * Sets the slider value text alignment.
     * 
     * @param alignment the slider value text alignment. 
     */
    void setValueTextAlignment(Font::Justify alignment);

    /**
     * Gets the slider value text alignment.
     *
     * @return The slider value text alignment. 
     */
    Font::Justify getValueTextAlignment() const;

    /**
     * Sets the precision, which is the number floating point digits after the decimal.
     *
     * @param precision The number floating point precision/digits after the decimal.
     */
    void setValueTextPrecision(unsigned int precision);

    /**
     * Gets the precision, which is the number floating point digits after the decimal.
     *
     * @return The number floating point precision/digits after the decimal.
     */
    unsigned int getValueTextPrecision() const;

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

protected:

    /**
     * Constructor.
     */
    Slider();

    /**
     * Destructor.
     */
    ~Slider();

    /**
     * Create a slider with a given style and properties.
     *
     * @param style The style to apply to this slider.
     * @param properties The properties to set on this slider.
     *
     * @return The new slider.
     */
    static Slider* create(Theme::Style* style, Properties* properties);

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
     * Mouse callback on mouse events.
     *
     * @param evt The mouse event that occurred.
     * @param x The x position of the mouse in pixels. Left edge is zero.
     * @param y The y position of the mouse in pixels. Top edge is zero.
     * @param wheelDelta The number of mouse wheel ticks. Positive is up (forward), negative is down (backward).
     *
     * @return True if the mouse event is consumed or false if it is not consumed.
     *
     * @see Mouse::MouseEvent
     */
    bool mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta);

    /**
     * Gamepad callback on gamepad events.
     *
     * @see Control::gamepadEvent
     */
    bool gamepadEvent(Gamepad::GamepadEvent evt, Gamepad* gamepad, unsigned int analogIndex);

    /**
     * Keyboard callback on key events.
     *
     * @see Keyboard::KeyEvent
     * @see Keyboard::Key
     */
    bool keyEvent(Keyboard::KeyEvent evt, int key);

    /**
     * Slider overrides draw() so that it can avoid resetting the _dirty flag
     * when a joystick is being used to change its value.
     */
    void draw(SpriteBatch* spriteBatch, const Rectangle& clip, bool needsClear, bool cleared, float targetHeight);

    /**
     * Draw the images associated with this control.
     *
     * @param spriteBatch The sprite batch containing this control's icons.
     * @param clip The clipping rectangle of this control's parent container.
     */
    void drawImages(SpriteBatch* spriteBatch, const Rectangle& clip);

    /**
     * Draw this slider's text.
     *
     * @param clip The clipping rectangle of this slider's parent container.
     */
    void drawText(const Rectangle& clip);

    /**
     * Called when a slider's properties change. Updates this slider's internal rendering
     * properties, such as its text viewport.
     *
     * @param container This slider's parent container.
     * @param offset The scroll offset of this slider's parent container.
     */
    void update(const Control* container, const Vector2& offset);

    /**
     * The minimum value for the Slider.
     */
    float _min;
    
    /**
     * The maximum value for the Slider
     */
    float _max;
    
    /**
     * The Slider's step size.
     */
    float _step;
    
    /**
     * The Slider's current value.
     */
    float _value;

    /**
     * When a gamepad is in use, this stores how much to move the slider's value.
     */
    float _delta;

    /**
     * The X coordinate of the first touch event in a sequence.
     */
    float _originalX;

    /**
     * The Slider's original value at the start of a sequence of touch events.
     */
    float _originalValue;

    /**
     * The Slider's original setting of _consumeInputEvents at the start of a sequence of touch events.
     */
    bool _originalConsumeInputEvents;

    /**
     * Whether the Slider's current movement has been cancelled, e.g. because the user is scrolling the parent container.
     */
    bool _moveCancelled;

    /**
     * The image for the minimum slider value.
     */
    Theme::ThemeImage* _minImage;
    
    /**
     * The image for the maximum slider value.
     */
    Theme::ThemeImage* _maxImage;
    
    /**
     * The image for the slider track.
     */
    Theme::ThemeImage* _trackImage;
    
    /**
     * The image for the slider marker.
     */
    Theme::ThemeImage* _markerImage;

    /**
     * Whether to display this slider's value.
     */
    bool _valueTextVisible;

    /**
     * Alignment of value text.
     */
    Font::Justify _valueTextAlignment;

    /**
     * Number of digits after the decimal to draw for value text.
     */
    unsigned int _valueTextPrecision;

    /**
     * The text displayed by this slider if set to display its value.
     */
    std::string _valueText;

    // Used by gamepads to toggle Slider state between FOCUS and ACTIVE.
    bool _selectButtonDown;

    bool _directionButtonDown;

    float _gamepadValue;

private:

    /**
     * Constructor.
     */
    Slider(const Slider& copy);
};

}

#endif
