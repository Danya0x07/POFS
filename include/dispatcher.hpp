#ifndef _DISPATCHER_HPP
#define _DISPATCHER_HPP

#include "program_memory.hpp"
#include "program_runner.hpp"

class CommandDispatcher
{
public:
    enum State {REALTIME, RECORDING, EXECUTING};
    enum Status {OK, ERROR};

    CommandDispatcher(ProgramMemory &memory, ProgramRunner &runner);
    Status dispatch(const Command &command);
    State getState();
    void notifyExecutionFinished();

private:
    ProgramMemory &memory_;
    ProgramRunner &runner_;
    State state_;

    Status dispatchRealTime(const Command &command);
    Status dispatchRecording(const Command &command);
    Status dispatchExecuting(const Command &command);
};

#endif // _DISPATCHER_HPP