#include "executor.hpp"

Executor::Executor(Timer &timer)
{
}

void Executor::startExecuting(const Command &cmd, uint32_t now)
{
}

bool Executor::isExecuting()
{
    return false;
}

void Executor::stopExecuting()
{
}

Executor::Event Executor::run(uint32_t ms)
{
    return Event();
}
