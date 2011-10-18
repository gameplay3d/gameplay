/*
 * SpaceshipGame.h
 */

#ifndef SPACESHIPGAME_H_
#define SPACESHIPGAME_H_

#include "gameplay.h"
#include "gameplay-main.h"
using namespace gameplay;

/**
 * Spaceship game.
 */
class SpaceshipGame : public Game
{
public:

    SpaceshipGame();

    virtual ~SpaceshipGame();

    void initialize();

    void finalize();
    
    void update(long elapsedTime);

    void render(long elapsedTime);

    void touch(int x, int y, int touchEvent);

private:

    void visitNode(Node* node, long cookie);
    void initSpaceship();
    void initEnvironment();
    void setLightMaterialParams(Material* material);
    void handleCollisions(float t);
    void updateCamera();
    void resetGame();

    // Scene variables
    Scene* _scene;
    Node* _cameraNode;
    Node* _shipGroupNode;
    Node* _shipNode;
    Node* _propulsionNode;
    Node* _glowNode;
    Vector3 _initialShipPos;
    Quaternion _initialShipRot;
    Vector3 _initialCameraPos;

    // Font for text rendering
    Font* _font;

    // Ship physics variables
    Vector2 _velocity;
    Vector2 _acceleration;
    Vector2 _force;
    float _throttle;
    float _shipTilt;

    // State variables
    bool _finished;
    long _finishedTime;
    bool _pushing;
    Vector2 _pushPoint;
    
    // Game time in seconds
    float _time;

    // Frequently updated material parameters
    MaterialParameter* _glowDiffuseParameter;
    MaterialParameter* _shipSpecularParameter;

    // Space ship sound
    AudioSource* _spaceshipSound;

};

#endif
