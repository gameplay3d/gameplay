#ifndef GestureTest_H_
#define GestureTest_H_

#include "gameplay.h"
#include "Test.h"

using namespace gameplay;

/**
 * Tests gestures.
 */
class GestureTest : public Test
{
public:

    GestureTest();

    void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

    void gestureSwipeEvent(int x, int y, int direction);
    
    void gesturePinchEvent(int x, int y, float scale);
    
    void gestureTapEvent(int x, int y);

protected:

    void initialize();

    void finalize();

    void update(float elapsedTime);

    void render(float elapsedTime);

private:

    Font* _font;
    std::list<std::string> _eventLog;
};

#endif
