#ifndef SOCIALACHIEVEMENT_H_
#define SOCIALACHIEVEMENT_H_

namespace gameplay
{

/**
 * Defines a class for typical social game activities running with an autenicated session.
 */
class SocialAchievement
{

public:

    /**
     * The name/title of the achievement.
     */
    std::string name;

    /**
     * The value of progress (0 to getTotal) representing the current unit value of completion.
     */
    unsigned int value;

    /**
     * The total value of progress needed to complete the achievement.
     */
    unsigned int total;

    /**
     * The percent that the achievement is complete. Equivalent to getValue() / getTotal() * 100.0f
     */
    float percentCompleted;

    /**
     * The date/time the achievement was achieved. (UTC string format "2013-06-27T07:40::05")
     */
    std::string dateTimeAchieved;

    /**
     * The provider handle for an achievement.
     */
    SocialAchievementHandle handle;

    /**
     * Constructor.
     */
    SocialAchievement();

    /**
     * Destructor.
     */
    ~SocialAchievement();
};

}

#endif
