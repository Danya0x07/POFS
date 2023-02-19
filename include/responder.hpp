#ifndef _RESPONDER_HPP
#define _RESPONDER_HPP

enum class Response {
    PARSING_OK,
    PARSING_ERR,
    DISPATCH_ERR,
    EXEC_FINISH
};

void reply(char *output, Response response);

#endif // _RESPONDER_HPP