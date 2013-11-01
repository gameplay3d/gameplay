#if defined(__ANDROID__) && defined(GP_USE_SOCIAL)

#include "Base.h"
#include "GooglePlaySocialSession.h"
#include <android_native_app_glue.h>
#include <android/log.h>

extern struct android_app* __state;

namespace gameplay
{


GooglePlaySocialSession* GooglePlaySocialSession::_session = NULL;

GooglePlaySocialSession::GooglePlaySocialSession()
    : SocialSession(),
      _listener(NULL), _properties(NULL), _pendingUserResponse(false), _pendingFriendsResponse(false),
      _pendingScoresResponse(false), _pendingSubmitScoreResponse(false), _pendingAchievementResponse(false),
      _pendingDataResponse(false),
      _key(NULL)
{
    _userOp = USEROP_GET_LOCALUSER;
}

GooglePlaySocialSession::~GooglePlaySocialSession()
{
}

SocialSessionListener* GooglePlaySocialSession::getListener()
{
    return _listener;
}

SocialSession *GooglePlaySocialSession::authenticate(SocialSessionListener* listener, Properties* properties)
{
    if (!_session)
    {
		_session = new GooglePlaySocialSession();
		_session->_listener = listener;
		_session->_properties = properties;

		const char* gameId = properties->getString("id");
		const char* gameSecret = properties->getString("secret");
		const char* gameVersion = properties->getString("version");
		const char* gameCurrency = properties->getString("currency");
		const char* gameLanguage = properties->getString("language");
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
    }

    return _session;
}

const SocialPlayer& GooglePlaySocialSession::getUser() const
{
	return _user;
}

/**
 * @see SocialSession::loadFriends
 */
void GooglePlaySocialSession::loadFriends()
{
}

/**
 * @see SocialSession::loadAchievements
 */
void GooglePlaySocialSession::loadAchievements()
{
}

/**
 * @see SocialSession::submitAchievement
 */
void GooglePlaySocialSession::submitAchievement(const char* achievementId, unsigned int value, bool achieved)
{
}

/**
 * @see SocialSession::incrementAchievement
 */
void GooglePlaySocialSession::incrementAchievement(const char* achievementId, unsigned int increment)
{
}

/**
  * @see SocialSession::synchronizeAchievements
  */
void GooglePlaySocialSession::synchronizeAchievements()
{
}

/**
 * @see SocialSession::loadScores
 */
void GooglePlaySocialSession::loadScores(const char* leaderboardId, SocialSession::CommunityScope community, SocialSession::TimeScope time, const SocialPlayer& player, unsigned int count)
{
}

/**
 * @see SocialSession::loadScores
 */
void GooglePlaySocialSession::loadScores(const char* leaderboardId, SocialSession::CommunityScope community, SocialSession::TimeScope time, unsigned int start, unsigned int count)
{
}

/**
 * @see SocialSession::submitScore
 */
void GooglePlaySocialSession::submitScore(const char* leaderboardId, float score)
{
}

/**
  * @see SocialSession::submitChallenge
  */
void GooglePlaySocialSession::submitChallenge(const SocialPlayer *player, unsigned int wager, float score, const char* leaderboardId)
{
}

/**
  * @see SocialSession::loadChallenges
  */
void GooglePlaySocialSession::loadChallenges(bool showOpenChallengesOnly)
{
}

/**
  * @see SocialSession::acceptChallenge
  */
void GooglePlaySocialSession::replyToChallenge(const SocialChallenge *challenge, bool accept)
{
}

/**
 * @see SocialSession::requestSavedData
 */
void GooglePlaySocialSession::loadSavedData(const char* key) {}

/**
 * @see SocialSession::submitSavedData
 */
void GooglePlaySocialSession::submitSavedData(const char* key, std::string data) {}

void GooglePlaySocialSession::displayLeaderboard(const char* leaderboardId) {}

void GooglePlaySocialSession::displayAchievements() {}

void GooglePlaySocialSession::displayChallenges() {}

void GooglePlaySocialSession::displayChallengeSubmit(const SocialChallenge *challenge, float score) {}


}

#endif
