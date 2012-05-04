#ifndef SCREENDISPLAYER_H_
#define SCREENDISPLAYER_H_

#include "Game.h"
#include "Platform.h"

namespace gameplay
{

/**
 * Used for displaying screens (i.e. splash or level loading screens).
 */
class ScreenDisplayer
{
public:

	/**
	 * Constructor.
	 */
	ScreenDisplayer();

    /**
     * Displays a screen using the {@link Game#renderOnce} mechanism for at least the given amount of time.
     * 
     * @param instance See {@link Game#renderOnce}.
     * @param method See {@link Game#renderOnce}.
     * @param cookie See {@link Game#renderOnce}.
     * @param time The minimum amount of time to display the screen (in milliseconds).
     */
    template <typename T> void run(T* instance, void (T::*method) (void*), void* cookie, long time);

    /**
     * Destructor.
     */
    ~ScreenDisplayer();

private:

    long _time;
    long _startTime;
};

inline ScreenDisplayer::ScreenDisplayer() : _time(0L), _startTime(0L)
{
}

template <typename T> void ScreenDisplayer::run(T* instance, void (T::*method) (void*), void* cookie, long time)
{
    _time = time;
    Game::getInstance()->renderOnce(instance, method, cookie);
    _startTime = Game::getInstance()->getGameTime();
}

inline ScreenDisplayer::~ScreenDisplayer()
{
    long elapsedTime = Game::getInstance()->getGameTime() - _startTime;
    if (elapsedTime < _time)
        Platform::sleep(_time - (Game::getInstance()->getGameTime() - _startTime));
}

/**
 * Displays a screen using the {@link Game#renderOnce} mechanism for at least the given amount
 * of time. This function is intended to be called at the beginning of a block of code that is be 
 * executed while the screen is displayed (i.e. Game#initialize). This function will block 
 * at the end of the block of code in which it is called for the amount of time that has not yet elapsed.
 * 
 * @param instance See {@link Game#renderOnce}.
 * @param method See {@link Game#renderOnce}.
 * @param cookie See {@link Game#renderOnce}.
 * @param time The minimum amount of time to display the screen (in milliseconds).
 */
#define displayScreen(instance, method, cookie, time) \
    ScreenDisplayer __##instance##ScreenDisplayer; \
    __##instance##ScreenDisplayer.run(instance, method, cookie, time)

}

#endif
