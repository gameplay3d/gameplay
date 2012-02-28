#ifndef BUTTON_H_
#define BUTTON_H_

#include "Label.h"
#include "Touch.h"
#include "Theme.h"
#include "Properties.h"

namespace gameplay
{

class Button : public Label
{
    class Callback;

public:
    Button();
    virtual ~Button();

    static Button* create(Theme::Style* style, Properties* properties);
    static Button* create(const char* id, unsigned int x, unsigned int y, unsigned int width, unsigned int height);
    static Button* getButton(const char* id);

    template <class ClassType>
    void setCallback(ClassType* instance, void (ClassType::*callbackMethod)(Button*));

    void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

protected:
    Callback* _callback;

private:
    Button(const Button& copy);

    class Callback
    {
    public:
        virtual ~Callback() { }
        virtual void trigger(Button* button) = 0;
    };

    template <class ClassType>
    class CallbackImpl : public Callback
    {
        typedef void (ClassType::*CallbackMethod)(Button*);
    public:
        CallbackImpl(ClassType* instance, CallbackMethod method);
        void trigger(Button* button);
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
void Button::setCallback(ClassType* instance, void (ClassType::*callbackMethod)(Button*))
{
    _callback = new CallbackImpl<ClassType>(instance, callbackMethod);
}

template <class ClassType>
void Button::CallbackImpl<ClassType>::trigger(Button* button)
{
    (_instance->*_method)(button);
}

}

#endif