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
    CALIBRATE,
    SAVE_CALIBRATION,
    PRINT_CALIBRATION,
    ERROR,
    EMERGENCY,
};

#define CMDKEY_FLAP     'G'
#define CMDKEY_FILTER   'F'
#define CMDKEY_WAIT     'W'
#define CMDKEY_LOAD     'L'
#define CMDKEY_PROG     'P'
#define CMDKEY_EXEC     'E'
#define CMDKEY_CALIB    'C'
#define CMDKEY_SAVE     'S'
#define CMDKEY_PRINT    'c'
#define CMDKEY_RST      'R'
#define CMDKEY_EMG      '!'

enum class FlapStatus: uint8_t {
    CLOSED, OPENED
};

enum class FilterState: uint8_t {
    FS0,
    FS1, FS2, FS3, FS4,
};

enum MotorID : uint8_t {
    FLAP, 
    F1, F2, F3, F4
};

struct LoopData {
    uint8_t beginMark;
    uint8_t endMark;
    uint8_t numRepetitions;
};

struct CalibrationData {
    MotorID motorID;
    uint8_t openedAngle;
    uint8_t closedAngle;
};

struct Command {
    CommandType type;
    union {
        FlapStatus flapStatus;
        FilterState filterState;
        uint32_t waitTime;
        LoopData loop;
        CalibrationData calibration;
    };
};

#endif // _COMMANDS_H