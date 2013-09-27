#include "Base.h"
#include "SocialController.h"
#include "Game.h"
#include "social/ScoreloopSocialSession.h"

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
}

void SocialController::update(float elapsedTime)
{
}

void SocialController::authenticate(SocialSessionListener* listener)
{
#if defined(__QNX__) && defined(GP_USE_SOCIAL)
    Properties* socialProperties = Game::getInstance()->getConfig()->getNamespace("social");
    const char* providerStr = socialProperties->getString("provider");
    if(strcmp(providerStr, "Scoreloop") == 0)
    {
        ScoreloopSocialSession::authenticate(listener, socialProperties);
    }
    else
    {
        listener->authenticateEvent(SocialSessionListener::ERROR_INITIALIZATION, NULL);
    }
#endif
}

}
