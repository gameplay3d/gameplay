/*
 * CharacterGame.h
 */

#ifndef CHARACTERGAME_H_
#define CHARACTERGAME_H_

#include "gameplay.h"

using namespace gameplay;

/**
 * This is a mesh demo game for rendering Mesh.
 */
class CharacterGame: public Game
{

public:

    /**
     * Constructor.
     */
    CharacterGame();

    /**
     * Destructor.
     */
    virtual ~CharacterGame();

    /**
     * @see Game::touch
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

    /**
     * Draws the model if the node contains a model.
     */
    void drawModel(Node* node, long cookie);

    /**
     * Gets the first model node. This is intended to be used with visit.
     */
    void getModelNode(Node* node, long cookie);

    /**
     * Creates the default camera.
     */
    void createDefaultCamera(Scene* scene);

    /**
     * Loads the characters animations.
     */
    void loadCharacterAnimations();

    Scene* _scene;
    Font* _font;
    Node* _modelNode;
    Animation* _animation;
    unsigned int _animationState;
    int _rotateX;
};

#endif
