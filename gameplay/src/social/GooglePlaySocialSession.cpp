#if defined(__ANDROID__) && defined(GP_USE_SOCIAL)

#include "Base.h"
#include "GooglePlaySocialSession.h"
#include <android_native_app_glue.h>
#include <android/log.h>

namespace gameplay
{

// Externally referenced global variable.
struct android_app* __state;

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
#if 0
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
#endif

    return _session;
}

const SocialPlayer& GooglePlaySocialSession::getUser() const
{
	return _user;
}

void GooglePlaySocialSession::loadFriends()
{
}

void GooglePlaySocialSession::loadAchievementData()
{

}

void GooglePlaySocialSession::loadAchievements()
{

}

const SocialAchievement* GooglePlaySocialSession::getAchievement(const char* achievementId) const
{
    uint size = _achievements.size();

    for (uint i = 0; i < size; i++)
    {
        if (strcmp(_achievements[i].name.data(), achievementId) == 0)
            return &_achievements[i];
    }
    return 0;
}

void GooglePlaySocialSession::submitAchievement(const char* achievementId, unsigned int value, bool achieved)
{

}

void GooglePlaySocialSession::incrementAchievement(const char* achievementId, unsigned int totalSteps)
{

}

void GooglePlaySocialSession::synchronizeAchievements()
{
}

void GooglePlaySocialSession::loadScores(const char* leaderboardId, SocialSession::CommunityScope community, SocialSession::TimeScope time, unsigned int start, unsigned int count)
{

}

void GooglePlaySocialSession::loadScores(const char* leaderboardId, SocialSession::CommunityScope community, SocialSession::TimeScope time, const SocialPlayer& player, unsigned int count)
{

}

void GooglePlaySocialSession::submitScore(const char* leaderboardId, float score)
{

}

void GooglePlaySocialSession::submitChallenge(const SocialPlayer *player, float score, const char* leaderboardId, unsigned int wager)
{
    SocialChallenge challenge;
    _listener->submitChallengeEvent(SocialSessionListener::ERROR_NOT_SUPPORTED, challenge);
}

void GooglePlaySocialSession::submitAchievementChallenge(const SocialPlayer *player, const char* achievementId, unsigned int wager)
{
    SocialChallenge challenge;
    _listener->submitChallengeEvent(SocialSessionListener::ERROR_NOT_SUPPORTED, challenge);
}

void GooglePlaySocialSession::loadChallenges(bool showOpenChallengesOnly)
{
	_listener->loadChallengesEvent(SocialSessionListener::ERROR_NOT_SUPPORTED, _challenges);
}


void GooglePlaySocialSession::replyToChallenge(const SocialChallenge *challenge, bool accept)
{
}

void GooglePlaySocialSession::loadSavedData(const char* key)
{
}

void GooglePlaySocialSession::submitSavedData(const char* key, std::string data)
{

}

void GooglePlaySocialSession::displayLeaderboard(const char* leaderboardId) const
{
}

void GooglePlaySocialSession::displayAchievements() const
{
}

void GooglePlaySocialSession::displayChallenges() const
{
}

void GooglePlaySocialSession::displayChallengeSubmit(const SocialChallenge *challenge, float score) const
{
}

void GooglePlaySocialSession::displayPopup(const char *popupMessage, const char *title) const
{

}

}

#endif
