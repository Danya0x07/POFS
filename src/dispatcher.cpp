#include "dispatcher.hpp"

CommandDispatcher::CommandDispatcher(ProgramMemory &memory, ProgramRunner &runner)
: memory_(memory), runner_(runner), state_(REALTIME)
{
}

CommandDispatcher::Status CommandDispatcher::dispatch(const Command &command)
{
    switch (state_)
    {
    case REALTIME: return dispatchRealTime(command);
    case RECORDING: return dispatchRecording(command);
    case EXECUTING: return dispatchExecuting(command);
    }
    return ERROR;
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

CommandDispatcher::Status CommandDispatcher::dispatchRealTime(const Command &command)
{
    Status status = OK;
    ProgramRunner::Status runStatus;

    switch (command.type)
    {
    case CommandType::SET_FLAP:
    case CommandType::SET_FILTER:
    case CommandType::WAIT:
    case CommandType::CALIBRATE:
    case CommandType::SAVE_CALIBRATION:
        runner_.setExtraordinaryCommand(command);
        state_ = EXECUTING;
        break;
    
    case CommandType::LOADING_MODE:
        memory_.reset();
        state_ = RECORDING;
        break;
    
    case CommandType::EXECUTE_PROGRAM:
        runStatus = runner_.setupCommandSequence(memory_.getLenUsed());
        
        if (runStatus == ProgramRunner::OK) {
            state_ = EXECUTING;
        } else {
            status = ERROR;
        }
        break;
    
    case CommandType::RESET:
        memory_.reset();
        runner_.reset();
        runner_.setUrgentCommand(command);
        break;
    
    case CommandType::SAVE_PROGRAM:
    case CommandType::ERROR:
        status = ERROR;
        break;
    }
    return status;
}

CommandDispatcher::Status CommandDispatcher::dispatchRecording(const Command &command)
{
    Status status = OK;
    ProgramMemory::Status memStatus;

    switch (command.type)
    {
    case CommandType::SET_FLAP:
    case CommandType::SET_FILTER:
    case CommandType::WAIT:
    case CommandType::SAVE_PROGRAM:
        memStatus = memory_.store(command);
        if (memStatus == ProgramMemory::ERROR) {
            memory_.reset();
            state_ = REALTIME;
            status = ERROR;
        }
        if (memStatus == ProgramMemory::COMPLETE) {
            state_ = REALTIME;
        }
        break;
    
    case CommandType::LOADING_MODE:
    case CommandType::EXECUTE_PROGRAM:
    case CommandType::CALIBRATE:
    case CommandType::SAVE_CALIBRATION:
    case CommandType::ERROR:
        memory_.reset();
        state_ = REALTIME;
        status = ERROR;
        break;
    
    case CommandType::RESET:
        memory_.reset();
        state_ = REALTIME;
        runner_.setUrgentCommand(command);
        break;
    }
    return status;
}

CommandDispatcher::Status CommandDispatcher::dispatchExecuting(const Command &command)
{
    Status status = OK;
    
    switch (command.type)
    {
    case CommandType::SET_FLAP:
    case CommandType::SET_FILTER:
    case CommandType::WAIT:
    case CommandType::LOADING_MODE:
    case CommandType::SAVE_PROGRAM:
    case CommandType::EXECUTE_PROGRAM:
    case CommandType::CALIBRATE:
    case CommandType::SAVE_CALIBRATION:
    case CommandType::ERROR:
        status = ERROR;
        break;
    case CommandType::RESET:
        runner_.reset();
        runner_.setUrgentCommand(command);
        state_ = REALTIME;
        break;
    }
    return status;
}
