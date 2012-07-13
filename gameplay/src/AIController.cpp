#include "Base.h"
#include "AIController.h"
#include "Game.h"

// TODO:
//
// 1) Is std::string OK for message sender/receiver?
// 2) Is AIMessage ok and is "dobule" ok for message parameters?
// 3) Design of creating and deleting message (AIController deletes them)??
// 4) Is setListener(Listener*) OK for AIState and AIStateMachine, or do we need addListener(Listener*)???

// TODO: Add a way to snif messages on AIController??

// TODO: only dispatch messages to agents that are in this list AND enabled. If not in the list, discard the message (and log) and if they are in the list and DISABLED, just hold on to the message until they are re-enabled.

namespace gameplay
{

AIController::AIController()
    : _paused(false), _firstMessage(NULL), _firstAgent(NULL)
{
}

AIController::~AIController()
{
}

void AIController::initialize()
{
}

void AIController::finalize()
{
    // Remove all agents
    AIAgent* agent = _firstAgent;
    while (agent)
    {
        AIAgent* temp = agent;
        agent = agent->_next;
        SAFE_RELEASE(temp);
    }
    _firstAgent = NULL;

    // Remove all messages
    AIMessage* message = _firstMessage;
    while (message)
    {
        AIMessage* temp = message;
        message = message->_next;
        SAFE_DELETE(temp);
    }
    _firstMessage = NULL;
}

void AIController::pause()
{
    _paused = true;
}

void AIController::resume()
{
    _paused = false;
}

void AIController::sendMessage(AIMessage* message, float delay)
{
    if (delay <= 0)
    {
        // Send instantly
        if (message->getReceiver() == NULL || strlen(message->getReceiver()) == 0)
        {
            // Broadcast message to all agents
            AIAgent* agent = _firstAgent;
            while (agent)
            {
                if (agent->processMessage(message))
                    break; // message consumed by this agent - stop bubbling
                agent = agent->_next;
            }
        }
        else
        {
            // Single recipient
            AIAgent* agent = findAgent(message->getReceiver());
            if (agent)
            {
                agent->processMessage(message);
            }
            else
            {
                GP_WARN("Failed to locate AIAgent for message recipient: %s", message->getReceiver());
            }
        }

        // Delete the message, since it is finished being processed
        SAFE_DELETE(message);
    }
    else
    {
        // Queue for later delivery
        if (_firstMessage)
            message->_next = _firstMessage;
        _firstMessage = message;
    }
}

void AIController::update(float elapsedTime)
{
    if (_paused)
        return;

    static Game* game = Game::getInstance();

    // Send all pending messages that have expired
    AIMessage* prevMsg = NULL;
    AIMessage* msg = _firstMessage;
    while (msg)
    {
        // If the message delivery time has expired, send it (this also deletes it)
        if (msg->getDeliveryTime() >= game->getGameTime())
        {
            // Link the message out of our list
            if (prevMsg)
                prevMsg->_next = msg->_next;

            AIMessage* temp = msg;
            msg = msg->_next;
            temp->_next = NULL;
            sendMessage(temp);
        }
        else
        {
            prevMsg = msg;
            msg = msg->_next;
        }
    }

    // Update all enabled agents
    AIAgent* agent = _firstAgent;
    while (agent)
    {
        if (agent->isEnabled())
            agent->update(elapsedTime);

        agent = agent->_next;
    }
}

void AIController::addAgent(AIAgent* agent)
{
    agent->addRef();

    if (_firstAgent)
        agent->_next = _firstAgent;

    _firstAgent = agent;
}

void AIController::removeAgent(AIAgent* agent)
{
    // Search our linked list of agents and link this agent out.
    AIAgent* prevAgent = NULL;
    AIAgent* itr = _firstAgent;
    while (itr)
    {
        if (itr == agent)
        {
            if (prevAgent)
                prevAgent->_next = agent->_next;
            else
                _firstAgent = agent->_next;

            agent->_next = NULL;
            agent->release();
            break;
        }

        prevAgent = itr;
        itr = itr->_next;
    }
}

AIAgent* AIController::findAgent(const char* id) const
{
    GP_ASSERT(id);

    AIAgent* agent = _firstAgent;
    while (agent)
    {
        if (strcmp(id, agent->getId()) == 0)
            return agent;

        agent = agent->_next;
    }

    return NULL;
}

}
