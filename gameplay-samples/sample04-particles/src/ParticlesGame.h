#ifndef PARTICLESGAME_H_
#define PARTICLESGAME_H_

#include "gameplay.h"

using namespace gameplay;

/**
 * Main game class.
 */
class ParticlesGame: public Game, Control::Listener
{
public:

    /**
     * Constructor.
     */
    ParticlesGame();

    /**
     * @see Game::touchEvent
     */
    void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

    /**
     * @see Game::keyEvent
     */
    void keyEvent(Keyboard::KeyEvent evt, int key);

    /**
     * @see Control::controlEvent
     */
    void controlEvent(Control* control, EventType evt);

protected:

    /**
     * @see Game::initialize
     */
    void initialize();

    /**
     * @see Game::finalize
     */
    void finalize();

    /**
     * @see Game::update
     */
    void update(float elapsedTime);

    /**
     * @see Game::render
     */
    void render(float elapsedTime);

private:

    bool drawScene(Node* node, void* cookie);

    void drawSplash(void* param);

    void loadEmitters();

    void emitterChanged();

    void drawFrameRate(Font* font, const Vector4& color, unsigned int x, unsigned int y, unsigned int fps);

    Scene* _scene;
    Node* _particleEmitterNode;
    Node* _cameraParent;
    Form* _form;
    bool _wDown, _sDown, _aDown, _dDown;
    bool _touched;
    int _prevX, _prevY;
    std::vector<ParticleEmitter*> _particleEmitters;
    unsigned int _particleEmitterIndex;
    Font* _font;
    
    Slider* _startRed;
    Slider* _startGreen;
    Slider* _startBlue;
    Slider* _startAlpha;
    Slider* _endRed;
    Slider* _endGreen;
    Slider* _endBlue;
    Slider* _endAlpha;
    Slider* _startMin;
    Slider* _startMax;
    Slider* _endMin;
    Slider* _endMax;
    Slider* _energyMin;
    Slider* _energyMax;
    Slider* _emissionRate;
    Slider* _posVarX;
    Slider* _posVarY;
    Slider* _posVarZ;
    Slider* _velX;
    Slider* _velY;
    Slider* _velZ;
    Slider* _velVarX;
    Slider* _velVarY;
    Slider* _velVarZ;
    Slider* _accelX;
    Slider* _accelY;
    Slider* _accelZ;
    Slider* _accelVarX;
    Slider* _accelVarY;
    Slider* _accelVarZ;
    Slider* _spinSpeedMin;
    Slider* _spinSpeedMax;
    Slider* _axisX;
    Slider* _axisY;
    Slider* _axisZ;
    Slider* _axisVarX;
    Slider* _axisVarY;
    Slider* _axisVarZ;
    Slider* _rotationSpeedMin;
    Slider* _rotationSpeedMax;
    CheckBox* _started;
    Button* _reset;
    Button* _emit;
    Button* _zoomIn;
    Button* _zoomOut;
    RadioButton* _spiralFlame;
    RadioButton* _smoke;
    RadioButton* _explosion;
    Slider* _burstSize;
    Container* _position;
    Container* _particleProperties;
};

#endif
