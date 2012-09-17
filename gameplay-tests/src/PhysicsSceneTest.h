#ifndef PHYSICSSCENETEST_H_
#define PHYSICSSCENETEST_H_

#include "gameplay.h"
#include "Test.h"

using namespace gameplay;

/**
 * Test loading a physics scene from .scene file with .physics bindings
 */
class PhysicsSceneTest : public Test
{
public:

    PhysicsSceneTest();

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

    void fireProjectile(const Ray& ray);

    void incrementDebugDraw();

    enum ObjectsTypes
    {
        SPHERE = 0, 
        BOX = 1,
        CAPSULE = 2
    };

    Font* _font;
    Scene* _scene;
    Node* _lightNode;
    int _objectType;
    bool _throw;
    int _drawDebug;
    bool _wireFrame;
    std::vector<const char*> _collisionObjectPaths;
    std::vector<const char*> _nodeIds;
    std::vector<Vector4> _colors;
};

#endif
