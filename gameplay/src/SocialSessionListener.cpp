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

void SocialSessionListener::submitAchievementEvent(ResponseCode code)
{
}

void SocialSessionListener::loadScoresEvent(ResponseCode code, std::vector<SocialScore> scores)
{
}

void SocialSessionListener::submitScoreEvent(ResponseCode code)
{
}

void SocialSessionListener::loadSavedDataEvent(ResponseCode code, std::string data)
{
}

void SocialSessionListener::submitSavedDataEvent(ResponseCode code)
{
}

}


