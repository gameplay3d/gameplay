#if defined(__QNX__) && defined(GP_USE_SOCIAL)

#include "Base.h"
#include "ScoreloopSocialSession.h"

namespace gameplay
{

ScoreloopSocialSession* ScoreloopSocialSession::_session = NULL;

ScoreloopSocialSession::ScoreloopSocialSession()
    : SocialSession(),
      _listener(NULL), _properties(NULL), _pendingResponse(false),
      _client(NULL), _userController(NULL), _usersController(NULL),
      _localAchievementsController(NULL), _scoresController(NULL), _scoreController(NULL),
      _key(NULL)
{
    pthread_cond_init(&_channelCond, NULL);
    pthread_mutex_init(&_channelMutex, NULL);
    _userOp = USEROP_GET_LOCALUSER;
}

ScoreloopSocialSession::~ScoreloopSocialSession()
{
}

SocialSessionListener* ScoreloopSocialSession::getListener()
{
    return _listener;
}

void* ScoreloopSocialSession::platformEventCallback(void* data)
{
    _session = (ScoreloopSocialSession*)data;

    int rc = bps_initialize();

    if (rc == BPS_FAILURE)
        GP_ERROR("ScoreloopSocialSession::platformEventHandler - bps_initialize failed.");

    pthread_mutex_lock(&_session->_channelMutex);
    pthread_mutex_unlock(&_session->_channelMutex);
    pthread_cond_signal(&_session->_channelCond);

    bps_event_t* event = NULL;
    while (true)
    {
        rc = bps_get_event(&event, -1);

        if (rc == BPS_FAILURE)
            GP_ERROR("ScoreloopSocialSession::platformEventHandler - bps_get_event failed.");

        if (bps_event_get_domain(event) == SC_GetBPSEventDomain(&_session->_initData))
        {
            SC_HandleBPSEvent(&_session->_initData, event);
        }
    }
    bps_shutdown();
    return 0;
}

void ScoreloopSocialSession::authenicate(SocialSessionListener* listener, Properties* properties)
{
    if (_session)
        return;

    _session = new ScoreloopSocialSession();
    _session->_listener = listener;
    _session->_properties = properties;

    const char* gameId = properties->getString("game_id");
    const char* gameSecret = properties->getString("game_secret");
    const char* gameVersion = properties->getString("game_version");
    const char* gameCurrency = properties->getString("game_currency");
    const char* gameLanguage = properties->getString("gameLanguage");

    SC_InitData_Init(&_session->_initData);
    SC_Error_t rc = SC_Client_New(&_session->_client, &_session->_initData, gameId, gameSecret, gameVersion, gameCurrency, gameLanguage);
    if (rc != SC_OK)
    {
        listener->authenticateEvent(SocialSessionListener::ERROR_INITIALIZATION, NULL);
        return;
    }

    rc = SC_Client_CreateUserController(_session->_client, &_session->_userController, ScoreloopSocialSession::userCallback, _session);
    if (rc != SC_OK)
    {
        listener->authenticateEvent(SocialSessionListener::ERROR_INITIALIZATION, NULL);
        return;
    }

    rc = SC_UserController_LoadUser(_session->_userController);
    if (rc != SC_OK)
    {
        SC_UserController_Release(_session->_userController);
        _session->_userController = NULL;
        listener->authenticateEvent(SocialSessionListener::ERROR_INITIALIZATION, NULL);
    }

    // Create thread for platform event handler needed by Scoreloop
    pthread_mutex_lock(&_session->_channelMutex);
    pthread_create(NULL, NULL, platformEventCallback, _session);
    pthread_cond_wait(&_session->_channelCond, &_session->_channelMutex);
    pthread_mutex_unlock(&_session->_channelMutex);
}

void ScoreloopSocialSession::userCallback(void* cookie, SC_Error_t result)
{
    ScoreloopSocialSession* session = (ScoreloopSocialSession*)cookie;

    switch (result)
    {
    case SC_OK:
        {
            switch(session->_userOp)
            {
            case USEROP_GET_LOCALUSER:
                {
                    session->_user.handle =  SC_UserController_GetUser(session->_userController);
                    SC_String_h name = SC_User_GetLogin(session->_user.handle);
                    session->_user.name = SC_String_GetData(name);
                    session->getListener()->authenticateEvent(SocialSessionListener::SUCCESS, session);
                }
                break;
            case USEROP_WRITE_CONTEXT:
                {
                    session->getListener()->submitSavedDataEvent(SocialSessionListener::SUCCESS);
                }
                break;
            case USEROP_READ_CONTEXT:
                {
                    SC_Context_h context = SC_User_GetContext(session->_user.handle);
                    SC_String_h str;
                    SC_Error_t rc = SC_Context_Get(context, session->_key, &str);
                    if (rc != SC_OK)
                    {
                        session->getListener()->loadSavedDataEvent(SocialSessionListener::ERROR_INVALID_ARG, "");
                    }
                    else
                    {
                        session->getListener()->loadSavedDataEvent(SocialSessionListener::SUCCESS, std::string(SC_String_GetData(str)));
                    }
                }
                break;
            }
        }
        break;

    case SC_INVALID_USER_DATA:
        {
            switch(session->_userOp)
            {
                case USEROP_GET_LOCALUSER:
                    session->getListener()->authenticateEvent(SocialSessionListener::ERROR_INVALID_USER, NULL);
                    break;
                case USEROP_WRITE_CONTEXT:
                    session->getListener()->submitSavedDataEvent(SocialSessionListener::ERROR_INVALID_USER);
                    break;
                case USEROP_READ_CONTEXT:
                    session->getListener()->loadSavedDataEvent(SocialSessionListener::ERROR_INVALID_USER, "");
                    break;
            }
        }
        break;

    case SC_HTTP_SERVER_ERROR:
        {
            switch(session->_userOp)
            {
                case USEROP_GET_LOCALUSER:
                    session->getListener()->authenticateEvent(SocialSessionListener::ERROR_SERVER, NULL);
                    break;
                case USEROP_WRITE_CONTEXT:
                    session->getListener()->submitSavedDataEvent(SocialSessionListener::ERROR_SERVER);
                    break;
                case USEROP_READ_CONTEXT:
                    session->getListener()->loadSavedDataEvent(SocialSessionListener::ERROR_SERVER, "");
                    break;
            }
        }
        break;

    default:
        {
            switch(session->_userOp)
            {
                case USEROP_GET_LOCALUSER:
                    session->getListener()->authenticateEvent(SocialSessionListener::ERROR_UNKNOWN, NULL);
                    break;
                case USEROP_WRITE_CONTEXT:
                    session->getListener()->submitSavedDataEvent(SocialSessionListener::ERROR_UNKNOWN);
                    break;
                case USEROP_READ_CONTEXT:
                    session->getListener()->loadSavedDataEvent(SocialSessionListener::ERROR_UNKNOWN, "");
                    break;
            }
        }
        break;
    }
    session->_pendingResponse = false;
}

void ScoreloopSocialSession::loadFriends()
{
    if (_pendingResponse)
    {
        _listener->loadFriendsEvent(SocialSessionListener::ERROR_PENDING_RESPONSE, _friends);
        return;
    }
    _pendingResponse = true;

    // Create a scoreloop users controller (for finding friends)
    SC_Error_t rc;
    if (_usersController == NULL)
    {
        rc = SC_Client_CreateUsersController(_client, &_usersController, ScoreloopSocialSession::loadFriendsCallback, this);
        if (rc != SC_OK)
        {
            _listener->loadFriendsEvent(SocialSessionListener::ERROR_INITIALIZATION, _friends);
            return;
        }
    }
    rc = SC_UsersController_LoadBuddies(_usersController, _user.handle);
    if (rc != SC_OK)
   {
       SC_UsersController_Release(_usersController);
       _usersController = NULL;
       _listener->loadFriendsEvent(SocialSessionListener::ERROR_INITIALIZATION, _friends);
   }
}

void ScoreloopSocialSession::loadFriendsCallback(void* cookie, SC_Error_t result)
{
    ScoreloopSocialSession* session = (ScoreloopSocialSession*)cookie;
    switch (result)
    {
    case SC_OK:
        {
            SC_UserList_h usersList = SC_UsersController_GetUsers(session->_usersController);
            unsigned int usersCount = SC_UserList_GetCount(usersList);
            session->_friends.clear();
            for (unsigned int i = 0; i < usersCount; i++)
            {
                SocialPlayer player;
                player.handle = SC_UserList_GetAt(usersList, i);
                SC_UserController_GetUser(session->_userController);
                SC_String_h name = SC_User_GetLogin(player.handle);
                player.name = SC_String_GetData(name);
                session->_friends.push_back(player);
            }
            session->getListener()->loadFriendsEvent(SocialSessionListener::SUCCESS, session->_friends);
        }
        break;
    case SC_INVALID_USER_DATA:
        session->getListener()->loadFriendsEvent(SocialSessionListener::ERROR_INVALID_USER, session->_friends);
        break;
    case SC_HTTP_SERVER_ERROR:
        session->getListener()->loadFriendsEvent(SocialSessionListener::ERROR_SERVER, session->_friends);
        break;
    default:
        session->getListener()->loadFriendsEvent(SocialSessionListener::ERROR_UNKNOWN, session->_friends);
        break;
    }
    session->_pendingResponse = false;
}

void ScoreloopSocialSession::loadAchievements()
{
    if (_pendingResponse)
    {
        _listener->loadAchievementsEvent(SocialSessionListener::ERROR_PENDING_RESPONSE, _achievements);
        return;
    }
    _pendingResponse = true;

    SC_Error_t rc;
    rc = SC_Client_CreateLocalAchievementsController(_client, &_localAchievementsController, ScoreloopSocialSession::loadAchievementsCallback, this);
    if (rc != SC_OK)
    {
        _listener->loadAchievementsEvent(SocialSessionListener::ERROR_INITIALIZATION, _achievements);
        return;
    }

    if (SC_LocalAchievementsController_ShouldSynchronize(_localAchievementsController))
    {
        rc = SC_LocalAchievementsController_Synchronize(_localAchievementsController);
        {
            SC_LocalAchievementsController_Release(_localAchievementsController);
            _localAchievementsController = NULL;
            _listener->loadAchievementsEvent(SocialSessionListener::ERROR_INITIALIZATION, _achievements);
        }
    }
    else
    {
        ScoreloopSocialSession::loadAchievementsCallback(this, SC_OK);
    }
}

void ScoreloopSocialSession::loadAchievementsCallback(void* cookie, SC_Error_t result)
{
    ScoreloopSocialSession* session = (ScoreloopSocialSession*)cookie;
    switch (result)
    {
    case SC_OK:
        {
            SC_AchievementList_h achievementsList = SC_LocalAchievementsController_GetAchievements(session->_localAchievementsController);
            unsigned int achievementsCount = SC_AchievementList_GetCount(achievementsList);
            session->_achievements.clear();
            for (unsigned int i = 0; i < achievementsCount; i++)
            {
                SocialAchievement achievement;
                achievement.handle = SC_AchievementList_GetAt(achievementsList, i);
                SC_String_h id = SC_Achievement_GetIdentifier(achievement.handle);
                achievement.name = SC_String_GetData(id);
                achievement.value = (unsigned int) SC_Achievement_GetValue(achievement.handle);
                SC_Award_h award = SC_Achievement_GetAward(achievement.handle);
                achievement.total = SC_Award_GetAchievingValue(award);
                achievement.percentCompleted = achievement.value == 0 ? 0.0f : ((float)achievement.value / (float)achievement.total) * 100.f;
                SC_String_h dateTime = SC_Achievement_GetDate(achievement.handle);
                achievement.dateTimeAchieved = SC_String_GetData(id);
                session->_achievements.push_back(achievement);
            }
            session->getListener()->loadAchievementsEvent(SocialSessionListener::SUCCESS, _session->_achievements);
        }
        break;
    case SC_INVALID_USER_DATA:
        session->getListener()->loadAchievementsEvent(SocialSessionListener::ERROR_INVALID_USER, _session->_achievements);
        break;
    case SC_HTTP_SERVER_ERROR:
        session->getListener()->loadAchievementsEvent(SocialSessionListener::ERROR_SERVER, _session->_achievements);
        break;
    default:
        session->getListener()->loadAchievementsEvent(SocialSessionListener::ERROR_UNKNOWN, _session->_achievements);
        break;
    }
    session->_pendingResponse = false;
}

void ScoreloopSocialSession::submitAchievement(const char* achievementId, unsigned int value)
{
    if (_pendingResponse)
    {
        _listener->submitAchievementEvent(SocialSessionListener::ERROR_PENDING_RESPONSE);
        return;
    }
    _pendingResponse = true;

    SC_Error_t rc;
    rc = SC_Client_CreateLocalAchievementsController(_client, &_localAchievementsController, ScoreloopSocialSession::submitAchievementCallback, this);
    if (rc != SC_OK)
    {
        _listener->submitAchievementEvent(SocialSessionListener::ERROR_INITIALIZATION);
    }
    SC_Bool_t achieved;
    rc = SC_LocalAchievementsController_SetValueForAwardIdentifier(_localAchievementsController, achievementId, value, &achieved);

    if (rc != SC_OK)
    {
        SC_LocalAchievementsController_Release(_localAchievementsController);
        _localAchievementsController = NULL;
      _listener->submitAchievementEvent(SocialSessionListener::ERROR_INITIALIZATION);
    }
}

void ScoreloopSocialSession::submitAchievementCallback(void* cookie, SC_Error_t result)
{
    ScoreloopSocialSession* session = (ScoreloopSocialSession*)cookie;
    switch (result)
    {
    case SC_OK:
        session->getListener()->submitAchievementEvent(SocialSessionListener::SUCCESS);
        break;
    case SC_INVALID_USER_DATA:
        session->getListener()->submitAchievementEvent(SocialSessionListener::ERROR_INVALID_USER);
        break;
    case SC_HTTP_SERVER_ERROR:
        session->getListener()->submitAchievementEvent(SocialSessionListener::ERROR_SERVER);
        break;
    default:
        session->getListener()->submitAchievementEvent(SocialSessionListener::ERROR_UNKNOWN);
        break;
    }
    session->_pendingResponse = false;
}

void ScoreloopSocialSession::loadScores(const char* leaderboardId, CommunityScope community, TimeScope time, const SocialPlayer& player, unsigned int count)
{
    if (_pendingResponse)
     {
         _listener->loadScoresEvent(SocialSessionListener::ERROR_PENDING_RESPONSE, _scores);
         return;
     }
     _pendingResponse = true;

    SC_Error_t rc;
    if (_scoresController == NULL)
    {
        rc = SC_Client_CreateScoresController(_client, &_scoresController, ScoreloopSocialSession::loadScoresCallback, this);
        if (rc != SC_OK)
        {
            _listener->loadScoresEvent(SocialSessionListener::ERROR_INITIALIZATION, _scores);
            return;
        }
    }

    // Lookup the mode to be mapped for the specified leaderboard id
    if (leaderboardId != NULL)
    {
        Properties* leaderboardMappings = _properties->getNamespace("leaderboard_mappings", false);
        if (leaderboardMappings)
        {
            int mode = leaderboardMappings->getInt(leaderboardId);
            if (mode >=  0)
            {
                SC_ScoresController_SetMode(_scoresController, (unsigned int)mode);
            }
        }
    }

    // Set the search list filter
    SC_ScoresSearchList_t searchList = SC_SCORES_SEARCH_LIST_USER_COUNTRY;
    switch(community)
    {
    case COMMUNITY_SCOPE_FRIENDS:
        searchList.usersSelector = SC_USERS_SELECTOR_BUDDYHOOD;
        break;
    case COMMUNITY_SCOPE_ALL:
        searchList.usersSelector = SC_USERS_SELECTOR_ALL;
        break;
    }
    switch(time)
    {
    case TIME_SCOPE_TODAY:
        searchList.timeInterval = SC_TIME_INTERVAL_24H;
        break;
    case TIME_SCOPE_WEEK:
            searchList.timeInterval = SC_TIME_INTERVAL_7DAYS;
           break;
    case TIME_SCOPE_ALL:
            searchList.timeInterval = SC_TIME_INTERVAL_ALL;
           break;
    }
    SC_ScoresController_SetSearchList(_scoresController, searchList);

    // Specify the player and range count to get the scores around
    SC_User_h user = player.handle;
    rc = SC_ScoresController_LoadScoresAroundUser(_scoresController, _user.handle, count);
    if (rc != SC_OK)
    {
      SC_ScoresController_Release(_scoresController);
      _scoresController = NULL;
      _listener->loadScoresEvent(SocialSessionListener::ERROR_INITIALIZATION, _scores);
    }
}

void ScoreloopSocialSession::loadScores(const char* leaderboardId, CommunityScope community, TimeScope time, unsigned int start, unsigned int count)
{
    if (_pendingResponse)
     {
         _listener->loadScoresEvent(SocialSessionListener::ERROR_PENDING_RESPONSE, _scores);
         return;
     }
     _pendingResponse = true;

    SC_Error_t rc;
    if (_scoresController == NULL)
    {
        rc = SC_Client_CreateScoresController(_client, &_scoresController, ScoreloopSocialSession::loadScoresCallback, this);
        if (rc != SC_OK)
        {
            _listener->loadScoresEvent(SocialSessionListener::ERROR_INITIALIZATION, _scores);
            return;
        }
    }

    // Lookup the mode to be mapped for the specified leaderboard id
    if (leaderboardId != NULL)
    {
        Properties* leaderboardMappings = _properties->getNamespace("leaderboard_mappings", false);
        if (leaderboardMappings)
        {

            int mode = leaderboardMappings->getInt(leaderboardId);
            if (mode >=  0)
            {
                SC_ScoresController_SetMode(_scoresController, (unsigned int)mode);
            }
        }
    }

    // Set the search list filter
    SC_ScoresSearchList_t searchList = SC_SCORES_SEARCH_LIST_USER_COUNTRY;
    switch(community)
    {
    case COMMUNITY_SCOPE_FRIENDS:
        searchList.usersSelector = SC_USERS_SELECTOR_BUDDYHOOD;
        break;
    case COMMUNITY_SCOPE_ALL:
        searchList.usersSelector = SC_USERS_SELECTOR_ALL;
        break;
    }
    switch(time)
    {
    case TIME_SCOPE_TODAY:
        searchList.timeInterval = SC_TIME_INTERVAL_24H;
        break;
    case TIME_SCOPE_WEEK:
            searchList.timeInterval = SC_TIME_INTERVAL_7DAYS;
           break;
    case TIME_SCOPE_ALL:
            searchList.timeInterval = SC_TIME_INTERVAL_ALL;
           break;
    }
    SC_ScoresController_SetSearchList(_scoresController, searchList);

    // Set the search range
   SC_Range_t range;
   range.offset = start;
   range.length = count;
   rc = SC_ScoresController_LoadScores(_scoresController, range);
   if (rc != SC_OK)
   {
     SC_ScoresController_Release(_scoresController);
     _scoresController = NULL;
     _listener->loadScoresEvent(SocialSessionListener::ERROR_INITIALIZATION, _scores);
   }
}

void ScoreloopSocialSession::loadScoresCallback(void* cookie, SC_Error_t result)
{
    ScoreloopSocialSession* session = (ScoreloopSocialSession*)cookie;
    switch (result)
    {
    case SC_OK:
        {
            SC_ScoreFormatter_h scoreFormatter = SC_Client_GetScoreFormatter(session->_client);

            SC_ScoreList_h scoresList = SC_ScoresController_GetScores(session->_scoresController);
            unsigned int scoresCount = SC_ScoreList_GetCount(scoresList);
            session->_scores.clear();
            for (unsigned int i = 0; i < scoresCount; i++)
            {
                SocialScore score;
                score.handle = SC_ScoreList_GetAt(scoresList, i);
                score.rank = SC_Score_GetRank(score.handle);
                score.value = (float)SC_Score_GetResult(score.handle);
                SC_String_h formatted;
                SC_ScoreFormatter_FormatScore(scoreFormatter, score.handle, SC_SCORE_FORMAT_DEFAULT, &formatted);
                score.valueFormatted = SC_String_GetData(formatted);
                SC_User_h player = SC_Score_GetUser(score.handle);
                SC_String_h playerStr = SC_User_GetLogin(player);
                score.playerName = SC_String_GetData(playerStr);
                session->_scores.push_back(score);
            }
            session->getListener()->loadScoresEvent(SocialSessionListener::SUCCESS, session->_scores);
        }
        break;
    case SC_INVALID_USER_DATA:
        session->getListener()->loadScoresEvent(SocialSessionListener::ERROR_INVALID_USER, session->_scores);
        break;
    case SC_HTTP_SERVER_ERROR:
        session->getListener()->loadScoresEvent(SocialSessionListener::ERROR_SERVER, session->_scores);
        break;
    default:
        session->getListener()->loadScoresEvent(SocialSessionListener::ERROR_UNKNOWN, session->_scores);
        break;
    }
    session->_pendingResponse = false;
}

void ScoreloopSocialSession::submitScore(const char* leaderboardId, float value)
{
    if (_pendingResponse)
    {
        _listener->submitScoreEvent(SocialSessionListener::ERROR_PENDING_RESPONSE);
        return;
    }
    _pendingResponse = true;

    SC_Error_t rc;
    rc = SC_Client_CreateScoreController(_client, &_scoreController, ScoreloopSocialSession::submitScoreCallback, this);
    if (rc != SC_OK)
    {
        _listener->submitScoreEvent(SocialSessionListener::ERROR_INITIALIZATION);
        return;
    }

    // Create a score to submit
    SC_Score_h score;
    rc = SC_Client_CreateScore(_client, &score);
    if (rc != SC_OK)
    {
        _listener->submitScoreEvent(SocialSessionListener::ERROR_INITIALIZATION);
        return;
    }

    // Set the leaderboard to associate this score with
    if (leaderboardId != NULL)
    {
        Properties* leaderboardMappings = _properties->getNamespace("leaderboard_mappings", false);
        if (leaderboardMappings)
        {
            int mode = leaderboardMappings->getInt(leaderboardId);
            if (mode >=  0)
            {
                SC_Score_SetMode(score, (unsigned int)mode);
            }
        }
    }

    // Set the score value and submit the score
    SC_Score_SetResult(score, value);
    rc = SC_ScoreController_SubmitScore(_scoreController, score);
    if (rc != SC_OK)
    {
         SC_ScoreController_Release(_scoreController);
         _scoreController = NULL;
         _listener->submitScoreEvent(SocialSessionListener::ERROR_INITIALIZATION);
    }
    SC_Score_Release(score);
}

void ScoreloopSocialSession::submitScoreCallback(void* cookie, SC_Error_t result)
{
    ScoreloopSocialSession* session = (ScoreloopSocialSession*)cookie;
    switch (result)
    {
    case SC_OK:
        session->getListener()->submitScoreEvent(SocialSessionListener::SUCCESS);
        break;
    case SC_INVALID_USER_DATA:
        session->getListener()->submitScoreEvent(SocialSessionListener::ERROR_INVALID_USER);
        break;
    case SC_HTTP_SERVER_ERROR:
        session->getListener()->submitScoreEvent(SocialSessionListener::ERROR_SERVER);
        break;
    default:
        session->getListener()->submitScoreEvent(SocialSessionListener::ERROR_UNKNOWN);
        break;
    }
    session->_pendingResponse = false;
}

void ScoreloopSocialSession::loadSavedData(const char* key)
{
    if (_pendingResponse)
    {
        _listener->loadSavedDataEvent(SocialSessionListener::ERROR_PENDING_RESPONSE, "");
        return;
    }

    SC_Error_t rc = SC_UserController_LoadUserContext(_userController);
    if (rc != SC_OK)
    {
        _listener->loadSavedDataEvent(SocialSessionListener::ERROR_INITIALIZATION, "");
        return;
    }
    _pendingResponse = true;
    _key = key;
}

void ScoreloopSocialSession::submitSavedData(const char* key, std::string data)
{
    if (_pendingResponse)
    {
        _listener->submitSavedDataEvent(SocialSessionListener::ERROR_PENDING_RESPONSE);
        return;
    }
    _pendingResponse = true;

    SC_Context_h context;
    SC_Error_t rc = SC_Context_New(&context);
    if (rc != SC_OK)
    {
        _listener->submitSavedDataEvent(SocialSessionListener::ERROR_INITIALIZATION);
        return;
    }
    SC_String_h str;
    rc = SC_String_New(&str, data.c_str());
    if (rc != SC_OK)
    {
        _listener->submitSavedDataEvent(SocialSessionListener::ERROR_INITIALIZATION);
        return;
    }
    SC_Context_Put(context, key, str);
    SC_User_SetContext(_user.handle, context);
    SC_UserController_SetUser(_userController, _user.handle);
    rc = SC_UserController_UpdateUserContext(_userController);
    if (rc != SC_OK)
    {
        _listener->submitSavedDataEvent(SocialSessionListener::ERROR_INITIALIZATION);
        return;
    }
}

}

#endif
