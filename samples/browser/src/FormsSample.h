#ifndef FORMSSAMPLE_H_
#define FORMSSAMPLE_H_

#include "SamplesGame.h"

using namespace gameplay;

class FormsSample : public Sample, Control::Listener
{
public:
    
    FormsSample();

    void gamepadEvent(Gamepad::GamepadEvent evt, Gamepad* gamepad);

protected:

    void initialize();

    void finalize();

    void update(float elapsedTime);

    void render(float elapsedTime);

    void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

    void keyEvent(Keyboard::KeyEvent keyEvent, int key);

    void controlEvent(Control* control, EventType evt);

private:
    
    void formChanged();
    
    void createSampleForm(Theme* theme);
    
    Scene* _scene;
    Node* _formNode;
    Node* _formNodeParent;
    Form* _formSelect;
    Form* _activeForm;
    std::vector<Form*> _forms;
    Gamepad* _gamepad;
    unsigned int _keyFlags;
    bool _touched;
    int _touchX;
    unsigned int _formIndex;
    std::vector<const char*> _formFiles;
    std::string _sampleString;
    Vector2 _joysticks[2];

};

#endif
