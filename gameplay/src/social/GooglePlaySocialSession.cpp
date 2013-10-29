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
#if 0
    GKLocalPlayer *lp = [GKLocalPlayer localPlayer];
    if (lp.authenticated)
    {
        [lp loadFriendsWithCompletionHandler:^(NSArray *friendIDs, NSError *error)
        {
            _friends.clear();

            if (friendIDs != nil && friendIDs.count > 0)
            {
                [GKPlayer loadPlayersForIdentifiers:friendIDs withCompletionHandler:^(NSArray *players, NSError *error)
                {
                    if (error == nil)
                    {
                        if (players != nil)
                        {
                            // Process the array of GKPlayer objects.
                            for (GKPlayer* friendPlayer in players)
                            {
                                SocialPlayer player;
                                player.handle = friendPlayer;
                                player.name = [friendPlayer.displayName UTF8String];
                                _friends.push_back(player);
                            }
                        }

                        _listener->loadFriendsEvent(SocialSessionListener::SUCCESS, _friends);
                    }
                    else
                    {
                        NSLog(@"Error in loadFriends: %@", error);

                        if (error.code == GKErrorInvalidParameter || error.code == GKErrorInvalidPlayer)
                            _listener->loadFriendsEvent(SocialSessionListener::ERROR_INVALID_ARG, _friends);
                        else if (error.code == GKErrorCommunicationsFailure)
                            _listener->loadFriendsEvent(SocialSessionListener::ERROR_SERVER, _friends);
                        else if (error.code == GKErrorCancelled)
                            _listener->loadFriendsEvent(SocialSessionListener::ERROR_CANCELLED, _friends);
                        else
                            _listener->loadFriendsEvent(SocialSessionListener::ERROR_UNKNOWN, _friends);
                    }

                }];
            }
            else
            {
                // user currently has 0 friends so just return the empty list back
                _listener->loadFriendsEvent(SocialSessionListener::SUCCESS, _friends);
            }
        }];
    }
#endif
}

void GooglePlaySocialSession::loadAchievementData()
{
#if 0
    [GKAchievement loadAchievementsWithCompletionHandler: ^(NSArray *awards, NSError *error)
     {
         if (error == nil)
         {
             NSDateFormatter *formatter = [[NSDateFormatter alloc] init];
             [formatter setDateFormat:@"ddmmyyyy"];
             // Optionally for time zone converstions
             [formatter setTimeZone:[NSTimeZone timeZoneWithName:@"..."]];

             for (int i = 0; i < _achievements.size(); i++)
             {
                 for (GKAchievement* award in awards)
                 {
                     if (_achievements[i].name == [award.identifier UTF8String])
                     {
                         NSString *dateString = [formatter stringFromDate:award.lastReportedDate];

                         _achievements[i].value = award.percentComplete;
                         _achievements[i].percentCompleted = _achievements[i].value;
                         _achievements[i].dateTimeAchieved = [dateString UTF8String];

                         break;
                     }
                 }
             }

             _listener->loadAchievementsEvent(SocialSessionListener::SUCCESS, _achievements);

             [formatter release];
         }
         else
         {
             NSLog(@"Error in loadAchievementData: %@", error);

             if (error.code == GKErrorInvalidParameter || error.code == GKErrorInvalidPlayer)
                 _listener->loadAchievementsEvent(SocialSessionListener::ERROR_INVALID_ARG, _achievements);
             else if (error.code == GKErrorCommunicationsFailure)
                 _listener->loadAchievementsEvent(SocialSessionListener::ERROR_SERVER, _achievements);
             else if (error.code == GKErrorCancelled)
                 _listener->loadAchievementsEvent(SocialSessionListener::ERROR_CANCELLED, _achievements);
             else
                 _listener->loadAchievementsEvent(SocialSessionListener::ERROR_UNKNOWN, _achievements);
         }
     }];
#endif
}

