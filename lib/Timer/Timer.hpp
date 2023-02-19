#ifndef _TIMER_HPP
#define _TIMER_HPP

#include <stdint.h>

class Timer
{
public:
    Timer() : mark_(0), interval_(0), running_(false) {}
    void setInterval(uint32_t interval);
    void start(uint32_t now);
    void stop();
    bool expired(uint32_t ms);
    bool isRunning();

private:
    uint32_t mark_;
    uint32_t interval_;
    bool running_;
};

#endif // _TIMER_HPP