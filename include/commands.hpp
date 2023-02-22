#ifndef _COMMANDS_H
#define _COMMANDS_H

#include <stdint.h>

enum class CommandType: uint8_t {
    SET_FLAP,
    SET_FILTER,
    WAIT,
    LOADING_MODE,
    SAVE_PROGRAM,
    EXECUTE_PROGRAM,
    RESET,
    ERROR
};

enum class FlapStatus: uint8_t {
    CLOSED, OPENED
};

enum class FilterState: uint8_t {
    FS0,
    FS1, FS2, FS3, FS4,
};

struct LoopData {
    uint8_t beginMark;
    uint8_t endMark;
    uint8_t numRepetitions;
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