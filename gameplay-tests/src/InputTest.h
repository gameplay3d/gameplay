#ifndef InputTest_H_
#define InputTest_H_

#include <vector>
#include <set>

#include "gameplay.h"
#include "Test.h"

using namespace gameplay;

class InputTest : public Test
{
public:
    InputTest();
    void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);
    void keyEvent(Keyboard::KeyEvent evt, int key);
    bool mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta);

protected:
    void initialize();
    void finalize();
    void update(float elapsedTime);
    void render(float elapsedTime);

private:
    bool drawScene(Node* node);

private:
    static const unsigned int MAX_TOUCH_POINTS = 5;

    typedef std::vector<Vector2 > TouchPositions;
    typedef std::set<int> KeySet;

    TouchPositions _touchPoints;
    KeySet _downKeys;

    Vector2 _mousePoint;
    Vector2 _mouseWheelPoint;
    std::string _keyboardString;
    std::string _symbolsString;
    std::string _mouseString;

    Font* _font;
    int _mouseWheel;
};

#endif
