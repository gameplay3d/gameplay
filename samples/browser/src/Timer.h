#ifndef TIMER_H_
#define TIMER_H_

#include <Game.h>

using namespace gameplay;

class Time
{
public:
	explicit Time(double time = 0.0) : _time(time){}

	float asSeconds() const { return static_cast<float>(_time / 1000.0); }
	double asMilliseconds() const { return _time; }

private:
	double _time;
};

class Timer
{
public:

	Timer() : _startTime(Game::getGameTime()){}
	Time restart()
	{
		Time time = elapsed();
		_startTime = Game::getGameTime();
		return time;
	}

	Time elapsed() const { return Time(Game::getGameTime() - _startTime); }

private:
	double _startTime;
};

#endif