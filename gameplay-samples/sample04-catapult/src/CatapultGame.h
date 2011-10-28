/*
 * CatapultGame.h
 */

#ifndef CHARACTERGAME_H_
#define CHARACTERGAME_H_

#include "gameplay.h"
#include "gameplay-main.h"
using namespace gameplay;


/**
 * This is a demo game that demonstrates physics, 
 * particle systems, visibility culling and skybox rendering.
 */
class CatapultGame: public Game
{

public:

    /**
     * Destructor.
     */
    virtual ~CatapultGame();

    /**
     * Touch event handler.
     */
    void touch(int x, int y, int touchEvent);

    /**
     * Key press event handler.
     */
    void keyPress(int key, int keyEvent);

protected:

    /**
     * @see Game::initialize
     */
    void initialize();

    /**
     * @see Game::update
     */
    void update(long elapsedTime);

    /**
     * @see Game::render
     */
    void render(long elapsedTime);

    /**
     * @see Game::finalize
     */
    void finalize();

private:

    void visitNode(Node* node, long cookie);

    Scene* _scene;
    Font* _font;

    Node* _ground;
	Node* _walls;
	Node* _wallWest;
	Node* _wallEast;
	Node* _wallSouth;
	Node* _wallNorth;
	Node* _dieBottom;
    Node* _dieTop;
    Node* _rope;
	Node* _basketball;
	Node* _volleyball;

    float _scale;
};

#endif
