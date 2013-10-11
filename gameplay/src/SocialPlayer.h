#ifndef SOCIALPLAYER_H_
#define SOCIALPLAYER_H_

namespace gameplay
{

/**
 * Defines a social player.
 *
 * This can either be the authenticated user, friend or community user (not friend yet...).
 *
 * @script{ignore}
 */
class SocialPlayer
{

public:

    /**
     * The players friendly name.
     */
    std::string name;

    /**
     * The provider handle for a player.
     */
    SocialPlayerHandle handle;

    /**
     * Constructor
     */
    SocialPlayer();

    /**
     * Destructor
     */
    ~SocialPlayer();
};

}

#endif
