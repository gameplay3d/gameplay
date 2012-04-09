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
     * @see Game::keyEvent
     */
    void keyEvent(Keyboard::KeyEvent evt, int key);

    /**
     * @see Game::touchEvent
     */
    void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

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

    bool drawScene(Node* node, void* cookie);

    void drawFrameRate(Font* font, const Vector4& color, unsigned int x, unsigned int y, unsigned int fps);

    void drawSplash(void* param);

    /**
     * Creates a grid model.
     * 
     * @param lineCount The number of lines wide to create the grid.
     * 
     * @return A newly created model that contains the mesh.
     */
    Model* createGridModel(unsigned int lineCount = 41);

    Font* _font;
    Scene* _scene;
    Node* _modelNode;
    bool _touched;
    int _touchX;
};

#endif
