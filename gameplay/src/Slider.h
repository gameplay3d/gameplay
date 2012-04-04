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
 *  A slider consists of a marker that can slide along a track between two end-caps.
 *  The following properties are available for sliders:
 *
 *  slider
 *  {
 *      style       = <Style ID>                // A Style from the Theme.
 *      position    = <x, y>                    // Position of the Control on-screen, measured in pixels.
 *      size        = <width, height>           // Size of the Control, measured in pixels.
 *      // TODO: orientation = <HORIZONTAL or VERTICAL>  // Determines whether a slider is stretched along its width or its height
 *      min         = <float>                   // The value of the left- / bottom-most point on the slider.
 *      max         = <float>                   // The value of the right- / top-most point on the slider.
 *      value       = <float>                   // The default position of the marker.
 *      step        = <float>                   // If greater than 0, force the marker to snap to discrete multiples of 'step'.
 *      text        = <string>                  // Text to display above, below or alongside the slider (depending on the style).
 *  }
 */
class Slider : public Label
{
    friend class Container;

public:
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
    float getMin();

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
    float getMax();

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
    float getStep();

    /**
     * Set this slider's value.  The new value will be clamped to fit within
     * the slider's minimum and maximum values.
     *
     * @param The new value.
     */
    void setValue(float value);

    /**
     * Get this slider's current value.
     *
     * @return This slider's current value.
     */
    float getValue();

    void addListener(Control::Listener* listener, int eventFlags);

protected:
    Slider();
    ~Slider();

    static Slider* create(Theme::Style* style, Properties* properties);

    bool touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

    void drawImages(SpriteBatch* spriteBatch, const Rectangle& clip);

    float _min;
    float _max;
    float _step;
    float _value;

private:
    Slider(const Slider& copy);
};

}

#endif
