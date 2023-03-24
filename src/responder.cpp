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

void reply(char *output, Response response)
{
    switch (response)
    {
    case Response::PARSING_OK:
        output[0] = '0';
        break;
    
    case Response::PARSING_ERR:
        output[0] = '1';
        break;
    
    case Response::DISPATCH_ERR:
        output[0] = '2';
        break;
    
    case Response::EXEC_FINISH:
        output[0] = '3';
        break;
    }
    output[1] = '\n';
    output[2] = '\0';
}

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
        str += (int)cmd.flapStatus;
        break;

    case CommandType::SET_FILTER:
        str += (int)cmd.filterState;
        break;
    
    case CommandType::WAIT:
        str += cmd.waitTime;
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
    case CommandType::EMERGENCY:
    case CommandType::ERROR:
        break;
    }
    str += '\n';

    strcpy(output, str.c_str());
}
