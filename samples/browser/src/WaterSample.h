#ifndef WATERSAMPLE_H_
#define WATERSAMPLE_H_

#include "gameplay.h"
#include "Sample.h"

using namespace gameplay;

/**
* Sample creating a water effect.
*/
class WaterSample : public Sample
{
public:

    WaterSample();

    void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);
    
    void keyEvent(Keyboard::KeyEvent evt, int key);

    bool mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta);

    void gamepadEvent(Gamepad::GamepadEvent evt, Gamepad* gamepad);

protected:

    void initialize();

    void finalize();

    void update(float elapsedTime);

    void render(float elapsedTime);

private:
    enum CameraMovement
    {
        MOVE_FORWARD     = (1 << 0),
        MOVE_BACKWARD    = (1 << 1),
        MOVE_LEFT        = (1 << 2),
        MOVE_RIGHT       = (1 << 3)
    };

    Font* _font;
    Scene* _scene;
    Node* _cameraNode;
    Node* _reflectCameraNode;

    Vector3 _cameraAcceleration;
    float _waterHeight;

    unsigned _inputMask;
    int _prevX, _prevY;

    FrameBuffer* _refractBuffer;
    SpriteBatch* _refractBatch;
    FrameBuffer* _reflectBuffer;
    SpriteBatch* _reflectBatch;

    bool _showBuffers;
    Vector4 _clipPlane;
    const Vector4& getClipPlane() const
    {
        return _clipPlane;
    }
    Matrix m_worldViewProjectionReflection;
    const Matrix& getReflectionMatrix() const
    {
        return m_worldViewProjectionReflection;
    }

    float getTime() const
    {
        return Game::getGameTime() * 0.0001;
    }

    Gamepad* _gamepad;
    bool drawScene(Node* node, bool drawWater);
};

#endif
