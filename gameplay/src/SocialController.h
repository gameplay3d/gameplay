#ifndef SOCIALCONTROLLER_H_
#define SOCIALCONTROLLER_H_

#include "Properties.h"
#include "SocialSession.h"
#include "SocialSessionListener.h"

namespace gameplay
{

/**
 * Defines a class for controlling various back-end support for social capabilities.
 *
 * Back-end social providers include Friends, Leaderboards, Achievements and Multiplayer.
 * Social back-ends supported are Scoreloop, GameCenter and Google Play Game Services and Facebook.
 * Configuration for these are specified in game.config:

  @verbatim
    social
    {
         provider = Scoreloop
         id  = d346c484-12aa-49a2-a0a0-de2f87492d72
         secret = aAa+DehBfyGO/CYaE3nWomgu7SIbWFczUih+Qwf3/n7u0y3nyq5Hag==
         version = "1.0"
         language = "en"
         currency = "ASC"
         leaderboard_mappings
         {
             // Format: leaderboardId = provider value
             easy = 0
             medium = 1
             hard = 2
         }
    }

 */
class SocialController
{
    friend class Game;

public:

    /**
     * Asyncrhonously authenticates a new user session.
     *
     * @param listener The listener
     */
    void authenticate(SocialSessionListener* listener);

private:

    /**
     * Constructor.
     */
    SocialController();

    /**
     * Destructor.
     */
    ~SocialController();

    /**
     * Callback for when the controller is initialized.
     */
    void initialize();

    /*
     * Callback for when the controller is finalized.
     */
    void finalize();

    /**
     * Callback for when the controller receives a frame update event.
     */
    void update(float elapsedTime);

private:

    SocialSession* _session;

};

}

#endif

