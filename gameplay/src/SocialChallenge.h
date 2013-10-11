#ifndef SOCIALCHALLENGE_H_
#define SOCIALCHALLENGE_H_

namespace gameplay
{

/**
 * Defines a social challenge between social players.
 *
 * @script{ignore}
 */
class SocialChallenge
{

public:

	/**
	 * The challenge state.
	 */
    enum State
    {
        PENDING,
        COMPLETE,
        DECLINED,
        INVALID
    };

    /**
     * The name of the challenge.
     */
    std::string name;

    /**
     * The title of the challenge.
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

    /**
     * The state of the challenge.
     */
    State state;

    /**
     * The score for the challenge.
     */
    float score;

    /**
     * The date/time the challenge was issued. (UTC string format "2013-06-27T07:40::05")
     */
    std::string dateTimeIssued;

    /**
     * The date/time the challenge was completed. (UTC string format "2013-06-27T07:40::05")
     */
    std::string dateTimeCompleted;

    /**
     * The provider handle for an challenge.
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
