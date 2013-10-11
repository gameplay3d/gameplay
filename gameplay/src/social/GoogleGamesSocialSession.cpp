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
void GoogleGamesSocialSession::loadFriends()
{

}

/**
 * @see SocialSession::loadAchievements
 */
void GoogleGamesSocialSession::loadAchievements()
{

}

/**
 * @see SocialSession::submitAchievement
 */
void GoogleGamesSocialSession::submitAchievement(const char* achievementId, unsigned int value, bool achieved=false)
{

}

/**
 * @see SocialSession::incrementAchievement
 */
void GoogleGamesSocialSession::incrementAchievement(const char* achievementId, unsigned int increment=1)
{

}

/**
  * @see SocialSession::syncAchievements
  */
void GoogleGamesSocialSession::synchronizeAchievements()
{

}

/**
 * @see SocialSession::loadScores
 */
void GoogleGamesSocialSession::loadScores(const char* leaderboardId, SocialSession::CommunityScope community, SocialSession::TimeScope time, const SocialPlayer& player, unsigned int count)
{

}

/**
 * @see SocialSession::loadScores
 */
void GoogleGamesSocialSession::loadScores(const char* leaderboardId, SocialSession::CommunityScope community, SocialSession::TimeScope time, unsigned int start, unsigned int count)
{

}

/**
 * @see SocialSession::submitScore
 */
void GoogleGamesSocialSession::submitScore(const char* leaderboardId, float score)
{

}

/**
  * @see SocialSession::submitChallenge
  */
void GoogleGamesSocialSession::submitChallenge(const SocialPlayer *player, unsigned int wager, float score, const char* leaderboardId=0)
{

}

/**
  * @see SocialSession::loadChallenges
  */
void GoogleGamesSocialSession::loadChallenges(bool showOpenChallengesOnly=true)
{

}

/**
  * @see SocialSession::acceptChallenge
  */
void GoogleGamesSocialSession::replyToChallenge(const SocialChallenge *challenge, bool accept)
{

}

/**
 * @see SocialSession::requestSavedData
 */
void GoogleGamesSocialSession::loadSavedData(const char* key) {}

/**
 * @see SocialSession::submitSavedData
 */
void GoogleGamesSocialSession::submitSavedData(const char* key, std::string data) {}

void GoogleGamesSocialSession::displayLeaderboard(const char* leaderboardId) {}

void GoogleGamesSocialSession::displayAchievements() {}

void GoogleGamesSocialSession::displayChallenges() {}

void GoogleGamesSocialSession::displayChallengeSubmit(const SocialChallenge *challenge, float score) {}


}

#endif
