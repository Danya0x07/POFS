#ifndef _PARSER_H
#define _PARSER_H

#include "commands.hpp"

enum class ParsingStatus {OK, ERROR};

ParsingStatus parse(const char *input, Command &cmd);

#endif // _PARSER_H