#ifndef CREATESCENESAMPLE_H_
#define CREATESCENESAMPLE_H_

#include "gameplay.h"
#include "Sample.h"

using namespace gameplay;

/**
 * Samples programattically contructing a scene.
 */
class CreateSceneSample : public Sample
{
public:

    CreateSceneSample();

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
