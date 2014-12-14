#ifndef PARTICLESSAMPLE_H_
#define PARTICLESSAMPLE_H_

#include "gameplay.h"
#include "Sample.h"

using namespace gameplay;

/**
 * Main game class.
 */
class ParticlesSample: public Sample, Control::Listener
{
public:

    /**
     * Constructor.
     */
    ParticlesSample();

    /**
     * @see Game::touchEvent
     */
    void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

    /**
     * @see Game::mouseEvent
     */
    bool mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta);

    /**
     * @see Game::keyEvent
     */
    void keyEvent(Keyboard::KeyEvent evt, int key);

    /**
     * @see Game::resizeEvent
     */
    void resizeEvent(unsigned int width, unsigned int height);

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

    void loadEmitters();

    void emitterChanged();

    void drawFrameRate(Font* font, const Vector4& color, unsigned int x, unsigned int y, unsigned int fps);

    void updateTexture();

    void updateImageControl();

    void updateFrames();
    
    void addGrid(unsigned int lineCount);

    void saveFile();
    
    std::string toString(bool b);
    
    std::string toString(int i);
    
    std::string toString(unsigned int i);
    
    std::string toString(const Vector3& v);
    
    std::string toString(const Vector4& v);
    
    std::string toString(const Quaternion& q);
    
    std::string toString(ParticleEmitter::BlendMode blendMode);

    Scene* _scene;
    Node* _cameraParent;
    Node* _particleEmitterNode;
    ParticleEmitter* _particleEmitter;
    bool _wDown, _sDown, _aDown, _dDown;
    bool _touched;
    int _prevX, _prevY;
    bool _panning;
    bool _rotating;
    bool _zooming;
    Font* _font;
    Form* _form;
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
    Button* _save;
    Button* _load;
    Slider* _burstSize;
    Container* _position;
    Container* _particleProperties;
    CheckBox* _vsync;
    std::string _url;
};

#endif
