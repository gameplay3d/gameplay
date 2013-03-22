#ifndef AUDIO3DSAMPLE_H_
#define AUDIO3DSAMPLE_H_

#include "gameplay.h"
#include "Sample.h"
#include "FirstPersonCamera.h"

using namespace gameplay;

/**
 * Sample 3D audio within multiple sources and a listener bound to Nodes.
 */
class Audio3DSample: public Sample
{
public:

    Audio3DSample();
    
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
    bool drawScene(Node* node);

    void addSound(const std::string& file);

    void drawDebugText(int x = 0, int y = 0);

    void drawVector3(const char* str, const Vector3 vector, int x, int y);

    void loadGrid(Scene* scene);

private:

    FirstPersonCamera _fpCamera;
    std::map<std::string, Node*> _audioNodes;
    Font* _font;
    Scene* _scene;
    Node* _cubeNode;
    Gamepad* _gamepad;
    unsigned int _moveFlags;
    int _prevX;
    int _prevY;
    bool _buttonPressed;
};

#endif
