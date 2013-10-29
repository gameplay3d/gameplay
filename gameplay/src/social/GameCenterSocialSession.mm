#if defined(__APPLE__) && defined(GP_USE_SOCIAL)

#include "Base.h"
#import "GameCenterSocialSession.h"
#import <Foundation/Foundation.h>
#import <GameKit/GameKit.h>

@interface GameCenterController : NSObject
+ (BOOL) isGameCenterAvailable;
+ (void) resetAchievements;
@end

@implementation GameCenterController

+ (BOOL) isGameCenterAvailable
{
	// check for presence of GKLocalPlayer API
	Class gcClass = (NSClassFromString(@"GKLocalPlayer"));
	
	// check if the device is running iOS 4.1 or later
	NSString *reqSysVer = @"4.1";
	NSString *currSysVer = [[UIDevice currentDevice] systemVersion];
	BOOL osVersionSupported = ([currSysVer compare:reqSysVer options:NSNumericSearch] != NSOrderedAscending);
	
	return (gcClass && osVersionSupported);
}

+ (void) resetAchievements
{
    // Clear all progress saved on Game Center
    [GKAchievement resetAchievementsWithCompletionHandler:^(NSError *error)
     {
         if (error != nil)
             NSLog(@"Error resetting achievmenets %@", error);
         
         NSLog(@"Reset all achievements to default");
     }];
}

@end


