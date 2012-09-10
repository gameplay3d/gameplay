#ifndef Audio3DTest_H_
#define Audio3DTest_H_

#include "gameplay.h"
#include "Test.h"
#include "FirstPersonCamera.h"

using namespace gameplay;

class Audio3DTest: public Test
{
public:
    Audio3DTest();
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

    /**
     * Adds a sound to the scene at the current camera position.
     */
    void addSound(const std::string& file);

    void drawDebugText();

    /**
     * Draws a Vector3 at the given screen coordinates.
     */
    void drawVector3(const char* str, const Vector3 vector, int x, int y);

private:

    typedef std::map<std::string, Node*> NodeMap;

    FirstPersonCamera _fpCamera;
    std::vector<std::string> _soundFiles;
    NodeMap _audioNodes;

    Font* _font;
    Scene* _scene;
    Node* _cubeNode;
    unsigned int _moveFlags;
    int _prevX;
    int _prevY;
};

#endif
