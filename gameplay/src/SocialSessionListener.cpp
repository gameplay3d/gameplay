#include "Base.h"
#include "SocialSessionListener.h"

namespace gameplay
{
SocialSessionListener::SocialSessionListener()
{
}

SocialSessionListener::SocialSessionListener(const SocialSessionListener& copy)
{
}

SocialSessionListener::~SocialSessionListener()
{
}

void SocialSessionListener::authenticateEvent(ResponseCode code, SocialSession* session)
{
}

void SocialSessionListener::loadFriendsEvent(ResponseCode code, std::vector<SocialPlayer> friends)
{
}

void SocialSessionListener::loadAchievementsEvent(ResponseCode code, std::vector<SocialAchievement> achievements)
{
}

void SocialSessionListener::synchronizeAchievementEvent(ResponseCode code)
{
}

void SocialSessionListener::submitAchievementEvent(ResponseCode code)
{
}

void SocialSessionListener::awardAchievedEvent(ResponseCode code, const SocialAchievement &achievement)
{
}

void SocialSessionListener::loadScoresEvent(ResponseCode code, std::vector<SocialScore> scores)
{
}

void SocialSessionListener::submitScoreEvent(ResponseCode code)
{
}

void SocialSessionListener::submitChallengeEvent(ResponseCode code, const SocialChallenge &challenge)
{
}

void SocialSessionListener::startChallengeEvent(ResponseCode code, const SocialChallenge &challenge)
{
}

void SocialSessionListener::replyToChallengeEvent(ResponseCode code)
{
}

void SocialSessionListener::loadChallengesEvent(ResponseCode code, std::vector<SocialChallenge> challenges)
{
}

void SocialSessionListener::loadSavedDataEvent(ResponseCode code, std::string data)
{
}

void SocialSessionListener::submitSavedDataEvent(ResponseCode code)
{
}

void SocialSessionListener::uiEvent(ResponseCode code, std::string errorMessage)
{
}

void SocialSessionListener::displayedLeaderboardEvent(ResponseCode code, std::string errorMessage)
{
}

void SocialSessionListener::displayedAchievementsEvent(ResponseCode code, std::string errorMessage)
{
}

void SocialSessionListener::displayedChallengesEvent(ResponseCode code, std::string errorMessage)
{
}

}


