#ifndef SPRITEBATCHSAMPLE_H_
#define SPRITEBATCHSAMPLE_H_

#include "gameplay.h"
#include "Sample.h"

using namespace gameplay;

/**
 * Sample drawing many quad sprites with SpriteBatch.
 */
class SpriteBatchSample : public Sample
{
public:

    SpriteBatchSample();

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