void GooglePlaySocialSession::loadAchievements()
{
#if 0
    [GKAchievementDescription loadAchievementDescriptionsWithCompletionHandler: ^(NSArray *awards, NSError *error)
     {
         if (error == nil)
         {
             _achievements.clear();

             for (GKAchievementDescription* award in awards)
             {
                 SocialAchievement achievement;
                 achievement.handle = award;

                 achievement.name = [award.identifier UTF8String];
                 achievement.title = [award.title UTF8String];
                 achievement.value = 0;
                 achievement.total = 100;
                 achievement.percentCompleted = 0;

                 _achievements.push_back(achievement);
             }

             // we now have to load the achievement data to complete our achievement information
             loadAchievementData();
         }
         else
         {
             NSLog(@"Error in loadAchievements: %@", error);

             if (error.code == GKErrorInvalidParameter || error.code == GKErrorInvalidPlayer)
                 _listener->loadAchievementsEvent(SocialSessionListener::ERROR_INVALID_ARG, _achievements);
             else if (error.code == GKErrorCommunicationsFailure)
                 _listener->loadAchievementsEvent(SocialSessionListener::ERROR_SERVER, _achievements);
             else if (error.code == GKErrorCancelled)
                 _listener->loadAchievementsEvent(SocialSessionListener::ERROR_CANCELLED, _achievements);
             else
                 _listener->loadAchievementsEvent(SocialSessionListener::ERROR_UNKNOWN, _achievements);
         }
     }];
#endif
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
#if 0
    NSString *achievementName = [[NSString alloc] initWithUTF8String:achievementId];
    GKAchievement *achievement = [[GKAchievement alloc] initWithIdentifier: achievementName];
    if (achievement)
    {
        achievement.percentComplete = (achieved) ? 100 : value;
        [achievement reportAchievementWithCompletionHandler:^(NSError *error)
         {
             if (error == nil)
             {
                 if (achieved)
                 {
                     const SocialAchievement *award = getAchievement(achievementId);
                     if (award)
                         _listener->awardAchievedEvent(SocialSessionListener::SUCCESS, *award);
                 }
                 else
                 {
                    _listener->submitAchievementEvent(SocialSessionListener::SUCCESS);
                 }
             }
             else
             {
                 NSLog(@"Error in submitAchievement: %@", error);

                 if (error.code == GKErrorInvalidParameter || error.code == GKErrorInvalidPlayer)
                     _listener->submitAchievementEvent(SocialSessionListener::ERROR_INVALID_ARG);
                 else if (error.code == GKErrorCommunicationsFailure)
                     _listener->submitAchievementEvent(SocialSessionListener::ERROR_SERVER);
                 else if (error.code == GKErrorCancelled)
                     _listener->submitAchievementEvent(SocialSessionListener::ERROR_CANCELLED);
                 else
                     _listener->submitAchievementEvent(SocialSessionListener::ERROR_UNKNOWN);
             }
         }];
    }
#endif
}

void GooglePlaySocialSession::incrementAchievement(const char* achievementId, unsigned int totalSteps)
{
#if 0
    NSString *achievementName = [[NSString alloc] initWithUTF8String:achievementId];
    GKAchievement *achievement = [[GKAchievement alloc] initWithIdentifier: achievementName];
    if ([achievement isCompleted] == NO)
    {
        double newPercent = ((achievement.percentComplete/100.0f * totalSteps) + 1) / (float)totalSteps * 100;
        fprintf(stderr, "percentComplete is %lf\n", achievement.percentComplete);
        achievement.percentComplete = newPercent;
        [achievement reportAchievementWithCompletionHandler:^(NSError *error)
         {
             if (error == nil)
             {
                 if (achievement.percentComplete == 100)
                 {
                     const SocialAchievement *award = getAchievement(achievementId);
                     if (award)
                         _listener->awardAchievedEvent(SocialSessionListener::SUCCESS, *award);
                 }
                 else
                 {
                     _listener->submitAchievementEvent(SocialSessionListener::SUCCESS);
                 }

                 fprintf(stderr, "the achievements percent complete is now %lf\n", achievement.percentComplete);
             }
             else
             {
                 NSLog(@"Error in incrementAchievement: %@", error);

                 if (error.code == GKErrorInvalidParameter || error.code == GKErrorInvalidPlayer)
                     _listener->submitAchievementEvent(SocialSessionListener::ERROR_INVALID_ARG);
                 else if (error.code == GKErrorCommunicationsFailure)
                     _listener->submitAchievementEvent(SocialSessionListener::ERROR_SERVER);
                 else if (error.code == GKErrorCancelled)
                     _listener->submitAchievementEvent(SocialSessionListener::ERROR_CANCELLED);
                 else
                     _listener->submitAchievementEvent(SocialSessionListener::ERROR_UNKNOWN);
             }
         }];
    }
#endif
}

