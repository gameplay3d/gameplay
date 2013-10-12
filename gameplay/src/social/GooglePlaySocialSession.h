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
    void incrementAchievement(const char* achievementId, unsigned int increment=1);

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
    void submitChallenge(const SocialPlayer *player, unsigned int wager, float score, const char* leaderboardId=0);

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

    void displayLeaderboard(const char* leaderboardId);

    void displayAchievements();

    void displayChallenges();

    void displayChallengeSubmit(const SocialChallenge *challenge, float score);


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
#if 0

    static void userCallback(void* cookie, unsigned int result);

    static void uiCallback(void *cookie, SCUI_Result_t result, const void *data);

    static void loadFriendsCallback(void* cookie, SC_Error_t result);

    static void loadAchievementsCallback(void* cookie, SC_Error_t result);

    static void submitAchievementCallback(void* cookie, SC_Error_t result);

    static void loadScoresCallback(void* cookie, SC_Error_t result);

    static void submitScoreCallback(void* cookie, SC_Error_t result);

    const SocialAchievement* getAchievement(const char* achievementId) const;

    SocialChallenge &addChallenge(SC_Challenge_h challenge);
#endif
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
};

}

#endif

#endif
