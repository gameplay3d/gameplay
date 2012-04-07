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

    void loadEmitters();

    void emitterChanged();

    Scene* _scene;
    Node* _particleEmitterNode;
    Node* _cameraParent;
    Form* _form;
    bool _wDown, _sDown, _aDown, _dDown;
    bool _touched;
    int _prevX, _prevY;
    std::vector<ParticleEmitter*> _particleEmitters;
    unsigned int _particleEmitterIndex;
    
    Slider* _startMin;
    Slider* _startMax;
    Slider* _endMin;
    Slider* _endMax;
    Slider* _energyMin;
    Slider* _energyMax;
    Slider* _emissionRate;
    CheckBox* _started;
    Button* _emit;
    Button* _zoomIn;
    Button* _zoomOut;
    RadioButton* _spiralFlame;
    RadioButton* _smoke;
    RadioButton* _explosion;
    Slider* _burstSize;
    Container* _position;
    Label* _particlesCount;
    Label* _fps;
};

#endif
