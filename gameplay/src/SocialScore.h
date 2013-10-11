#ifndef SOCIALSCORE_H_
#define SOCIALSCORE_H_

namespace gameplay
{

/**
 * Defines an abstract class for a score that resides in a leaderboard.
 *
 * @script{ignore}
 */
class SocialScore
{
public:

    /**
     * The value for the score.
     */
    float value;

    /**
     * The formatted value for the score.
     */
    std::string valueFormatted;

    /**
     * The rank index of the score within the leaderboard.
     */
    unsigned int rank;

    /**
     * The handle of the player that achieved the score
     */
    std::string playerName;

    /**
     * The score
     */
    SocialScoreHandle handle;

    /**
     * Constructor
     */
    SocialScore();

    /**
     * Destructor
     */
    ~SocialScore();
};

}

#endif
