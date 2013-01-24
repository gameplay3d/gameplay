#ifndef CREATESCENETEST_H_
#define CREATESCENETEST_H_

#include "gameplay.h"
#include "Test.h"

using namespace gameplay;

/**
 * Tests programattically contructing a scene.
 */
class CreateSceneTest : public Test
{
public:

    CreateSceneTest();

    void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

protected:

    void initialize();

    void finalize();

    void update(float elapsedTime);

    void render(float elapsedTime);

private:

    bool drawScene(Node* node);

    Font* _font;
    Scene* _scene;
    Node* _cubeNode;
};

#endif
