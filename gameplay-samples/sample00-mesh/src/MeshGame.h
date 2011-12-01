#ifndef MESHGAME_H_
#define MESHGAME_H_

#include "gameplay.h"

using namespace gameplay;

/**
 * Sample game for rendering a scene with a model/mesh.
 */
class MeshGame: public Game
{
public:

    /**
     * Constructror.
     */
    MeshGame();

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

    void drawScene(Node* node, long cookie);

    void drawFrameRate(Font* font, const Vector4& color, unsigned int x, unsigned int y, unsigned int fps);

    Font* _font;
    Scene* _scene;
    Node* _modelNode;
    bool _touched;
    int _touchX;
};

#endif
