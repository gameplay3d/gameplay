#ifndef ParticlesGame_H_
#define ParticlesGame_H_

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
     * Touch event handler.
     */
    void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);
    
    void keyEvent(Keyboard::KeyEvent evt, int key);

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

    /**
     * Draws the scene each frame.
     */
    bool drawScene(Node* node, void* cookie);

    void loadParticleEmitters();
    void emitterChanged();

    // Scene.
    Scene* _scene;
    Node* _particleEmitterNode;
    Node* _cameraParent;
    Form* _form;

    // Keyboard and mouse state, for camera movement.
    bool _wDown, _sDown, _aDown, _dDown;
    bool _touched;
    int _prevX, _prevY;

    // Particle emitters.
    std::vector<ParticleEmitter*> _particleEmitters;
    unsigned int _particleEmitterIndex;

    // UI controls.
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
};

#endif
