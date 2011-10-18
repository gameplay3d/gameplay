/*
 * CharacterGame.h
 */

#ifndef CHARACTERGAME_H_
#define CHARACTERGAME_H_

#include "gameplay.h"
#include "gameplay-main.h"
using namespace gameplay;

#define ANIMATION_ID    "boyShape-lib-skin"

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
     * Touch event handler.
     */
    void touch(int x, int y, int touchEvent);

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

    /**
     * Draws the model if the node contains a model.
     */
    void drawModel(Node* node, long cookie);

    /**
     * Returns the first model node. This is intended to be used with visit.
     */
    void getModelNode(Node* node, long cookie);

    void createDefaultCamera(Scene* scene);

    void loadCharacterAnimations();

    Scene* _scene;
    Font* _font;
    Node* _modelNode;
    AnimationClip** _clips;
    unsigned int _animationState;
};

#endif
