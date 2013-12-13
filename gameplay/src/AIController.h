#ifndef AICONTROLLER_H_
#define AICONTROLLER_H_

#include "AIAgent.h"
#include "AIMessage.h"

namespace gameplay
{

/**
 * Defines and facilitates the state machine execution and message passing
 * between AI objects in the game. This class is generally not interfaced
 * with directly.
 */
class AIController
{
    friend class Game;
    friend class Node;

public:

    /**
     * Routes the specified message to its intended recipient(s).
     *
     * Messages are arbitrary packets of data that are sent either to a single or to multiple
     * recipients in the game.
     *
     * Once the specified message has been delivered, it is automatically destroyed by the AIController.
     * For this reason, AIMessage pointers should NOT be held or explicitly destroyed by any code after
     * they are sent through the AIController.
     *
     * @param message The message to send.
     * @param delay The delay (in milliseconds) to wait before sending the message.
     */
    void sendMessage(AIMessage* message, float delay = 0);

    /**
     * Searches for an AIAgent that is registered with the AIController with the specified ID.
     *
     * @param id ID of the agent to find.
     *
     * @return The first agent matching the specified ID, or NULL if no matching agent could be found.
     */
    AIAgent* findAgent(const char* id) const;

private:

    /**
     * Constructor.
     */
    AIController();

    /**
     * Destructor.
     */
    ~AIController();

    /**
     * Hidden copy constructor.
     */
    AIController(const AIController&);

    /**
     * Hidden copy assignment operator.
     */
    AIController& operator=(const AIController&);

    /**
     * Called during startup to initialize the AIController.
     */
    void initialize();

    /**
     * Called during shutdown to finalize the AIController.
     */
    void finalize();

    /**
     * Pauses the AIController.
     */
    void pause();

    /**
     * Resumes the AIController.
     */
    void resume();

    /**
     * Called each frame to update the AIController.
     *
     * @param elapsedTime The elapsed time, in milliseconds.
     */
    void update(float elapsedTime);

    void addAgent(AIAgent* agent);

    void removeAgent(AIAgent* agent);

    bool _paused;
    AIMessage* _firstMessage;
    AIAgent* _firstAgent;

};

}

#endif
