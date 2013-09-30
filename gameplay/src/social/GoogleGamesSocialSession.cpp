#if defined(__ANDROID__) && defined(GP_USE_SOCIAL)

#include "Base.h"
#include "GoogleGamesSocialSession.h"
#include <android_native_app_glue.h>
#include <android/log.h>

namespace gameplay
{

extern struct android_app* __state;

GoogleGamesSocialSession* GoogleGamesSocialSession::_session = NULL;

GoogleGamesSocialSession::GoogleGamesSocialSession()
    : SocialSession(),
      _listener(NULL), _properties(NULL), _pendingUserResponse(false), _pendingFriendsResponse(false),
      _pendingScoresResponse(false), _pendingSubmitScoreResponse(false), _pendingAchievementResponse(false),
      _pendingDataResponse(false),
      _key(NULL)
{
    _userOp = USEROP_GET_LOCALUSER;
}

GoogleGamesSocialSession::~GoogleGamesSocialSession()
{
}

SocialSessionListener* GoogleGamesSocialSession::getListener()
{
    return _listener;
}

SocialSession *GoogleGamesSocialSession::authenticate(SocialSessionListener* listener, Properties* properties)
{
    if (!_session)
    {
		_session = new GoogleGamesSocialSession();
		_session->_listener = listener;
		_session->_properties = properties;

		const char* gameId = properties->getString("id");
		const char* gameSecret = properties->getString("secret");
		const char* gameVersion = properties->getString("version");
		const char* gameCurrency = properties->getString("currency");
		const char* gameLanguage = properties->getString("language");
#ifdef __ANDROID__
	    android_app* state = __state;
	    GP_ASSERT(state && state->activity && state->activity->vm);
	    JavaVM* jvm = state->activity->vm;
	    JNIEnv* env = NULL;
	    jvm->GetEnv((void **)&env, JNI_VERSION_1_6);
	    jint r = jvm->AttachCurrentThread(&env, NULL);
	    if (r == JNI_ERR)
	    {
	        GP_ERROR("Failed to retrieve JVM environment to authenticate.");
	        return 0;
	    }
	    GP_ASSERT(env);

		jclass testClassHandle = env->FindClass("org/gameplay3d/lib/TestClass");
		jmethodID computeMultHandle = env->GetMethodID(testClassHandle, "ComputeMult", "(FF)I");
		jmethodID testClassCtor = env->GetMethodID(testClassHandle, "<init>", "()V");
		jobject testClassInstance = env->NewObject(testClassHandle, testClassCtor);

		jfloat a = 4.0f;
		jfloat b = 0.5f;
		jint result = env->CallIntMethod(testClassInstance, computeMultHandle, a, b);
		fprintf(stderr, "the results is %d\n", result);

		jvm->DetachCurrentThread();
#endif
    }

    return _session;
}

const SocialPlayer& GoogleGamesSocialSession::getUser() const
{
	return _user;
}

/**
 * @see SocialSession::loadFriends
 */
void loadFriends()
{

}

/**
 * @see SocialSession::loadAchievements
 */
void loadAchievements()
{

}

/**
 * @see SocialSession::submitAchievement
 */
void submitAchievement(const char* achievementId, unsigned int value, bool achieved=false)
{

}

/**
 * @see SocialSession::incrementAchievement
 */
void incrementAchievement(const char* achievementId, unsigned int increment=1)
{

}

/**
  * @see SocialSession::syncAchievements
  */
void synchronizeAchievements()
{

}

/**
 * @see SocialSession::loadScores
 */
void loadScores(const char* leaderboardId, SocialSession::CommunityScope community, SocialSession::TimeScope time, const SocialPlayer& player, unsigned int count)
{

}

/**
 * @see SocialSession::loadScores
 */
void loadScores(const char* leaderboardId, SocialSession::CommunityScope community, SocialSession::TimeScope time, unsigned int start, unsigned int count)
{

}

/**
 * @see SocialSession::submitScore
 */
void submitScore(const char* leaderboardId, float score)
{

}

/**
  * @see SocialSession::submitChallenge
  */
void submitChallenge(const SocialPlayer *player, unsigned int wager, float score, const char* leaderboardId=0)
{

}

/**
  * @see SocialSession::loadChallenges
  */
void loadChallenges(bool showOpenChallengesOnly=true)
{

}

/**
  * @see SocialSession::acceptChallenge
  */
void replyToChallenge(const SocialChallenge *challenge, bool accept)
{

}

/**
 * @see SocialSession::requestSavedData
 */
void loadSavedData(const char* key) {}

/**
 * @see SocialSession::submitSavedData
 */
void submitSavedData(const char* key, std::string data) {}

void displayLeaderboard(const char* leaderboardId) {}

void displayAchievements() {}

void displayChallenges() {}

void displayChallengeSubmit(const SocialChallenge *challenge, float score) {}

#if 0

void GoogleGamesSocialSession::loadFriends()
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
        rc = SC_Client_CreateUsersController(_client, &_usersController, GoogleGamesSocialSession::loadFriendsCallback, this);
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

void GoogleGamesSocialSession::loadFriendsCallback(void* cookie, SC_Error_t result)
{
    GoogleGamesSocialSession* session = (GoogleGamesSocialSession*)cookie;

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

void GoogleGamesSocialSession::loadAchievements()
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
		rc = SC_Client_CreateLocalAchievementsController(_client, &_localAchievementsController, GoogleGamesSocialSession::loadAchievementsCallback, this);
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
    	GoogleGamesSocialSession::loadAchievementsCallback(this, SC_OK);
}

void GoogleGamesSocialSession::loadAchievementsCallback(void* cookie, SC_Error_t result)
{
    GoogleGamesSocialSession* session = (GoogleGamesSocialSession*)cookie;

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
#if 0
					fprintf(stderr, "achievement %s\n", achievement.name.data());
					fprintf(stderr, "isachieved %d\n", SC_Achievement_IsAchieved((SC_Achievement_h)achievement.handle));
						fprintf(stderr, "value %d\n", SC_Achievement_GetValue((SC_Achievement_h)achievement.handle));
						fprintf(stderr, "title %s\n", SC_String_GetData(SC_Award_GetLocalizedTitle(award)));
						fprintf(stderr, "desc %s\n", SC_String_GetData(SC_Award_GetLocalizedDescription(award)));
						fprintf(stderr, "money %d\n", SC_Money_GetAmount(SC_Award_GetRewardedMoney(award)));
						fprintf(stderr, "image name %s\n", SC_String_GetData(SC_Achievement_GetImageName((SC_Achievement_h)achievement.handle)));
#endif
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

const SocialAchievement* GoogleGamesSocialSession::getAchievement(const char* achievementId) const
{
	uint size = _achievements.size();

	for (uint i = 0; i < size; i++)
	{
		if (strcmp(_achievements[i].name.data(), achievementId) == 0)
			return &_achievements[i];
	}
	return 0;
}

void GoogleGamesSocialSession::submitAchievement(const char* achievementId, unsigned int value, bool isAchieved)
{
    SC_Error_t rc;
    if (_localAchievementsController == NULL)
    {
		rc = SC_Client_CreateLocalAchievementsController(_client, &_localAchievementsController, GoogleGamesSocialSession::submitAchievementCallback, this);
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

void GoogleGamesSocialSession::incrementAchievement(const char* achievementId, unsigned int increment)
{
    SC_Error_t rc;
    if (_localAchievementsController == NULL)
    {
		rc = SC_Client_CreateLocalAchievementsController(_client, &_localAchievementsController, GoogleGamesSocialSession::submitAchievementCallback, this);
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

void GoogleGamesSocialSession::synchronizeAchievements()
{
    SC_Error_t rc;
    if (_localAchievementsController == NULL)
    {
		rc = SC_Client_CreateLocalAchievementsController(_client, &_localAchievementsController, GoogleGamesSocialSession::submitAchievementCallback, this);
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

void GoogleGamesSocialSession::submitAchievementCallback(void* cookie, SC_Error_t result)
{
    GoogleGamesSocialSession* session = (GoogleGamesSocialSession*)cookie;

    fprintf(stderr, "submitAchievementCallback is called here\n");

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

void GoogleGamesSocialSession::loadScores(const char* leaderboardId, CommunityScope community, TimeScope time, const SocialPlayer& player, unsigned int count)
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
        rc = SC_Client_CreateScoresController(_client, &_scoresController, GoogleGamesSocialSession::loadScoresCallback, this);
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

void GoogleGamesSocialSession::loadScores(const char* leaderboardId, CommunityScope community, TimeScope time, unsigned int start, unsigned int count)
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
        rc = SC_Client_CreateScoresController(_client, &_scoresController, GoogleGamesSocialSession::loadScoresCallback, this);
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

void GoogleGamesSocialSession::loadScoresCallback(void* cookie, SC_Error_t result)
{
    GoogleGamesSocialSession* session = (GoogleGamesSocialSession*)cookie;

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

void GoogleGamesSocialSession::submitScore(const char* leaderboardId, float value)
{
    if (_pendingSubmitScoreResponse)
    {
        _listener->submitScoreEvent(SocialSessionListener::ERROR_PENDING_RESPONSE);
        return;
    }
    _pendingSubmitScoreResponse = true;

    SC_Error_t rc;
    rc = SC_Client_CreateScoreController(_client, &_scoreController, GoogleGamesSocialSession::submitScoreCallback, this);
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

void GoogleGamesSocialSession::submitScoreCallback(void* cookie, SC_Error_t result)
{
    GoogleGamesSocialSession* session = (GoogleGamesSocialSession*)cookie;

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

void GoogleGamesSocialSession::submitChallenge(const SocialPlayer *player, unsigned int wager, float score, const char* leaderboardId)
{
	_listener->submitChallengeEvent(SocialSessionListener::ERROR_NOT_SUPPORTED, 0);
}

void GoogleGamesSocialSession::replyToChallenge(const SocialChallenge *challenge, bool accept)
{
	_listener->replyToChallengeEvent(SocialSessionListener::ERROR_NOT_SUPPORTED);
}

void GoogleGamesSocialSession::loadChallenges(bool showOpenChallengesOnly)
{
	_listener->loadChallengesEvent(SocialSessionListener::ERROR_NOT_SUPPORTED, _challenges);
}

void GoogleGamesSocialSession::loadSavedData(const char* key)
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

void GoogleGamesSocialSession::submitSavedData(const char* key, std::string data)
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

void GoogleGamesSocialSession::displayLeaderboard(const char* leaderboardId)
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

void GoogleGamesSocialSession::displayAchievements()
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

void GoogleGamesSocialSession::displayChallenges()
{
	_listener->uiEvent(SocialSessionListener::ERROR_NOT_SUPPORTED, "displayChallenges is not supported by Google Games Services");
}

void GoogleGamesSocialSession::displayChallengeSubmit(const SocialChallenge *challenge, float score)
{
	_listener->uiEvent(SocialSessionListener::ERROR_NOT_SUPPORTED, "displayChallengeSubmit is not supported by Google Games Services");
}
#endif

}

#endif
