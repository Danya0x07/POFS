#ifndef _COMMANDS_H
#define _COMMANDS_H

#include <stdint.h>

enum class CommandType {
    SET_FLAP,
    SET_FILTER,
    WAIT,
    LOADING_MODE,
    SAVE_PROGRAM,
    EXECUTE_PROGRAM,
    RESET,
    ERROR
};

enum class FlapStatus {
    CLOSED, OPENED
};

enum class FilterState {
    FS0,
    FS1, FS2, FS3, FS4,
};

struct LoopData {
    uint16_t beginMark;
    uint16_t endMark;
    uint16_t numIterations;
};

struct Command {
    CommandType type;
    union {
        FlapStatus flapStatus;
        FilterState filterState;
        uint32_t waitTime;
        LoopData loop;
    };
};

#endif // _COMMANDS_H