#ifndef SOCIALSESSION_H_
#define SOCIALSESSION_H_

#include "SocialSessionListener.h"
#include "SocialPlayer.h"
#include "SocialAchievement.h"
#include "SocialScore.h"
#include "SocialChallenge.h"
#include "Properties.h"

namespace gameplay
{

/**
 * Defines an abstract class for typical social game activities running with an authenticated session.
 *
 * @script{ignore}
 */
class SocialSession
{
    friend class SocialController;

public:

    enum CommunityScope
    {
        COMMUNITY_SCOPE_FRIENDS,
        COMMUNITY_SCOPE_ALL
    };

    enum TimeScope
    {
        TIME_SCOPE_TODAY,
        TIME_SCOPE_WEEK,
        TIME_SCOPE_ALL,
    };

    /**
     * Gets the asynchronous response listener that registered for this session.
     *
     * @return The asynchronous response listener that registered for this session.
     */
    virtual SocialSessionListener* getListener() = 0;

    virtual const SocialPlayer& getUser() const = 0;

    virtual void loadFriends() = 0;

    virtual void loadAchievements() = 0;

    virtual void submitAchievement(const char* achievementId, unsigned int value, bool achieved=false) = 0;

    virtual void incrementAchievement(const char* achievementId, unsigned int totalSteps=1) = 0;

    virtual void synchronizeAchievements() = 0;

    /**
     * Asynchronously request the scores for the count where the player is in the middle.
     *
     * @param leaderboardId The leaderboard to get populated with the scores.
     * @param community The community scope to filter the search list.
     * @param time The time scope to filter teh search list.
     * @param player The player to narrow the search around.
     * @param count The number to scores return in the result.
     */
    virtual void loadScores(const char* leaderboardId, CommunityScope community, TimeScope time, const SocialPlayer& player, unsigned int count) = 0;

    /**
     * Asynchronously request the scores for a specified leaderboard for this game.
     *
     * @param leaderboardId The leaderboard to get populated with the scores.
     */
    virtual void loadScores(const char* leaderboardId, CommunityScope community, TimeScope time, unsigned int start, unsigned int count) = 0;

    virtual void submitScore(const char* leaderboardId, float value) = 0;

    virtual void submitChallenge(const SocialPlayer *player, float score, const char* leaderboardId=0, unsigned int wager=0) = 0;

    virtual void submitAchievementChallenge(const SocialPlayer *player, const char* achievementId=0, unsigned int wager=0) = 0;

    virtual void loadChallenges(bool showOpenChallengesOnly=true) = 0;

    virtual void replyToChallenge(const SocialChallenge *challenge, bool accept) = 0;

    virtual void loadSavedData(const char* key) = 0;

    virtual void submitSavedData(const char* key, std::string data) = 0;

    virtual void displayLeaderboard(const char* leaderboardId) const = 0;

    virtual void displayAchievements() const = 0;

    virtual void displayChallenges() const = 0;

    virtual void displayChallengeSubmit(const SocialChallenge *challenge, float score) const = 0;

    virtual void displayPopup(const char *string, const char *title=0) const = 0;

    virtual bool handleEvent(void *event) { return true; }

    virtual bool supportsChallenges() const = 0;

    virtual bool supportsAchievementChallenges() const = 0;

    virtual bool supportsMultiplayer() const = 0;

    virtual bool supportsTurns() const = 0;

protected:

    /**
     * Contructor
     */
    SocialSession() { }

    /**
     * Destructor
     */
    virtual ~SocialSession() { }

};

}

#endif
