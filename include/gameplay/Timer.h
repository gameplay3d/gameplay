#pragma once

#include "Defines.h"
#include <chrono>

namespace gameplay
{
/**
 * Defines a timer with configurable resolution.
 */
class GP_API Timer
{
public:

    enum class Resolution
    {
        SECONDS,
        MILLISECONDS,
        MICROSECONDS,
        NANOSECONDS
    };

    /**
     * Constructor.
     */
    Timer() {}

    /**
     * Destructor.
     */
    ~Timer() {}

    /**
     * Gets the precision of the timer (minimal tick duration).
     *
     * @return The precision of the timer (minimal tick duration).
     */
    double get_precision();

    /**
     * Starts the timer.
     */
    void start();

    /**
     * Stops the timer.
     */
    void stop();

    /**
     * Gets elapsed time in a specified form, using specified time resolution.
     *
     * If timer wasn't stopped before, this return the elapsed time between
     * timer start and this function call, timer will continue to tick.
     * 
     * @param resolution Time resolution that you want to get result in.
     * @return The elapsed time between timer start, and timer stop events. 
     */
    template <typename T = int64_t>
    inline T get_elapsed_time(Resolution resolution = Resolution::MILLISECONDS);

private:
    bool _running{false};
    std::chrono::high_resolution_clock::time_point _start;
    std::chrono::high_resolution_clock::time_point _stop;
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// impl

double Timer::get_precision()
{
    return std::chrono::high_resolution_clock::period::num / (double)std::chrono::high_resolution_clock::period::den;
}

void Timer::start()
{
    _start= std::chrono::high_resolution_clock::now();
    _running = true;
}

void Timer::stop()
{
    _stop = std::chrono::high_resolution_clock::now();
    _running = false;
}

template <typename T>
T Timer::get_elapsed_time(Resolution resolution)
{
    std::chrono::high_resolution_clock::time_point stop;
    if (_running)
    {
        stop = std::chrono::high_resolution_clock::now();
    }
    else
    {
        stop = _stop;
    }
    auto elapsedTime = stop - _start;
    using dblSeconds = std::chrono::duration<T, std::ratio<1>>;
    using dblMilliseconds = std::chrono::duration<T, std::milli>;
    using dblMicroseconds = std::chrono::duration<T, std::micro>;
    using dblNanoseconds = std::chrono::duration<T, std::nano>;

    switch (resolution)
    {
    case Resolution::SECONDS:
        return std::chrono::duration_cast<dblSeconds>(elapsedTime).count();
    case Resolution::MILLISECONDS:
        return std::chrono::duration_cast<dblMilliseconds>(elapsedTime).count();
    case Resolution::MICROSECONDS:
        return std::chrono::duration_cast<dblMicroseconds>(elapsedTime).count();
    case Resolution::NANOSECONDS:
        return std::chrono::duration_cast<dblNanoseconds>(elapsedTime).count();
    default:
        return std::chrono::duration_cast<dblMilliseconds>(elapsedTime).count();
    }
}
}
