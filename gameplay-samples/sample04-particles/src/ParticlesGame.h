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
     * Constructror.
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
    void update(long elapsedTime);

    /**
     * @see Game::render
     */
    void render(long elapsedTime);

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
    CheckBox* _started;
    Button* _reset;
    Button* _emit;
    Button* _zoomIn;
    Button* _zoomOut;
    Button* _minimize;
    RadioButton* _spiralFlame;
    RadioButton* _smoke;
    RadioButton* _explosion;
    Slider* _burstSize;
    Container* _position;
    Container* _particleProperties;
};

#endif
