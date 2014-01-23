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
 * Defines a slider control.
 *
 * A slider consists of a marker (grabber) that can slide along a track between two end-caps.
 * 
 * @see http://blackberry.github.io/GamePlay/docs/file-formats.html#wiki-UI_Forms
 */
class Slider : public Label
{
    friend class Container;
    friend class ControlFactory;

public:

    /**
     * Creates a new Slider.
     *
     * @param id The slider ID.
     * @param style The slider style (optional).
     *
     * @return The new slider.
     * @script{create}
     */
    static Slider* create(const char* id, Theme::Style* style = NULL);

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
     * @param properties A properties object containing a definition of the slider (optional).
     *
     * @return The new slider.
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
     * @see Control::drawImages
     */
    unsigned int drawImages(Form* form, const Rectangle& clip);

    /**
     * @see Control::drawText
     */
    unsigned int drawText(Form* form, const Rectangle& clip);

    /**
     * @see Control::update
     */
    void update(float elapsedTime);

    /**
     * @see Control::updateState
     */
    void updateState(State state);

    /**
     * @see Control::updateBounds
     */
    void updateBounds();

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

private:

    /**
     * Constructor.
     */
    Slider(const Slider& copy);

    void updateValue(int x, int y);

    /**
     * The text displayed by this slider if set to display its value.
     */
    std::string _valueText;

    float _trackHeight;

    float _gamepadValue;
};

}

#endif
