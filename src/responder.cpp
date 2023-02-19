#include "responder.hpp"

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
