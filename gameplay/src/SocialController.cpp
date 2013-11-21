#include "Base.h"
#include "SocialController.h"
#include "Game.h"
#include "social/ScoreloopSocialSession.h"
#include "social/GooglePlaySocialSession.h"
#include "social/GameCenterSocialSession.h"

namespace gameplay
{

SocialController::SocialController()
    : _session(NULL)
{
}

SocialController::~SocialController()
{
}

void SocialController::initialize()
{
}

void SocialController::finalize()
{
	if (_session)
		_session->synchronizeAchievements();
}

void SocialController::pause()
{
	if (_session)
		_session->synchronizeAchievements();
}

void SocialController::resume()
{
}

void SocialController::update(float elapsedTime)
{
}

bool SocialController::handleEvent(void *event)
{
	if (_session)
		return _session->handleEvent(event);

	return false;
}

void SocialController::authenticate(SocialSessionListener* listener)
{
#ifdef GP_USE_SOCIAL
#if defined(__QNX__)
    Properties* socialProperties = Game::getInstance()->getConfig()->getNamespace("social", true);
    const char* providerStr = "";

    if (socialProperties)
    {
    	providerStr = socialProperties->getString("provider");
    }

    if (strcmp(providerStr, "Scoreloop") == 0)
    {
        _session = ScoreloopSocialSession::authenticate(listener, socialProperties);
    }
    else
    {
        listener->authenticateEvent(SocialSessionListener::ERROR_INITIALIZATION, NULL);
    }
#elif defined(__ANDROID__)
    Properties* socialProperties = Game::getInstance()->getConfig()->getNamespace("social", true);
    const char* providerStr = "";

    if (socialProperties)
    {
    	providerStr = socialProperties->getString("provider");
    }

    if (strcmp(providerStr, "GooglePlay") == 0)
    {
        _session = GooglePlaySocialSession::authenticate(listener, socialProperties);
    }
    else
    {
        listener->authenticateEvent(SocialSessionListener::ERROR_INITIALIZATION, NULL);
    }
#elif defined(__APPLE__)
    Properties* socialProperties = Game::getInstance()->getConfig()->getNamespace("social", true);
    const char* providerStr = "";

    if (socialProperties)
    {
    	providerStr = socialProperties->getString("provider");
    }

    if (strcmp(providerStr, "GameCenter") == 0)
    {
        _session = GameCenterSocialSession::authenticate(listener, socialProperties);
    }
    else
    {
        listener->authenticateEvent(SocialSessionListener::ERROR_INITIALIZATION, NULL);
    }
#endif
#endif
}

}
