#include "Base.h"
#include "AIState.h"
#include "AIAgent.h"
#include "AIStateMachine.h"
#include "Node.h"

namespace gameplay
{

AIState* AIState::_empty = NULL;

AIState::AIState(const char* id)
    : _id(id), _listener(NULL)
{
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

    Node* node = stateMachine->_agent->_node;
    if (node)
        node->fireScriptEvent<void>(GP_GET_SCRIPT_EVENT(Node, stateEnter), dynamic_cast<void*>(node), this);
}

void AIState::exit(AIStateMachine* stateMachine)
{
    if (_listener)
        _listener->stateExit(stateMachine->getAgent(), this);

    Node* node = stateMachine->_agent->_node;
    if (node)
        node->fireScriptEvent<void>(GP_GET_SCRIPT_EVENT(Node, stateExit), dynamic_cast<void*>(node), this);
}

void AIState::update(AIStateMachine* stateMachine, float elapsedTime)
{
    if (_listener)
        _listener->stateUpdate(stateMachine->getAgent(), this, elapsedTime);

    Node* node = stateMachine->_agent->_node;
    if (node)
        node->fireScriptEvent<void>(GP_GET_SCRIPT_EVENT(Node, stateUpdate), dynamic_cast<void*>(node), this, elapsedTime);
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
