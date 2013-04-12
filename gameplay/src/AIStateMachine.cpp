#include "Base.h"
#include "AIStateMachine.h"
#include "AIAgent.h"
#include "AIMessage.h"
#include "Game.h"

namespace gameplay
{

AIStateMachine::AIStateMachine(AIAgent* agent)
    : _agent(agent)
{
    GP_ASSERT(agent);
    if (AIState::_empty)
        AIState::_empty->addRef();
    else
        AIState::_empty = new AIState("");
    _currentState = AIState::_empty;
}

AIStateMachine::~AIStateMachine()
{
    // Release all states
    for (std::list<AIState*>::iterator itr = _states.begin(); itr != _states.end(); ++itr)
    {
        (*itr)->release();
    }

    if (AIState::_empty)
    {
        if (AIState::_empty->getRefCount() == 1)
        {
            SAFE_RELEASE(AIState::_empty);
        }
        else
        {
            AIState::_empty->release();
        }
    }
}

AIAgent* AIStateMachine::getAgent() const
{
    return _agent;
}

AIState* AIStateMachine::addState(const char* id)
{
    AIState* state = AIState::create(id);
    _states.push_back(state);
    return state;
}

void AIStateMachine::addState(AIState* state)
{
    state->addRef();
    _states.push_back(state);
}

void AIStateMachine::removeState(AIState* state)
{
    std::list<AIState*>::iterator itr = std::find(_states.begin(), _states.end(), state);
    if (itr != _states.end())
    {
        _states.erase(itr);
        state->release();
    }
}

AIState* AIStateMachine::getState(const char* id) const
{
    GP_ASSERT(id);

    AIState* state;
    for (std::list<AIState*>::const_iterator itr = _states.begin(); itr != _states.end(); ++itr)
    {
        state = (*itr);

        if (strcmp(id, state->getId()) == 0)
            return state;
    }

    return NULL;
}

AIState* AIStateMachine::getActiveState() const
{
    return _currentState;
}

bool AIStateMachine::hasState(AIState* state) const
{
    GP_ASSERT(state);

    return (std::find(_states.begin(), _states.end(), state) != _states.end());
}

AIState* AIStateMachine::setState(const char* id)
{
    AIState* state = getState(id);
    if (state)
        sendChangeStateMessage(state);
    return state;
}

bool AIStateMachine::setState(AIState* state)
{
    if (hasState(state))
    {
        sendChangeStateMessage(state);
        return true;
    }

    return false;
}

void AIStateMachine::sendChangeStateMessage(AIState* newState)
{
    AIMessage* message = AIMessage::create(0, _agent->getId(), _agent->getId(), 1);
    message->_messageType = AIMessage::MESSAGE_TYPE_STATE_CHANGE;
    message->setString(0, newState->getId());
    Game::getInstance()->getAIController()->sendMessage(message);
}

void AIStateMachine::setStateInternal(AIState* state)
{
    GP_ASSERT(hasState(state));

    // Fire the exit event for the current state
    _currentState->exit(this);

    // Set the new state
    _currentState = state;

    // Fire the enter event for the new state
    _currentState->enter(this);
}

void AIStateMachine::update(float elapsedTime)
{
    _currentState->update(this, elapsedTime);
}

}
