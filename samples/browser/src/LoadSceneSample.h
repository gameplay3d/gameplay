#ifndef LOADSCENESAMPLE_H_
#define LOADSCENESAMPLE_H_

#include "gameplay.h"
#include "Sample.h"

using namespace gameplay;

/**
 * Sample for loading a scene from a .scene file.
 */
class LoadSceneSample : public Sample
{
public:

    LoadSceneSample();

    void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

    void keyEvent(Keyboard::KeyEvent evt, int key);

protected:

    void initialize();

    void finalize();

    void update(float elapsedTime);

    void render(float elapsedTime);

private:

    bool drawScene(Node* node);

    bool bindLights(Node* node);

    Font* _font;
    Scene* _scene;
    Node* _lightNode;
    bool _wireFrame;
};

#endif
