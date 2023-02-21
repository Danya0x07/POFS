#include "program_memory.hpp"

ProgramMemory::ProgramMemory(Command *buffer, int bufferLen)
{
}

ProgramMemory::Status ProgramMemory::store(const Command &command)
{
    return Status();
}

void ProgramMemory::reset()
{
}

int ProgramMemory::getLenUsed()
{
    return 0;
}
