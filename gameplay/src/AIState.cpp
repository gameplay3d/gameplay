#include "Base.h"
#include "AIState.h"
#include "AIStateMachine.h"

namespace gameplay
{

AIState* AIState::_empty = NULL;

AIState::AIState(const char* id)
    : _id(id), _listener(NULL)
{
    addScriptEvent("enter", "<AIAgent><AIState>");
    addScriptEvent("exit", "<AIAgent><AIState>");
    addScriptEvent("update", "<AIAgent><AIState>f");
}

AIState::~AIState()
{
}

AIState* AIState::create(const char* id)
{
    return new AIState(id);
}

const char* AIState::getId() const
{
    return _id.c_str();
}

void AIState::setListener(Listener* listener)
{
    _listener = listener;
}

void AIState::enter(AIStateMachine* stateMachine)
{
    if (_listener)
        _listener->stateEnter(stateMachine->getAgent(), this);

    fireScriptEvent<void>("enter", stateMachine->getAgent(), this);
}

void AIState::exit(AIStateMachine* stateMachine)
{
    if (_listener)
        _listener->stateExit(stateMachine->getAgent(), this);

    fireScriptEvent<void>("exit", stateMachine->getAgent(), this);
}

void AIState::update(AIStateMachine* stateMachine, float elapsedTime)
{
    if (_listener)
        _listener->stateUpdate(stateMachine->getAgent(), this, elapsedTime);

    fireScriptEvent<void>("update", stateMachine->getAgent(), this, elapsedTime);
}

AIState::Listener::~Listener()
{
}

void AIState::Listener::stateEnter(AIAgent* agent, AIState* state)
{
}

void AIState::Listener::stateExit(AIAgent* agent, AIState* state)
{
}

void AIState::Listener::stateUpdate(AIAgent* agent, AIState* state, float elapsedTime)
{
}

}
