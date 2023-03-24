#ifndef _RESPONDER_HPP
#define _RESPONDER_HPP

#include "commands.hpp"

enum class Response {
    PARSING_OK,
    PARSING_ERR,
    DISPATCH_ERR,
    EXEC_FINISH
};

void reply(char *output, Response response);
void echo(char *output, const Command &cmd);

#endif // _RESPONDER_HPP