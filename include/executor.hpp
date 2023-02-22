#ifndef _EXECUTOR_HPP
#define _EXECUTOR_HPP

#include "commands.hpp"
#include <Timer.hpp>

class Executor
{
public:
    enum Event {
        NOTHING,
        FINISHED
    };
    Executor(Timer &timer);
    void startExecuting(const Command &cmd, uint32_t now);
    bool isExecuting();
    void stopExecuting();
    Event run(uint32_t ms);

private:
    Timer &timer_;
    Command lastCommand_;
    bool executing_;
};

#endif // _EXECUTOR_HPP