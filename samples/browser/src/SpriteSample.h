#ifndef SPRITESAMPLE_H_
#define SPRITESAMPLE_H_

#include "gameplay.h"
#include "Sample.h"

using namespace gameplay;

/**
 * Sample drawing sprites in a scene with the Sprite class.
 */
class SpriteSample : public Sample
{
public:
    
    enum Movement
    {
        WALK_FORWARD    = (1 << 0),
        WALK_BACKWARD   = (1 << 1),
    };

    SpriteSample();

    void keyEvent(Keyboard::KeyEvent evt, int key);
    
    void touchEvent(Touch::TouchEvent evt, int x, int y, unsigned int contactIndex);

protected:

    void initialize();

    void finalize();

    void update(float elapsedTime);

    void render(float elapsedTime);

private:
    
    bool drawScene(Node* node);
    
    float getTime() const;

    Font* _font;
    Scene* _scene;
    Node* _cameraNode;
    Sprite* _playerSprite;
    Node* _playerNode;
    Animation* _playerAnimation;
    int _playerMovement;
    
};

#endif
