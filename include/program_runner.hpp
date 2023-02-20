#ifndef _PROGRAM_RUNNER_HPP
#define _PROGRAM_RUNNER_HPP

#include "commands.hpp"

class ProgramRunner
{
public:
    enum Status {OK, ERROR};

    ProgramRunner(const Command *buffer);
    Status setupCommandSequence(int sequenceLen);
    void setExtraordinaryCommand(const Command &command);
    void setUrgentCommand(const Command &command);
    int commandsAvailable();
    bool hasUrgentCommand();
    Status readCommand(Command &command);
    void reset();
};

#endif // _PROGRAM_RUNNER_HPP