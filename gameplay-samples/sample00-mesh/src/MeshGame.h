#ifndef MESHGAME_H_
#define MESHGAME_H_

#include "gameplay.h"

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
    void touch(int x, int y, int touchEvent);

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
    void update(long elapsedTime);

    /**
     * @see Game::render
     */
    void render(long elapsedTime);

private:

    void visitNode(Node* node, long cookie);

    void drawFrameRate(Font* font, const Vector4& color, unsigned int x, unsigned int y, unsigned int fps);

    Font* _font;
    Scene* _scene;
    Node* _modelNode;
    int _touchX;
    bool _touched;
};

#endif
