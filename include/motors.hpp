#ifndef _MOTORS_HPP
#define _MOTORS_HPP

#include "commands.hpp"

#define NUM_MOTORS 5

void motorsSaveAngles();

void motorsInit();
void motorCalibrate(CalibrationData calibration);
void motorsPrintCalibration();

bool motorsHaveUnsavedCalibrationData();

void flapSet(FlapStatus status);
FlapStatus flapGet();

void filterSet(FilterState state);
FilterState filterGet();

#endif // _MOTORS_HPP