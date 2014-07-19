#include "Base.h"
#include "AIState.h"
#include "AIStateMachine.h"

GP_SCRIPT_EVENTS();
GP_SCRIPT_EVENT(enter, "<AIAgent><AIState>");
GP_SCRIPT_EVENT(exit, "<AIAgent><AIState>");
GP_SCRIPT_EVENT(update, "<AIAgent><AIState>f");

namespace gameplay
{

AIState* AIState::_empty = NULL;

AIState::AIState(const char* id)
    : _id(id), _listener(NULL)
{
    GP_REGISTER_SCRIPT_EVENTS();
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

    fireScriptEvent<void>(SCRIPT_EVENT_enter, stateMachine->getAgent(), this);
}

void AIState::exit(AIStateMachine* stateMachine)
{
    if (_listener)
        _listener->stateExit(stateMachine->getAgent(), this);

    fireScriptEvent<void>(SCRIPT_EVENT_exit, stateMachine->getAgent(), this);
}

void AIState::update(AIStateMachine* stateMachine, float elapsedTime)
{
    if (_listener)
        _listener->stateUpdate(stateMachine->getAgent(), this, elapsedTime);

    fireScriptEvent<void>(SCRIPT_EVENT_update, stateMachine->getAgent(), this, elapsedTime);
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
