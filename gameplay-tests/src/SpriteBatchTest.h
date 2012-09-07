#ifndef SpriteBatchTest_H_
#define SpriteBatchTest_H_

#include "gameplay.h"
#include "Test.h"

using namespace gameplay;

class SpriteBatchTest : public Test
{
public:
    SpriteBatchTest();
    void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

protected:
    void initialize();
    void finalize();
    void update(float elapsedTime);
    void render(float elapsedTime);

private:
    Font* _font;
    SpriteBatch* _spriteBatch;
    Matrix _worldViewProjectionMatrix;
};

#endif
