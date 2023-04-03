#ifndef _EXECUTOR_HPP
#define _EXECUTOR_HPP

#include <Timer.hpp>
#include "commands.hpp"
#include "responder.hpp"

class Executor
{
public:
    enum Event {
        NOTHING,
        DATA,
        FINISHED
    };
    Executor(Timer &timer);
    void startExecuting(const Command &cmd, uint32_t now);
    bool isExecuting();
    void stopExecuting();
    Event run(uint32_t ms, Response &response);
    const Command &getLastCommand();

private:
    Timer &timer_;
    Command lastCommand_;
    bool executing_;
};

#endif // _EXECUTOR_HPP