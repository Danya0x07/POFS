#ifndef _PROGRAM_MEMORY_HPP
#define _PROGRAM_MEMORY_HPP

#include "commands.hpp"

class ProgramMemory
{
public:
    enum Status {START, STORE, COMPLETE, ERROR};

    ProgramMemory(Command *buffer, int bufferLen);
    Status store(const Command &command);
    void reset();
    int getLenUsed();

private:
    Command *commandBuffer_;
    const int bufferLen_;
    int lenUsed_;
    int index_;
};

#endif // _PROGRAM_MEMORY_HPP