void GooglePlaySocialSession::synchronizeAchievements()
{
}

void GooglePlaySocialSession::loadScores(const char* leaderboardId, SocialSession::CommunityScope community, SocialSession::TimeScope time, unsigned int start, unsigned int count)
{
#if 0
    GKLeaderboard *leaderboardRequest = [[GKLeaderboard alloc] init];
    if (leaderboardRequest != nil)
    {
        switch (community)
        {
            case SocialSession::COMMUNITY_SCOPE_ALL:
                leaderboardRequest.playerScope = GKLeaderboardPlayerScopeGlobal;
                break;
            case SocialSession::COMMUNITY_SCOPE_FRIENDS:
                leaderboardRequest.playerScope = GKLeaderboardPlayerScopeFriendsOnly;
                break;
        }

        switch (time)
        {
            case SocialSession::TIME_SCOPE_ALL:
                leaderboardRequest.timeScope = GKLeaderboardTimeScopeAllTime;
                break;
            case SocialSession::TIME_SCOPE_TODAY:
                leaderboardRequest.timeScope = GKLeaderboardTimeScopeToday;
                break;
            case SocialSession::TIME_SCOPE_WEEK:
                leaderboardRequest.timeScope = GKLeaderboardTimeScopeWeek;
                break;
        }

        leaderboardRequest.identifier = [[NSString alloc] initWithUTF8String:leaderboardId];
        leaderboardRequest.range = NSMakeRange(start, start+count);

        [leaderboardRequest loadScoresWithCompletionHandler: ^(NSArray *scores, NSError *error)
         {
             if (error == nil)
             {
                 if (scores != nil)
                 {
                     // Process the score information.
                     _scores.clear();

                     for (GKScore* gk_score in scores)
                     {
                         SocialScore score;
                         score.handle = gk_score;
                         score.rank = gk_score.rank;
                         score.value = gk_score.value;
                         score.valueFormatted = [gk_score.formattedValue UTF8String];
                         score.playerName = [gk_score.playerID UTF8String];

                         _scores.push_back(score);

                         attachPlayerNameToScore([gk_score.playerID UTF8String], &_scores.back());
                     }
                 }
             }
             else
             {
                 NSLog(@"Error in loadScores: %@", error);

                 if (error.code == GKErrorInvalidParameter || error.code == GKErrorInvalidPlayer)
                     _listener->loadScoresEvent(SocialSessionListener::ERROR_INVALID_ARG, _scores);
                 else if (error.code == GKErrorCommunicationsFailure)
                     _listener->loadScoresEvent(SocialSessionListener::ERROR_SERVER, _scores);
                 else if (error.code == GKErrorCancelled)
                     _listener->loadScoresEvent(SocialSessionListener::ERROR_CANCELLED, _scores);
                 else
                     _listener->loadScoresEvent(SocialSessionListener::ERROR_UNKNOWN, _scores);

                 _listener->loadScoresEvent(SocialSessionListener::ERROR_UNKNOWN, _scores);
             }
         }];
    }
#endif
}

