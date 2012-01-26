#include "CharacterController.h"

namespace gameplay
{

CharacterController::CharacterController(Node* node)
    : _node(node), _currentMovement(NULL)
{
    node->addRef();
}

CharacterController::~CharacterController()
{
    SAFE_RELEASE(_node);
}

void CharacterController::addMovement(const char* name, AnimationClip* clip, const Vector3& velocity)
{
    Movement m;
    m.clip = clip;
    m.velocity = velocity;
    _movements[name] = m;
}

void CharacterController::setMovement(const char* name)
{
    std::map<const char*, Movement>::iterator itr = _movements.find(name);
    if (itr != _movements.end())
    {
        _currentMovement = &((*itr).second);
    }
}

void CharacterController::stop()
{
    _currentMovement = NULL;
}

void CharacterController::update(long elapsedTime)
{
    // TODO
}

}
