#ifndef TEXTURETEST_H_
#define TEXTURETEST_H_

#include "gameplay.h"
#include "Test.h"

using namespace gameplay;

/**
 * Test the creating and drawing with textures with different modes.
 */
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

    Font* _font;
    Scene* _scene;
    float _zOffset;
    std::list<Font::Text*> _text;
};

#endif
