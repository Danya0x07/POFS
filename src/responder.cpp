#include "responder.hpp"

#ifndef PIO_UNIT_TESTING
#   include <Arduino.h>
#   define WRAP(n) (n)
#else
#   include <string>
#   include <cstring>
#   define String   string

using std::string;

#   define WRAP(n) std::to_string((n))
#endif

static char getCmdKey(CommandType cmdType)
{
    switch (cmdType)
    {
    case CommandType::SET_FLAP: return CMDKEY_FLAP;
    case CommandType::SET_FILTER: return CMDKEY_FILTER;
    case CommandType::WAIT: return CMDKEY_WAIT;
    case CommandType::LOADING_MODE: return CMDKEY_LOAD;
    case CommandType::SAVE_PROGRAM: return CMDKEY_PROG;
    case CommandType::EXECUTE_PROGRAM: return CMDKEY_EXEC;
    case CommandType::RESET: return CMDKEY_RST;
    case CommandType::CALIBRATE: return CMDKEY_CALIB;
    case CommandType::SAVE_CALIBRATION: return CMDKEY_SAVE;
    case CommandType::PRINT_CALIBRATION: return CMDKEY_PRINT;
    case CommandType::EMERGENCY: return CMDKEY_EMG;
    case CommandType::ERROR: return '\0';
    }
    return 0;
}

void echo(char *output, const Command &cmd)
{
    char out[2] = {getCmdKey(cmd.type), 0};
    String str = out;

    switch (cmd.type)
    {
    case CommandType::SET_FLAP:
        str += String(",") + WRAP((int)cmd.flapStatus);
        break;

    case CommandType::SET_FILTER:
        str += String(",") + WRAP((int)cmd.filterState);
        break;
    
    case CommandType::WAIT:
        str += String(",") + WRAP(cmd.waitTime);
        break;
    
    case CommandType::CALIBRATE:
        str += String(WRAP(cmd.calibration.motorID))
                + String(",") + WRAP(cmd.calibration.openedAngle)
                + String(",") + WRAP(cmd.calibration.closedAngle);
        break;

    case CommandType::SAVE_PROGRAM:
        str += String(WRAP(cmd.loop.beginMark))
                + String(",") + WRAP(cmd.loop.endMark)
                + String(",") + WRAP(cmd.loop.numRepetitions);
        break;
    
    case CommandType::LOADING_MODE:
    case CommandType::EXECUTE_PROGRAM:
    case CommandType::RESET:
    case CommandType::SAVE_CALIBRATION:
    case CommandType::PRINT_CALIBRATION:
    case CommandType::EMERGENCY:
    case CommandType::ERROR:
        break;
    }
    str += '\n';

    strcpy(output, str.c_str());
}

void reply(char *output, const Response &response)
{
    switch (response.type)
    {
    case ResponseType::PARSING_OK:
    case ResponseType::PARSING_ERR:
    case ResponseType::DISPATCH_ERR:
        snprintf(output, 64, "%d\n", (uint8_t)response.type);
        break;
    
    case ResponseType::EXEC_FINISH:
        output[0] = '0' + (uint8_t)response.type;
        output[1] = ',';
        echo(output + 2, response.lastCommand);
        break;
    
    case ResponseType::DATA:
        snprintf(output, 64, "%d,%d %d,%d %d,%d %d,%d %d,%d %d\n",
            (uint8_t)response.type,
            response.angles.opened[0], response.angles.closed[0],
            response.angles.opened[1], response.angles.closed[1],
            response.angles.opened[2], response.angles.closed[2],
            response.angles.opened[3], response.angles.closed[3],
            response.angles.opened[4], response.angles.closed[4]
        );
        break;
    }
}