namespace gameplay {

GameCenterSocialSession* GameCenterSocialSession::_session = NULL;


GameCenterSocialSession::GameCenterSocialSession()
: SocialSession(),
_listener(NULL), _properties(NULL)
{
    [GameCenterController resetAchievements];
}

GameCenterSocialSession::~GameCenterSocialSession()
{
}

SocialSessionListener* GameCenterSocialSession::getListener()
{
    return _listener;
}

SocialSession *GameCenterSocialSession::authenticate(SocialSessionListener* listener, Properties* properties)
{
    if (!_session)
    {
		_session = new GameCenterSocialSession();
		_session->_listener = listener;
		_session->_properties = properties;
        
		const char* gameId = properties->getString("id");
		const char* gameSecret = properties->getString("secret");
		const char* gameVersion = properties->getString("version");
		const char* gameCurrency = properties->getString("currency");
		const char* gameLanguage = properties->getString("language");
        
        if ([GameCenterController isGameCenterAvailable])
        {
            GKLocalPlayer *player = [GKLocalPlayer localPlayer];
            [player authenticateWithCompletionHandler:^(NSError *error)
            {
                if (error == nil)
                {
                    BOOL authenticated = player.isAuthenticated;

                    if (player.isAuthenticated)
                    {
                        _session->_user.handle = player;
                        _session->_user.name = [player.displayName UTF8String];
                        
                        listener->authenticateEvent(SocialSessionListener::SUCCESS, _session);
                    }
                    else
                    {
                        listener->authenticateEvent(SocialSessionListener::ERROR_INITIALIZATION, NULL);
                    }
                }
                else
                {
                    NSLog(@"Error in authenticate: %@", error);
                    
                    if (error.code == GKErrorInvalidCredentials)
                        listener->authenticateEvent(SocialSessionListener::ERROR_INVALID_ARG, NULL);
                    if (error.code == GKErrorGameUnrecognized)
                        listener->authenticateEvent(SocialSessionListener::ERROR_INITIALIZATION, NULL);
                    else if (error.code == GKErrorCommunicationsFailure)
                        listener->authenticateEvent(SocialSessionListener::ERROR_SERVER, NULL);
                    else if (error.code == GKErrorAuthenticationInProgress)
                        listener->authenticateEvent(SocialSessionListener::ERROR_PENDING_RESPONSE, NULL);
                    else if (error.code == GKErrorCancelled)
                        listener->authenticateEvent(SocialSessionListener::ERROR_CANCELLED, NULL);
                    else
                        listener->authenticateEvent(SocialSessionListener::ERROR_UNKNOWN, NULL);
                }
            }];
        }
        else
        {
            // The current device does not support Game Center.
            NSLog(@"Current device does not support Game Center");
            listener->authenticateEvent(SocialSessionListener::ERROR_INITIALIZATION, NULL);
        }
    }
    
    return _session;
}

const SocialPlayer& GameCenterSocialSession::getUser() const
{
	return _user;
}

/**
 * @see SocialSession::loadFriends
 */
void GameCenterSocialSession::loadFriends()
{
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
}

void GameCenterSocialSession::loadAchievementData()
{
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
}
    
/**
 * @see SocialSession::loadAchievements
 */
void GameCenterSocialSession::loadAchievements()
{
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
}
    
const SocialAchievement* GameCenterSocialSession::getAchievement(const char* achievementId) const
{
    uint size = _achievements.size();
    
    for (uint i = 0; i < size; i++)
    {
        if (strcmp(_achievements[i].name.data(), achievementId) == 0)
            return &_achievements[i];
    }
    return 0;
}

/**
 * @see SocialSession::submitAchievement
 */
void GameCenterSocialSession::submitAchievement(const char* achievementId, unsigned int value, bool achieved)
{
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
}

/**
 * @see SocialSession::incrementAchievement
 */
void GameCenterSocialSession::incrementAchievement(const char* achievementId, unsigned int totalSteps)
{
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
}

/**
 * @see SocialSession::syncAchievements
 */
void GameCenterSocialSession::synchronizeAchievements()
{
    // not supported on IOS
}
    
void GameCenterSocialSession::attachPlayerNameToScore(const char *playerID, SocialScore *score)
{
    NSString* playerString = [NSString stringWithFormat:@"%s" , playerID];
    
    // have to look up the display name of the player given the playerID
    [GKPlayer loadPlayersForIdentifiers: [NSArray arrayWithObject: playerString] withCompletionHandler:^(NSArray *players, NSError *error)
     {
         if (error != nil)
         {
             // Handle the error.
         }
         if (players != nil)
         {
             for (GKPlayer* player in players)
             {
                 if ([player.playerID isEqualToString: playerString])
                 {
                     score->playerName = [player.displayName UTF8String];
                     break;
                 }
             }
         }
         
         _listener->loadScoresEvent(SocialSessionListener::SUCCESS, _scores);
     }];
}

/**
 * @see SocialSession::loadScores
 */
void GameCenterSocialSession::loadScores(const char* leaderboardId, SocialSession::CommunityScope community, SocialSession::TimeScope time, unsigned int start, unsigned int count)
{
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
}


/**
 * @see SocialSession::loadScores
 */
void GameCenterSocialSession::loadScores(const char* leaderboardId, SocialSession::CommunityScope community, SocialSession::TimeScope time, const SocialPlayer& player, unsigned int count)
{
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
}
    
/**
 * @see SocialSession::submitScore
 */
void GameCenterSocialSession::submitScore(const char* leaderboardId, float score)
{
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
}

/**
 * @see SocialSession::submitChallenge
 */
void GameCenterSocialSession::submitChallenge(const SocialPlayer *player, float score, const char* leaderboardId, unsigned int wager)
{
    SocialChallenge challenge;

    if (player)
    {
        GKScore *gkScore = [[GKScore alloc] init];
        gkScore.value = score;
        gkScore.context = 0;
        
        NSArray *challengedPlayer = [NSArray arrayWithObject: ((GKPlayer *)player->handle).playerID];
        
        if (challengedPlayer != nil)
        {
            [gkScore issueChallengeToPlayers:challengedPlayer message: @"You have been challenged"];
            
            
            _listener->submitChallengeEvent(SocialSessionListener::SUCCESS, challenge);
        }
        else
        {
            _listener->submitChallengeEvent(SocialSessionListener::ERROR_INITIALIZATION, challenge);
        }
    }
    else
    {
        _listener->submitChallengeEvent(SocialSessionListener::ERROR_INVALID_ARG, challenge);
    }
}
    
/**
 * @see SocialSession::submitChallenge
 */
void GameCenterSocialSession::submitAchievementChallenge(const SocialPlayer *player, const char* achievementId, unsigned int wager)
{
    SocialChallenge challenge;
    
    NSString *achievementIdName = [[NSString alloc] initWithUTF8String:achievementId];
    GKAchievement *gkAchievement = [[GKAchievement alloc] initWithIdentifier:achievementIdName];
    NSArray *challengedPlayer = [NSArray arrayWithObject: ((GKPlayer *)player->handle).playerID];
    
    if (challengedPlayer != nil)
    {
        [gkAchievement selectChallengeablePlayerIDs: challengedPlayer withCompletionHandler:^(NSArray *challengeablePlayerIDs, NSError *error)
         {
             [gkAchievement issueChallengeToPlayers:challengeablePlayerIDs message: @"You have been challenged"];
             
             
             _listener->submitChallengeEvent(SocialSessionListener::SUCCESS, challenge);
         }];
    }
    else
    {
        _listener->submitChallengeEvent(SocialSessionListener::ERROR_INVALID_ARG, challenge);
    }
}

/**
 * @see SocialSession::loadChallenges
 */
void GameCenterSocialSession::loadChallenges(bool showOpenChallengesOnly)
{
    if (showOpenChallengesOnly == false)
        NSLog(@"open challenges are only supported on IOS");
    
    [GKChallenge loadReceivedChallengesWithCompletionHandler:^(NSArray *challenges, NSError *error)
    {
        if (error == nil)
        {
            _challenges.clear();
            
            NSDateFormatter *formatter = [[NSDateFormatter alloc] init];
            [formatter setDateFormat:@"ddmmyyyy"];
            // Optionally for time zone converstions
            [formatter setTimeZone:[NSTimeZone timeZoneWithName:@"..."]];
            
            for (GKChallenge* gk_challenge in challenges)
            {
                NSString *dateIssuedString = [formatter stringFromDate:gk_challenge.issueDate];
                NSString *dateCompletedString = [formatter stringFromDate:gk_challenge.completionDate];
                
                SocialChallenge challenge;
                
                challenge.handle = gk_challenge;
                
                challenge.challengedPlayerName = [gk_challenge.receivingPlayerID UTF8String];
                challenge.issuedPlayerName = [gk_challenge.issuingPlayerID UTF8String];
                if ([gk_challenge isKindOfClass:[GKScoreChallenge class]])
                {
                    challenge.score = [[(GKScoreChallenge *)gk_challenge score] value];
                }
                
                challenge.dateTimeIssued = [dateIssuedString UTF8String];
                challenge.dateTimeCompleted = [dateCompletedString UTF8String];
                
                if (gk_challenge.state == GKChallengeStateCompleted)
                    challenge.state = SocialChallenge::COMPLETE;
                else if (gk_challenge.state == GKChallengeStateDeclined)
                    challenge.state = SocialChallenge::DECLINED;
                else if (gk_challenge.state == GKChallengeStatePending)
                    challenge.state = SocialChallenge::PENDING;
                else
                    challenge.state = SocialChallenge::INVALID;
                
                _challenges.push_back(challenge);
            }
            
            _listener->loadChallengesEvent(SocialSessionListener::SUCCESS, _challenges);
            
            [formatter release];
        }
        else
        {
            NSLog(@"Error in loadScores: %@", error);
            
            if (error.code == GKErrorInvalidParameter)
                _listener->loadChallengesEvent(SocialSessionListener::ERROR_INVALID_ARG, _challenges);
            else if (error.code == GKErrorChallengeInvalid)
                _listener->loadChallengesEvent(SocialSessionListener::ERROR_INITIALIZATION, _challenges);
            else if (error.code == GKErrorCommunicationsFailure)
                _listener->loadChallengesEvent(SocialSessionListener::ERROR_SERVER, _challenges);
            else if (error.code == GKErrorCancelled)
                _listener->loadChallengesEvent(SocialSessionListener::ERROR_CANCELLED, _challenges);
            else
                _listener->loadChallengesEvent(SocialSessionListener::ERROR_UNKNOWN, _challenges);
            
            _listener->loadChallengesEvent(SocialSessionListener::ERROR_UNKNOWN, _challenges);
        }
    
        // if we wanted to show the list of challenges we could call something like this
        //[_viewController presentChallengeList: challenges];
    }];
}

/**
 * @see SocialSession::acceptChallenge
 */
void GameCenterSocialSession::replyToChallenge(const SocialChallenge *challenge, bool accept)
{
    // no equivalent in IOS
}

/**
 * @see SocialSession::requestSavedData
 */
void GameCenterSocialSession::loadSavedData(const char* key)
{
    // TODO: implement with IOS cloud storage
}

/**
 * @see SocialSession::submitSavedData
 */
void GameCenterSocialSession::submitSavedData(const char* key, std::string data)
{
    // TODO: implement with IOS cloud storage
}
  
void GameCenterSocialSession::showGameCenter(int startingScreen) const
{
    GKGameCenterViewController *gameCenterController = [[GKGameCenterViewController alloc] init];
    if (gameCenterController != nil)
    {
        gameCenterController.viewState = startingScreen;
        
#ifdef TARGET_OS_IPHONE        
        id viewController = [UIApplication sharedApplication].keyWindow.rootViewController;
        gameCenterController.gameCenterDelegate = viewController;
        [viewController presentViewController: gameCenterController animated: YES completion:nil];
#else
        GKDialogController *sdc = [GKDialogController sharedDialogController];
        sdc.parentWindow = [[NSApplication sharedApplication] mainWindow];
        [sdc presentViewController: gameCenterController];
#endif

    }
}

void GameCenterSocialSession::displayLeaderboard(const char* leaderboardId) const
{
    showGameCenter(GKGameCenterViewControllerStateLeaderboards);
}

void GameCenterSocialSession::displayAchievements() const
{
    showGameCenter(GKGameCenterViewControllerStateAchievements);
}

void GameCenterSocialSession::displayChallenges() const
{
    showGameCenter(GKGameCenterViewControllerStateChallenges);
}

void GameCenterSocialSession::displayChallengeSubmit(const SocialChallenge *challenge, float score) const
{
    return _listener->uiEvent(SocialSessionListener::ERROR_NOT_SUPPORTED);
}

void GameCenterSocialSession::displayPopup(const char *popupMessage) const
{
    NSLog(@"display the popup");
    
    NSString* title = @"SAMPLE TITLE";
    NSString* message = [[NSString alloc] initWithUTF8String:popupMessage];
    
    [GKNotificationBanner showBannerWithTitle: title message: message completionHandler:^{}];
}

}

#endif
