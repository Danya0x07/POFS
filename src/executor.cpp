#include "executor.hpp"
#include "motors.hpp"

Executor::Executor(Timer &timer) 
: timer_(timer), lastCommand_{.type = CommandType::ERROR}, executing_(false)
{

}

void Executor::startExecuting(const Command &cmd, uint32_t now)
{
    executing_ = true;
    if (cmd.type == CommandType::WAIT) {
        timer_.stop();
        timer_.setInterval(cmd.waitTime);
        timer_.start(now);
    }
    lastCommand_ = cmd;
}

bool Executor::isExecuting()
{
    return executing_;
}

void Executor::stopExecuting()
{
    if (timer_.isRunning())
        timer_.stop();
    executing_ = false;
}

Executor::Event Executor::run(uint32_t ms)
{
    if (!executing_)
        return NOTHING;
    
    switch (lastCommand_.type)
    {
    case CommandType::SET_FLAP:
        flapSet(lastCommand_.flapStatus);
        executing_ = false;
        break;
    
    case CommandType::SET_FILTER:
        filterSet(lastCommand_.filterState);
        executing_ = false;
        break;
    
    case CommandType::WAIT:
        if (timer_.expired(ms)) {
            executing_ = false;
        }
        break;
    
    case CommandType::RESET:
        stopExecuting();
        break;
    
    default:
        break;
    }

    return executing_ ? NOTHING : FINISHED;
}
