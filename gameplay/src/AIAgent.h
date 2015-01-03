#ifndef AIAGENT_H_
#define AIAGENT_H_

#include "Ref.h"
#include "AIStateMachine.h"
#include "AIMessage.h"

namespace gameplay
{

class Node;

/**
 * Defines an AI agent that can be added to nodes in a scene.
 *
 * Agents represent a unit of intelligence in a game and can be used
 * to program logic for a character or object in a game, using constructs
 * such as state machines. By default, an AIAgent has an empty state 
 * machine.
 */
class AIAgent : public Ref
{
    friend class Node;
    friend class AIState;
    friend class AIController;

public:

    /**
     * Interface for listening to AIAgent events.
     */
    class Listener
    {
    public:

        /**
         * Virtual destructor.
         */
        virtual ~Listener() { };

        /**
         * Called when a new message is sent to the AIAgent.
         *
         * Both global/broadcast messages and messages sent explicitly to the
         * AIAgent are sent through this method. Returning true from this method
         * will mark the message as handled and it will dispose of the message
         * and prevent any other possible recipients from receiving the message.
         * Alternatively, returning false allows the message to continue being
         * routed though the AI system.
         *
         * @param message The message received.
         *
         * @return true to mark the message as handled, false otherwise.
         */
        virtual bool messageReceived(AIMessage* message) = 0;
    };

    /**
     * Creates a new AIAgent.
     *
     * @return A new AIAgent.
     * @script{create}
     */
    static AIAgent* create();

    /**
     * Returns the identifier for the AIAgent.
     *
     * This method simply returns the ID of the Node which this AIAgent
     * is bound to. If this AIAgent is not bound to a Node, this method
     * returns an empty string.
     *
     * @return The identifier for the agent.
     */
    const char* getId() const;

    /**
     * Returns the Node this AIAgent is assigned to.
     *
     * @return The Node this agent is assigned to.
     */
    Node* getNode() const;

    /**
     * Returns the state machine for the AIAgent.
     *
     * @return The agent's state machine.
     */
    AIStateMachine* getStateMachine();

    /**
     * Determines if this AIAgent is currently enabled.
     *
     * Agents are always disabled until they have been associated
     * with a valid Node though Node::setAgent(AIAgent*). In addition,
     * an AIAgent can be explicitly enabled or disabled using the
     * setEnabled(bool) method.
     *
     * @return true if the agent is enabled, false otherwise.
     */
    bool isEnabled() const;

    /**
     * Sets whether this AIAgent is enabled.
     *
     * By default, AIAgents are enabled and they can receive messages and state
     * changes. When disabled, AIAgents stop receiving messages and their state
     * machines are halted until they are re-enabled.
     *
     * @param enabled true if the AIAgent should be enabled, false otherwise.
     */
    void setEnabled(bool enabled);

    /**
     * Sets an event listener for this AIAgent.
     *
     * @param listener The new AIAgent listener, or NULL to remove any existing listener.
     */
    void setListener(Listener* listener);

private:

    /**
     * Constructor.
     */
    AIAgent();

    /**
     * Destructor.
     *
     * Hidden, use SAFE_RELEASE instead.
     */
    virtual ~AIAgent();

    /**
     * Hidden copy constructor.
     */
    AIAgent(const AIAgent&);

    /**
     * Hidden copy assignment operator.
     */
    AIAgent& operator=(const AIAgent&);
    
    /**
     * Set the node this agent is attached to.
     */
    void setNode(Node* node);

    /**
     * Called by the AIController to process a message for the AIAgent.
     *
     * @param message The message to be processed.
     *
     * @return true if the message was handled, false otherwise.
     */
    bool processMessage(AIMessage* message);

    /**
     * Called once per frame by the AIController to update the agent.
     */
    void update(float elapsedTime);

    AIStateMachine* _stateMachine;
    Node* _node;
    bool _enabled;
    Listener* _listener;
    AIAgent* _next;

};

}

#endif
