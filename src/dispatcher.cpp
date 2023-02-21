#include "dispatcher.hpp"

CommandDispatcher::CommandDispatcher(const ProgramMemory &memory, const ProgramRunner &runner)
{
}

CommandDispatcher::Status CommandDispatcher::dispatch(const Command &command)
{
    return Status();
}

CommandDispatcher::State CommandDispatcher::getState()
{
    return State();
}

void CommandDispatcher::notifyExecutionFinished()
{
}
