#ifndef INPUTTEST_H_
#define INPUTTEST_H_

#include "gameplay.h"
#include "Test.h"

using namespace gameplay;

/**
 * Tests basic input for keyboard, mouse, touch and accelerometer.
 */
class InputTest : public Test, Control::Listener
{
public:

    InputTest();

    void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

    void keyEvent(Keyboard::KeyEvent evt, int key);

    bool mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta);
    
    void controlEvent(Control* control, EventType evt);

protected:

    void initialize();

    void finalize();

    void update(float elapsedTime);

    void render(float elapsedTime);

private:

    bool drawScene(Node* node);

private:

    std::map<unsigned int, Vector2>  _touchPoints;
    std::set<int> _downKeys;
    Vector2 _mousePoint;
    Vector2 _mouseWheelPoint;
    std::string _keyboardString;
    std::string _symbolsString;
    std::string _mouseString;
    Font* _font;
    Form* _keyboardSwitch;
    int _mouseWheel;
    bool _keyboardState;
};

#endif
