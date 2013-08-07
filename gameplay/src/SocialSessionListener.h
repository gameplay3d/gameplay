#ifndef SOCIALSESSIONLISTENER_H_
#define SOCIALSESSIONLISTENER_H_

#include "SocialPlayer.h"
#include "SocialAchievement.h"
#include "SocialScore.h"

namespace gameplay
{

class SocialSession;

/**
 * Defines a class for handling session responses to various session request.
 */
class SocialSessionListener
{
public:

    /**
     * Constructor
     */
    SocialSessionListener();

    /**
     * Constructor
     */
    SocialSessionListener(const SocialSessionListener& copy);

    /**
     * Destructor
     */
    virtual ~SocialSessionListener();

    /**
     * Response codes returned on listener events.
     */
    enum ResponseCode
    {
        SUCCESS = 0,
        ERROR_INITIALIZATION,
        ERROR_PENDING_RESPONSE,
        ERROR_INVALID_USER,
        ERROR_INVALID_ARG,
        ERROR_SERVER,
        ERROR_UNKNOWN
    };

    virtual void authenticateEvent(ResponseCode code, SocialSession* session);

    virtual void loadFriendsEvent(ResponseCode code, std::vector<SocialPlayer> friends);

    virtual void loadAchievementsEvent(ResponseCode code, std::vector<SocialAchievement> achievements);

    virtual void submitAchievementEvent(ResponseCode code);

    virtual void loadScoresEvent(ResponseCode code, std::vector<SocialScore> scores);

    virtual void submitScoreEvent(ResponseCode code);

    virtual void loadSavedDataEvent(ResponseCode code, std::string data);

    virtual void submitSavedDataEvent(ResponseCode code);

};

}


#endif
