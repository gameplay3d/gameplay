#if defined(__QNX__) && defined(GP_USE_SOCIAL)

#include "Base.h"
#include "ScoreloopSocialSession.h"
#include <bps/dialog.h>

namespace gameplay
{

ScoreloopSocialSession* ScoreloopSocialSession::_session = NULL;

ScoreloopSocialSession::ScoreloopSocialSession()
    : SocialSession(),
      _listener(NULL), _properties(NULL), _pendingUserResponse(false), _pendingFriendsResponse(false),
      _pendingScoresResponse(false), _pendingSubmitScoreResponse(false), _pendingAchievementResponse(false),
      _pendingChallengeResponse(false), _pendingChallengesResponse(false), _pendingDataResponse(false),
      _client(NULL), _uiClient(NULL), _userController(NULL), _usersController(NULL),
      _localAchievementsController(NULL), _scoresController(NULL), _scoreController(NULL),
      _challengeController(NULL), _challengesController(NULL), _key(NULL), _acceptedChallenge(NULL)
{
    pthread_cond_init(&_channelCond, NULL);
    pthread_mutex_init(&_channelMutex, NULL);
    _userOp = USEROP_GET_LOCALUSER;
}

ScoreloopSocialSession::~ScoreloopSocialSession()
{
#if 0
	SC_Client_Release(_client);
	SCUI_Client_Release(_uiClient);
	SC_ScoresController_Release(_scoresController);
	SC_ScoreController_Release(_scoreController);
	SC_UserController_Release(_userController);
	SC_UsersController_Release(_usersController);
	SC_LocalAchievementsController_Release(_localAchievementsController);
	SC_ChallengeController_Release(_challengeController);
	SC_ChallengesController_Release(_challengesController);
#endif
}

SocialSessionListener* ScoreloopSocialSession::getListener()
{
    return _listener;
}

void* ScoreloopSocialSession::platformEventCallback(void* data)
{
	fprintf(stderr, "platformEventCallback!\n");
    _session = (ScoreloopSocialSession*)data;

    int rc = bps_initialize();

    if (rc == BPS_FAILURE)
        GP_ERROR("ScoreloopSocialSession::platformEventHandler - bps_initialize failed.");

//    pthread_mutex_lock(&_session->_channelMutex);
//    pthread_mutex_unlock(&_session->_channelMutex);
//   pthread_cond_signal(&_session->_channelCond);
fprintf(stderr, "hello here\n");
    bps_event_t* event = NULL;
    while (true)
    {
    	fprintf(stderr, "weird stuff start loop\n");
        rc = bps_get_event(&event, 0);
        fprintf(stderr, "bps_get_event %d\n", rc);
        if (rc == BPS_FAILURE)
            GP_ERROR("ScoreloopSocialSession::platformEventHandler - bps_get_event failed.");
fprintf(stderr, "we are in a bps loop here\n");
     //   if (bps_event_get_domain(event) == SC_GetBPSEventDomain(&_session->_initData) || bps_event_get_domain(event) == SCUI_GetBPSEventDomain())
        {
        	fprintf(stderr, "found a bps event!\n");
        	// can either be a scoreloop event or a ui event - need to handle both
            if ((SC_HandleBPSEvent(&_session->_initData, event) == BPS_SUCCESS) || SCUI_Client_HandleEvent(_session->_uiClient, event))
            	continue;
        }
    }
    bps_shutdown();
    return 0;
}

SocialSession *ScoreloopSocialSession::authenticate(SocialSessionListener* listener, Properties* properties)
{
    if (!_session)
    {
		_session = new ScoreloopSocialSession();
		_session->_listener = listener;
		_session->_properties = properties;

		const char* gameId = properties->getString("id");
		const char* gameSecret = properties->getString("secret");
		const char* gameVersion = properties->getString("version");
		const char* gameCurrency = properties->getString("currency");
		const char* gameLanguage = properties->getString("language");

		// Create thread for platform event handler needed by Scoreloop
//		pthread_mutex_lock(&_session->_channelMutex);
//		pthread_create(NULL, NULL, platformEventCallback, _session);
//		pthread_cond_wait(&_session->_channelCond, &_session->_channelMutex);
//		pthread_mutex_unlock(&_session->_channelMutex);

		SC_InitData_Init(&_session->_initData);

		SC_Error_t rc = SC_Client_New(&_session->_client, &_session->_initData, gameId, gameSecret, gameVersion, gameCurrency, gameLanguage);
		if (rc != SC_OK)
		{
			listener->authenticateEvent(SocialSessionListener::ERROR_INITIALIZATION, NULL);
			return 0;
		}

		rc = SCUI_Client_NewWithCallback(&_session->_uiClient, _session->_client, ScoreloopSocialSession::uiCallback, _session);
	    if (rc != SC_OK)
	    {
	    	listener->authenticateEvent(SocialSessionListener::ERROR_INITIALIZATION, NULL);
	    }

		rc = SC_Client_CreateUserController(_session->_client, &_session->_userController, ScoreloopSocialSession::userCallback, _session);
		if (rc != SC_OK)
		{
			listener->authenticateEvent(SocialSessionListener::ERROR_INITIALIZATION, NULL);
			return 0;
		}

		rc = SC_UserController_LoadUser(_session->_userController);
		if (rc != SC_OK)
		{
			SC_UserController_Release(_session->_userController);
			_session->_userController = NULL;
			listener->authenticateEvent(SocialSessionListener::ERROR_INITIALIZATION, NULL);
		}
    }

    return _session;
}

bool ScoreloopSocialSession::handleEvent(void *event)
{
#if 1
	bps_event_t* bps_event = (bps_event_t *)event;

	if (SC_HandleBPSEvent(&_session->_initData, bps_event) == BPS_SUCCESS)
		return true;

	if (SCUI_Client_HandleEvent(_session->_uiClient, bps_event))
		return true;

#endif
    return false;
}

const SocialPlayer& ScoreloopSocialSession::getUser() const
{
	return _user;
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
                    SC_String_h name = SC_User_GetLogin((SC_User_h)session->_user.handle);
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
                    SC_Context_h context = SC_User_GetContext((SC_User_h)session->_user.handle);
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

    session->_pendingUserResponse = false;
}


static SC_Challenge_h challenge1 = 0;

void ScoreloopSocialSession::uiCallback(void *cookie, SCUI_Result_t result, const void *data)
{
	ScoreloopSocialSession* session = (ScoreloopSocialSession*)cookie;
    SC_Error_t rc = SC_INVALID_ARGUMENT;

    switch (result)
    {
		case SCUI_RESULT_OK:
			session->getListener()->uiEvent(SocialSessionListener::SUCCESS);
			break;

		case SCUI_RESULT_ERROR:
			if (data != NULL)
				rc = *((SC_Error_t *)data);

			session->getListener()->uiEvent(SocialSessionListener::SUCCESS, SC_MapErrorToStr(rc));
			break;

		case SCUI_RESULT_START_CHALLENGE:
			{
				fprintf(stderr, "START challenge\n");

				// Get details of the Challenge from 'data' argument by casting it.
				SocialChallenge &challenge = session->addChallenge((SC_Challenge_h)data);

				session->getListener()->startChallengeEvent(SocialSessionListener::SUCCESS, challenge);
			}
			break;

		default:
			if (result == SCUI_RESULT_CANCEL)
				session->getListener()->uiEvent(SocialSessionListener::ERROR_CANCELLED);
			break;
    }

    fprintf(stderr, "UI result: %s\n", SCUI_MapResultCodeToStr(result));
}

void ScoreloopSocialSession::loadFriends()
{
    if (_pendingFriendsResponse)
    {
        _listener->loadFriendsEvent(SocialSessionListener::ERROR_PENDING_RESPONSE, _friends);
        return;
    }
    _pendingFriendsResponse = true;

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
    rc = SC_UsersController_LoadBuddies(_usersController, (SC_User_h)_user.handle);
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
					SC_String_h name = SC_User_GetLogin((SC_User_h)player.handle);
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

    session->_pendingFriendsResponse = false;
}

void ScoreloopSocialSession::loadAchievements()
{
    if (_pendingAchievementResponse)
    {
        _listener->loadAchievementsEvent(SocialSessionListener::ERROR_PENDING_RESPONSE, _achievements);
        return;
    }
    _pendingAchievementResponse = true;

    SC_Error_t rc;

    if (_localAchievementsController == NULL)
    {
		rc = SC_Client_CreateLocalAchievementsController(_client, &_localAchievementsController, ScoreloopSocialSession::loadAchievementsCallback, this);
		if (rc != SC_OK)
		{
			_listener->loadAchievementsEvent(SocialSessionListener::ERROR_INITIALIZATION, _achievements);
			_pendingAchievementResponse = false;
			return;
		}
    }

    if (SC_LocalAchievementsController_ShouldSynchronize(_localAchievementsController))
    {
        rc = SC_LocalAchievementsController_Synchronize(_localAchievementsController);
        if (rc != SC_OK)
        {
            _listener->loadAchievementsEvent(SocialSessionListener::ERROR_INITIALIZATION, _achievements);
            _pendingAchievementResponse = false;
            return;
        }
    }
    else
    	ScoreloopSocialSession::loadAchievementsCallback(this, SC_OK);
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
					SC_Award_h award = SC_Achievement_GetAward((SC_Achievement_h)achievement.handle);

					SC_String_h id = SC_Award_GetIdentifier(award);
					achievement.name = (id) ? SC_String_GetData(id) : "";
					SC_String_h title = SC_Award_GetLocalizedTitle(award);
					achievement.title = (title) ? SC_String_GetData(title) : "";
					achievement.value = (unsigned int)SC_Achievement_GetValue((SC_Achievement_h)achievement.handle);
					SC_String_h dateTime = SC_Achievement_GetDate((SC_Achievement_h)achievement.handle);
					achievement.dateTimeAchieved = (dateTime) ? SC_String_GetData(dateTime) : "";
					achievement.total = SC_Award_GetAchievingValue(award);
					achievement.percentCompleted = achievement.value == 0 ? 0.0f : ((float)achievement.value / (float)achievement.total) * 100.f;

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

    session->_pendingAchievementResponse = false;
}

const SocialAchievement* ScoreloopSocialSession::getAchievement(const char* achievementId) const
{
	uint size = _achievements.size();

	for (uint i = 0; i < size; i++)
	{
		if (strcmp(_achievements[i].name.data(), achievementId) == 0)
			return &_achievements[i];
	}
	return 0;
}

void ScoreloopSocialSession::submitAchievement(const char* achievementId, unsigned int value, bool isAchieved)
{
    SC_Error_t rc;
    if (_localAchievementsController == NULL)
    {
		rc = SC_Client_CreateLocalAchievementsController(_client, &_localAchievementsController, ScoreloopSocialSession::submitAchievementCallback, this);
		if (rc != SC_OK)
		{
			_listener->submitAchievementEvent(SocialSessionListener::ERROR_INITIALIZATION);
			return;
		}
    }

    SC_Bool_t achieved = SC_FALSE;
    if (isAchieved)
    {
    	rc = SC_LocalAchievementsController_SetAchievedValueForAwardIdentifier(_localAchievementsController, achievementId, &achieved);
    }
    else
    {
		rc = SC_LocalAchievementsController_SetValueForAwardIdentifier(_localAchievementsController, achievementId, value, &achieved);
    }

    if (rc != SC_OK)
    {
        _listener->submitAchievementEvent(SocialSessionListener::ERROR_INITIALIZATION);
        return;
    }

    if (achieved == SC_TRUE)
    {
    	const SocialAchievement *achievement = getAchievement(achievementId);
    	if (achievement)
    		_listener->awardAchievedEvent(SocialSessionListener::SUCCESS, *achievement);

    	fprintf(stderr, "AWARD ACHIEVED %s!!!\n", achievementId);
    }
}

void ScoreloopSocialSession::incrementAchievement(const char* achievementId, unsigned int increment)
{
    SC_Error_t rc;
    if (_localAchievementsController == NULL)
    {
		rc = SC_Client_CreateLocalAchievementsController(_client, &_localAchievementsController, ScoreloopSocialSession::submitAchievementCallback, this);
		if (rc != SC_OK)
		{
			_listener->submitAchievementEvent(SocialSessionListener::ERROR_INITIALIZATION);
			return;
		}
    }
    SC_Bool_t achieved;
	rc = SC_LocalAchievementsController_IncrementValueForAwardIdentifier(_localAchievementsController, achievementId, &achieved);

    if (achieved == SC_TRUE)
    {
    	const SocialAchievement *achievement = getAchievement(achievementId);
    	if (achievement)
    		_listener->awardAchievedEvent(SocialSessionListener::SUCCESS, *achievement);
    }

    if (rc != SC_OK)
    {
        _listener->submitAchievementEvent(SocialSessionListener::ERROR_INITIALIZATION);
        return;
    }
}

void ScoreloopSocialSession::synchronizeAchievements()
{
    SC_Error_t rc;
    if (_localAchievementsController == NULL)
    {
		rc = SC_Client_CreateLocalAchievementsController(_client, &_localAchievementsController, ScoreloopSocialSession::submitAchievementCallback, this);
		if (rc != SC_OK)
		{
			_listener->submitAchievementEvent(SocialSessionListener::ERROR_INITIALIZATION);
			_pendingAchievementResponse = false;
			return;
		}
    }

	if (SC_LocalAchievementsController_ShouldSynchronize(_localAchievementsController) == SC_TRUE)
	{
		_pendingAchievementResponse = true;

fprintf(stderr, "we are attempting to synchronize our achievements\n");
		rc = SC_LocalAchievementsController_Synchronize(_localAchievementsController);
		if (rc != SC_OK)
		{
			_listener->submitAchievementEvent(SocialSessionListener::ERROR_INITIALIZATION);
			_pendingAchievementResponse = false;
			return;
		}
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

    session->_pendingAchievementResponse = false;
}

void ScoreloopSocialSession::loadScores(const char* leaderboardId, CommunityScope community, TimeScope time, const SocialPlayer& player, unsigned int count)
{
    if (_pendingScoresResponse)
     {
         _listener->loadScoresEvent(SocialSessionListener::ERROR_PENDING_RESPONSE, _scores);
         return;
     }
    _pendingScoresResponse = true;

    SC_Error_t rc;
    if (_scoresController == NULL)
    {
        rc = SC_Client_CreateScoresController(_client, &_scoresController, ScoreloopSocialSession::loadScoresCallback, this);
        if (rc != SC_OK)
        {
            _listener->loadScoresEvent(SocialSessionListener::ERROR_INITIALIZATION, _scores);
            _pendingScoresResponse = false;
            return;
        }
    }

    // Lookup the mode to be mapped for the specified leader board id
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
    SC_ScoresSearchList_t searchList;
    searchList.countrySelector = SC_COUNTRY_SELECTOR_ALL;
    searchList.buddyhoodUser = (SC_User_h)_user.handle;

    switch (community)
    {
		case COMMUNITY_SCOPE_FRIENDS:
			searchList.usersSelector = SC_USERS_SELECTOR_BUDDYHOOD;
			break;
		case COMMUNITY_SCOPE_ALL:
		default:
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
		default:
            searchList.timeInterval = SC_TIME_INTERVAL_ALL;
            break;
    }
    SC_ScoresController_SetSearchList(_scoresController, searchList);

    // Specify the player and range count to get the scores around
    SC_User_h user = (SC_User_h)player.handle;
    rc = SC_ScoresController_LoadScoresAroundUser(_scoresController, (SC_User_h)_user.handle, count);
    if (rc != SC_OK)
    {
        SC_ScoresController_Release(_scoresController);
        _scoresController = NULL;
        _listener->loadScoresEvent(SocialSessionListener::ERROR_INITIALIZATION, _scores);
    }
}

void ScoreloopSocialSession::loadScores(const char* leaderboardId, CommunityScope community, TimeScope time, unsigned int start, unsigned int count)
{
    if (_pendingScoresResponse)
    {
        _listener->loadScoresEvent(SocialSessionListener::ERROR_PENDING_RESPONSE, _scores);
        return;
    }
    _pendingScoresResponse = true;

    SC_Error_t rc;
    if (_scoresController == NULL)
    {
        rc = SC_Client_CreateScoresController(_client, &_scoresController, ScoreloopSocialSession::loadScoresCallback, this);
        if (rc != SC_OK)
        {
            _listener->loadScoresEvent(SocialSessionListener::ERROR_INITIALIZATION, _scores);
            _pendingScoresResponse = false;
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
    SC_ScoresSearchList_t searchList;
    searchList.countrySelector = SC_COUNTRY_SELECTOR_ALL;
    searchList.buddyhoodUser = (SC_User_h)_user.handle;

    switch (community)
    {
		case COMMUNITY_SCOPE_FRIENDS:
			searchList.usersSelector = SC_USERS_SELECTOR_BUDDYHOOD;
			break;
		case COMMUNITY_SCOPE_ALL:
		default:
			searchList.usersSelector = SC_USERS_SELECTOR_ALL;
			break;
    }

    switch (time)
    {
		case TIME_SCOPE_TODAY:
			searchList.timeInterval = SC_TIME_INTERVAL_24H;
			break;
		case TIME_SCOPE_WEEK:
			searchList.timeInterval = SC_TIME_INTERVAL_7DAYS;
			break;
		case TIME_SCOPE_ALL:
		default:
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
       _pendingScoresResponse = false;
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
                score.rank = SC_Score_GetRank((SC_Score_h)score.handle);
                score.value = (float)SC_Score_GetResult((SC_Score_h)score.handle);
                SC_String_h formatted;
                SC_ScoreFormatter_FormatScore(scoreFormatter, (SC_Score_h)score.handle, SC_SCORE_FORMAT_DEFAULT, &formatted);
                score.valueFormatted = SC_String_GetData(formatted);
                SC_User_h player = SC_Score_GetUser((SC_Score_h)score.handle);
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

    session->_pendingScoresResponse = false;
}

void ScoreloopSocialSession::submitScore(const char* leaderboardId, float value)
{
    if (_pendingSubmitScoreResponse)
    {
        _listener->submitScoreEvent(SocialSessionListener::ERROR_PENDING_RESPONSE);
        return;
    }
    _pendingSubmitScoreResponse = true;

    SC_Error_t rc;
    rc = SC_Client_CreateScoreController(_client, &_scoreController, ScoreloopSocialSession::submitScoreCallback, this);
    if (rc != SC_OK)
    {
        _listener->submitScoreEvent(SocialSessionListener::ERROR_INITIALIZATION);
        _pendingSubmitScoreResponse = false;
        return;
    }

    // Create a score to submit
    SC_Score_h score;
    rc = SC_Client_CreateScore(_client, &score);
    if (rc != SC_OK)
    {
        _listener->submitScoreEvent(SocialSessionListener::ERROR_INITIALIZATION);
        _pendingSubmitScoreResponse = false;
        return;
    }

    // Set the leaderboard to associate this score with
    if (leaderboardId != NULL)
    {
        Properties* leaderboardMappings = _properties->getNamespace("leaderboard_mappings", true);
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
         _pendingSubmitScoreResponse = false;
         _listener->submitScoreEvent(SocialSessionListener::ERROR_INITIALIZATION);
    }

    if (_acceptedChallenge)
    {
        if (_pendingChallengeResponse)
        {
            _listener->submitChallengeEvent(SocialSessionListener::ERROR_PENDING_RESPONSE, *_acceptedChallenge);
            return;
        }
        _pendingChallengeResponse = true;

    	rc = SC_ChallengeController_SubmitChallengeScore(_challengeController, score);
    	if (rc != SC_OK)
    	{
    		_listener->submitChallengeEvent(SocialSessionListener::ERROR_INITIALIZATION, *_acceptedChallenge);
    		return;
    	}
    	_acceptedChallenge = 0;
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

    session->_pendingSubmitScoreResponse = false;
}

void ScoreloopSocialSession::submitChallenge(const SocialPlayer *player, float score, const char* leaderboardId, unsigned int wager)
{
	SocialChallenge challenge;

    if (_pendingChallengeResponse)
    {
        _listener->submitChallengeEvent(SocialSessionListener::ERROR_PENDING_RESPONSE, challenge);
        return;
    }
    _pendingChallengeResponse = true;

	SC_Error_t rc;
	SC_Challenge_h sc_challenge;
	SC_Money_h stake = NULL;

	if (!_challengeController)
	{
		rc = SC_Client_CreateChallengeController(_client, &_challengeController, ScoreloopSocialSession::submitChallengeCallback, this);
	    if (rc != SC_OK)
	    {
	    	_listener->submitChallengeEvent(SocialSessionListener::ERROR_INITIALIZATION, challenge);
	        return;
	    }
	}

	rc = SC_Client_CreateMoney(_client, &stake, wager);
    if (rc != SC_OK)
	{
		_listener->submitChallengeEvent(SocialSessionListener::ERROR_INITIALIZATION, challenge);
		return;
	}

    if (stake == NULL)
    {
        SC_MoneyList_h moneyList = SC_Session_GetChallengeStakes(SC_Client_GetSession(_client));
        /* Just pick the first stake here - if available */
        if (SC_MoneyList_GetCount(moneyList) > 0)
        {
            stake = SC_MoneyList_GetAt(moneyList, 0);
        }
    }

    if (stake == NULL)
	{
		_listener->submitChallengeEvent(SocialSessionListener::ERROR_INITIALIZATION, challenge);
	}

    // Lookup the mode to be mapped for the specified leaderboard id
	int mode = 0;
    if (leaderboardId != NULL)
    {
        Properties* leaderboardMappings = _properties->getNamespace("leaderboard_mappings", false);
        if (leaderboardMappings)
        {
            mode = leaderboardMappings->getInt(leaderboardId);
            if (mode >=  0)
            {
                SC_ScoresController_SetMode(_scoresController, (unsigned int)mode);
            }
        }
    }

	//Passing null as the contestant argument creates an open challenge.
	//The contender is automatically equal to the current session user.
    SC_User_h challenger = (player) ? (SC_User_h)player->handle : NULL;
	rc = SC_Client_CreateChallenge(_client, stake, challenger, mode, 0, &sc_challenge);
	if (rc != SC_OK)
	{
		_listener->submitChallengeEvent(SocialSessionListener::ERROR_INITIALIZATION, challenge);
		return;
	}

	rc = SC_ChallengeController_SetChallenge(_challengeController, sc_challenge);
	if (rc != SC_OK)
	{
		_listener->submitChallengeEvent(SocialSessionListener::ERROR_INITIALIZATION, challenge);
		return;
	}

	SC_Score_h sc_score;
	rc = SC_Client_CreateScore(_client, &sc_score);
	if (rc != SC_OK)
	{
		//_listener->submitChallengeScoreEvent(SocialSessionListener::ERROR_INITIALIZATION);
		return;
	}

	SC_Score_SetMode(sc_score, (unsigned int)mode);
	SC_Score_SetResult(sc_score, score);

	rc = SC_ChallengeController_SubmitChallengeScore(_challengeController, sc_score);
	if (rc != SC_OK)
	{
		_listener->submitChallengeEvent(SocialSessionListener::ERROR_INITIALIZATION, challenge);
		return;
	}
	SC_Score_Release(sc_score);

//	_listener->submitChallengeEvent(SocialSessionListener::SUCESS, challenge);
}

/**
 * @see SocialSession::submitChallenge
 */
void ScoreloopSocialSession::submitAchievementChallenge(const SocialPlayer *player, const char* achievementId, unsigned int wager)
{
	SocialChallenge challenge;
    _listener->submitChallengeEvent(SocialSessionListener::ERROR_NOT_SUPPORTED, challenge);
}

void ScoreloopSocialSession::submitChallengeCallback(void* cookie, SC_Error_t result)
{
    ScoreloopSocialSession* session = (ScoreloopSocialSession*)cookie;

    SocialChallenge challenge;

fprintf(stderr, "issueChallengeCallback is called!\n");
    switch (result)
    {
    	case SC_OK:
			{
				challenge.handle = SC_ChallengeController_GetChallenge(session->_challengeController);

				SC_User_h user = SC_Challenge_GetContestant((SC_Challenge_h)challenge.handle);
				SC_String_h playerStr = (user) ? SC_User_GetLogin(user) : NULL;
				challenge.challengedPlayerName = (playerStr) ? SC_String_GetData(playerStr) : "";

				user = SC_Challenge_GetContender((SC_Challenge_h)challenge.handle);
				playerStr = (user) ? SC_User_GetLogin(user) : NULL;
				challenge.issuedPlayerName = (playerStr) ? SC_String_GetData(playerStr) : "";

				SC_Score_h score = SC_Challenge_GetContenderScore((SC_Challenge_h)challenge.handle);
				challenge.score = (float)SC_Score_GetResult(score);
				SC_String_h dateStr = SC_Challenge_GetCreatedAt((SC_Challenge_h)challenge.handle);
				challenge.dateTimeIssued = (dateStr) ? SC_String_GetData(dateStr) : "";

				if (SC_Challenge_IsComplete((SC_Challenge_h)challenge.handle))
					challenge.state = SocialChallenge::COMPLETE;
				else if (SC_Challenge_IsRejected((SC_Challenge_h)challenge.handle))
					challenge.state = SocialChallenge::DECLINED;
				else if (SC_Challenge_IsAssigned((SC_Challenge_h)challenge.handle))
					challenge.state = SocialChallenge::PENDING;
				else
					challenge.state = SocialChallenge::INVALID;

				fprintf(stderr, "%s challenged player %s on %s\n", challenge.issuedPlayerName.data(), challenge.challengedPlayerName.data(), challenge.dateTimeIssued.data());
				fprintf(stderr, "score to beat is %lf current state is %d\n", challenge.score, challenge.state);

				session->getListener()->submitChallengeEvent(SocialSessionListener::SUCCESS, challenge);
			}
        	break;
		case SC_INVALID_USER_DATA:
			fprintf(stderr, "Hey, who the heck got us in here?\n");
			session->getListener()->submitChallengeEvent(SocialSessionListener::ERROR_INVALID_USER, challenge);
			break;
		case SC_HTTP_SERVER_ERROR:
			session->getListener()->submitChallengeEvent(SocialSessionListener::ERROR_SERVER, challenge);
			break;
		default:
			session->getListener()->submitChallengeEvent(SocialSessionListener::ERROR_UNKNOWN, challenge);
			break;
    }

    session->_pendingChallengeResponse = false;
}


void ScoreloopSocialSession::replyToChallenge(const SocialChallenge *challenge, bool accept)
{
	if (!challenge)
		return;

    if (_pendingChallengeResponse)
    {
        _listener->replyToChallengeEvent(SocialSessionListener::ERROR_PENDING_RESPONSE);
        return;
    }
    _pendingChallengeResponse = true;

	SC_Error_t rc;

	if (_challengeController)
	{
		SC_ChallengeController_Release(_challengeController);
fprintf(stderr, "creating new challenge controller\n");
		rc = SC_Client_CreateChallengeController(_client, &_challengeController, ScoreloopSocialSession::replyToChallengeCallback, this);
	    if (rc != SC_OK)
	    {
	    	_listener->replyToChallengeEvent(SocialSessionListener::ERROR_INITIALIZATION);
	        return;
	    }
	}

	rc = SC_ChallengeController_SetChallenge(_challengeController, challenge1); //(SC_Challenge_h)challenge->handle);
	if (rc != SC_OK)
	{
		SC_ChallengeController_Release(_challengeController);
		_challengeController = 0;
		fprintf(stderr, "have error SetChallenge %d %s\n", rc, SC_MapErrorToStr(rc));
		_listener->replyToChallengeEvent(SocialSessionListener::ERROR_INITIALIZATION);
		return;
	}

fprintf(stderr, "replyTochallenge %d\n", accept);
	if (accept)
	{
		rc = SC_ChallengeController_AcceptChallenge(_challengeController);
		_acceptedChallenge = challenge;
	}
	else
	{
		rc = SC_ChallengeController_RejectChallenge(_challengeController);
	}

	if (rc != SC_OK)
	{
		fprintf(stderr, "have error in accept/reject challenge %d %s\n", rc, SC_MapErrorToStr(rc));
		SC_ChallengeController_Release(_challengeController);
		_challengeController = 0;
	//	_listener->issueChallengeEvent(SocialSessionListener::ERROR_INITIALIZATION, challenge);
		return;
	}
}

void ScoreloopSocialSession::replyToChallengeCallback(void* cookie, SC_Error_t result)
{
    ScoreloopSocialSession* session = (ScoreloopSocialSession*)cookie;

fprintf(stderr, "replyToChallengeCallback is called!\n");
    switch (result)
    {
    	case SC_OK:
			session->getListener()->replyToChallengeEvent(SocialSessionListener::SUCCESS);
        	break;
		case SC_INVALID_USER_DATA:
			session->_acceptedChallenge = 0;
			session->getListener()->replyToChallengeEvent(SocialSessionListener::ERROR_INVALID_USER);
			break;
		case SC_HTTP_SERVER_ERROR:
			session->_acceptedChallenge = 0;
			session->getListener()->replyToChallengeEvent(SocialSessionListener::ERROR_SERVER);
			break;
		default:
			session->_acceptedChallenge = 0;
			session->getListener()->replyToChallengeEvent(SocialSessionListener::ERROR_UNKNOWN);
			break;
    }

    session->_pendingChallengeResponse = false;

    SC_ChallengeController_Release(session->_challengeController);
    session->_challengeController = 0;
}

void ScoreloopSocialSession::loadChallenges(bool showOpenChallengesOnly)
{
    if (_pendingChallengesResponse)
    {
    	_listener->loadChallengesEvent(SocialSessionListener::ERROR_PENDING_RESPONSE, _challenges);
        return;
    }
    _pendingChallengesResponse = true;

	int mode;
	SC_Error_t rc;

	if (!_challengesController)
	{
		rc = SC_Client_CreateChallengesController(_client, &_challengesController, ScoreloopSocialSession::loadChallengesCallback, this);
	    if (rc != SC_OK)
	    {
	    	_listener->loadChallengesEvent(SocialSessionListener::ERROR_INITIALIZATION, _challenges);
	    	_pendingChallengesResponse = false;
	        return;
	    }
	}

	if (showOpenChallengesOnly)
		rc = SC_ChallengesController_LoadOpenChallenges(_challengesController);
	else
		rc = SC_ChallengesController_LoadChallengeHistory(_challengesController);

    if (rc != SC_OK)
    {
    	_listener->loadChallengesEvent(SocialSessionListener::ERROR_INITIALIZATION, _challenges);
       	_pendingChallengesResponse = false;
        return;
    }
}

SocialChallenge &ScoreloopSocialSession::addChallenge(SC_Challenge_h scoreloopChallenge)
{
	SocialChallenge challenge;

	challenge.handle = scoreloopChallenge;

	SC_User_h user = SC_Challenge_GetContestant((SC_Challenge_h)challenge.handle);
	SC_String_h playerStr = (user) ? SC_User_GetLogin(user) : NULL;
	challenge.challengedPlayerName = (playerStr) ? SC_String_GetData(playerStr) : "";

	user = SC_Challenge_GetContender((SC_Challenge_h)challenge.handle);
	playerStr = (user) ? SC_User_GetLogin(user) : NULL;
	challenge.issuedPlayerName = (playerStr) ? SC_String_GetData(playerStr) : "";

	SC_Score_h score = SC_Challenge_GetContenderScore((SC_Challenge_h)challenge.handle);
	challenge.score = (score) ? (float)SC_Score_GetResult(score) : 0;
	SC_String_h dateStr = SC_Challenge_GetCreatedAt((SC_Challenge_h)challenge.handle);
	challenge.dateTimeIssued = (dateStr) ? SC_String_GetData(dateStr) : "";
	dateStr = SC_Challenge_GetCompletedAt((SC_Challenge_h)challenge.handle);
	challenge.dateTimeCompleted = (dateStr) ? SC_String_GetData(dateStr) : "";

	if (SC_Challenge_IsComplete((SC_Challenge_h)challenge.handle))
		challenge.state = SocialChallenge::COMPLETE;
	else if (SC_Challenge_IsRejected((SC_Challenge_h)challenge.handle))
		challenge.state = SocialChallenge::DECLINED;
	else if (SC_Challenge_IsAssigned((SC_Challenge_h)challenge.handle))
		challenge.state = SocialChallenge::PENDING;
	else
		challenge.state = SocialChallenge::INVALID;

    _challenges.push_back(challenge);

    return _challenges.back();
}

void ScoreloopSocialSession::loadChallengesCallback(void* cookie, SC_Error_t result)
{
    ScoreloopSocialSession* session = (ScoreloopSocialSession*)cookie;

    switch (result)
    {
    	case SC_OK:
        {
        	SC_ChallengeList_h challengeList = SC_ChallengesController_GetChallenges(session->_challengesController);

            unsigned int count = SC_ChallengeList_GetCount(challengeList);
            session->_challenges.clear();

            for (unsigned int i = 0; i < count; i++)
            {
            	SC_Challenge_h sc_challenge = SC_ChallengeList_GetAt(challengeList, i);
            	session->addChallenge(sc_challenge);
            }

            session->getListener()->loadChallengesEvent(SocialSessionListener::SUCCESS, session->_challenges);
        }
        break;

		case SC_INVALID_USER_DATA:
			session->getListener()->loadChallengesEvent(SocialSessionListener::ERROR_INVALID_USER, session->_challenges);
			break;
		case SC_HTTP_SERVER_ERROR:
			session->getListener()->loadChallengesEvent(SocialSessionListener::ERROR_SERVER, session->_challenges);
			break;
		default:
			session->getListener()->loadChallengesEvent(SocialSessionListener::ERROR_UNKNOWN, session->_challenges);
			break;
    }

    session->_pendingChallengesResponse = false;
}

void ScoreloopSocialSession::loadSavedData(const char* key)
{
    if (_pendingDataResponse)
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
    _pendingDataResponse = true;
    _key = key;
}

void ScoreloopSocialSession::submitSavedData(const char* key, std::string data)
{
    if (_pendingDataResponse)
    {
        _listener->submitSavedDataEvent(SocialSessionListener::ERROR_PENDING_RESPONSE);
        return;
    }
    _pendingDataResponse = true;

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
    SC_User_SetContext((SC_User_h)_user.handle, context);
    SC_UserController_SetUser(_userController, (SC_User_h)_user.handle);
    rc = SC_UserController_UpdateUserContext(_userController);
    if (rc != SC_OK)
    {
        _listener->submitSavedDataEvent(SocialSessionListener::ERROR_INITIALIZATION);
        return;
    }
}

void ScoreloopSocialSession::displayLeaderboard(const char* leaderboardId) const
{
	SC_Error_t rc;
	int mode = 0;

    // Set the leaderboard to associate this leaderboard with
    if (leaderboardId != NULL)
    {
        Properties* leaderboardMappings = _properties->getNamespace("leaderboard_mappings", true);
        if (leaderboardMappings)
        {
            mode = leaderboardMappings->getInt(leaderboardId);
        }
    }

	rc = SCUI_Client_SetLeadearboardFlags(_uiClient, SCUI_LEADERBOARD_FLAGS_SHOW_LIST_AROUND_USER);
	if (rc != SC_OK)
	{
		_listener->uiEvent(SocialSessionListener::ERROR_INITIALIZATION, SC_MapErrorToStr(rc));
		return;
	}

	rc = SCUI_Client_ShowLeaderboardView(_uiClient, mode, SCUI_LEADERBOARD_TYPE_GLOBAL, NULL);
	if (rc != SC_OK)
	{
		_listener->uiEvent(SocialSessionListener::ERROR_INITIALIZATION, SC_MapErrorToStr(rc));
	}
}

void ScoreloopSocialSession::displayAchievements() const
{
	SC_Error_t rc;

	rc = SCUI_Client_SetAchievementFlags(_uiClient, SCUI_ACHIEVEMENT_FLAGS_DEFAULT);
	if (rc != SC_OK)
	{
		_listener->uiEvent(SocialSessionListener::ERROR_INITIALIZATION, SC_MapErrorToStr(rc));
		return;
	}

	rc = SCUI_Client_ShowAchievementsView(_uiClient);
	if (rc != SC_OK)
	{
		_listener->uiEvent(SocialSessionListener::ERROR_INITIALIZATION, SC_MapErrorToStr(rc));
	}
}

void ScoreloopSocialSession::displayChallenges() const
{
	fprintf(stderr, "displayChallenges has been called\n");
    SC_Error_t rc = SC_NOT_IMPLEMENTED_YET;

    rc = SCUI_Client_SetChallengeFlags(_uiClient, SCUI_CHALLENGE_FLAGS_DEFAULT);
    if (rc != SC_OK)
    {
        fprintf(stderr, "Can't set challenge flags\n");
        return;
    }

    char format[] = "%.2f";
    rc = SCUI_Client_SetSimpleMoneyFormat(_uiClient, format);
    if (rc != SC_OK)
    {
        fprintf(stderr, "Can't set money format\n");
        return;
    }

    rc = SCUI_Client_ShowChallengesView(_uiClient);
   // rc = SCUI_Client_ShowChallengeCreateView(_uiClient);
    if (rc != SC_OK)
    {
        fprintf(stderr, "Failed to start a Challenges card! %s\n", SC_MapErrorToStr(rc));
    }
}

void ScoreloopSocialSession::displayChallengeSubmit(const SocialChallenge *challenge, float score) const
{
	SC_Error_t rc = SC_NOT_IMPLEMENTED_YET;

	if (!challenge)
		return;

	rc = SCUI_Client_SetChallengeFlags(_uiClient, SCUI_CHALLENGE_FLAGS_DEFAULT);
    if (rc != SC_OK)
    {
    	fprintf(stderr, "Can't set challenge flags\n");
        return;
    }

    char format[] = "%.2f";
    rc = SCUI_Client_SetSimpleMoneyFormat(_uiClient, format);
    if (rc != SC_OK)
    {
    	fprintf(stderr, "Can't set money format\n");
        return;
    }
    fprintf(stderr, "just before setting the score\n");
	SC_Score_h sc_score;
	rc = SC_Client_CreateScore(_client, &sc_score);
	if (rc != SC_OK)
	{
		//_listener->submitChallengeScoreEvent(SocialSessionListener::ERROR_INITIALIZATION);
		return;
	}

	int mode = 0;
	SC_Score_SetMode(sc_score, (unsigned int)mode);
	SC_Score_SetResult(sc_score, score);
    rc = SCUI_Client_ShowChallengeSubmitView(_uiClient, (SC_Challenge_h)challenge->handle, sc_score);
    if (rc != SC_OK)
    {
    	fprintf(stderr, "Failed to start a Challenges card! %s\n", SC_MapErrorToStr(rc));
    }
}

void ScoreloopSocialSession::displayPopup(const char *popupMessage, const char *title) const
{
	dialog_instance_t dialog = 0;

	dialog_create_toast(&dialog);

	dialog_set_group_id(dialog, "dialogId");

	dialog_set_toast_position(dialog, DIALOG_POSITION_TOP_CENTER);
	dialog_set_toast_message_text(dialog, popupMessage);
    
    if (title)
        dialog_set_title_text(dialog, title);

	dialog_show(dialog);
}

}

#endif
