#ifndef CreateSceneTest_H_
#define CreateSceneTest_H_

#include "gameplay.h"
#include "Test.h"

using namespace gameplay;

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

private:
    Font* _font;
    Scene* _scene;
};

#endif
