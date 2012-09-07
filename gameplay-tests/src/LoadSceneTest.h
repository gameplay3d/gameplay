#ifndef LoadSceneTest_H_
#define LoadSceneTest_H_

#include "gameplay.h"
#include "Test.h"

using namespace gameplay;

class LoadSceneTest : public Test
{
public:
    LoadSceneTest();
    void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);
    void keyEvent(Keyboard::KeyEvent evt, int key);

protected:
    void initialize();
    void finalize();
    void update(float elapsedTime);
    void render(float elapsedTime);

private:
    bool drawScene(Node* node);
    bool bindLights(Node* node);

private:
    Font* _font;
    Scene* _scene;
    Node* _lightNode;
    bool _wireFrame;
};

#endif
