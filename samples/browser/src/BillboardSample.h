#ifndef BILLBOARDSAMPLE_H_
#define BILLBOARDSAMPLE_H_

#include "gameplay.h"
#include "Sample.h"
#include "FirstPersonCamera.h"

using namespace gameplay;

/**
 * Sample for 3D billboarding with cloud sprites
 */
class BillboardSample: public Sample
{
public:

    BillboardSample();
    
    void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);
    
    void keyEvent(Keyboard::KeyEvent evt, int key);
   
    bool mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta);

    void gamepadEvent(Gamepad::GamepadEvent evt, Gamepad* gamepad);

protected:

    void initialize();

    void finalize();

    void update(float elapsedTime);

    void render(float elapsedTime);

private:

	void loadGround();

    void loadBillboards();

    bool drawScene(Node* node);

private:

    FirstPersonCamera _camera;
	std::vector<Node*> _billboards;
    Font* _font;
	Scene* _scene;
	Model* _ground;
    Gamepad* _gamepad;
    unsigned int _moveFlags;
    int _prevX;
    int _prevY;
    bool _buttonPressed;
};

#endif
