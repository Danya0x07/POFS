#ifndef _MOTORS_HPP
#define _MOTORS_HPP

#include "commands.hpp"

void motorsInit();

void flapSet(FlapStatus status);
FlapStatus flapGet();

void filterSet(FilterState state);
FilterState filterGet();

#endif // _MOTORS_HPP