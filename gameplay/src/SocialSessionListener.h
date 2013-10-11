#ifndef SOCIALSESSIONLISTENER_H_
#define SOCIALSESSIONLISTENER_H_

#include "SocialPlayer.h"
#include "SocialAchievement.h"
#include "SocialScore.h"
#include "SocialChallenge.h"

namespace gameplay
{

class SocialSession;

/**
 * Defines a class for handling session responses to various session request.
 *
 * @script{ignore}
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
        ERROR_CANCELLED,
        ERROR_NOT_SUPPORTED,
        ERROR_UNKNOWN
    };

    virtual void authenticateEvent(ResponseCode code, SocialSession* session);

    virtual void loadFriendsEvent(ResponseCode code, std::vector<SocialPlayer> friends);

    virtual void loadAchievementsEvent(ResponseCode code, std::vector<SocialAchievement> achievements);

    virtual void synchronizeAchievementEvent(ResponseCode code);

    virtual void submitAchievementEvent(ResponseCode code);

    virtual void awardAchievedEvent(ResponseCode code, const SocialAchievement &achievement);

    virtual void loadScoresEvent(ResponseCode code, std::vector<SocialScore> scores);

    virtual void submitScoreEvent(ResponseCode code);

    virtual void submitChallengeEvent(ResponseCode code, const SocialChallenge &challenge);

    virtual void startChallengeEvent(ResponseCode code, const SocialChallenge &challenge);

    virtual void replyToChallengeEvent(ResponseCode code);

    virtual void loadChallengesEvent(ResponseCode code, std::vector<SocialChallenge> challenges);

    virtual void loadSavedDataEvent(ResponseCode code, std::string data);

    virtual void submitSavedDataEvent(ResponseCode code);

    virtual void uiEvent(ResponseCode code, std::string errorMessage="");

    virtual void displayedLeaderboardEvent(ResponseCode code, std::string errorMessage="");

    virtual void displayedAchievementsEvent(ResponseCode code, std::string errorMessage="");

    virtual void displayedChallengesEvent(ResponseCode code, std::string errorMessage="");

};

}


#endif
