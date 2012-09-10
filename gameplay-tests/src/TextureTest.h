#ifndef TextureTest_H_
#define TextureTest_H_

#include "gameplay.h"
#include "Test.h"

using namespace gameplay;

class TextureTest : public Test
{
public:
    TextureTest();
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
    float _zOffset;
    std::list<Font::Text*> _text;
};

#endif
