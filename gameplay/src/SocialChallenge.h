#ifndef SOCIALCHALLENGE_H_
#define SOCIALCHALLENGE_H_

namespace gameplay
{

/**
 * Defines a social challenge between social players.
 */
class SocialChallenge
{

public:

    enum State
    {
        PENDING,
        COMPLETE,
        DECLINED,
        INVALID
    };

    /**
     * The name of the achievement.
     */
    std::string name;

    /**
     * The title of the achievement.
     */
    std::string title;

    /**
     * The name of the player issuing the challenge.
     */
    std::string issuedPlayerName;

    /**
     * The name of the player being challenged.
     */
    std::string challengedPlayerName;

    State state;

    float score;

    /**
     * The date/time the challenge was issued. (UTC string format "2013-06-27T07:40::05")
     */
    std::string dateTimeIssued;

    /**
     * The date/time the achievement was achieved. (UTC string format "2013-06-27T07:40::05")
     */
    std::string dateTimeAchieved;

    /**
     * The provider handle for an achievement.
     */
    SocialChallengeHandle handle;

    /**
     * Constructor.
     */
    SocialChallenge();

    /**
     * Destructor.
     */
    ~SocialChallenge();
};

}

#endif