#ifndef _PARSER_H
#define _PARSER_H

#include "commands.hpp"

#define CMDKEY_FLAP     'G'
#define CMDKEY_FILTER   'F'
#define CMDKEY_WAIT     'W'
#define CMDKEY_LOAD     'L'
#define CMDKEY_PROG     'P'
#define CMDKEY_EXEC     'E'
#define CMDKEY_RST      'R'

enum class ParsingStatus {OK, ERROR};

ParsingStatus parse(const char *input, Command &cmd);

#endif // _PARSER_H