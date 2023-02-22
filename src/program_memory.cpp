#include "program_memory.hpp"

ProgramMemory::ProgramMemory(Command *buffer, int bufferLen) 
: commandBuffer_(buffer), bufferLen_(bufferLen), lenUsed_(0), index_(1)
{
    reset();
}

bool commandValid(const Command &cmd)
{
    return (cmd.type == CommandType::SET_FILTER
            || cmd.type == CommandType::SET_FLAP
            || cmd.type == CommandType::WAIT);
}

ProgramMemory::Status ProgramMemory::store(const Command &command)
{
    if (command.type == CommandType::SAVE_PROGRAM) {
        LoopData loop = command.loop;
        if (loop.endMark > lenUsed_ || loop.beginMark > loop.endMark)
            return ERROR;

        commandBuffer_[0] = command;
        return COMPLETE;
    } else {
        if (commandValid(command) && index_ < bufferLen_) {
            commandBuffer_[index_++] = command;
            lenUsed_++;
            return index_ == 2 ? START : STORE;
        } else {
            reset();
            return ERROR;
        }
    }    
}

void ProgramMemory::reset()
{
    lenUsed_ = 0;
    index_ = 1;
    for (int i = 0; i < bufferLen_; i++) {
        commandBuffer_[i].type = CommandType::ERROR;
        commandBuffer_[i].waitTime = 0;
    }
}

int ProgramMemory::getLenUsed()
{
    return commandBuffer_[0].type == CommandType::SAVE_PROGRAM ? lenUsed_ : 0;
}
