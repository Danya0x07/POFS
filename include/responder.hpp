#ifndef _RESPONDER_HPP
#define _RESPONDER_HPP

#include "commands.hpp"

enum class ResponseType: uint8_t {
    PARSING_OK,
    PARSING_ERR,
    DISPATCH_ERR,
    EXEC_FINISH,
    DATA
};

struct Response {
    ResponseType type;
    union {
        Command lastCommand;
        struct {
            uint8_t opened[NUM_MOTORS];
            uint8_t closed[NUM_MOTORS];
        } angles;
    };
};

void reply(char *output, const Response &response);

#endif // _RESPONDER_HPP