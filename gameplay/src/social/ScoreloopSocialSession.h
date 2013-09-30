#if defined(__QNX__) && defined(GP_USE_SOCIAL)

#ifndef SCORELOOPSOCIALSESSION_H_
#define SCORELOOPSOCIALSESSION_H_

#include "SocialSession.h"
#include <scoreloop/sc_client.h>
#include <scoreloop/sc_init.h>
#include <pthread.h>

namespace gameplay
{

/**
 * Scoreloop implementation of SocialSession
 *
 * Note: ensure game.config has the following properties
 *
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
class ScoreloopSocialSession : public SocialSession
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
    static void authenticate(SocialSessionListener* listener, Properties* properties);

    /**
     * @see SocialSession::getUser
     */
    SocialPlayer* getUser();

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
    void submitAchievement(const char* achievementId, unsigned int value);

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
     * @see SocialSession::requestSavedData
     */
    void loadSavedData(const char* key);

    /**
     * @see SocialSession::submitSavedData
     */
    void submitSavedData(const char* key, std::string data);


private:

    /**
     * Contructor
     */
    ScoreloopSocialSession();

    /**
     * Destructor
     */
    virtual ~ScoreloopSocialSession();

    static void authenticate(SocialSessionListener* listener, Properties* properties);

    static void* platformEventCallback(void* data);

    static void userCallback(void* cookie, unsigned int result);

    static void loadFriendsCallback(void* cookie, SC_Error_t result);

    static void loadAchievementsCallback(void* cookie, SC_Error_t result);

    static void submitAchievementCallback(void* cookie, SC_Error_t result);

    static void loadScoresCallback(void* cookie, SC_Error_t result);

    static void submitScoreCallback(void* cookie, SC_Error_t result);

    static ScoreloopSocialSession* _session;

    enum UserOp
    {
        USEROP_GET_LOCALUSER,
        USEROP_WRITE_CONTEXT,
        USEROP_READ_CONTEXT
    };

    SocialSessionListener* _listener;
    Properties* _properties;
    bool _pendingResponse;
    SC_InitData_t _initData;
    SC_Client_h _client;
    SC_UserController_h _userController;
    SC_UsersController_h _usersController;
    SC_LocalAchievementsController_h _localAchievementsController;
    SC_ScoresController_h _scoresController;
    SC_ScoreController_h _scoreController;
    pthread_cond_t _channelCond;
    pthread_mutex_t _channelMutex;
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
