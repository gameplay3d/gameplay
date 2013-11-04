#ifndef SCENERENDERERSAMPLE_H_
#define SCENERENDERERSAMPLE_H_

#include "gameplay.h"
#include "Sample.h"

using namespace gameplay;

/**
 * Sample to show case the SceneRenderer
 */
class SceneRendererSample : public Sample
{
public:

    SceneRendererSample();

protected:
    void initialize();
    void finalize();
    void update(float elapsedTime);
    void render(float elapsedTime);
    void touchEvent(Touch::TouchEvent event, int x, int y, unsigned int contactIndex);
    void keyEvent(Keyboard::KeyEvent evt, int key);

private:

    bool initializeMaterials(Node* node);

    Font* _font;
    Scene* _scene;
    VisibleSet* _visibleSet;
    SceneRenderer* _renderer;
};

#endif
