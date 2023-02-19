#ifndef _MOTORS_HPP
#define _MOTORS_HPP

#include "commands.hpp"

void motorsInit();

void flapSet(bool open);
bool flapGet();

void filterSet(FilterState filterState);
FilterState filterGet();

#endif // _MOTORS_HPP