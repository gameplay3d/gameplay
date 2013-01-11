#ifndef FORMSTEST_H_
#define FORMSTEST_H_

#include "TestsGame.h"

using namespace gameplay;

class FormsTest : public Test, Control::Listener
{
public:
    FormsTest();

    virtual ~FormsTest();

    static FormsTest* create();

protected:
    void initialize();
    void finalize();
    void update(float elapsedTime);
    void render(float elapsedTime);
    void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);
    void keyEvent(Keyboard::KeyEvent keyEvent, int key);
    void controlEvent(Control* control, EventType evt);

private:
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

    void formChanged();
    void createTestForm(Theme* theme);

    std::string _testString;
};

#endif
