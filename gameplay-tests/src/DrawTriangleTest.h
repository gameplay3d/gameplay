#ifndef DrawTriangleTest_H_
#define DrawTriangleTest_H_

#include "gameplay.h"
#include "Test.h"

using namespace gameplay;

class DrawTriangleTest : public Test
{
public:
    DrawTriangleTest();
    void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

protected:
    void initialize();
    void finalize();
    void update(float elapsedTime);
    void render(float elapsedTime);

private:
    Font* _font;
    Model* _model;
    float _spinDirection;
    Matrix _worldViewProjectionMatrix;
};

#endif
