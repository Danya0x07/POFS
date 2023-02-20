#include "program_runner.hpp"

ProgramRunner::ProgramRunner(const Command *buffer)
{
}

ProgramRunner::Status ProgramRunner::setupCommandSequence(int sequenceLen)
{
    return Status();
}

void ProgramRunner::setExtraordinaryCommand(const Command &command)
{
}

void ProgramRunner::setUrgentCommand(const Command &command)
{
}

int ProgramRunner::commandsAvailable()
{
    return 0;
}

bool ProgramRunner::hasUrgentCommand()
{
    return false;
}

ProgramRunner::Status ProgramRunner::readCommand(Command &command)
{
    return Status();
}

void ProgramRunner::reset()
{
}
