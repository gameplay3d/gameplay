#if defined(__ANDROID__) && defined(GP_USE_SOCIAL)

#ifndef GOOGLEPLAYSOCIALSESSION_H_
#define GOOGLEPLAYSOCIALSESSION_H_

#include "SocialSession.h"

namespace gameplay
{

/**
 * Google Play (game services) implementation of SocialSession
 *
 * Note: ensure game.config has the following properties
 *
  @verbatim
    social
    {
         provider = GooglePlay
         id = d346c484-12aa-49a2-a0a0-de2f87492d72
         secret = aAa+DehBfyGO/CYaE3nWomgu7SIbWFczUih+Qwf3/n7u0y3nyq5Hag==
         version = 1.0
         language = en
         currency = ASC
         leaderboard_mappings
         {
             // Format: leaderboardId =  mode <unsigned int>
             easy = 0
             medium = 1
             hard = 2
         }

         achievement_mappings
         {
             // Format: myAchievementKey =  DestinationAchievementID
             gameplay.spaceship.winchallenge = destination.spaceship.winchallengeid
         }
    }
 *
 * Note: Ensure your bar-descriptor.xml has:
 *
 * <action>read_device_identifying_information</action>
 *
 * @script{ignore}
 */
class GooglePlaySocialSession : public SocialSession
{
    friend class SocialController;

public:

    /**
      * @see SocialSession::getListener
      */
     SocialSessionListener* getListener();

     /**
      * Initializes the session with the local client definition for this game.
      *
      * @param listener The listener for responses for this session
      * @param properties The properties to initialize this session with for this game.
      */
     static SocialSession *authenticate(SocialSessionListener* listener, Properties* properties);

     /**
      * @see SocialSession::getUser
      */
     const SocialPlayer& getUser() const;

     /**
      * @see SocialSession::loadFriends
      */
     void loadFriends();

     /**
      * @see SocialSession::loadAchievements
      */
     void loadAchievements();

     /**
      * @see SocialSession::submitAchievement
      */
     void submitAchievement(const char* achievementId, unsigned int value, bool achieved=false);

     /**
      * @see SocialSession::incrementAchievement
      */
     void incrementAchievement(const char* achievementId, unsigned int totalSteps=1);

     /**
      * @see SocialSession::syncAchievements
      */
     void synchronizeAchievements();

     /**
      * @see SocialSession::loadScores
      */
     void loadScores(const char* leaderboardId, CommunityScope community, TimeScope time, const SocialPlayer& player, unsigned int count);

     /**
      * @see SocialSession::loadScores
      */
     void loadScores(const char* leaderboardId, CommunityScope community, TimeScope time, unsigned int start, unsigned int count);

     /**
      * @see SocialSession::submitScore
      */
     void submitScore(const char* leaderboardId, float score);

     /**
      * @see SocialSession::submitChallenge
      */
     void submitChallenge(const SocialPlayer *player, float score, const char* leaderboardId, unsigned int wager=0);

     /**
      * @see SocialSession::submitAchievementChallenge
      */
     void submitAchievementChallenge(const SocialPlayer *player, const char* achievementId, unsigned int wager=0);

     /**
      * @see SocialSession::loadChallenges
      */
     void loadChallenges(bool showOpenChallengesOnly=true);

     /**
      * @see SocialSession::acceptChallenge
      */
     void replyToChallenge(const SocialChallenge *challenge, bool accept);

    /**
     * @see SocialSession::requestSavedData
     */
    void loadSavedData(const char* key);
    
    /**
     * @see SocialSession::submitSavedData
     */
    void submitSavedData(const char* key, std::string data);
    
    /**
     * @see SocialSession::displayLeaderboard
     */
    void displayLeaderboard(const char* leaderboardId) const;
    
    /**
     * @see SocialSession::displayAchievements
     */
    void displayAchievements() const;
    
    /**
     * @see SocialSession::displayChallenges
     */
    void displayChallenges() const;
    
    /**
     * @see SocialSession::displayChallengeSubmit
     */
    void displayChallengeSubmit(const SocialChallenge *challenge, float score) const;
    
    /**
     * @see SocialSession::displayPopup
     */
    void displayPopup(const char *string, const char *title) const;
    
    /**
     * @see SocialSession::supportsChallenges
     */
    bool supportsChallenges() const { return true; }
    
    /**
     * @see SocialSession::supportsAchievementChallenges
     */
    bool supportsAchievementChallenges() const { return false; }
    
    /**
     * @see SocialSession::supportsMultiplayer
     */
    bool supportsMultiplayer() const { return false; }
    
    /**
     * @see SocialSession::supportsTurns
     */
    bool supportsTurns() const { return false; }


protected:

private:

    /**
     * Contructor
     */
    GooglePlaySocialSession();

    /**
     * Destructor
     */
    virtual ~GooglePlaySocialSession();

    const SocialAchievement* getAchievement(const char* achievementId) const;
    void loadAchievementData();

    static GooglePlaySocialSession* _session;

    enum UserOp
    {
        USEROP_GET_LOCALUSER,
        USEROP_WRITE_CONTEXT,
        USEROP_READ_CONTEXT
    };

    SocialSessionListener* _listener;
    Properties* _properties;
    bool _pendingUserResponse;
    bool _pendingFriendsResponse;
    bool _pendingScoresResponse;
    bool _pendingSubmitScoreResponse;
    bool _pendingAchievementResponse;
    bool _pendingDataResponse;
    const char* _key;
    std::string _data;
    SocialPlayer _user;
    UserOp _userOp;
    std::vector<SocialPlayer> _friends;
    std::vector<SocialAchievement> _achievements;
    std::vector<SocialScore> _scores;
    std::vector<SocialChallenge> _challenges;
};

}

#endif

#endif
