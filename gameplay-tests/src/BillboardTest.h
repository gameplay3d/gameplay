#ifndef BILLBOARDTEST_H_
#define BILLBOARDTEST_H_

#include "gameplay.h"
#include "Test.h"
#include "FirstPersonCamera.h"

using namespace gameplay;

/**
 * Test for 3D billboarding with cloud sprites
 */
class BillboardTest: public Test
{
public:

    BillboardTest();
    
    void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);
    
    void keyEvent(Keyboard::KeyEvent evt, int key);
   
    bool mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta);

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
