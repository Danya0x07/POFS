#include "motors.hpp"
#include "pinconfig.hpp"

#ifdef ARDUINO
#   include <Servo.h>
#else
#   include <stdio.h>
#   define pinMode(pin, mode) 

class Servo
{
    int pin_;

public:
    void attach(int pin)
    {
        pin_ = pin;
    }

    bool attached()
    {
        return false;
    }

    void write(int value)
    {
        printf("Servo %d written angle %d\n", pin_, value);
    }
};
#endif

Servo s0, s1, s2, s3, s4;

#define S0_INACT    0
#define S0_ACT  180
#define S1_INACT    0
#define S1_ACT  180
#define S2_INACT    0
#define S2_ACT  180
#define S3_INACT    0
#define S3_ACT  180
#define S4_INACT    0
#define S4_ACT  180

FlapStatus flapStatus;
FilterState filterState;

void motorsInit()
{
    pinMode(PIN_S0, OUTPUT);
    pinMode(PIN_S1, OUTPUT);
    pinMode(PIN_S2, OUTPUT);
    pinMode(PIN_S3, OUTPUT);
    pinMode(PIN_S4, OUTPUT);

    if (!s0.attached())
        s0.attach(PIN_S0);
    if (!s1.attached())
        s1.attach(PIN_S1);
    if (!s2.attached())
        s2.attach(PIN_S2);
    if (!s3.attached())
        s3.attach(PIN_S3);
    if (!s4.attached())
        s4.attach(PIN_S4);
    
    flapSet(FlapStatus::CLOSED);
    filterSet(FilterState::FS0);
}

void flapSet(FlapStatus status)
{
    s0.write(status == FlapStatus::OPENED ? S0_ACT : S0_INACT);
    flapStatus = status;
}

FlapStatus flapGet()
{
    return flapStatus;
}

void filterSet(FilterState state)
{
    s1.write(state == FilterState::FS1 ? S1_ACT : S1_INACT);
    s2.write(state == FilterState::FS2 ? S2_ACT : S2_INACT);
    s3.write(state == FilterState::FS3 ? S3_ACT : S3_INACT);
    s4.write(state == FilterState::FS4 ? S4_ACT : S4_INACT);
    filterState = state;
}

FilterState filterGet()
{
    return filterState;
}