void GooglePlaySocialSession::loadScores(const char* leaderboardId, SocialSession::CommunityScope community, SocialSession::TimeScope time, const SocialPlayer& player, unsigned int count)
{
#if 0
    GKLeaderboard *leaderboardRequest = [[GKLeaderboard alloc] init];
    if (leaderboardRequest != nil)
    {
        switch (community)
        {
            case SocialSession::COMMUNITY_SCOPE_ALL:
                leaderboardRequest.playerScope = GKLeaderboardPlayerScopeGlobal;
                break;
            case SocialSession::COMMUNITY_SCOPE_FRIENDS:
                leaderboardRequest.playerScope = GKLeaderboardPlayerScopeFriendsOnly;
                break;
        }

        switch (time)
        {
            case SocialSession::TIME_SCOPE_ALL:
                leaderboardRequest.timeScope = GKLeaderboardTimeScopeAllTime;
                break;
            case SocialSession::TIME_SCOPE_TODAY:
                leaderboardRequest.timeScope = GKLeaderboardTimeScopeToday;
                break;
            case SocialSession::TIME_SCOPE_WEEK:
                leaderboardRequest.timeScope = GKLeaderboardTimeScopeWeek;
                break;
        }

        leaderboardRequest.identifier = [[NSString alloc] initWithUTF8String:leaderboardId];

        // first load all of the scores and find the player
        [leaderboardRequest loadScoresWithCompletionHandler: ^(NSArray *scores, NSError *error)
         {
             if (error == nil)
             {
                 if (scores != nil)
                 {
                     int rank;

                     for (GKScore* gk_score in scores)
                     {
                         if (gk_score.playerID == ((GKPlayer *)player.handle).playerID)
                         {
                             rank = gk_score.rank;
                             break;
                         }
                     }

                     int start = rank - count / 2;
                     if (start < 1)
                         start = 1;

                     // now launch another request and get the data back
                     loadScores(leaderboardId, community, time, start, start + count);
                 }
             }
             else
             {
                 NSLog(@"Error in loadScores: %@", error);

                 if (error.code == GKErrorInvalidParameter || error.code == GKErrorInvalidPlayer)
                     _listener->loadScoresEvent(SocialSessionListener::ERROR_INVALID_ARG, _scores);
                 else if (error.code == GKErrorCommunicationsFailure)
                     _listener->loadScoresEvent(SocialSessionListener::ERROR_SERVER, _scores);
                 else if (error.code == GKErrorCancelled)
                     _listener->loadScoresEvent(SocialSessionListener::ERROR_CANCELLED, _scores);
                 else
                     _listener->loadScoresEvent(SocialSessionListener::ERROR_UNKNOWN, _scores);

                 _listener->loadScoresEvent(SocialSessionListener::ERROR_UNKNOWN, _scores);
                 return;
             }
        }];
    }
#endif
}

void GooglePlaySocialSession::submitScore(const char* leaderboardId, float score)
{
#if 0
    NSString *leaderboardName = [[NSString alloc] initWithUTF8String:leaderboardId];
    GKScore *scoreReporter = [[GKScore alloc] initWithCategory:leaderboardName];
    scoreReporter.value = score;
    scoreReporter.context = 0;

    [scoreReporter reportScoreWithCompletionHandler:^(NSError *error)
    {
        // Do something interesting here.
        if (error == nil)
        {
            _session->getListener()->submitScoreEvent(SocialSessionListener::SUCCESS);
        }
        else
        {
            NSLog(@"Error in submitScore: %@", error);

            if (error.code == GKErrorScoreNotSet || error.code == GKErrorInvalidParameter)
                _listener->submitScoreEvent(SocialSessionListener::ERROR_INVALID_ARG);
            else if (error.code == GKErrorCommunicationsFailure)
                _listener->submitScoreEvent(SocialSessionListener::ERROR_SERVER);
            else
                _listener->submitScoreEvent(SocialSessionListener::ERROR_UNKNOWN);
        }
    }];
#endif
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

void GooglePlaySocialSession::displayPopup(const char *popupMessage) const
{

}

}

#endif
