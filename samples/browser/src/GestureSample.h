#ifndef GESTURESAMPLE_H_
#define GESTURESAMPLE_H_

#include "gameplay.h"
#include "Sample.h"

using namespace gameplay;

/**
 * Samples gestures.
 */
class GestureSample : public Sample
{
public:

    GestureSample();

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
