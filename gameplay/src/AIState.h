#ifndef AISTATE_H_
#define AISTATE_H_

#include "Ref.h"

namespace gameplay
{

class AIAgent;
class AIStateMachine;

/**
 * Defines a single state in an AIStateMachine.
 *
 * An AIState encapsulates a state and unit of work within an AI
 * state machine. Events can be programmed or scripted when the
 * state is entered, exited and each frame/tick in its update event.
 */
class AIState : public Ref
{
    friend class AIStateMachine;

public:

    /**
     * Interface for listening to AIState events.
     */
    class Listener
    {
    public:

        /**
         * Virtual destructor.
         */
        virtual ~Listener();

        /** 
         * Called when a state is entered.
         *
         * @param agent The AIAgent this state event is for.
         * @param state The state that was entered.
         */
        virtual void stateEnter(AIAgent* agent, AIState* state);

        /**
         * Called when a state is exited.
         *
         * @param agent The AIAgent this state event is for.
         * @param state The state that was exited.
         */
        virtual void stateExit(AIAgent* agent, AIState* state);

        /**
         * Called once per frame when for a state when it is active.
         *
         * This method is normally where the logic for a state is implemented.
         *
         * @param agent The AIAgent this state event is for.
         * @param state The active AIState.
         * @param elapsedTime The elapsed time, in milliseconds.
         */
        virtual void stateUpdate(AIAgent* agent, AIState* state, float elapsedTime);
    };

    /**
     * Creates a new AISTate.
     *
     * @param id The ID of the new AIState.
     *
     * @return The new AIState.
     * @script{create}
     */
    static AIState* create(const char* id);

    /**
     * Returns the ID of this state.
     *
     * @return The state ID.
     */
    const char* getId() const;

    /**
     * Sets a listener to dispatch state events to.
     * 
     * @param listener Listener to dispatch state events to, or NULL to disable event dispatching.
     */
    void setListener(Listener* listener);

private:

    /**
     * Constructs a new AIState.
     */
    AIState(const char* id);

    /**
     * Destructor.
     */
    ~AIState();

    /**
     * Hidden copy constructor.
     */
    AIState(const AIState&);

    /**
     * Hidden copy assignment operator.
     */
    AIState& operator=(const AIState&);

    /**
     * Called by AIStateMachine when this state is being entered.
     */
    void enter(AIStateMachine* stateMachine);

    /**
     * Called by AIStateMachine when this state is being exited.
     */
    void exit(AIStateMachine* stateMachine);

    /**
     * Called by AIStateMachine once per frame to update this state when it is active.
     */
    void update(AIStateMachine* stateMachine, float elapsedTime);

    std::string _id;
    Listener* _listener;

    // The default/empty state.
    static AIState* _empty;

};

}

#endif
