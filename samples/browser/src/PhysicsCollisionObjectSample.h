#ifndef PHYSICSCOLLISIONOBJECTSAMPLE_H_
#define PHYSICSCOLLISIONOBJECTSAMPLE_H_

#include "gameplay.h"
#include "Sample.h"

using namespace gameplay;

/**
 * Sample loading a physics scene from .scene file with .physics bindings
 */
class PhysicsCollisionObjectSample : public Sample, Control::Listener
{
public:

    PhysicsCollisionObjectSample();

    void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

    void keyEvent(Keyboard::KeyEvent evt, int key);

    void controlEvent(Control* control, EventType evt);

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

    void toggleWireframe();

    enum ObjectsTypes
    {
        SPHERE = 0, 
        BOX = 1,
        CAPSULE = 2,
        DUCK = 3
    };

    Font* _font;
    Scene* _scene;
    Node* _lightNode;
    Form* _form;
    int _objectType;
    bool _throw;
    int _drawDebug;
    bool _wireFrame;
    std::vector<const char*> _collisionObjectPaths;
    std::vector<const char*> _nodeIds;
    std::vector<const char*> _nodeNames;
    std::vector<Vector4> _colors;
};

#endif
