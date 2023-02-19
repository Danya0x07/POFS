#include "Timer.hpp"

void Timer::setInterval(uint32_t interval)
{
    interval_ = interval;
}

void Timer::start(uint32_t now)
{
    mark_ = now;
    running_ = true;
}

void Timer::stop()
{
    running_ = false;
}

bool Timer::expired(uint32_t ms)
{
    return running_ && ms - mark_ >= interval_;
}

bool Timer::isRunning()
{
    return running_;
}
