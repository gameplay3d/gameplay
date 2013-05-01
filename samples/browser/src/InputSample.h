#ifndef INPUTSAMPLE_H_
#define INPUTSAMPLE_H_

#include "gameplay.h"
#include "Sample.h"

using namespace gameplay;

/**
 * Samples basic input for keyboard, mouse, touch and accelerometer.
 */
class InputSample : public Sample, Control::Listener
{
public:

    InputSample();

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
    
    void setCaptured(bool captured);

private:
    
    class TouchPoint
    {
    public:
        unsigned int _id;
        Vector2 _coord;
        bool _isStale;
    };

    std::list<TouchPoint> _touchPoints;
    std::set<int> _downKeys;
    Vector2 _mousePoint;
    Vector2 _mouseWheelPoint;
    std::string _keyboardString;
    std::string _symbolsString;
    std::string _mouseString;
    Font* _font;
    Form* _inputSampleControls;
    int _mouseWheel;
    bool _keyboardState;
    SpriteBatch* _crosshair;
    Rectangle _crosshairDstRect;
    Rectangle _crosshairSrcRect;
    Vector2 _crosshairLowerLimit;
    Vector2 _crosshairUpperLimit;
    Scene* _scene;
    Node* _formNode;
    Node* _formNodeParent;
    Vector3 _formNodeRestPosition;
};

#endif
