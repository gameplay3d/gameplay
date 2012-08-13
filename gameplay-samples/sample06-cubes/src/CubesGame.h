#ifndef TEMPLATEGAME_H_
#define TEMPLATEGAME_H_

#include "gameplay.h"

using namespace gameplay;

/**
 * Main game class.
 */
class CubesGame: public Game, Control::Listener
{
public:

    /**
     * Constructor.
     */
    CubesGame();

	void keyEvent(Keyboard::KeyEvent evt, int key);
    void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);
    bool mouseEvent(Mouse::MouseEvent evt, int x, int y, int wheelDelta);
    void controlEvent(Control* control, EventType evt);

protected:

    /**
     * @see Game::initialize
     */
    void initialize();

    /**
     * @see Game::finalize
     */
    void finalize();

    /**
     * @see Game::update
     */
    void update(float elapsedTime);

    /**
     * @see Game::render
     */
    void render(float elapsedTime);

private:
    //dimensions of the grid
    int _grid;
    //scale of the cubes relative to their cell size
    float _scale;
    //spacing of the cells
    float _disperse;
    
    Matrix matProj;
    void setProjMatrix( bool ortho );
    
    void readForm();
};

#endif
