#include "dispatcher.hpp"

CommandDispatcher::CommandDispatcher(ProgramMemory &memory, ProgramRunner &runner)
: memory_(memory), runner_(runner), state_(REALTIME)
{
}

CommandDispatcher::Status CommandDispatcher::dispatch(const Command &command)
{
    switch (state_)
    {
    case REALTIME:
        if (command.type == CommandType::SAVE_PROGRAM || command.type == CommandType::ERROR)
            return ERROR;
        if (command.type == CommandType::LOADING_MODE) {
            memory_.reset();
            state_ = RECORDING;
        } else if (command.type == CommandType::EXECUTE_PROGRAM) {
            ProgramRunner::Status status = runner_.setupCommandSequence(memory_.getLenUsed());
            if (status == ProgramRunner::OK) {
                state_ = EXECUTING;
            } else {
                return ERROR;
            }
        } else if (command.type == CommandType::RESET) {
            memory_.reset();
            runner_.reset();
        } else {
            runner_.setExtraordinaryCommand(command);
            state_ = EXECUTING;
        }
        break;
    
    case RECORDING:
        if (command.type == CommandType::LOADING_MODE || command.type == CommandType::EXECUTE_PROGRAM) {
            memory_.reset();
            state_ = REALTIME;
            return ERROR;
        }
        if (command.type == CommandType::RESET) {
            memory_.reset();
            state_ = REALTIME;
        } else {
            ProgramMemory::Status status = memory_.store(command);
            if (status == ProgramMemory::ERROR) {
                memory_.reset();
                state_ = REALTIME;
                return ERROR;
            }
            if (status == ProgramMemory::COMPLETE) {
                state_ = REALTIME;
            }
        }
        break;
    
    case EXECUTING:
        runner_.reset();
        runner_.setUrgentCommand(command);
        state_ = REALTIME;
        if (command.type != CommandType::RESET)
            return ERROR;
        break;
    }
    
    return OK;
}

CommandDispatcher::State CommandDispatcher::getState()
{
    return state_;
}

void CommandDispatcher::notifyExecutionFinished()
{
    if (state_ == EXECUTING) {
        state_ = REALTIME;
    }
}
