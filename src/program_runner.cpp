#include "program_runner.hpp"

ProgramRunner::ProgramRunner(const Command *buffer)
: extraordinaryCommand_{.type = CommandType::ERROR},
urgentCommand_{.type = CommandType::ERROR},
loopData_{0}, commandBuffer_(buffer), 
sequenceLen_(0), currentIndex_(1), commandsLeft_(0)
{
}

ProgramRunner::Status ProgramRunner::setupCommandSequence(int sequenceLen)
{
    if (commandBuffer_[0].type != CommandType::SAVE_PROGRAM)
        return ERROR;
    for (int i = 1; i <= sequenceLen; i++) {
        if (
            !(commandBuffer_[i].type == CommandType::SET_FLAP
            || commandBuffer_[i].type == CommandType::SET_FILTER
            || commandBuffer_[i].type == CommandType::WAIT)
        ) {
            return ERROR;
        }
    }

    loopData_ = commandBuffer_[0].loop;
    if (loopData_.beginMark > loopData_.endMark || loopData_.endMark > sequenceLen)
        return ERROR;
    
    sequenceLen_ = sequenceLen;
    currentIndex_ = 1;
    int loopLen = loopData_.numRepetitions ? (loopData_.endMark - loopData_.beginMark + 1) : 0;
    commandsLeft_ = sequenceLen_ + loopLen * (loopData_.numRepetitions);

    return OK;
}

void ProgramRunner::setExtraordinaryCommand(const Command &command)
{
    extraordinaryCommand_ = command;
}

void ProgramRunner::setUrgentCommand(const Command &command)
{
    urgentCommand_ = command;
}

int ProgramRunner::commandsAvailable()
{
    
    return hasExtraordinaryCommand() + hasUrgentCommand() + commandsLeft_;
}

bool ProgramRunner::hasExtraordinaryCommand()
{
    return extraordinaryCommand_.type != CommandType::ERROR;
}

bool ProgramRunner::hasUrgentCommand()
{
    return urgentCommand_.type != CommandType::ERROR;
}

ProgramRunner::Status ProgramRunner::readCommand(Command &command)
{
    if (hasUrgentCommand()) {
        command = urgentCommand_;
        urgentCommand_.type = CommandType::ERROR;
        return OK;
    }

    if (hasExtraordinaryCommand()) {
        command = extraordinaryCommand_;
        extraordinaryCommand_.type = CommandType::ERROR;
        return OK;
    }

    if (commandsLeft_ < 1) {
        command.type = CommandType::ERROR;
        return ERROR;
    }

    command = commandBuffer_[currentIndex_];
    commandsLeft_--;
    
    if (loopData_.beginMark == 0 || loopData_.endMark == 0) {
        currentIndex_++;
    } else {
        if (currentIndex_ == loopData_.endMark && loopData_.numRepetitions) {
            currentIndex_ = loopData_.beginMark;
            loopData_.numRepetitions--;
        } else {
            currentIndex_++;
        }
    }
    if (commandsLeft_ == 0) {
        currentIndex_ = 1;
    }

    return OK;
}

void ProgramRunner::reset()
{
    sequenceLen_ = 0;
    currentIndex_ = 1;
    commandsLeft_ = 0;
    extraordinaryCommand_.type = CommandType::ERROR;
    urgentCommand_.type = CommandType::ERROR;
    loopData_ = {0};
}
