#ifndef BUTTON_H_
#define BUTTON_H_

#include "Label.h"
#include "Touch.h"
#include "Theme.h"
#include "Properties.h"

namespace gameplay
{

/**
 * Defines a button UI control.  This is essentially a label that can have a callback method set on it.
 *
 * The following properties are available for buttons:
 *
 * button <Button ID>
 * {
 *      style       = <Style ID>
 *      position    = <x, y>
 *      size        = <width, height>
 *      text        = <string>
 * }
 */
class Button : public Label
{
    friend class Container;

    class Callback;

public:
    /**
     * Set a callback method on this button.  The callback will be triggered when the button is
     * clicked -- i.e. consecutive TOUCH_PRESS and TOUCH_RELEASE events that both fall within
     * the bounds of the button.  
     *
     * @param instance The object to call the method on.
     * @param callbackMethod The method to call.
     */
    template <class ClassType>
    void setCallback(ClassType* instance, void (ClassType::*callbackMethod)(Control*));

protected:
    /**
     * Create a button with a given style and properties.
     *
     * @param style The style to apply to this button.
     * @param properties The properties to set on this button.
     *
     * @return The new button.
     */
    static Button* create(Theme::Style* style, Properties* properties);

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

    Button();
    virtual ~Button();

    Callback* _callback;    // The callback method pointer interface.

private:
    Button(const Button& copy);

    /**
     * Abstract callback interface.
     */
    class Callback
    {
    public:
        virtual ~Callback() { }
        virtual void trigger(Control* button) = 0;
    };

    /**
     * Implementation of the callback interface for a specific class.
     */
    template <class ClassType>
    class CallbackImpl : public Callback
    {
        typedef void (ClassType::*CallbackMethod)(Control*);
    public:
        CallbackImpl(ClassType* instance, CallbackMethod method);
        void trigger(Control* control);
    private:
        ClassType* _instance;
        CallbackMethod _method;
    };
};

template <class ClassType>
Button::CallbackImpl<ClassType>::CallbackImpl(ClassType* instance, CallbackMethod method)
    : _instance(instance), _method(method)
{
}

template <class ClassType>
void Button::setCallback(ClassType* instance, void (ClassType::*callbackMethod)(Control*))
{
    _callback = new CallbackImpl<ClassType>(instance, callbackMethod);
}

template <class ClassType>
void Button::CallbackImpl<ClassType>::trigger(Control* control)
{
    (_instance->*_method)(control);
}

}

#endif