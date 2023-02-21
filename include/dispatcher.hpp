#ifndef _DISPATCHER_HPP
#define _DISPATCHER_HPP

#include "program_memory.hpp"
#include "program_runner.hpp"

class CommandDispatcher
{
public:
    enum State {REALTIME, RECORDING, EXECUTING};
    enum Status {OK, ERROR};

    CommandDispatcher(const ProgramMemory &memory, const ProgramRunner &runner);
    Status dispatch(const Command &command);
    State getState();
    void notifyExecutionFinished();
};

#endif // _DISPATCHER_HPP