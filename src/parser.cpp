#include "parser.hpp"
#include "motors.hpp"
#include <string.h>
#include <stdlib.h>

static CommandType retrieveCommandType(const char *input)
{
    switch (input[0])
    {
    case CMDKEY_FLAP:
        return CommandType::SET_FLAP;
    
    case CMDKEY_FILTER:
        return CommandType::SET_FILTER;
    
    case CMDKEY_WAIT:
        return CommandType::WAIT;

    case CMDKEY_LOAD:
        return CommandType::LOADING_MODE;
    
    case CMDKEY_PROG:
        return CommandType::SAVE_PROGRAM;
    
    case CMDKEY_EXEC:
        return CommandType::EXECUTE_PROGRAM;
    
    case CMDKEY_CALIB:
        return CommandType::CALIBRATE;
    
    case CMDKEY_SAVE:
        return CommandType::SAVE_CALIBRATION;
    
    case CMDKEY_RST:
        return CommandType::RESET;
    
    default:
        return CommandType::ERROR;
    }
}

const char * nextParameter(const char *marker, int16_t *param)
{
    const char *comma = strchr(marker, ',');

    if (!comma)
        return NULL;
    
    const char *next = comma + 1;
    if (*next == '\0')
        return NULL;
    *param = atoi(next);
    return next;
}

ParsingStatus parse(const char *input, Command &cmd)
{
    const char *marker;
    int16_t tmp;
    
    cmd.type = retrieveCommandType(input);

    switch (cmd.type)
    {
    case CommandType::SET_FLAP:
        if (input[1] == '0')
            cmd.flapStatus = FlapStatus::CLOSED;
        else if (input[1] == '1')
            cmd.flapStatus = FlapStatus::OPENED;
        else
            return ParsingStatus::ERROR;
        if (input[2] != '\0')
            return ParsingStatus::ERROR;
        break;
    
    case CommandType::SET_FILTER:
        switch (input[1])
        {
        case '0': cmd.filterState = FilterState::FS0; break;
        case '1': cmd.filterState = FilterState::FS1; break;
        case '2': cmd.filterState = FilterState::FS2; break;
        case '3': cmd.filterState = FilterState::FS3; break;
        case '4': cmd.filterState = FilterState::FS4; break;
        default: return ParsingStatus::ERROR;
        }
        if (input[2] != '\0')
            return ParsingStatus::ERROR;
        break;

    case CommandType::WAIT:
        cmd.waitTime = atol(input + 1);
        if (cmd.waitTime > 36000000L || 
            uint32_t(input[strlen(input) - 1] - '0') != cmd.waitTime % 10)
            return ParsingStatus::ERROR;
        break;

    case CommandType::LOADING_MODE:
        if (input[1] != '\0')
            return ParsingStatus::ERROR;
        break;

    case CommandType::SAVE_PROGRAM:
        marker = input + 1;
        cmd.loop.beginMark = atoi(marker);
        if (!(marker = nextParameter(marker, &tmp))) {
            return ParsingStatus::ERROR;
        }
        if (tmp < 0 || tmp > 100 || tmp < cmd.loop.beginMark) {
            return ParsingStatus::ERROR;
        }
        cmd.loop.endMark = (uint16_t) tmp;

        if (!(marker = nextParameter(marker, &tmp))) {
            return ParsingStatus::ERROR;
        }
        if (tmp < 0 || tmp > 10000) {
            return ParsingStatus::ERROR;
        }
        cmd.loop.numRepetitions = (uint16_t) tmp;

        if (nextParameter(marker, &tmp))
            return ParsingStatus::ERROR;
        
        if (cmd.loop.beginMark == 0 || cmd.loop.endMark == 0 || cmd.loop.numRepetitions == 0)
            cmd.loop = {0};
        break;

    case CommandType::EXECUTE_PROGRAM:
        if (input[1] != '\0')
            return ParsingStatus::ERROR;
        break;
    
    case CommandType::CALIBRATE:
        marker = input + 1;
        tmp = atoi(marker);
        if (tmp > NUM_MOTORS)
            return ParsingStatus::ERROR;
        cmd.calibration.motorID = (MotorID)tmp;

        if (!(marker = nextParameter(marker, &tmp))) {
            return ParsingStatus::ERROR;
        }
        if (tmp > 180)
            return ParsingStatus::ERROR;
        cmd.calibration.openedAngle = (MotorID)tmp;

        if (!(marker = nextParameter(marker, &tmp))) {
            return ParsingStatus::ERROR;
        }
        if (tmp > 180)
            return ParsingStatus::ERROR;
        cmd.calibration.closedAngle = (MotorID)tmp;

        if (nextParameter(marker, &tmp))
            return ParsingStatus::ERROR;
        break;

    case CommandType::SAVE_CALIBRATION:
        if (input[1] != '\0')
            return ParsingStatus::ERROR;
        break;
    
    case CommandType::RESET:
        if (input[1] != '\0')
            return ParsingStatus::ERROR;
        break;
    
    case CommandType::ERROR:
        return ParsingStatus::ERROR;
    }
    
    return ParsingStatus::OK;
}
