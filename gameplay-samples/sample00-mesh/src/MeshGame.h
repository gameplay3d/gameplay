/*
 * MeshGame.h
 */

#ifndef MESHGAME_H_
#define MESHGAME_H_

#include "gameplay.h"
#include "gameplay-main.h"
using namespace gameplay;


/**
 * This is a mesh demo game for rendering Mesh.
 */
class MeshGame: public Game
{

public:

    /**
     * Destructor.
     */
    virtual ~MeshGame();

    /**
     * Touch event handler.
     */
    void touch(int x, int y, int touchEven);

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
    void getModelNode(Node* node, long cookie);

    void createDefaultCamera(Scene* scene);

    Font* _font;
    Scene* _scene;
    Node* _modelNode;
    int _prevX, _prevY;
    bool _touched;
};

#endif
