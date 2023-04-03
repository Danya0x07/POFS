#ifndef _MOTORS_HPP
#define _MOTORS_HPP

#include "commands.hpp"
#include "responder.hpp"

void motorsSaveAngles();

void motorsInit();
void motorCalibrate(CalibrationData calibration);
void motorsGetCalibration(Response &response);

bool motorsHaveUnsavedCalibrationData();

void flapSet(FlapStatus status);
FlapStatus flapGet();

void filterSet(FilterState state);
FilterState filterGet();

#endif // _MOTORS_HPP